#include "HTTPHandler.h"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPSession.h"

HTTPHandler::HTTPHandler( HTTPSession * session )
{
    this->session_ = session;

    router_.get( "/", [this] (HTTPRequest* req,HTTPResponse* rep) {
        rep->content("<body><h1>Hello REST API!!!!!</h1></body>" );
    } );

    router_.get( "/api", [this] ( HTTPRequest* req, HTTPResponse* rep ) {
        rep->content( "<body><h1>This is the application program interface</h1></body>" );
    } );
}
