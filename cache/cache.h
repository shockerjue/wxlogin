#ifndef H_CACHE_H_
#define H_CACHE_H_

#include <string>
#include <iostream>
#include "util/redisutil.h"

using namespace util;
using namespace util::db;

namespace cache {
    /**
     * 缓存项信息
     */
    class CacheItem {
    public:
        CacheItem();
        ~CacheItem();

        string toString() const;
        string toJsonStr() const;

        inline void set_token(const string& token) {
            if("" == token) {
                return ;
            } 

            this->m_token = token;
        }

        inline void set_mac(const string& mac) {
            if("" == mac) {
                return ;
            }

            this->m_mac = mac;
        }

        inline void set_logintime(const string& logintime) {
            if("" == logintime) {
                return ;
            }

            this->m_logintime = logintime;
        }

        inline string token() const {
            return this->m_token;
        }

        inline string mac() const {
            return this->m_mac;
        }

        inline string logintime() const {
            return this->m_logintime;
        }

        friend ostream & operator<<(ostream &out,const CacheItem &cache);
    private:
        string m_token;
        string m_mac;
        string m_logintime;
    };

    class Cache{
    public:
        /**
         * 读取缓存信息
         * 
         * @param account
         * @param cache
         */
        static bool GetCacheByAccount(const string& account,string* cache);

        /**
         * 更新或者写入缓存信息
         * 
         * @param account
         * @param cache
         */
        static bool UpdateCacheByAccount(const string& account,const string& cache);
    };
}

#endif