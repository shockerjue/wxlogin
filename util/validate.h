#include <iostream>
#include <memory>
#include <string>

#include "md5/src/md5.h"

#ifdef BAZEL_BUILD
	#include "examples/protocol/wxloginpro.grpc.pb.h"
#else
	#include "protocol/wxloginpro.grpc.pb.h"
#endif

using namespace std;

using wxlogin::LoginRequest;
using wxlogin::LoginReply;
using wxlogin::SignupRequest;
using wxlogin::SignupReply;

/**
 * 验证签名的正确信息以及获取签名
 */
class Validate {
public:
    /**
     * 根据登录信息产生签名信息
     * 
     * @param request
     */
    static const string GenLoginSing(const LoginRequest* request) {
        if(nullptr == request) {
            return "";
        }

        string info(
            "&account=" + request->account() +
            "device=" + request->device() +
            "&mac=" + request->mac() +
            "&ostype=" + request->ostype() +
            "&pass=" + request->pass()
        );

        return MD5(info).toStr();
    }

    /**
     * 验证签名是否正确
     * 
     * @param request
     */
    static bool IsValidateLogin(const LoginRequest* request) {
        if(nullptr == request) {
            return false;
        }

        const string sign = GenLoginSing(request);
        if(sign == request->sign()) {
            return true;
        }

        return false;
    }

    /**
     *  更新注册信息产生签名信息
     * 
     * @param request
     */
    static const string GenSignUpSign(const SignupRequest* request) {
        if(nullptr == request) {
            return "";
        }

        string info(
            "account=" + request->account() +
            "device=" + request->device() +
            "&email=" + request->email() +
            "&mac=" + request->mac() +
            "&name=" + request->name() +
            "&ostype=" + request->ostype() +
            "&pass=" + request->pass() + 
            "&tel=" + request->tel()
        );

        return MD5(info).toStr();
    }

    /**
     *  验证注册的签名信是否正确
     * 
     * @param request
     */
    static bool IsValidateSignUp(const SignupRequest* request) {
        if(nullptr == request) {
            return false;
        }

        const string sign = GenSignUpSign(request);
        if(sign == request->sign()) {
            return true;
        }

        return false;
    }

    /**
     * 产生登录响应的签名信息
     * 
     * @param reply
     */
    static const string GenLoginReplySign(LoginReply* reply) {
        if(nullptr == reply) {
            return "";
        }

        const string info("token=" + reply->info().token());
        string sign(MD5(info).toStr());

        reply->set_sign(sign);

        return sign;
    }

    /**
     *  产生注册响应的签名信息
     * 
     * @param reply
     */
    static const string GenSignUpReplySign(SignupReply* reply) {
        if(nullptr == reply) {
            return "";
        }

        const string info("token=" + reply->info().token());
        string sign(MD5(info).toStr());

        reply->set_sign(sign);

        return sign;
    }
};