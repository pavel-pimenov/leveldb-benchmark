rem wget http://51.254.84.24/leveldb/tth-history.leveldb.zip
rem winrar x tth-history.leveldb.zip
call build-go.bat
call build-visualc-2015.bat
leveldb-clone-cpp.exe
leveldb-clone-go.exe
