#include "cache.h"

namespace cache {
    CacheItem::CacheItem():
        m_token(""),
        m_mac(""),
        m_logintime("") {

    }

    CacheItem::~CacheItem() {

    }

    string CacheItem::toString() const {
        string str("token: " + this->m_token + 
            ",mac: " + this->m_mac + 
            ",logintime: " + this->m_logintime);

        return str;
    }

    string CacheItem::toJsonStr() const {
        string json("{\"token\":\"" + this->m_token + 
            "\",\"mac\":\"" + this->m_mac + 
            "\",\"logintime\":\"" + this->m_logintime + "\"}");

        return json;
    }

    ostream & operator<<(ostream &out,const CacheItem &cache) {
        cout<<cache.toString()<<endl;
        
        return out;
    }

    bool Cache::GetCacheByAccount(const string& account,string* cache) {
        if("" == account || nullptr == cache) {
            return false;
        }

        if(nullptr == RedisPool::Pool()) {
            return false;
        }

        shared_ptr<RedisUtil> redis = RedisPool::Pool()->redis();
        if(nullptr == redis) {
            return false;
        }

        string temp = redis->Get(account).toString();
        if("" == temp) {
            return false;
        }

        *cache = temp;

        return true;
    }

    bool Cache::UpdateCacheByAccount(const string& account,const string& cache) {
        if("" == account || "" == cache) {
            return false;
        }

        if(nullptr == RedisPool::Pool()) {
            return false;
        }

        shared_ptr<RedisUtil> redis = RedisPool::Pool()->redis();
        if(nullptr == redis) {
            return false;
        }

        redis->Set(account,cache);

        return true;
    }
}