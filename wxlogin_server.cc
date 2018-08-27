#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include <mysql/mysql.h>
#include <grpcpp/grpcpp.h>

#include "md5/src/md5.h"
#include "util/configmodule.h"
#include "util/validate.h"
#include "util/const.h"
#include "model/user.h"
#include "cache/cache.h"
#include "util/redisutil.h"
#include "util/mysqlutil.h"
#include "json/json.hpp"
#include "uuid/uuid.h"
#include "protocol/wxloginpro.grpc.pb.h"

using namespace std;
using namespace util;
using namespace model;
using namespace cache;
using json = nlohmann::json;

using grpc::Server;
using grpc::ServerAsyncResponseWriter;
using grpc::ServerCompletionQueue;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using wxlogin::LoginRequest;
using wxlogin::LoginReply;
using wxlogin::SignupRequest;
using wxlogin::SignupReply;
using wxlogin::HeartRequest;
using wxlogin::HeartReply;
using wxlogin::ReplyInfo;
using wxlogin::WxLogin;

/**
 *  RPC回调方法
 */
class WxLoginServiceImpl final : public WxLogin::Service {
    /**
     *  用户登录调用
     * 
     * @param context
     * @param request
     * @param reply
     */
    Status OnLogin(ServerContext* context,const LoginRequest* request,
        LoginReply* reply) override {
        if("" == request->account()) {
            // 账号不能为空
            reply->set_status(Const::StatusCode::ACCOUNT_EMPTY);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::ACCOUNT_EMPTY));

