package main

import (
	"fmt"
	"github.com/syndtr/goleveldb/leveldb"
	"runtime"
	"time"
)

var db_old *leveldb.DB
var db_new *leveldb.DB

func timeTrack(start time.Time, name string) {
	elapsed := time.Since(start)
	fmt.Printf("%s duration: %s\n", name, elapsed)
}
func printMemStats() {
	var m runtime.MemStats
	runtime.ReadMemStats(&m)
	//	fmt.Printf("sys: %d, alloc: %d, idle: %d\n", m.HeapSys,
	//		m.HeapAlloc, m.HeapIdle)
}
func main() {
	start := time.Now()
	var err error
	printMemStats()
	db_old, err := leveldb.OpenFile("tth-history-old.leveldb", nil)
	if err != nil {
		panic(err)
	}
	timeTrack(start, "open tth-history-old.leveldb")
	printMemStats()
	start = time.Now()
	db_new, err2 := leveldb.OpenFile("tth-history.leveldb", nil)
	if err2 != nil {
		panic(err2)
	}
	printMemStats()
	timeTrack(start, "open tth-history.leveldb")
	start = time.Now()
	iter := db_old.NewIterator(nil, nil)
	var i = 0
	for iter.Next() {
		err = db_new.Put(iter.Key(), iter.Value(), nil)
		if err != nil {
			panic(err)
		}
		i++
		if i%100000 == 0 {
			fmt.Printf("copy %d record\n", i)
			printMemStats()
		}
	}
	iter.Release()
	err = iter.Error()
	fmt.Printf("stop level-db count = %d\n", i)
	timeTrack(start, "end scan")
	defer db_old.Close()
	defer db_new.Close()
}
