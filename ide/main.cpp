//#include "SysProcess.h"
//#include "AsyncWorker.h"
//#include "SyncWorker.h"
//
//static void CB_2( SysProcess* p , size_t status )
//{
//    printf( "CB_2\r\n" );
//}
//
//
//static void CB_1( SysProcess* p , size_t status )
//{
//    printf( "CB_1 %s \r\n" , p->std_out().c_str() );
//    auto r = SysProcess::create( "C:\\Windows\\notepad.exe" , "D:\\test.txt" , CB_2 );
//    r->start();
//}
//
//
//int main()
//{
//    
//    
// 
//    auto r = SysProcess::create( "/bin/sh" ,"-c 'adef efa efef adfasd efrefe aaa'", CB_1 );
//    r->start();
//
//    while ( 1 )
//    {
//        
//        uv_run( uv_default_loop() , UV_RUN_DEFAULT );
//    }
//
//    return 0;
//}