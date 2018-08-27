#include <iostream>
#include "md5/src/md5.h"
#include "util/redisutil.h"
#include "util/mysqlutil.h"
#include "util/configmodule.h"
#include "json/json.hpp"
#include "model/user.h"

using namespace std;
using namespace util;
using namespace util::db;
using json = nlohmann::json;
using namespace model;

int main(int argc,const char** argv) {
    cout<<MD5("123").toStr()<<endl;
  
    config::ConfigModule conf;
    conf.onLoad("config.ini");

    RedisPool* redis_pool = RedisPool::Pool();
    redis_pool->Init(&conf,conf.getIntByKey("redis_pool"));
    shared_ptr<RedisUtil> redis = redis_pool->redis();
    cout<<"Read reids: " << redis->Get("name").toString()<<endl;
    redis->Del("shockerjue");
    redis->Del("name");

    MysqlPool* pool = MysqlPool::Pool();
    pool->Init(&conf,conf.getIntByKey("mysql_pool"));
    User user;
    user.GetByName("shockerjue");
    cout<<user.toString()<<user<<endl;

    auto j3 = json::parse(string("{\"wgs\":12345}"));
	cout<<j3["wgs"]<<endl;

    MysqlPool::Free();
    RedisPool::Free();

    return 0;
}