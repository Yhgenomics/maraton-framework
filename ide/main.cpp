//#include "SysProcess.h"
//#include "AsyncWorker.h"
//
//static void CB_2( SysProcess* p , size_t status )
//{
//    printf( "CB_2\r\n" );
//}
//
//
//static void CB_1( SysProcess* p , size_t status )
//{
//    printf( "CB_1\r\n" );
//    auto r = SysProcess::create( "C:\\Windows\\notepad.exe" , "D:\\test.txt" , CB_2 );
//    r->start();
//}
//
//
//int main()
//{
//
//    auto r = SysProcess::create( "C:\\Windows\\notepad.exe" , "D:\\test.txt" , CB_1 );
//    r->start();
//
//    while ( 1 )
//    {
//        uv_run( uv_default_loop() , UV_RUN_DEFAULT );
//    }
//
//    return 0;
//}