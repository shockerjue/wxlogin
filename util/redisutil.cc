#include "redisutil.h"

namespace util {
	namespace db {
		RedisUtil::RedisUtil(void){
			this->m_pConnect = nullptr;
			this->m_isopen = false;
		}

		RedisUtil::~RedisUtil(void){
			if(nullptr != this->m_pConnect){
				redisFree(this->m_pConnect);

				this->m_pConnect = nullptr;
			}

			this->m_isopen = false;
		}

		bool RedisUtil::InitConnection(void){
			if(nullptr != this->m_pConnect){
				Log::Warning(__func__, "Redis already create instance!");

				return true;
			}

			if (this->m_isopen){
			 	Log::Warning(__func__, "Is connected to Redis!");

	            return true;    
	        } 

	        if(!this->can_connect()){
	        	Log::Perror(__func__, "Can't connection Redis, param error!");

	        	return false;
	        }

			this->m_pConnect = redisConnect(this->GetHost().c_str(), 
				this->GetPort());

	        if(this->m_pConnect != nullptr && this->m_pConnect->err)
	        {
	            Log::Perror(__func__, "Connect Redis error! msg:" + 
	            	string(this->m_pConnect->errstr));

	            return false;
	        }

	        this->m_isopen = true;

	        return true;
		}

		bool RedisUtil::InitConnection(const string host,const int port){
			if(nullptr != this->m_pConnect){
				Log::Warning(__func__, "Redis already create instance!");

				return true;
			}

			if (this->m_isopen){
			 	Log::Warning(__func__, "Is connected to Redis!");

	            return true;    
	        } 

	        this->SetHost(host);
	        this->SetPort(port);

	        if(!this->can_connect()){
	        	Log::Perror(__func__, "Can't connection Redis, param error!");

	        	return false;
	        }

	        this->m_pConnect = redisConnect(this->GetHost().c_str(), 
				this->GetPort());

	        if(this->m_pConnect != nullptr && this->m_pConnect->err)
	        {
	            Log::Perror(__func__, "Connect Redis error! msg:" + 
	            	string(this->m_pConnect->errstr));

	            return false;
	        }

	        this->m_isopen = true;

	        return true;
		}

		Value 	RedisUtil::Get(string key){
			Value _value;
			
			if(0 == key.size()){
				return _value;
			}

			if(!this->m_isopen){
				Log::Perror(__func__, "Redis connection isn't opened!");

				return _value;
			}

			redisReply* 	_reply;	
			_reply = (redisReply*)redisCommand(this->m_pConnect, "GET %s", key.c_str());
			if(nullptr != _reply) {
				string str = "";
				if(nullptr != _reply->str) {
					str = string(_reply->str);
				}

				freeReplyObject(_reply);
				
				_value.SetValue(str);
			}

	        return _value;
		}

		void  RedisUtil::Set(string key, string value){
			if(0 == key.size() || 0 == value.size()){
				return ;
			}

			if(!this->m_isopen){
				Log::Perror(__func__, "Redis connection isn't opened!");

				return ;
			}

			redisCommand(this->m_pConnect, "SET %s %s", key.c_str(), value.c_str());
			redisCommand(this->m_pConnect, "EXPIRE %s %d", key.c_str(), 2 * 60);
		}

		void RedisUtil::Del(string key) {
			if(0 == key.size()) {
				return ;
			}

			if(!this->m_isopen){
				Log::Perror(__func__, "Redis connection isn't opened!");

				return ;
			}

			redisCommand(this->m_pConnect, "DEL %s", key.c_str());
		}

		bool RedisUtil::can_connect(void){
			if(0 == this->GetHost().size() ||
				!this->GetPort()){
					
				return false;
			}

			return true;
		}

		RedisPool* RedisPool::s_pool = nullptr;
		RedisPool* RedisPool::Pool() {
			if(nullptr != RedisPool::s_pool) {
				return RedisPool::s_pool;
			}

			RedisPool::s_pool = new RedisPool();

			return RedisPool::s_pool;
		}

		void RedisPool::Free() {
			if(nullptr == RedisPool::s_pool) {
				return ;
			}

			delete RedisPool::s_pool;
			RedisPool::s_pool = nullptr;
		}

		RedisPool::RedisPool() {

		}

		RedisPool::~RedisPool() {
			
		}

		void RedisPool::Init(config::ConfigModule* config,int size /* = 1 */) {
			if(nullptr == config) {
				return ;
			}

			if(1 > size) {
				size = 1;
			}

			if(50 < size) {
				size = 50;
			}

			for(int i = 0; i < size; ++i) {
				shared_ptr<RedisUtil> redis(new RedisUtil);
				redis->SetHost(config->getStringByKey("redis_host"));
				redis->SetPort(config->getIntByKey("redis_port"));
				redis->InitConnection();

				this->m_redis.push_back(redis);
			}
		}

		shared_ptr<RedisUtil> RedisPool::redis() {
			if(0 == this->m_redis.size()) {
				return nullptr;
			}

			time_t tt = time(NULL);
			int index = tt % this->m_redis.size();

			cout<<"RedisPool::redis() : " <<index<<endl;

			return this->m_redis[index];
		}
	}

}