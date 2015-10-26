/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */

#ifndef EXECUTOR_H_
#define EXECUTOR_H_ 

#include "maraton.h"
#include "ExecutorSession.h"
#include "json.hpp"
#include "Message.h"
#include <string.h>

class Executor
{
public:

    Executor( ExecutorSession* session ) { this->session_ = session; };
    ~Executor() {};

    void run();
    void message( std::string json_str );
    void send( Message message );

private:

    Executor( Executor & ) = delete;
    Executor( Executor && ) = delete;
    Executor operator=( Executor & ) = delete;

    ExecutorSession* session_;
};

#endif //!EXECUTOR_H_ 