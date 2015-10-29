/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef LOGER_H_
#define LOGER_H_ 

#include "stdio.h"
#include <stdarg.h>
#include "Singleton.hpp"

class Logger
{
public:

    static void sys( const char* fmt, ... )
    {
        va_list args;

#ifdef WIN32
        printf( "\033[1;33m[SYS]" );
#else
        printf( "[SYS]" );
#endif
        va_start( args, fmt );
        printf( fmt, *args );
        va_end( args );

#ifdef WIN32
        printf( "\033[0m\r\n" );
#else
        printf( "\r\n" );
#endif      

    }

    static void error( const char* fmt, ... )
    {
        va_list args;

#ifdef WIN32
        printf( "\033[0;31m[ERROR]" );
#else
        printf( "[ERROR]" );
#endif

        va_start( args, fmt );
        printf( fmt, *args );
        va_end( args );

#ifdef WIN32
        printf( "\033[0;31m\r\n" );
#else
        printf( "\r\n" );
#endif    
    }

private:

    Logger() {};
    ~Logger() {};
};

#endif //LOGER_H_ 