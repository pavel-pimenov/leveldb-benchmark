@del lveldb-clone.exe
@del *.obj

@call "%VS140COMNTOOLS%\..\..\VC\bin\vcvars32.bat"

@rem https://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx

cl /MT /I ".\leveldb" /I ".\leveldb\include" /I ".\leveldb\util" /I ".\leveldb\table" /I ".\leveldb\db" /I ".\leveldb\port\win" /D _CONSOLE /D LEVELDB_PLATFORM_WINDOWS /W4 /O2 leveldb-clone.cpp @leveldb-files -Felveldb-clone.exe
@del *.obj

@rem /I \leveldb 
@rem 
@rem  ./leveldb/util;./leveldb/table;./leveldb/db"