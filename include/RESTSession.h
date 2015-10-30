/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */
#ifndef REST_SESSION_H_
#define REST_SESSION_H_ 

#include "ClusterSession.h"
#include "HTTPRouter.hpp"

class RESTSession :
    public ClusterSession
{
public:

    virtual void message( Message * message ) override;
    virtual void send( Message* message ) override;

protected:

    virtual void recv( const char* data, int len ) override;
    virtual void send( const char* data, int len ) override;

private:

    HTTPRouter router;
};

#endif //!REST_SESSION_H_ 