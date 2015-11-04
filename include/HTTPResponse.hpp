#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include "Buffer.hpp"
#include "stdio.h"
#include <string.h>
#include <map>
#include <memory.h>

class HTTPResponse
{
public:

    HTTPResponse()
    {
        this->content_ = new char[HTTP_RESPONSE_CONTENT_SIZE];
        this->clear();
    }

    ~HTTPResponse()
    {
        delete ( this->content_ );
    }

    void status( int status ) { status_ = status; };
    int status() { return status_; };

    void content_type( std::string type ) { header( "Content-Type", type ); };//content_type_ = type; };

    void header( std::string name, std::string value )
    {
        header_[name] = value;
    }

    void content(const char* data, int len) 
    {
        memcpy( this->content_, data, len );
        this->content_len_ = len;
    };

    void content( std::string content )
    {
        memcpy( this->content_, content.c_str(), content.length() );
        this->content_len_ = static_cast< int > ( content.length());
    };

    int length() 
    {
        return this->content_len_;
    };

    Buffer bytes() 
    {
        Buffer ret;
        int len = 0;
        char* buffer = new char[HTTP_RESPONSE_CONTENT_SIZE];
        string head = "";

        memset( buffer , 0 , HTTP_RESPONSE_CONTENT_SIZE );

        head += "HTTP/1.0 "+ this->status_description( this->status_ ) + "\r\n";
#ifdef WIN32
        sprintf_s( buffer, sizeof(buffer), "%d", this->content_len_ );
#else
        snprintf( buffer , sizeof(buffer), "%d", this->content_len_);
#endif
        
        
        header_["Content-Length"] = buffer;

        for ( auto kv : header_ )
        {
            head += kv.first + ": " + kv.second + "\r\n";
        }

        head += "\r\n";

        memcpy( buffer + len , head.c_str(), head.length() );
        len += static_cast< int >( head.length() );
        memcpy( buffer + len , this->content_ , this->content_len_ );
        len += this->content_len_;

        ret.raw( buffer, len );

        SAFE_DELETE( buffer );

        return ret;
    };

    void clear()
    {
        this->header_.clear();
        header_["Server"] = "YHGenomics/Maraton";
        header_["Content-Length"] = "0";
        header_["Connection"] = "close";

        if ( this->content_ == nullptr )
        {
            this->content_ = new char[1024 * 1024];
        }

        memset( this->content_, 0, 1024 * 1024 );
    };

private:

    map<string, string> header_;
    int status_ = 200;
    std::string content_type_ = "application/json";

    char* content_ = nullptr;
    int content_len_ = 0;

    std::string status_description( int status )
    {
        switch ( status )
        {
        case 200:
            return "200 OK";
            break;

        case 404:
            return "404 Not Found";
            break;

        default:
            break;
        }

        return "404 Not Found";
    }
};

#endif // !HTTP_RESPONSE_H_
