#include "RESTSession.h"

void RESTSession::message( Message * message )
{
    router.response( [this] ( HTTPResponse* rep ) {
        auto result = rep->bytes();
        this->send( result.raw(), result.length() );
    } );

    router.request( [this] ( HTTPRequest* req ) {
        Message message( req->body() );
        message.owner( this );
        this->message( &message );
        ClusterSession::dispatch_message( &message );
    } );
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
 
