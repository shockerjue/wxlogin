#ifndef H_CONST_H_
#define H_CONST_H_

#include <map>
#include <iostream>

using namespace std;

namespace Const {
    enum StatusCode {
        OK              = 200,
        ACCOUNT_EMPTY   = 2001,
        NAME_EMPTY      = 2002,
        PASS_EMPTY      = 2003,
        INFO_ERROR      = 2004,
        SIGN_ERROR      = 2005,
        PASS_ERROR      = 2006,
        ACCOUNT_EXIST   = 2007,
        LOGIN_ERROR     = 2008,
        ACCOUNT_NOTEXIST= 2009,
        SIGNUP_FAILED   = 2010,
        USER_ALREADY_LG = 2011,
        LOGIN_OTHER_MAC = 2012,
        NOT_LOGIN       = 2013,
    };

    map<StatusCode,string> StatusMsg {
        pair<StatusCode, string>(StatusCode::OK,"成功"),
        pair<StatusCode, string>(StatusCode::ACCOUNT_EMPTY,"账号为空"),
        pair<StatusCode, string>(StatusCode::NAME_EMPTY,"用户名为空"),
        pair<StatusCode, string>(StatusCode::PASS_EMPTY,"密码为空"),
        pair<StatusCode, string>(StatusCode::INFO_ERROR,"信息错误"),
        pair<StatusCode, string>(StatusCode::SIGN_ERROR,"签名错误"),
        pair<StatusCode, string>(StatusCode::PASS_ERROR,"密码错误"),
        pair<StatusCode, string>(StatusCode::ACCOUNT_EXIST,"账号已经存在"),
        pair<StatusCode, string>(StatusCode::LOGIN_ERROR,"用户名或密码错误"),
        pair<StatusCode, string>(StatusCode::ACCOUNT_NOTEXIST,"账号不存在"),
        pair<StatusCode, string>(StatusCode::SIGNUP_FAILED,"注册失败"),
        pair<StatusCode, string>(StatusCode::USER_ALREADY_LG,"账号已经登录"),
        pair<StatusCode, string>(StatusCode::LOGIN_OTHER_MAC,"账号在其他设备上登录"),
        pair<StatusCode, string>(StatusCode::NOT_LOGIN,"账号没有登录或登录已经失效"),
    };

    string GetStatusMsg(StatusCode code) {
        map<StatusCode,string>::iterator it = StatusMsg.find(code);
        if(it == StatusMsg.end()) {
            return "异常错误";
        }

        return it->second;
    }
};

#endif