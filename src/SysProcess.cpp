#include "SysProcess.h"
#include <thread>
#include <string.h>
#include <memory>


SysProcess::~SysProcess()
{
    if ( this->file_ != NULL )
    {
        delete this->file_;
        this->file_ = NULL;
    }

    if ( this->args_ != NULL )
    {
        delete this->args_;
        this->args_ = NULL;
    }

    if ( this->directory_ != NULL )
    {
        delete this->directory_;
        this->directory_ = NULL;
    }
}

SysProcess::SysProcess()
{
    
}

SysProcess::SysProcess( std::string  file, std::string  args, std::string  directry, std::function<void( size_t )> on_finish )
    : SysProcess()
{
    this->file_ = new char[this->STR_LENGTH];
    this->args_ = new char[this->STR_LENGTH];
    this->directory_ = new char[this->STR_LENGTH];
    memset( this->file_, 0, this->STR_LENGTH );
    memset( this->args_, 0, this->STR_LENGTH );
    memset( this->directory_, 0, this->STR_LENGTH );

    memcpy( this->file_, file.c_str() , file.length() );
    memcpy( this->args_, args.c_str(), args.length() );
    memcpy( this->directory_, directry.c_str(), directry.length() );
    this->callback = on_finish;
    this->invoke();
}

SysProcess::SysProcess( std::string  file, std::string  args, std::function<void( size_t )> on_finish )
    : SysProcess()
{
    this->file_ = new char[this->STR_LENGTH];
    this->args_ = new char[this->STR_LENGTH]; 
    memset( this->file_, 0, this->STR_LENGTH );
    memset( this->args_, 0, this->STR_LENGTH ); 

    memcpy( this->file_, file.c_str(), file.length() );
    memcpy( this->args_, args.c_str(), args.length() ); 
    this->callback = on_finish;
    this->invoke();
}

SysProcess::SysProcess( std::string  file, std::function<void( size_t )> on_finish )
    : SysProcess()
{
    this->file_ = new char[this->STR_LENGTH]; 
    memset( this->file_, 0, this->STR_LENGTH );

    memcpy( this->file_, file.c_str(), file.length() ); 
    this->callback = on_finish;
    this->invoke();
}

void SysProcess::invoke()
{
#ifdef _WIN32

    this->pi_ = { };
    this->si_ = { sizeof( this->si_ ) };

    if ( !CreateProcess( 
        this->file_, 
        this->args_, 
        NULL, 
        NULL, 
        FALSE, 
        CREATE_NO_WINDOW, 
        NULL, 
        this->directory_,
        &this->si_,
        &this->pi_ ) )
    {
        result = GetLastError();
    }

#else

#endif // _WIN32
}
void SysProcess::wait_for_exit()
{
    if ( result != 0 )
    {
        this->callback( result );
        return;
    }

#ifdef _WIN32

    WaitForSingleObject( this->pi_.hProcess, INFINITE );
    DWORD dwExitCode;
    GetExitCodeProcess( this->pi_.hProcess, &dwExitCode );
    result = ( size_t ) dwExitCode;
    CloseHandle( this->pi_.hThread );
    CloseHandle( this->pi_.hProcess );


    memset( &this->si_, 0, sizeof( this->si_ ) );
    memset( &this->pi_, 0, sizeof( this->pi_ ) );

#else

#endif // _WIN32

    this->callback( result );
}

void SysProcess::kill()
{
#ifdef _WIN32

    TerminateProcess( this->pi_.hProcess, -1 );

#else

#endif // _WIN32   
}

