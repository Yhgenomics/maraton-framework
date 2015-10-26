/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */

#ifndef TYPES_H_
#define TYPES_H_ 

#include "uv.h"
#include "Buffer.hpp"


struct UV_SEND_DATA
{
    Buffer* data;
    uv_stream_t* stream;
    uv_sem_t* signal;
    uv_loop_t* loop;
};

#endif //!TYPES_H_ 