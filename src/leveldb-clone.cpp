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
public:
    CElapsedSeconds(const std::string& p_caption) : m_caption(p_caption)
    {
        m_start = std::chrono::system_clock::now();
    }
    ~CElapsedSeconds()
    {
        const auto l_end = std::chrono::system_clock::now();
        std::cout << m_caption << "[ duration: " << std::chrono::duration <double, std::milli> (l_end - m_start).count() << " ms ]" << std::endl;
    }
};

int main() {
    int result = 0;
    leveldb::DB* db_old = NULL;
    leveldb::Options options;
    options.create_if_missing = true;
    const char* name_db_old = "tth-history-old.leveldb";
    leveldb::Status status = leveldb::DB::Open(options,name_db_old, &db_old);
    if(!status.ok())
    {
        std::cout << "Error open "<< name_db_old << " message: "<< status.ToString() << std::endl;
        return -1;
    }

    leveldb::DB* db_new = NULL;
    leveldb::Options options_new;
    options_new.create_if_missing = true;
    const char* name_db_new = "tth-history-new.leveldb";
    status = leveldb::DB::Open(options_new,name_db_new, &db_new);
    if(!status.ok())
    {
        std::cout << "Error open "<< name_db_new << " message: "<< status.ToString() << std::endl;
        return -1;
    }
    std::string key;
    std::string value;
    {
        CElapsedSeconds l_time("Create test leveldb - 1000000 record");
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
        }
    }
    /*
    //write key1,value1
        std::string key="key";
        std::string value = "value";

        status = db->Put(leveldb::WriteOptions(), key,value);
        assert(status.ok());

        status = db->Get(leveldb::ReadOptions(), key, &value);
        assert(status.ok());
        std::cout<<value<<std::endl;
        std::string key2 = "key2";
    */
//move the value under key to key2
    /*

        status = db->Put(leveldb::WriteOptions(),key2,value);
        assert(status.ok());
        status = db->Delete(leveldb::WriteOptions(), key);

        assert(status.ok());

        status = db->Get(leveldb::ReadOptions(),key2, &value);

        assert(status.ok());
        std::cout<<key2<<"==="<<value<<std::endl;

        status = db->Get(leveldb::ReadOptions(),key, &value);

        if(!status.ok()) std::cerr <<key<<" error   " << status.ToString()<<std::endl;
        else std::cout<<key<<"==="<<value<<std::endl;
    */
    {
        CElapsedSeconds l_time("Clone 1000000 record");
        unsigned count = 0;
        leveldb::Iterator* it = db_old->NewIterator(leveldb::ReadOptions());
        int i = 0;
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            status = db_new->Put(leveldb::WriteOptions(), it->key(),it->value());
            ++i;
            if(!status.ok())
            {
                std::cout << "Error Put(new)" << i << " message: "<< status.ToString() << std::endl;
            }
            if(++count % 100000 == 0)
            {
                std::cout << "count = " << count << std::endl;
            }
        }
        if(!it->status().ok())  // Check for any errors found during the scan
        {
            std::cout << "Error iterator: "<< it->status().ToString() << std::endl;
            result = -1;
        }
        delete it;
        std::cout << "Count record = " << count << std::endl;
    }
    delete db_old;
    delete db_new;
    return result;
}
