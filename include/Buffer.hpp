/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef BYTES_HPP_
#define BYTES_HPP_

#include "Macros.h"

class Buffer
{
public:

    Buffer() {};

    ~Buffer()
    {
        SAFE_DELETE( this->raw_ );
    };

    Buffer( const char* data, int len )
    {
        this->raw( data, len );
    };

    Buffer(const Buffer & buffer )
    {
        this->raw( buffer.raw_, buffer.length_ );
    };

    Buffer( Buffer&& buffer )
    {
        SAFE_DELETE( this->raw_ );
        
        this->raw_ = buffer.raw_;
        this->length_ = buffer.length_;

        buffer.raw_ = nullptr;
        buffer.length_ = 0;
    }

    Buffer& operator=(const Buffer& buffer )
    {
        this->raw( buffer.raw_, buffer.length_ );

        return *this;
    }

    Buffer& operator=( Buffer&& buffer )
    {
        SAFE_DELETE( this->raw_ );
        
        this->raw_ = buffer.raw_;
        this->length_ = buffer.length_;

        buffer.raw_ = nullptr;
        buffer.length_ = 0;

        return *this;
    }

    void raw(const char* data , int length) 
    {
        SAFE_DELETE( this->raw_ );
        this->raw_ = new char[length];
        this->length_ = length;
        memcpy( this->raw_, data, length );
    }
    
    char* raw() { return this->raw_; };
    int length() { return this->length_; };

private:

    char* raw_ = nullptr;
    int length_ = 0;
};


#endif // !BYTES_HPP_
