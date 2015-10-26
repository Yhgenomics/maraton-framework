/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef NETWORK_DATA_BUILDER_H_
#define NETWORK_DATA_BUILDER_H_

#include "Macros.h"
#include <memory.h>

// in some cases two tcp message will be combined into one or chopped
// this class will solve the problem
class CircleBuffer
{
public:
     
    CircleBuffer()
    { 
        this->circle_buffer_ = new char[this->buffer_length_];
     
        this->available_len_ = this->buffer_length_;
        this->used_len_ = 0;
    }

    ~CircleBuffer()
    {
        if ( this->circle_buffer_ != nullptr )
        {
            delete this->circle_buffer_;
            this->circle_buffer_ = nullptr;
        }
    }
   
    virtual bool push( const char* data, int len )
    {
        if ( available_length() < len )
        {
            throw "[CircleBuffer.cpp] not enough buffer";
        }

        if ( tail_ + len > this->buffer_length_ )
        {
            char* ptail = this->circle_buffer_ + this->tail_;
            int delta_len = this->buffer_length_ - tail_;
            memcpy( ptail, data, delta_len );
            memcpy( this->circle_buffer_, data + delta_len, len - delta_len );
        }
        else
        {
            char* ptail = this->circle_buffer_ + this->tail_;

            memcpy( ptail, data, len );
        }

        this->tail_ = ( this->tail_ + len ) % this->buffer_length_;

        this->used_len_ += len;

        this->available_len_ = this->buffer_length_ - this->used_len_;

        return true;
    }

    virtual char* pop( int len )
    {
        if ( len > this->used_length() )
        {
            return nullptr;
        }

        char* result = new char[len];
        int result_pos = 0;
        char* phead = this->circle_buffer_ + this->head_;

        // there are two pices data
        // one is at end of circle buffer
        // antoher is at front of circle buffer
        if ( ( this->head_ + len ) > this->buffer_length_ )
        {
            int end_len = this->buffer_length_ - this->head_;
            memcpy( result, phead + result_pos, end_len );
            result_pos += this->buffer_length_ - this->head_;
            memcpy( result + result_pos, this->circle_buffer_, ( len - end_len ) );
        }
        // circle buffer contains all data inside
        else
        {
            memcpy( result, phead + result_pos, len );
        }

        this->head_ = ( this->head_ + len ) % this->buffer_length_;

        this->used_len_ -= len;

        this->available_len_ = this->buffer_length_ - this->used_len_;

        return result;
    }

    int length() { return this->buffer_length_; };
    int used_length() { return this->used_len_; };
    int available_length() { return this->available_len_; };

    int tail_ = 0;
    int head_ = 0;

private:

    CircleBuffer( CircleBuffer & ) = delete;
    CircleBuffer( CircleBuffer && ) = delete;
    CircleBuffer operator=( CircleBuffer ) = delete;

    int used_len_ = 0;
    int available_len_ = 0;

    const int buffer_length_ = CIRCLE_BUFFER_SIZE;

    char* circle_buffer_ = nullptr;
};

#endif // !NETWORK_DATA_BUILDER_H_
