#include "Session.h"
#include "maraton.h"
#include "uv.h"
#include <memory.h>
#include <thread>
#include <atomic>
#include <mutex>
#include "UVSockService.h"

Session::Session( uv_tcp_t * conn )
{
    this->recv_buffer_ = new char[this->buffer_len()];
  
    this->conn_ = conn;

    this->id_ = Session::create_session_id();
}

Session::~Session()
{
    SAFE_DELETE( this->recv_buffer_ );
    SAFE_DELETE( this->conn_ );
}

void Session::send( const char * data, int len )
{ 
    uv_write_t* write_req_ = new uv_write_t();
    memset( write_req_, 0, sizeof( uv_write_t ) );

    uv_buf_t p_buf;
    p_buf.base = new char[len];
    memcpy( p_buf.base , data, len );
    p_buf.len = len;

    write_req_->data = p_buf.base;

    int result = 1;
    while ( result )
    {
        result = uv_write( write_req_, ( uv_stream_t* )this->conn_, &p_buf, 1 , UVSockService::uv_write_cb_process );
    }
}

void Session::close()
{
    uv_close( (uv_handle_t*)this->conn_, UVSockService::uv_close_cb_process );
}

int Session::create_session_id()
{
    static int session_id = 10000;
    static std::mutex m_lock;

    m_lock.lock();
    session_id = session_id + 1;
    m_lock.unlock();

    return session_id;
}
