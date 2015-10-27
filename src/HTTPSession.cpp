#include "HTTPSession.h"
#include "stdio.h"
#include "maraton.h"

HTTPSession::HTTPSession( uv_tcp_t* conn )
    : Session::Session( conn )
{ 
    this->handler = new HTTPHandler( this );
    this->handler->router()->response( 
        [this] ( HTTPResponse* rep ) { 
            auto buffer = rep->bytes();
            this->send( buffer.raw(), buffer.length() );
            this->close();
        }
    );
    printf( "Session[%d] incoming request...\r\n", this->id() );
}              

HTTPSession::~HTTPSession()
{
    SAFE_DELETE( this->handler );
}

void HTTPSession::recv( const char* data, int len )
{
    handler->router()->parse( data, len );
} 