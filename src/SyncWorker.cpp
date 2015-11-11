#include "SyncWorker.h"
#include "maraton.h"

void SyncWorker::create_loop( size_t loop_time , syncworker_callback_t callback , void * data )
{
    SyncWorker* worker = new SyncWorker();
    worker->callback_ = callback;
    worker->loop_time_ = loop_time;
    worker->data( data );
    worker->start();
}

void SyncWorker::create_once( syncworker_callback_t callback , void * data )
{
    SyncWorker* worker = new SyncWorker();
    worker->callback_ = callback;
    worker->loop_time_ = 1;
    worker->data( data );
    worker->finished = true;
    worker->start();
}

void SyncWorker::stop()
{
    this->finished = true;
}

void SyncWorker::uv_process_timer_tick_callback( uv_timer_t * handle )
{
    SyncWorker* inst = static_cast< SyncWorker* >( handle->data );

    if ( inst == nullptr )
    {
        uv_timer_stop( handle );
        return;
    }

    inst->callback_( inst );

    ++inst->loop_count_;

    if ( inst->finished )
    {
        uv_timer_stop( &inst->timer_ );
        SAFE_DELETE( inst );
    }
}

SyncWorker::SyncWorker()
{
    this->timer_ = { 0 };
    this->timer_.data = this;
    this->loop_count_ = 1;
    this->loop_time_ = 1;
    uv_timer_init( uv_default_loop() , &this->timer_ );
}

SyncWorker::~SyncWorker()
{
}

void SyncWorker::start()
{
    uv_timer_start( &this->timer_ , SyncWorker::uv_process_timer_tick_callback , 0 , this->loop_time_ );
}
