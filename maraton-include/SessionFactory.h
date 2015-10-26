/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-16 14:24
* Description:
* * * * * * * * * * * * * * * */
#ifndef SESSION_FACTORY_H_
#define SESSION_FACTORY_H_ 

#include "Session.h"
#include "uv.h"
#include "maraton.h"

class SessionFactory
    : public Singleton<SessionFactory>
{
public:

    Session* create( uv_tcp_t* conn, int port );

private:

    friend Singleton<SessionFactory>;

    SessionFactory();
    ~SessionFactory();
};

#endif //MASTER_SESSION_FACTORY_H_ 