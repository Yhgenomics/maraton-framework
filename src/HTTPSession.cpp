#include "HTTPSession.h"
#include "stdio.h"
#include "maraton.h"

HTTPSession::HTTPSession( uv_tcp_t* conn )
    : Session::Session( conn )
{ 
    this->handler_ = new HTTPHandler( this );
    this->handler_->router()->response(
        [this] ( HTTPResponse* rep ) { 
            auto buffer = rep->bytes();
            this->send( buffer.raw(), buffer.length() );
            this->close();
        }
    );
    //printf( "Session[%d] incoming request...\r\n", this->id() );*/
}              

HTTPSession::~HTTPSession()
{
    SAFE_DELETE( this->handler_ );
}

void HTTPSession::recv( const char* data, int len )
{
    this->handler_->router()->parse( data, len );
} 