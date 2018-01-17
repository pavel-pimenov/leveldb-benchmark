package main

import (
	"fmt"
	"github.com/syndtr/goleveldb/leveldb"
	"runtime"
)

var db *leveldb.DB
var db2 *leveldb.DB

func printMemStats() {
	var m runtime.MemStats
	runtime.ReadMemStats(&m)
	fmt.Printf("sys: %d, alloc: %d, idle: %d\n", m.HeapSys,
		m.HeapAlloc, m.HeapIdle)
}
func main() {
	var err error
	printMemStats()
	db, err := leveldb.OpenFile("tth-history.leveldb", nil)
	if err != nil {
		panic(err)
	}
	printMemStats()
	db2, err2 := leveldb.OpenFile("tth-history.leveldb-new", nil)
	if err2 != nil {
		panic(err2)
	}
	printMemStats()
	iter := db.NewIterator(nil, nil)
	var i = 0
	for iter.Next() {
		//err = db2.Put(iter.Key(), iter.Value(), nil)
		if err != nil {
			panic(err)
		}
		i++
		if i%10000 == 0 {
			fmt.Printf("copy %d record\n", i)
			printMemStats()
		}
	}
	iter.Release()
	err = iter.Error()
	fmt.Println("stop level-db count = %d", i)
	defer db.Close()
	defer db2.Close()
}
