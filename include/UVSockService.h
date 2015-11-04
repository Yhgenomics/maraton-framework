/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef UV_SOCK_SERVICE_H_
#define UV_SOCK_SERVICE_H_

#include <string>
#include <memory.h>

#include "uv.h"

#include "maraton.h"
#include "MasterSession.h"
#include "Session.h" 

class UVSockService
{
public:

    UVSockService();
    ~UVSockService();

    UVSockService( UVSockService& ) = delete;
    UVSockService( UVSockService&& ) = delete;
    UVSockService& operator = ( UVSockService & ) = delete;
    UVSockService& operator = ( UVSockService && ) = delete;

    bool listen( std::string ip, int port );
    bool connect( std::string ip, int port );
    void run();

    uv_loop_t* loop();

protected:

    friend Session;
    static void uv_connection_cb_process( uv_stream_t * server, int status );
    static void uv_connected_cb_process( uv_connect_t* req, int status );
    static void uv_alloc_cb_process( uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf );
    static void uv_read_cb_process( uv_stream_t * stream, ssize_t nread, const uv_buf_t * buf );
    static void uv_close_cb_process( uv_handle_t* handle );
    static void uv_write_cb_process( uv_write_t* req, int status );

private:

    uv_async_t exit_handle_;
    uv_loop_t* loop_;
    struct sockaddr_in addr_in;

    struct listen_data
    {
        uv_loop_t* loop;
        std::string ip;
        int port;
    };

    uv_tcp_t* socket_;

    static bool is_connecting;
};

#endif // !UV_SOCK_SERVICE_H_
