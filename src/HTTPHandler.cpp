#include "HTTPHandler.h"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include "HTTPSession.h"

HTTPHandler::HTTPHandler( HTTPSession * session )
{
    this->session_ = session;
}
