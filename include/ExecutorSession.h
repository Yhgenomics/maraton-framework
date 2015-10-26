/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */
#ifndef EXECUTOR_SESSION_H_
#define EXECUTOR_SESSION_H_ 

#include <vector>
#include <mutex>

#include "maraton.h"
#include "ClusterSession.h"
#include "uv.h"

class ExecutorSession :
    public ClusterSession
{
public:

    ExecutorSession(uv_tcp_t* conn);
    virtual ~ExecutorSession() override;

protected: 

    void message( Message* message ) override;
    virtual void shutdown() override;

private:

    std::vector<Buffer> buffers_;
    std::mutex mtx;
    
};


#endif //EXECUTOR_SESSION_H_ 