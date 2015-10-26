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

        printf( "\033[1;33m[SYS]" );
        
        va_start( args, fmt );
        printf( fmt, *args );
        va_end( args );
        
        printf( "\033[0m\r\n" );
    }

    static void error( const char* fmt, ...)
    {
        va_list args;
        
        printf( "\033[0;31m[ERROR]" );
        
        va_start( args, fmt );
        printf( fmt, *args );
        va_end( args );
        
        printf( "\033[0m\r\n" );
    }

private:
    
    Logger() {};
    ~Logger() {};
};

#endif //LOGER_H_ 