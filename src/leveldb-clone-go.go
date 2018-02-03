package main

import (
	"fmt"
	"github.com/syndtr/goleveldb/leveldb"
	"github.com/syndtr/goleveldb/leveldb/opt"
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
	db_old, err := leveldb.OpenFile("tth-history.leveldb", nil)
	if err != nil {
		panic(err)
	}
	timeTrack(start, "open tth-history.leveldb")
	printMemStats()
	start = time.Now()
	//	o := &opt.Options{
	//		Compression: NoCompression,
	//	}
	opt.DefaultCompressionType = opt.NoCompression
	//opt.DefaultCompressionType = opt.SnappyCompression
	db_new, err2 := leveldb.OpenFile("tth-history-go.leveldb", nil)
	if err2 != nil {
		panic(err2)
	}
	printMemStats()
	timeTrack(start, "open tth-history-go.leveldb")
	start = time.Now()
	iter := db_old.NewIterator(nil, nil)
	var i = 0
	var skip = 0
	var copy = 0
	for iter.Next() {
		if i%100000 == 0 {
			fmt.Printf("count Go =  %d\n", i)
			printMemStats()
		}
		i++
		var is_exist bool
		is_exist, err = db_new.Has(iter.Key(), nil)
		if err != nil {
			panic(err)
		}
		if is_exist == false {
			err = db_new.Put(iter.Key(), iter.Value(), nil)
			if err != nil {
				panic(err)
			}
			copy++
			continue
		}
		skip++
	}
	iter.Release()
	err = iter.Error()
	fmt.Printf("stop level-db count = %d copy %d skip = %d\n", i, copy, skip)
	timeTrack(start, "end scan")
	defer db_old.Close()
	defer db_new.Close()
}
