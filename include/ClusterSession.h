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

    typedef std::function<void( Message* )> callback_t;

    ClusterSession( uv_tcp_t* conn );
    virtual ~ClusterSession() override;

    void send( Message* message );
    virtual void on_message( callback_t callback );

protected:

    void recv( const char* data, int len ) override;
    void send( const char* data, int len ) override;
    virtual void message( Message* message ) = 0;

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
    void invoke_message( std::string json );

    std::vector<callback_t> callback_list;
};

#endif // !CLUSTER_SESSION_H_
