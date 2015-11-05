#ifndef PRCOESS_H_
#define PRCOESS_H_

#include <stdio.h>
#include <functional>
#include <string>

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

    void wait_for_exit();
    void kill();
    
private:

#ifdef _WIN32
    STARTUPINFO si_;
    PROCESS_INFORMATION pi_;
#else

#endif

    std::function<void( size_t )> callback;

    void invoke();

    const int STR_LENGTH = 1024;

    char* file_ = NULL;
    char* args_ = NULL;;
    char* directory_ = NULL;

    int result = 0;
};

#endif // !PROCESS_H_