/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */
#ifndef HTTP_SESSION_H_
#define HTTP_SESSION_H_ 

#include "uv.h"
#include "Session.h"
#include "HTTPHandler.h"

class HTTPSession :
    public Session
{
public:

    HTTPSession( uv_tcp_t* conn );
    virtual ~HTTPSession() override;
     
    virtual void on_recv(const char* data, int len ) override;

private:

    HTTPHandler* handler; 
}; 

#endif //HTTP_SESSION_H_H_ 