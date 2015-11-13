#include "ClusterSession.h"
#include <memory>

ClusterSession::ClusterSession( uv_tcp_t * conn )
    : Session::Session( conn )
{
}

ClusterSession::~ClusterSession()
{
}

void ClusterSession::recv( const char* data, int len )
{
    circle_buffer_.push( data, len );

    do
    {
        switch ( this->read_state_ )
        {
            case ES_READSTATE::FLAG:
                {
                    if ( this->try_read_flag() )
                    {
                        this->read_state_ = ES_READSTATE::HEAD;
                        continue;
                    }

                    return;
                }
                break;

        case ES_READSTATE::HEAD:
            {
                if ( this->try_read_head() )
                {
                    this->read_state_ = ES_READSTATE::BODY;
                    continue;
                }

                return;
            }
            break;

        case ES_READSTATE::BODY:
            {
                if ( this->try_read_body() )
                {
                    this->read_state_ = ES_READSTATE::FLAG;
                    continue;
                }
                return;
            }
            break;

        default:
            break;
        }

    } 
    while ( true );
} 

void ClusterSession::send( const char * data, int len )
{
    auto compressed_buffer = this->compressor_.compress( data, len );

    //0-1 YH
    //2-3 Compressed Length
    //4-5 Oringal Length
    int package_len = sizeof( short ) * 2 + compressed_buffer.length() + 2;

    char* package = new char[package_len];
    char* p_package = package;

    *p_package = 'Y';
    p_package++;
    *p_package = 'H';
    p_package++;

    *( short* ) p_package = static_cast< short >( compressed_buffer.length() );
    p_package += sizeof( short );
     
    *( short* ) p_package = static_cast< short >( len );
    p_package += sizeof( short );

    memcpy( p_package, compressed_buffer.raw(), compressed_buffer.length() );

    Session::send( package, package_len );

    SAFE_DELETE( package );
}

void ClusterSession::shutdown()
{
    this->dispatch_close();
}

void ClusterSession::dispatch_message( Message* message )
{
    for ( auto cb : callback_list )
    {
        cb( message );
    } 
}

void ClusterSession::dispatch_close()
{
    for ( auto cb : this->callback_session_list )
    {
        cb(this);
    }
}

void ClusterSession::on_message( callback_message_t callback )
{
    callback_list.push_back( callback );
}

void ClusterSession::on_close( callback_session_t callback )
{
    callback_session_list.push_back( callback );
}
 
void ClusterSession::send( Message * message )
{
    auto buf = message->bytes();
    this->send( buf.raw(), buf.length() );
}

bool ClusterSession::try_read_flag()
{
    std::unique_ptr<char> data( this->circle_buffer_.pop( 2 ) );

    if ( data == nullptr )
    {
        return false;
    }

    if ( data.get()[0] == 'Y' && data.get()[1] == 'H' )
    {
        return true;
    }

    return false;
}

bool ClusterSession::try_read_head()
{
    // 0-1 Compressed Length
    // 2-3 Oringal Length
    std::unique_ptr<char> data( this->circle_buffer_.pop( sizeof( short ) * 2 ));

    if ( data == nullptr )
    {
        return false;
    }

    char* pdata = data.get();

    this->compressed_length_ = *( unsigned short* ) pdata;
    pdata  += sizeof(short);

    this->oringal_length_ = *( unsigned short* ) pdata;

    return true;
}

bool ClusterSession::try_read_body()
{
    std::unique_ptr<char> data( this->circle_buffer_.pop( this->compressed_length_ ) );

    if ( data == nullptr )
    {
        return false;
    }

    auto raw_data = this->compressor_.uncompress( data.get() , this->compressed_length_ );
    
    Message message( std::string( raw_data.raw(), raw_data.length() ) );
    message.owner( this );
    this->message( &message );

    this->dispatch_message( &message );

    return true;
} 