            return Status::OK;
        }

        if("" == request->pass()) {
            // 密码不能为空
            reply->set_status(Const::StatusCode::PASS_EMPTY);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::PASS_EMPTY));

            return Status::OK;
        }

        if("" == request->mac() || "" == request->device() || 
           "" == request->ostype()) {
            reply->set_status(Const::StatusCode::INFO_ERROR);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::INFO_ERROR));

            return Status::OK;
        }

        if(!Validate::IsValidateLogin(request)) {
            reply->set_status(Const::StatusCode::SIGN_ERROR);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::SIGN_ERROR));

            return Status::OK;
        }

        User user;
        bool ret = user.GetByAccount(request->account());
        if(!ret) {
            reply->set_status(Const::StatusCode::ACCOUNT_NOTEXIST);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::ACCOUNT_NOTEXIST));

            return Status::OK;
        }

        string md5pass = MD5(request->pass()).toStr();
        if(md5pass != user.pass()) {
            reply->set_status(Const::StatusCode::LOGIN_ERROR);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::LOGIN_ERROR));

            return Status::OK;
        }

        stringstream stream;  
        stream<<Datetime::getTimeustamp();  
        string stamp = stream.str(); 

        string cache("");
        CacheItem cacheItem;
        ret = Cache::GetCacheByAccount(user.account(),&cache);
        if(ret) {
            // 已经登录在线,可以剔除
            auto j3 = json::parse(cache);
            cacheItem.set_token(j3["token"]);
            cacheItem.set_mac(j3["mac"]);
            cacheItem.set_logintime(j3["logintime"]);

            if(cacheItem.mac() != request->mac()) {
                cacheItem.set_mac(request->mac());
                cacheItem.set_logintime(stamp);

                Cache::UpdateCacheByAccount(user.account(),cacheItem.toJsonStr());
            }

            cout<<"Already Login: " <<cacheItem<<endl;
        }else {
            char buf[UUID4_LEN] = {0};
            UUID::uuid4_generate(buf);
            string token(user.account() + buf);

            cacheItem.set_token(token);
            cacheItem.set_mac(request->mac());
            cacheItem.set_logintime(stamp);

            Cache::UpdateCacheByAccount(user.account(),cacheItem.toJsonStr());

            cout<<"OnLogin : " << cacheItem<<endl;
        }

        ReplyInfo info;
        reply->set_status(Const::StatusCode::OK);
        reply->set_message(Const::GetStatusMsg(Const::StatusCode::OK));

        return Status::OK;
    }

    /**
     * 用户请求注册回调,注册成功没有说明已经登录了，仅仅是执行注册动作
     * 
     * @param context
     * @param request
     * @param reply
     */
    Status OnSignup(ServerContext* context,const SignupRequest* request,
        SignupReply* reply) override {
        if("" == request->account()) {
            reply->set_status(Const::StatusCode::ACCOUNT_EMPTY);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::ACCOUNT_EMPTY));

            return Status::OK;
        }

        if("" == request->name()) {
            reply->set_status(Const::StatusCode::NAME_EMPTY);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::NAME_EMPTY));

            return Status::OK;
        }

        if("" == request->pass()) {
            reply->set_status(Const::StatusCode::PASS_EMPTY);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::PASS_EMPTY));

            return Status::OK;
        }

        if("" == request->mac() || "" == request->device() || 
           "" == request->ostype()) {
            reply->set_status(Const::StatusCode::INFO_ERROR);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::INFO_ERROR));

            return Status::OK;
        }

        if(!Validate::IsValidateSignUp(request)) {
            reply->set_status(Const::StatusCode::SIGN_ERROR);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::SIGN_ERROR));

            return Status::OK;
        }

        User info;
        bool isExist = info.GetByAccount(request->account());
        if(isExist) {
            reply->set_status(Const::StatusCode::ACCOUNT_EXIST);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::ACCOUNT_EXIST));

            return Status::OK;
        }
        
        string md5pass = MD5(request->pass()).toStr();

        User user;
        user.set_name(request->name());
        user.set_account(request->account());
        user.set_pass(md5pass);
        user.set_email(request->email());
        user.set_tel(request->tel());
        bool ret = user.Create();
        if(!ret) {
            reply->set_status(Const::StatusCode::SIGNUP_FAILED);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::SIGNUP_FAILED));

            return Status::OK;
        }

        cout<<"OnSignup:"<<user<<"  " + user.pass()<<endl;

        ReplyInfo replyinfo;
        reply->set_status(Const::StatusCode::OK);
        reply->set_message(Const::GetStatusMsg(Const::StatusCode::OK));

        return Status::OK;
    }

    /**
     * 用户在线心跳保持以及实时消息响应
     * 
     * @param context
     * @param request
     * @param reply
     */
	Status OnHeart(ServerContext* context,const HeartRequest* request,
		HeartReply* reply) override{

        string cache("");
        CacheItem cacheItem;
        bool ret = Cache::GetCacheByAccount(request->account(),&cache);
        if(!ret) {
            reply->set_status(Const::StatusCode::NOT_LOGIN);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::NOT_LOGIN));

            return Status::OK;
        }

        auto j3 = json::parse(cache);
        cacheItem.set_token(j3["token"]);
        cacheItem.set_mac(j3["mac"]);
        cacheItem.set_logintime(j3["logintime"]);

        if(request->mac() != cacheItem.mac()) {
            reply->set_status(Const::StatusCode::LOGIN_OTHER_MAC);
            reply->set_message(Const::GetStatusMsg(Const::StatusCode::LOGIN_OTHER_MAC));

            return Status::OK;
        }

		cout<<"Request Heart :" << request->account()<<endl;
        //将对应的消息返回给用户

        // 更新缓存消息
        Cache::UpdateCacheByAccount(request->account(),cache);

		reply->set_status(Const::StatusCode::OK);
        reply->set_message(Const::GetStatusMsg(Const::StatusCode::OK));

		return Status::OK;
	}
};

void RunServer() {
    config::ConfigModule conf;
    bool ret = conf.onLoad("config.ini");
    if(!ret) {
        exit(1);

        return ;
    }

    string server_address = conf.getStringByKey("server_host") + ":" + conf.getStringByKey("server_port");

    ServerBuilder builder;
    WxLoginServiceImpl service;

    builder.AddListeningPort(server_address,grpc::InsecureServerCredentials());

    builder.RegisterService(&service);

    unique_ptr<Server> server(builder.BuildAndStart());
    cout<<"Server Listening on " << server_address << endl;

    server->Wait();
}

int main(int argc,const char* argv[]) {
    config::ConfigModule conf;
    conf.onLoad("config.ini");

    RedisPool* redis_pool = RedisPool::Pool();
    redis_pool->Init(&conf,conf.getIntByKey("redis_pool"));

    MysqlPool* pool = MysqlPool::Pool();
    pool->Init(&conf,conf.getIntByKey("mysql_pool"));

    RunServer();
    MysqlPool::Free();
    RedisPool::Free();
    
    return 0;
}