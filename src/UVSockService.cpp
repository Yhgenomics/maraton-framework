#include "UVSockService.h"
#include "SessionManager.hpp"
#include "ExecutorSession.h"
#include "MasterSession.h"
#include "HTTPSession.h"
#include "RESTSession.h"

bool UVSockService::is_connecting = false;

UVSockService::UVSockService()
{
    this->loop_ = nullptr;
    this->socket_ = nullptr;
}

UVSockService::~UVSockService()
{
    SAFE_DELETE( this->socket_ );
    SAFE_DELETE( this->loop_ );
}

bool UVSockService::listen( std::string ip, int port )
{
    this->loop_ = uv_default_loop(); 
    auto socket_ = new uv_tcp_t();

    uv_tcp_init( this->loop_, socket_ );
    uv_ip4_addr( ip.c_str(), port, &addr_in );

    uv_tcp_bind( socket_, ( const struct sockaddr* )&addr_in, 0 );
    int result = uv_listen( ( uv_stream_t* )socket_, 
                       10000, 
                       UVSockService::uv_connection_cb_process );

    listen_data* data = new listen_data();

    data->ip = ip;
    data->port = port;
    data->loop = this->loop_;

    socket_->data = data;

    if ( result > 0 )
    {
        Logger::error( "Can't create port %d", port );
        return false;
    }

    Logger::sys( "Create port %d successed", port );

    return true;
}

bool UVSockService::connect( std::string ip, int port )
{
    if ( is_connecting )
        return false;

    this->loop_ = uv_default_loop();
    auto socket_ = new uv_tcp_t();

    uv_tcp_init( this->loop_, socket_ ); 

    //mem leak
    uv_connect_t* connect = ( uv_connect_t* ) malloc( sizeof( uv_connect_t ) );

    uv_ip4_addr( ip.c_str(), port, &addr_in );

    connect->data = socket_;

    socket_->data = connect;

    auto result = uv_tcp_connect( 
        connect, socket_, 
        ( const struct sockaddr* )&addr_in, 
        UVSockService::uv_connected_cb_process );

    is_connecting = true;

    return result == 0;
}

void UVSockService::run()
{ 
    uv_run( uv_default_loop() , UV_RUN_DEFAULT );
}

uv_loop_t * UVSockService::loop()
{
    return uv_default_loop();
}

void UVSockService::uv_connection_cb_process( uv_stream_t * server, int status )
{
    if ( status < 0 )
    {
        return;
    }

    listen_data* data = static_cast< listen_data* >( server->data );

    uv_tcp_t *client = new uv_tcp_t();
    uv_tcp_init( uv_default_loop(), client );
    

    if ( uv_accept( server, ( uv_stream_t* ) client ) == 0 ) 
    { 
        Session* session = nullptr;

        switch ( data->port )
        {
        case MASTER:
            session = SessionManager<MasterSession>::instance()->create( client );
            break;

        case RESTAPI:
            session = SessionManager<RESTSession>::instance()->create( client );
            break;

        case EXECUTOR:
            session = SessionManager<ExecutorSession>::instance()->create( client );
            break;

        default:
            break;  
        }

        session->loop_ = uv_default_loop();

        client->data = static_cast< void* >( session );

        uv_read_start( ( uv_stream_t* ) client,
                       UVSockService::uv_alloc_cb_process,
                       UVSockService::uv_read_cb_process );

    }
    else 
    {
        uv_close( ( uv_handle_t* ) client, UVSockService::uv_close_cb_process );
    }

}

void UVSockService::uv_connected_cb_process( uv_connect_t * req, int status )
{ 
  
    if ( status < 0 )
    {
        //Logger::error( "%s", uv_strerror( static_cast< int >( status ) ) );

        auto sock = ( uv_tcp_t* ) req->data;

        auto connect = ( uv_connect_t* ) sock->data;

        SAFE_DELETE ( sock );
        SAFE_DELETE ( connect );

        is_connecting = false;

        return;
    }

    uv_tcp_t* client = static_cast< uv_tcp_t* >( req->data );

    auto session = SessionManager<MasterSession>::instance()->create( client );
 
    is_connecting = false;

    session->loop_ = client->loop;

    client->data = session;

    uv_read_start( ( uv_stream_t* ) client,
                   UVSockService::uv_alloc_cb_process,
                   UVSockService::uv_read_cb_process );

}

void UVSockService::uv_alloc_cb_process( uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf )
{
    Session* session = static_cast< Session* >( handle->data );

    buf->base = session->recv_buffer();
    buf->len = session->buffer_len();
    memset( buf->base , 0 , buf->len );
}

void UVSockService::uv_read_cb_process( uv_stream_t * stream, ssize_t nread, const uv_buf_t * buf )
{
    Session* session = static_cast< Session* >( stream->data );

    if ( static_cast< int >( nread ) < 0 )
    {
        session->close();//uv_read_stop( stream );
        return;
    }
    
    session->recv( buf->base, static_cast< int >( nread ) );
}

void UVSockService::uv_close_cb_process( uv_handle_t * handle )
{
    Session* session = static_cast< Session* >( handle->data );
    handle->data = nullptr;

    session->shutdown();

    SAFE_DELETE( session );
}

void UVSockService::uv_write_cb_process( uv_write_t * req, int status )
{
    if ( status < 0 )
    {   
        return;
    } 

    char* buffer_data = static_cast< char* >( req->data );
    
    SAFE_DELETE( buffer_data );
    SAFE_DELETE( req );
}
