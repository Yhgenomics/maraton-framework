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

void SysProcess::uv_work_process_callback( uv_work_t * req )
{
    SysProcess* instance = static_cast< SysProcess* >( req->data );

#ifdef _WIN32

    instance->pi_ = { };
    instance->si_ = { sizeof( instance->si_ ) };

    if ( !CreateProcess(
        instance->file_ ,
        instance->args_ ,
        NULL ,
        NULL ,
        FALSE ,
        CREATE_NO_WINDOW ,
        NULL ,
        instance->directory_ ,
        &instance->si_ ,
        &instance->pi_ ) )
    {
        instance->result = GetLastError();
    }

    WaitForSingleObject( instance->pi_.hProcess , INFINITE );
    DWORD dwExitCode;
    GetExitCodeProcess( instance->pi_.hProcess , &dwExitCode );
    instance->result = ( size_t )dwExitCode;
    CloseHandle( instance->pi_.hThread );
    CloseHandle( instance->pi_.hProcess );

    memset( &instance->si_ , 0 , sizeof( instance->si_ ) );
    memset( &instance->pi_ , 0 , sizeof( instance->pi_ ) );

     
  
#else
 
    char tmp_buffer[10240] = { 0 };
    int file_length = strlen( instance->file_ );
    int args_length = strlen( instance->args_ );
    memcpy( tmp_buffer , instance->file_ , file_length );
    memcpy( tmp_buffer + file_length , " " , 1 );
    memcpy( tmp_buffer + file_length + 1 , instance->args_ , args_length );
     
    instance->p_stream = popen( tmp_buffer , "r" );

    pread( instance->output_buffer  , sizeof( char ) , sizeof( instance->output_buffer ), instance->p_stream );

    pclose( instance->p_stream );

#endif // _WIN32

}

void SysProcess::uv_after_work_process_callback( uv_work_t * req , int status )
{
    SysProcess* instance = static_cast< SysProcess* >( req->data );

    instance->callback( instance->result );

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
    this->worker->data = this;
    uv_queue_work( uv_default_loop() , this->worker , SysProcess::uv_work_process_callback , SysProcess::uv_after_work_process_callback );
}
void SysProcess::wait_for_exit()
{
    
    if ( result != 0 )
    {
        this->callback( result );
        return;
    }
     
    this->callback( result );
}

void SysProcess::kill()
{
    uv_cancel( ( uv_req_t*) this->worker );

#ifdef _WIN32
    
    TerminateProcess( this->pi_.hProcess, -1 );

#else

    pclose( p_stream );

#endif // _WIN32   
}

