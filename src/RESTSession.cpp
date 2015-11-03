#include "RESTSession.h"

RESTSession::RESTSession( uv_tcp_t * conn )
    : ClusterSession(conn)
{
    //router.response( [this] ( HTTPResponse* rep ) {
    //    auto result = rep->bytes();
    //    this->send( result.raw(), result.length() );
    //} );

    router.request( [this] ( HTTPRequest* req ) {
        std::string body_str = req->body();
        if ( body_str.empty() )
        {
            this->close();
            return;
        }

        try
        {
            Message message( body_str );
            message.owner( this );
            this->message( &message );
            ClusterSession::dispatch_message( &message );
        }
        catch ( exception ee )
        {
            this->close();
        }
    } );
}

RESTSession::~RESTSession()
{
}

void RESTSession::message( Message * message )
{
    
}

void RESTSession::send( Message * message )
{
    HTTPResponse rep;
    rep.status( 200 );
    auto message_buf = message->bytes();
    rep.content_type( "application/json" );
    rep.content( message_buf.raw(), message_buf.length() );

    auto rep_buf = rep.bytes();
    this->send( rep_buf.raw(), rep_buf.length() );
}

void RESTSession::recv( const char * data, int len )
{
    router.parse( data, len );
}

void RESTSession::send( const char * data, int len )
{
    Session::send( data, len );
}
 
