#!/bin/sh
rm -rf cmakebuild/
CMaker project:maraton out:lib head:*.h,*.hpp src:*.cpp,*.hpp,*.cc,*.c flag:-Wall-std=c++11-pthread lib:../lib/libuv.a
cd cmakebuild
cmake .
make -j 4
cp libmaraton.a ../lib/maraton-framework.a
rm libmaraton.a
cd ../
