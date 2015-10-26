#ifndef HTTP_ROUTER_H_
#define HTTP_ROUTER_H_

#include <map>
#include <string.h>
#include <vector>

#include "HTTP.h"
#include "HTTPRequest.hpp"
#include "HTTPResponse.hpp"
#include <functional>

class HTTPRouter
{
private:  

    typedef std::function<void( HTTPRequest* req, HTTPResponse* rep )> router_req_rep_cb;
    typedef std::function<void( HTTPResponse* rep )> router_rep_cb;

    struct router_path
    {
        std::string path;
        std::string method;
        router_req_rep_cb callback;
    };

    std::vector<router_path*> routers_;

    HTTPRequest* request_;
    router_rep_cb response_callback_ = nullptr;

    void add_cb( string path, std::string method, router_req_rep_cb cb )
    {
        router_path* rp = new router_path();
        rp->callback = cb;
        rp->path = path;
        rp->method = method;

        for ( auto p : routers_ )
        {
            if ( p->method == method && p->path == path )
            {
                throw "dumplicated route!";
            }
        }

        routers_.push_back( rp );
    };

    void dispatch(HTTPRequest* req)
    {
        bool exist = false;

        for ( auto p : routers_ )
        {

            if ( p->method == req->method() && p->path == req->path() )
            {
                HTTPResponse* rep = new HTTPResponse();
                rep->status( 200 );
                p->callback( req, rep );

                if( this->response_callback_!=nullptr )
                    this->response_callback_( rep );

                delete rep;
                exist = true;
            }

        }

        if ( !exist )
        {
            HTTPResponse* rep = new HTTPResponse();

            rep->status( 404 );
            this->response_callback_( rep );

            delete rep;
        }

    };
   
public:

    HTTPRouter()
    {
        this->request_ = new HTTPRequest();
    }

    ~HTTPRouter()
    {
        delete this->request_;

        for ( auto p : routers_ )
        {
            delete p;
            p = nullptr;
        }

        routers_.clear();
    }

    void parse( const char* data, int len )
    { 
        if ( request_->parse( data, len ) )
        {
            this->dispatch( request_ );
        }
    }

    void response( router_rep_cb cb ) 
    { 
        this->response_callback_ = cb; 
    };

    void get( string path, router_req_rep_cb cb)
    {
        this->add_cb( path, "GET", cb );
    }

    void post( string path, router_req_rep_cb cb )
    {
        this->add_cb( path, "POST", cb );
    }
};

#endif // !HTTP_ROUTER_H_
