#include "AsyncWorker.h"
#include "maraton.h"

AsyncWorker * AsyncWorker::create( callback_t acting ,
                                   callback_t finish , void * data )
{
    auto r = new AsyncWorker( acting, finish );
    r->data( data );
    r->start();
    return r;
}

AsyncWorker* AsyncWorker::create( callback_t callback, void * data )
{
    auto r = new AsyncWorker( callback , nullptr );
    r->data( data );
    r->start();
    return r;
}

AsyncWorker::AsyncWorker( callback_t acting , callback_t finish )
{
    this->acting_callback_ = acting;
    this->finish_callback_ = finish;
}

void AsyncWorker::start()
{
    this->worker.data = this;
    uv_queue_work( uv_default_loop() , &this->worker ,
                   AsyncWorker::uv_process_work_callback , 
                   AsyncWorker::uv_process_after_work_callback );
}

void AsyncWorker::uv_process_work_callback( uv_work_t * req )
{
    AsyncWorker* inst = static_cast< AsyncWorker* >( req->data );

    if ( inst == nullptr )return;
    
    inst->acting_callback_( inst );
}

void AsyncWorker::uv_process_after_work_callback( uv_work_t * req , int status )
{
    AsyncWorker* inst = static_cast< AsyncWorker* >( req->data );

    if ( inst == nullptr )return;

    if ( inst->finish_callback_ != nullptr )
    {
        inst->finish_callback_( inst );
    }

    SAFE_DELETE( inst );
}
