#ifndef HTTP_REQUEST_H_
#define HTTP_REQUEST_H_

#include "stdio.h"
#include <vector>
#include <map>
#include <memory.h>
#include <string>

#include "HTTP.h"

using namespace std;

class HTTPRequest
{
public:

    HTTPRequest()
    {
        reset();
    };
    ~HTTPRequest()
    {
    };

    string method()
    {
        return this->method_;
    } 

    string path()
    {
        return string( this->path_ );
    };

    map<string, string> header()
    {
        return map<string, string>( this->header_.begin(), this->header_.end() );
    }

    string body()
    {
        return string( this->body_ );
    };


    bool parse( const char* data, size_t len )
    {
        const char* phead = data;
        const char* pdata = phead;

        int body_count = 0;

        bool ret = false;

        do
        {
            switch ( state )
            {
                //header
            case HTTPPARSESTATE::METHOD: {
                if ( *pdata == ' ' )
                {
                    this->method_ = sequeue;
                    state = HTTPPARSESTATE::PATH;
                    sequeue.clear();
                    break;
                }
                add_sequeue( *pdata );
            }break;

            case HTTPPARSESTATE::PATH: {
                if ( *pdata == ' ' )
                {
                    this->path_ = sequeue;

                    //skip \n 
                    phead = pdata;
                    state = HTTPPARSESTATE::VERSION;
                    sequeue.clear();
                    break;
                }
                add_sequeue( *pdata );
            }break;

            case HTTPPARSESTATE::VERSION: {
                if ( *pdata == '\n' )
                {
                    this->version_ = sequeue;
                    state = HTTPPARSESTATE::HEADER_KEY;
                    sequeue.clear();
                    break;
                }
                add_sequeue( *pdata );
            }break;

            case HTTPPARSESTATE::HEADER_KEY: {
                if ( *pdata == ':' )
                {
                    key_ = sequeue;

                    while ( *pdata == ' ' )pdata++;

                    phead = pdata;
                    state = HTTPPARSESTATE::HEADER_VALUE;
                    sequeue.clear();
                    break;
                }

                if ( *pdata == '\n' )
                {
                    state = HTTPPARSESTATE::BODY;
                    phead = pdata;
                    ret = true;

                    string length = this->header_["Content-Length"];
                    if ( length != "" )
                    {
                        ret = false;
                        data_len = atoi( length.c_str() );
                    }
                    sequeue.clear();
                    break;
                }
                add_sequeue( *pdata );
            }break;

            case HTTPPARSESTATE::HEADER_VALUE: {
                if ( *pdata == '\n' )
                {
                    value_ = sequeue;

                    phead = pdata;
                    state = HTTPPARSESTATE::HEADER_KEY;

                    this->header_.insert( make_pair( string( key_ ), string( value_ ) ) );

                    sequeue.clear();
                    break;
                }

                add_sequeue( *pdata );
            }break;

            case HTTPPARSESTATE::BODY: {
                if ( data_len == 0 )
                {
                    ret = true;
                    break;
                }

                body_count++; 

                if ( body_count > data_len )
                {
                    this->body_ = sequeue;
                    ret = true;
                    sequeue.clear();
                    break;
                }  
                add_body( *pdata );
            }break;

            default:
            return false;
            }

            pdata++;

        } while ( ((size_t)( pdata - data )) <= len );

        return ret;
    }

    void reset()
    {
        this->state = HTTPPARSESTATE::METHOD;
        this->method_ = string();
        this->path_ = string();
        this->body_ = string();
        this->header_.clear();
        this->sequeue.clear();
        this->data_len = 0;
    }

private:

    enum HTTPPARSESTATE
    {
        METHOD= 1,
        PATH,
        VERSION,
        HEADER_KEY,
        HEADER_VALUE,
        BODY
    };

    map<string, string> header_;

    string method_;
    string path_;
    string body_;
    string version_;

    string key_, value_;

    string sequeue;

    int data_len = 0;

    HTTPPARSESTATE state = HTTPPARSESTATE::METHOD;

    void add_sequeue( char c )
    {
        if ( c != '\r' && c != '\n' && c != ' ' && c!=0 )
            this->sequeue.push_back( c );
    }
    void add_body( char c )
    {
        if ( c != '\0' )
            this->sequeue.push_back( c );
    }
};

#endif //HTTP_REQUEST_H_
