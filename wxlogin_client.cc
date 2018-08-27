#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#include "util/configmodule.h"

#include "util/validate.h"

#ifdef BAZEL_BUILD
    #include "example/protos/wxloginpro.grpc.pb.h"
#else
    #include "protocol/wxloginpro.grpc.pb.h"
#endif

using namespace std;
using namespace util;

using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;

using wxlogin::LoginRequest;
using wxlogin::LoginReply;
using wxlogin::SignupRequest;
using wxlogin::SignupReply;
using wxlogin::HeartRequest;
using wxlogin::HeartReply;
using wxlogin::WxLogin;

class WxLoginClient {
public:
    WxLoginClient(shared_ptr<Channel> channel)
         : stub_(WxLogin::NewStub(channel)) {}

    string OnLogin(const string& user,const string& pass,const string& mac,
        const string& ostype,const string& device) {
        LoginRequest request;
        request.set_account(user);
        request.set_pass(pass);
        request.set_mac(mac);
        request.set_ostype(ostype);
        request.set_device(device);

        request.set_sign(Validate::GenLoginSing(&request));

        LoginReply reply;
        ClientContext context;

        Status status = stub_->OnLogin(&context,request,&reply);

        if(status.ok()) {
            cout<<"OnLogin Status : " << reply.status()<<endl;

            return reply.message();
        }else {
            cout<<status.error_code() << ": "<< status.error_message()<< endl;

            return "RPC failed";
        }
    }

    string OnSignup(const string& account,const string& user,const string& pass,
        const string& email,const string& mac,const string& ostype,
        const string& device)  {
        SignupRequest request;
        request.set_account(account);
        request.set_name(user);
        request.set_pass(pass);
        request.set_email(email);
        request.set_mac(mac);
        request.set_ostype(ostype);
        request.set_device(device);

        request.set_sign(Validate::GenSignUpSign(&request));

        SignupReply reply;
        ClientContext context;
        Status status = stub_->OnSignup(&context,request,&reply);

        if(status.ok()) {
            cout<<"OnSignup Status : " << reply.status()<<endl;

            return reply.message();
        }else {
            cout<<status.error_code() << " : "<< status.error_message() <<endl;

            return "RPC Fail!";
        }
    }

    string OnHeart(const string& user,const string& mac,const string& token) {
        HeartRequest request;
        request.set_account(user);
        request.set_token(token);
        request.set_mac(mac);

        HeartReply reply;
        ClientContext context;

        Status status = stub_->OnHeart(&context,request,&reply);

        if(status.ok()) {
            cout<<"OnHeart Status : " << reply.status()<<endl;

            return reply.message();
        }else {
            cout<< status.error_code() << " : " <<status.error_message() <<endl;

            return "RPC Fail";
        }
    }

private:
    unique_ptr<WxLogin::Stub> stub_;
};

void SyncRPC(string type) {
    config::ConfigModule conf;
    bool ret = conf.onLoad("config.ini");
    if(!ret) {
        exit(1);

        return ;
    }

    string server_address = conf.getStringByKey("server_host") + ":" + conf.getStringByKey("server_port");

    WxLoginClient wxlogin(grpc::CreateChannel(server_address,grpc::InsecureChannelCredentials()));
    string account(conf.getStringByKey("account"));
    string user(conf.getStringByKey("user"));
    string pass(conf.getStringByKey("pass"));
    string email(conf.getStringByKey("email"));
    string mac(conf.getStringByKey("mac"));
    string ostype(conf.getStringByKey("ostype"));
    string device(conf.getStringByKey("device"));
    string token(conf.getStringByKey("token"));

    if("1" == type) {
        string sign = wxlogin.OnSignup(account,user,pass,email,mac,ostype,device);
        cout<< "OnSign message: " << sign<< endl;

        return ;
    }

    if("2" == type) {
        string reply = wxlogin.OnLogin(user,pass,mac,ostype,device);
        cout<< "OnLogin message : "<< reply << endl;

        return ;
    }

    string heart = wxlogin.OnHeart(user,mac,token);
    cout<<"OnHeart message : "<< heart<<endl;
}

int main(int argc,const char* argv[]) {
    string type("");
    if(1 < argc) {
        type = string(argv[1]);
    }

    SyncRPC(type);
    
    return 0;
}
