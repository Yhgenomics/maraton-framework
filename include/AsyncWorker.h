/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */
#ifndef ASYNC_WORKER_H_
#define ASYNC_WORKER_H_ 

#include <functional>
#include "uv.h"

class AsyncWorker
{
public:

    typedef std::function<void( AsyncWorker* )> callback_t;
    
    static AsyncWorker* create( callback_t acting , callback_t finish, void* data );
    static AsyncWorker* create( callback_t acting , void* data );


    void data( void* value ) { this->data_ = value; };
    void* data() { return this->data_; };

    void result( void* value )
    {
        this->result_ = value;
    };
    void* result( )
    {
        return this->result_;   
    }

private:

    AsyncWorker( callback_t acting, callback_t finish );

    void start();

    void* data_;
    void* result_;
    uv_work_t worker;
    callback_t acting_callback_;
    callback_t finish_callback_;

    static void uv_process_work_callback( uv_work_t* req );
    static void uv_process_after_work_callback( uv_work_t* req , int status );
};

#endif //!ASYNC_WORKER_H_ 
 