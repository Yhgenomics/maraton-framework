#ifndef PRCOESS_H_
#define PRCOESS_H_

#include <stdio.h>
#include <functional>
#include <string>
#include "uv.h"

#ifdef _WIN32
#include <windows.h>
#else

#endif

class SysProcess
{
public:

    SysProcess();
    SysProcess( std::string  file, std::string  args, std::string  directry, std::function<void(size_t)> on_finish );
    SysProcess( std::string  file, std::string  args, std::function<void( size_t )> on_finish );
    SysProcess( std::string  file, std::function<void( size_t )> on_finish );
    ~SysProcess();

    static void uv_work_process_callback( uv_work_t* req );
    static void uv_after_work_process_callback( uv_work_t* req , int status );

    void wait_for_exit();
    void kill();
    
private:

#ifdef _WIN32
    STARTUPINFO si_;
    PROCESS_INFORMATION pi_;
#else

    FILE* p_stream = NULL;

#endif

    std::function<void( size_t )> callback;

    void invoke();

    const int STR_LENGTH = 1024;

    char* file_ = NULL;
    char* args_ = NULL;;
    char* directory_ = NULL;

    char output_buffer_[10240] = { 0 };

    int result = 0;

    uv_work_t* worker;
};

#endif // !PROCESS_H_