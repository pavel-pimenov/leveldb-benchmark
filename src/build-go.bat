@del leveldb-clone-go.exe
@rem go get github.com/syndtr/goleveldb/leveldb
gofmt -w leveldb-clone-go.go
SET GOARCH=386
go build leveldb-clone-go.go
rem go build -ldflags "-s -w" leveldb-clone.go
