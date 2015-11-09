#include "SysProcess.h"
#include <thread>
#include <string.h>
#include <memory>
#include <thread>

#include "maraton.h"

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
 
void SysProcess::desctroy( SysProcess ** process )
{
    if ( process != nullptr && *process != nullptr )
    {
        delete *process;
        *process = nullptr;
    }
}

SysProcess * SysProcess::create( std::string file , std::string args , std::string directry , prceoss_callback_t on_finish )
{
    auto ret = new SysProcess( file , args , directry , on_finish );
    return ret;
}

SysProcess * SysProcess::create( std::string file , std::string args , prceoss_callback_t on_finish )
{
    auto ret = new SysProcess( file , args ,  on_finish );
    return ret;
}

SysProcess * SysProcess::create( std::string file , prceoss_callback_t on_finish )
{
    auto ret = new SysProcess( file , on_finish );
    return ret;
}

void SysProcess::uv_work_process_callback( uv_work_t * req )
{
    SysProcess* instance = static_cast< SysProcess* >( req->data );

#ifdef _WIN32

    instance->pi_ = { };
    instance->si_ = { sizeof( instance->si_ ) };
    instance->si_.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    instance->si_.wShowWindow = SW_HIDE; 
   
    //UUID uuid;
    //UuidCreate( &uuid );
    //char uuid_str[128] = { 0 };
    //sprintf( uuid_str , "%08X%08X%08X%08X" , uuid.Data1 , uuid.Data2 , uuid.Data3 , uuid.Data4 );
    //HANDLE hConsoleRedirect = CreateFile(
    //    uuid_str ,
    //    GENERIC_WRITE ,
    //    FILE_SHARE_READ | FILE_SHARE_WRITE ,
    //    NULL ,
    //    OPEN_ALWAYS ,
    //    FILE_ATTRIBUTE_NORMAL ,
    //    NULL
    //    );
    //instance->si_.hStdOutput = hConsoleRedirect;

    if ( !CreateProcess(
        instance->file_ ,
        instance->args_ ,
        NULL ,
        NULL ,
        FALSE ,
        CREATE_NO_WINDOW | CREATE_NEW_CONSOLE ,
        NULL ,
        instance->directory_ ,
        &instance->si_ ,
        &instance->pi_ ) )
    {
        instance->result = GetLastError();
        uv_sem_post( &instance->sem );
        return;
    }  

    WaitForSingleObject( instance->pi_.hProcess , INFINITE );
    DWORD dwExitCode;
    GetExitCodeProcess( instance->pi_.hProcess , &dwExitCode );
    instance->result = ( size_t )dwExitCode;
    CloseHandle( instance->pi_.hThread );
    CloseHandle( instance->pi_.hProcess );

    memset( &instance->si_ , 0 , sizeof( instance->si_ ) );
    memset( &instance->pi_ , 0 , sizeof( instance->pi_ ) );

    //DeleteFile( uuid_str );
    //FILE* io_file;

#else
 
    char tmp_buffer[10240] = { 0 };
    int file_length = strlen( instance->file_ );
    int args_length = strlen( instance->args_ );
    memcpy( tmp_buffer , instance->file_ , file_length );
    memcpy( tmp_buffer + file_length , " " , 1 );
    memcpy( tmp_buffer + file_length + 1 , instance->args_ , args_length );
     
    instance->p_stream = popen( tmp_buffer , "r" );

    fread( instance->output_buffer_  , sizeof( char ) , sizeof( instance->output_buffer_ ), instance->p_stream );

    pclose( instance->p_stream );

#endif // _WIN32

    uv_sem_post( &instance->sem );

}

void SysProcess::uv_after_work_process_callback( uv_work_t * req , int status )
{
    SysProcess* instance = static_cast< SysProcess* >( req->data );

    if ( instance!=nullptr && instance->callback != nullptr )
        instance->callback( instance , instance->result ); 

    SysProcess::desctroy( &instance ); 
}

SysProcess::SysProcess()
{
    uv_sem_init( &this->sem , 0 );
}

SysProcess::SysProcess( std::string  file, std::string  args, std::string  directry, prceoss_callback_t on_finish )
    : SysProcess()
{
    std::string newArgs = " " + args;

    this->file_ = new char[this->STR_LENGTH];
    this->args_ = new char[this->STR_LENGTH];
    this->directory_ = new char[this->STR_LENGTH];
    memset( this->file_, 0, this->STR_LENGTH );
    memset( this->args_, 0, this->STR_LENGTH );
    memset( this->directory_, 0, this->STR_LENGTH );

    memcpy( this->file_, file.c_str() , file.length() );
    memcpy( this->args_, newArgs.c_str(), newArgs.length() );
    memcpy( this->directory_, directry.c_str(), directry.length() );
    this->callback = on_finish;
    this->invoke();
}

SysProcess::SysProcess( std::string  file, std::string  args, prceoss_callback_t on_finish )
    : SysProcess()
{
    std::string newArgs = " " + args;

    this->file_ = new char[this->STR_LENGTH];
    this->args_ = new char[this->STR_LENGTH]; 
    memset( this->file_, 0, this->STR_LENGTH );
    memset( this->args_, 0, this->STR_LENGTH ); 

    memcpy( this->file_, file.c_str(), file.length() );
    memcpy( this->args_, newArgs.c_str(), newArgs.length() );

    this->callback = on_finish;
    this->invoke();
}

SysProcess::SysProcess( std::string  file, prceoss_callback_t on_finish )
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
    this->worker.data = this;
    uv_queue_work( uv_default_loop() , &this->worker , SysProcess::uv_work_process_callback , SysProcess::uv_after_work_process_callback );
}

size_t SysProcess::wait_for_exit()
{ 
    uv_sem_wait( &this->sem );
    uv_sem_destroy( &this->sem );
    return this->result;
}

void SysProcess::kill()
{

    uv_cancel( ( uv_req_t*) &this->worker );

#ifdef _WIN32
    
    TerminateProcess( this->pi_.hProcess, -1 );

#else

    pclose( p_stream );

#endif // _WIN32   
}

