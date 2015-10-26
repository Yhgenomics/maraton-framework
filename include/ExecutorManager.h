/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */

#ifndef EXECUTOR_MANAGER_H_
#define EXECUTOR_MANAGER_H_ 

#include "maraton.h"
#include "Executor.h"

class ExecutorManager :
    public Manager<Executor>,
    public Singleton<ExecutorManager>
{
public:

    void run();

private:

    friend Singleton<ExecutorManager>;
};

#endif //!EXECUTOR_MANAGER_H_ 