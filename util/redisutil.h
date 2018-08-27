#ifndef H_REDISUTIL_H_
#define H_REDISUTIL_H_

#include <time.h>
#include <memory>

#include "dbmodule.h"
#include "value.h"
#include "log.h"
#include "./../hiredis/hiredis.h"
#include "configmodule.h"

using namespace util;

namespace util {
	namespace db {
		class RedisUtil : public DBModule {
		public:
			RedisUtil(void);
			~RedisUtil(void);

			/**
			* 连接到redis服务器
			*
			* @return bool
			*/
			virtual bool InitConnection(void);

			/**
			* 使用主机地址和对应的端口链接redis
			* 
			* @param host
			* @param port
			* @return bool
			*/
			virtual bool InitConnection(const string,const int);	

			/**
			* 通过key获取对应的值
			*
			* @param key
			*/
			Value 	Get(string key);

			/**
			* 通过key和value将值设置到redis中
			*
			* @param key
			* @param value
			*/
			void  	Set(string , string );

			void 	Del(string key);
			
			virtual bool can_connect(void);

		private:
			RedisUtil(const RedisUtil&){}
			RedisUtil& operator=(const RedisUtil&){ return *this; }
			
		private:
			redisContext* 	m_pConnect;
		};

		class RedisPool {
		public:
			~RedisPool();

			static RedisPool* Pool();
			static void Free();

			void Init(config::ConfigModule* conf,int size = 1);
			shared_ptr<RedisUtil> redis();

		private:
			RedisPool();

			static RedisPool* s_pool;

			vector<shared_ptr<RedisUtil> > m_redis;
		};
	}

}
#endif