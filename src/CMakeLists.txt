cmake_minimum_required(VERSION 2.8)

project(leveldb-clone)

if(WIN32 AND MSVC)
  add_definitions(-DOS_WIN)
  add_definitions(-DLEVELDB_PLATFORM_WINDOWS)
  add_definitions(-DCOMPILER_MSVC)
  add_definitions(-DHAVE_SNAPPY)
  add_definitions(-DWINVER=0x0501 -DWIN32_LEAN_AND_MEAN -DNOMINMAX -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
  add_definitions(-Dssize_t=ptrdiff_t) #workaround for ssize_t
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  add_definitions(-DOS_LINUX)
  add_definitions(-DLEVELDB_PLATFORM_POSIX)
  add_definitions(-DHAVE_SNAPPY)
  add_definitions(-fno-builtin-memcmp)
else()
  error("Unknown platform")
endif()

include_directories(leveldb leveldb/include leveldb/port/win)

if(WIN32 AND MSVC)
add_executable(leveldb-clone-cpp leveldb-clone-cpp.cpp 
leveldb/db/write_batch.cc 
leveldb/db/version_set.cc
leveldb/db/version_edit.cc
leveldb/db/table_cache.cc
leveldb/db/repair.cc
leveldb/db/memtable.cc
leveldb/db/log_writer.cc
leveldb/db/log_reader.cc
leveldb/db/filename.cc
leveldb/db/dbformat.cc
leveldb/db/db_impl.cc
leveldb/db/db_iter.cc
leveldb/db/c.cc
leveldb/db/builder.cc
leveldb/table/two_level_iterator.cc
leveldb/table/table_builder.cc
leveldb/table/table.cc
leveldb/table/merger.cc
leveldb/table/iterator.cc
leveldb/table/format.cc
leveldb/table/filter_block.cc
leveldb/table/block_builder.cc
leveldb/table/block.cc
leveldb/util/status.cc
leveldb/util/options.cc
leveldb/util/logging.cc
leveldb/util/histogram.cc
leveldb/util/hash.cc
leveldb/util/filter_policy.cc
leveldb/port/port_win.cc
leveldb/util/env_win.cc
leveldb/util/env.cc
leveldb/util/crc32c.cc
leveldb/util/comparator.cc
leveldb/util/coding.cc
leveldb/util/cache.cc
leveldb/util/bloom.cc
leveldb/util/arena.cc
snappy/snappy.cc
snappy/snappy-c.cc
snappy/snappy-sinksource.cc
snappy/snappy-stubs-internal.cc
)

else()

add_executable(leveldb-clone-cpp leveldb-clone-cpp.cpp 
leveldb/db/write_batch.cc 
leveldb/db/version_set.cc
leveldb/db/version_edit.cc
leveldb/db/table_cache.cc
leveldb/db/repair.cc
leveldb/db/memtable.cc
leveldb/db/log_writer.cc
leveldb/db/log_reader.cc
leveldb/db/filename.cc
leveldb/db/dbformat.cc
leveldb/db/db_impl.cc
leveldb/db/db_iter.cc
leveldb/db/c.cc
leveldb/db/builder.cc
leveldb/table/two_level_iterator.cc
leveldb/table/table_builder.cc
leveldb/table/table.cc
leveldb/table/merger.cc
leveldb/table/iterator.cc
leveldb/table/format.cc
leveldb/table/filter_block.cc
leveldb/table/block_builder.cc
leveldb/table/block.cc
leveldb/util/status.cc
leveldb/util/options.cc
leveldb/util/logging.cc
leveldb/util/histogram.cc
leveldb/util/hash.cc
leveldb/util/filter_policy.cc
leveldb/port/port_posix.cc
leveldb/util/env_posix.cc
leveldb/util/env.cc
leveldb/util/crc32c.cc
leveldb/util/comparator.cc
leveldb/util/coding.cc
leveldb/util/cache.cc
leveldb/util/bloom.cc
leveldb/util/arena.cc
snappy/snappy.cc
snappy/snappy-c.cc
snappy/snappy-sinksource.cc
snappy/snappy-stubs-internal.cc
)

endif()