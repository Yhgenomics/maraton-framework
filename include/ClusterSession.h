#ifndef CLUSTER_SESSION_H_
#define CLUSTER_SESSION_H_

#include "maraton.h"
#include "Session.h"
#include "Message.h"
#include <vector>
#include <functional>

class ClusterSession :
    public Session
{
public:

    typedef std::function<void( Message* )> callback_message_t;
    typedef std::function<void( ClusterSession* )> callback_session_t;

    ClusterSession( uv_tcp_t* conn );
    virtual ~ClusterSession() override;

    virtual void send( Message* message );
    virtual void on_message( callback_message_t callback );
    virtual void on_close( callback_session_t callback);

protected:

    virtual void recv( const char* data, int len ) override;
    virtual void send( const char* data, int len ) override;
    virtual void message( Message* message ) = 0;
    virtual void shutdown() override;

    virtual void dispatch_message( Message* message );
    virtual void dispatch_close();

private:

    enum ES_READSTATE
    {
        FLAG = 0,
        HEAD,
        BODY
    };

    ES_READSTATE read_state_ = ES_READSTATE::FLAG;

    CircleBuffer circle_buffer_;
    Zlib compressor_;

    int compressed_length_ = 0;
    int oringal_length_ = 0;

    bool try_read_flag();
    bool try_read_head();
    bool try_read_body();

    std::vector<callback_message_t> callback_list;
    std::vector<callback_session_t> callback_session_list;
};

#endif // !CLUSTER_SESSION_H_
