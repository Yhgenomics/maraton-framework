/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-16 13:21
* Description:
* * * * * * * * * * * * * * * */

#ifndef MASTER_SESSION_H_
#define MASTER_SESSION_H_

#include "ClusterSession.h"

#include "maraton.h"
#include "uv.h"

class Master;

class MasterSession :
    public ClusterSession
{ 
public:

    MasterSession( uv_tcp_t * conn );
    virtual ~MasterSession() override;

protected:

    virtual void message( Message* message ) override;
    virtual void shutdown() override ;
};

#endif //MASTER_SESSION_H__H_ 