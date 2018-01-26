@del leveldb-clone-cpp.exe
@del *.obj
AStyle.exe leveldb-clone-cpp.cpp
@del *.cpp.orig

@call "%VS140COMNTOOLS%\..\..\VC\bin\vcvars32.bat"

@rem https://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx

cl /MT /I ".\leveldb" /I ".\leveldb\include" /I ".\leveldb\util" /I ".\leveldb\table" /I ".\leveldb\db" /I ".\leveldb\port\win" /I ".\snappy" /D "ssize_t=ptrdiff_t" /D _CONSOLE /D HAVE_SNAPPY /D LEVELDB_PLATFORM_WINDOWS /D WINVER=0x0501 /D WIN32_LEAN_AND_MEAN /D NOMINMAX /D _CRT_SECURE_NO_WARNINGS /D _CRT_NONSTDC_NO_WARNINGS /W3 /Oi /EHsc leveldb-clone-cpp.cpp @leveldb-files -Feleveldb-clone-cpp.exe

@del *.obj

@rem /I \leveldb 
@rem 
@rem  ./leveldb/util;./leveldb/table;./leveldb/db"
