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

    typedef std::function<void( SysProcess* , size_t )> prceoss_callback_t;
    static void uv_work_process_callback( uv_work_t* req );
    static void uv_after_work_process_callback( uv_work_t* req , int status );

    static SysProcess* create( std::string  file , std::string  args , std::string  directry , prceoss_callback_t on_finish );
    static SysProcess* create( std::string  file , std::string  args , prceoss_callback_t on_finish );
    static SysProcess* create( std::string  file , prceoss_callback_t on_finish );
    static void desctroy( SysProcess** process );

    
    void* data() { return this->data_; };
    void data( void* pdata ) { this->data_ = pdata; };

    void start();
    void kill();

private:

    static void uv_process_exit_callback( uv_process_t* , int64_t exit_status , int term_signal );
    static void uv_process_close_callback( uv_handle_t* handle );

    SysProcess();
    SysProcess( std::string  file , std::string  args , std::string  directry , prceoss_callback_t on_finish );
    SysProcess( std::string  file , std::string  args , prceoss_callback_t on_finish );
    SysProcess( std::string  file , prceoss_callback_t on_finish );
    ~SysProcess();

    size_t wait_for_exit();

#ifdef _WIN32
    STARTUPINFO si_;
    PROCESS_INFORMATION pi_;
#else

    FILE* p_stream = NULL;

#endif

    prceoss_callback_t callback;

    void invoke();

    const int STR_LENGTH = 1024;

    char* file_ = NULL;
    char* args_ = NULL;;
    char* directory_ = NULL;

    char output_buffer_[10240] = { 0 };

    int result = 0;

    uv_work_t worker;

    uv_sem_t sem;

    void* data_ = nullptr;

    uv_process_t child_req;
    uv_process_options_t options = { 0 };
};

#endif // !PROCESS_H_