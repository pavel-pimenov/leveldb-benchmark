@del leveldb-clone.exe
@rem go get github.com/syndtr/goleveldb/leveldb
gofmt -w leveldb-clone.go
SET GOARCH=386
go build leveldb-clone.go
