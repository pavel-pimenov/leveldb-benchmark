#define WINVER _WIN32_WINNT_WINXP
#define _WIN32_WINNT _WIN32_WINNT_WINXP
#define NTDDI_VERSION NTDDI_WINXP

#include <string.h>
#include "leveldb/db.h"
#include <iostream>
#include <chrono>

class CElapsedSeconds
{
public:
    std::chrono::time_point<std::chrono::system_clock> m_start;
    std::string m_caption;
    unsigned m_pos;
public:
    CElapsedSeconds(const std::string& p_caption) : m_caption(p_caption), m_pos(0)
    {
        std::cout << "[Start] " << m_caption << std::endl;
        m_start = std::chrono::system_clock::now();
    }
    ~CElapsedSeconds()
    {
        const auto l_end = std::chrono::system_clock::now();
        std::cout << "[Stop] " <<m_caption << "[ duration: " << std::chrono::duration <double, std::milli> (l_end - m_start).count() << " ms ]" << std::endl;
    }
    unsigned pos() const
    {
        return m_pos;
    }
    void stop() const
    {
        std::cout << "Stop. count = " << m_pos << std::endl;
    }
    void step()
    {
        if(++m_pos % 100000 == 0)
        {
            std::cout << "count = " << m_pos << std::endl;
        }
    }
};

//#define USE_LEVEL_DB_FILL_TEST

int main() {
    int result = 0;
    leveldb::DB* db_old = NULL;
    leveldb::Options options;
    options.create_if_missing = true;
    leveldb::Status status;
    const char* name_db_old = "tth-history-old.leveldb";
    {
        CElapsedSeconds l_time("Open tth-history-old.leveldb");
        status = leveldb::DB::Open(options,name_db_old, &db_old);
        if(!status.ok())
        {
            std::cout << "Error open "<< name_db_old << " message: "<< status.ToString() << std::endl;
            return -1;
        }
    }
    leveldb::DB* db_new = NULL;
    leveldb::Options options_new;
    options_new.create_if_missing = true;
    const char* name_db_new = "tth-history.leveldb";
    status = leveldb::DB::Open(options_new,name_db_new, &db_new);
    if(!status.ok())
    {
        std::cout << "Error open "<< name_db_new << " message: "<< status.ToString() << std::endl;
        return -1;
    }
#ifdef USE_LEVEL_DB_FILL_TEST
    {
        std::string key;
        std::string value;
        CElapsedSeconds l_time("Create test leveldb - 1000000 record");
        unsigned count = 0;
        for(int i = 0; i< 1000000; i++)
        {
            char buf[100];
            snprintf(buf, sizeof(buf), "key-%d-key-key-key-key-key-key-key-key-%d", i, i);
            key = buf;
            value = key+"~"+key;
            status = db_old->Put(leveldb::WriteOptions(), key,value);
            if(!status.ok())
            {
                std::cout << "Error Put " << i << " message: "<< status.ToString() << std::endl;
            }
            l_time.step();
        }
        l_time.stop();
    }
#endif
    {
        CElapsedSeconds l_time("Clone records");
        leveldb::Iterator* it = db_old->NewIterator(leveldb::ReadOptions());
        std::string value;
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            status = db_new->Get(leveldb::ReadOptions(),it->key(), &value);
            if(status.IsNotFound())
            {
                status = db_new->Put(leveldb::WriteOptions(), it->key(),it->value());
                l_time.step();
                if(!status.ok())
                {
                    std::cout << "Error Put(new)" << l_time.pos() << " message: "<< status.ToString() << std::endl;
                }
            }
#ifdef USE_CHECK_LEVEL_DB_UPDATE
            else if(status.ok())
            {
                if(value != it->value())
                {
                    std::cout << "Error value != it->value() key = " << it->key().ToString() << " pos = " <<  l_time.pos() << std::endl;
                }
            }
#endif
        }
        if(!it->status().ok())  // Check for any errors found during the scan
        {
            std::cout << "Error iterator: "<< it->status().ToString() << std::endl;
            result = -1;
        }
        delete it;
        l_time.stop();
    }
    delete db_old;
    delete db_new;
    return result;
}
