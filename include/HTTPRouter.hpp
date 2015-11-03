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
public:

    typedef std::function<void( HTTPRequest* req )> callback_request_t;
    typedef std::function<void( HTTPResponse* rep )> callback_response_t;
    typedef std::function<void( HTTPRequest* req, HTTPResponse* rep )> callback_request_response_t;

    HTTPRouter()
    {
        this->request_ = new HTTPRequest();
    }

    ~HTTPRouter()
    {
        delete this->request_;

        for ( auto p : request_data_list_ )
        {
            delete p;
            p = nullptr;
        }

        request_data_list_.clear();
    }

    bool parse( const char* data, int len )
    { 

        if ( request_->parse( data, len ) )
        {

            for ( auto callback : this->callback_requst_t_list_ )
            {
                callback( request_ );
            }

            this->dispatch( request_ );

            return true;
        }

        return false;
    }

    void request( callback_request_t  cb )
    {
        this->callback_requst_t_list_.push_back( cb );
    };
        
    void response( callback_response_t cb )
    { 
        this->response_callback_ = cb; 
    };

    void get( string path, callback_request_response_t cb)
    {
        this->add_on_request_cb( path, "GET", cb );
    }

    void post( string path, callback_request_response_t cb )
    {
        this->add_on_request_cb( path, "POST", cb );
    }


private:

    struct RequestData
    {
        std::string path;
        std::string method;
        callback_request_response_t callback;
    };

    std::vector<callback_request_t> callback_requst_t_list_;
    std::vector<RequestData*> request_data_list_;

    HTTPRequest* request_;
    callback_response_t response_callback_ = nullptr;

    void add_on_request_cb( string path, std::string method, callback_request_response_t cb )
    {
        RequestData* rp = new RequestData();
        rp->callback = cb;
        rp->path = path;
        rp->method = method;

        for ( auto p : request_data_list_ )
        {
            if ( p->method == method && p->path == path )
            {
                throw "dumplicated route!";
            }
        }

        request_data_list_.push_back( rp );
    };

    void dispatch( HTTPRequest* req )
    {
        bool exist = false;

        for ( auto p : request_data_list_ )
        {

            if ( p->method == req->method() && p->path == req->path() )
            {
                //std::unique_ptr<HTTPResponse> ptr_rep;
                HTTPResponse rep;
                rep.status( 200 );
                p->callback( req, &rep );

                if ( this->response_callback_ != nullptr )
                    this->response_callback_( &rep );
                 
                exist = true;
            }

        }

        if ( !exist )
        {
           
            if ( this->response_callback_ != nullptr )
            {
                HTTPResponse rep;
                rep.status( 404 );
                this->response_callback_( &rep );
            }

        }

    };

};

#endif // !HTTP_ROUTER_H_
