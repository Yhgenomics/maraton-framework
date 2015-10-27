/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-16 13:21
* Description:
* * * * * * * * * * * * * * * */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <functional>
#include <vector>

#include <uv.h>
#include <time.h>


class Timer
{
public:

    static int tick()
    {
        clock_t t = clock();
        return static_cast< int >( t );
    };
     
};

#endif // !TIMER_HPP_
