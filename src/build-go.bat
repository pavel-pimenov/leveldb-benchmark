@del leveldb-clone.exe
@rem go get github.com/syndtr/goleveldb/leveldb

SET GOARCH=386
go build leveldb-clone.go

