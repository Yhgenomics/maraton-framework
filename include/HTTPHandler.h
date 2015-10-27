/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */
#ifndef HTTP_HANDLER_H_
#define HTTP_HANDLER_H_ 

#include "HTTPRouter.hpp"

class HTTPSession;

class HTTPHandler
{
public:

    HTTPHandler( HTTPSession * session);
    HTTPRouter* router()  { return &this->router_; };

private:

    HTTPRouter router_;
    HTTPSession* session_;
};

#endif //!HTTP_HANDLER_H_ 