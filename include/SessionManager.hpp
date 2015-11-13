/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */

#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_ 

#include "Manager.hpp"
#include "Session.h"
#include "maraton.h"
#include <functional>
#include <vector>

template<class TT>
class SessionManager :
    public Manager<TT>,
    public Singleton<SessionManager<TT>>
{
public:

    typedef std::function<void( TT* )> callback_t;
     
    TT* create( uv_tcp_t* conn )
    {
        // Potential memory leaks
        //unique_ptr<TT> t;
        //shared_ptr<TT>
        TT* t = new TT( conn );
        this->push( t );
        invoke_callbacK( t );
        return t;
    };

    bool remove( TT*  t )
    {
        return this->pop( t );
    };

    TT* find( int session_id )
    {
        auto elements = Manager<TT>::instances();
        for ( auto s : elements )
        {
            auto session = static_cast< Session* >( s );
            if ( session->id() == session_id )
            {
                return static_cast< TT* >( session );
            }
        }

        return nullptr;
    };

    void on_create( callback_t callback )
    {
        callbacks.push_back( callback );
    };

private:


    friend Singleton<SessionManager<TT>>;
    
    std::vector<callback_t> callbacks;

    void invoke_callbacK( TT* instance )
    {
        for ( auto cb : callbacks )
        {
            cb( instance );
        }
    };

};

#endif //!SESSION_MANAGER_H_ 