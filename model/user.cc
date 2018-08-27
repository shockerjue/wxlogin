#include "user.h"

namespace model {
    User::User():
        m_id(-1),
        m_account(""),
        m_name(""),
        m_pass(""),
        m_email(""),
        m_tel(""){

    }

    User::~User(){

    }

    bool User::Get(int64_t id) {
        if(0 > id) {
            return false;
        }

        shared_ptr<MysqlUtil> mysql = MysqlPool::Pool()->mysql();
        if(nullptr == mysql) {
            return false;
        }

        stringstream stream;  
        stream<<id;;  
        string temp = stream.str(); 

        vector<map<string,Value> > users = mysql->Selection("SELECT * FROM user WHERE id=" + temp + " limit 1");
        vector<map<string,Value> >::const_iterator it = users.begin();
        if(it != users.end()) {
            map<string,Value> value = *it;
            map<string,Value>::const_iterator tit = value.begin();
            while(tit != value.end()) {
                if("id" == tit->first) {
                    this->m_id = Value(tit->second).toInt();
                }
                if("account" == tit->first) {
                    this->m_account = Value(tit->second).toString();
                }
                if("name" == tit->first) {
                    this->m_name = Value(tit->second).toString();
                }
                if("password" == tit->first) {
                    this->m_pass = Value(tit->second).toString();
                }
                if("email" == tit->first) {
                    this->m_email = Value(tit->second).toString();
                }
                if("tel" == tit->first) {
                    this->m_tel = Value(tit->second).toString();
                }

                tit++;
            }

            it++;
        }else {
            return false;
        }

        return true;
    }

    vector<User> User::Get(int offset,int limit) {
        vector<User> users;
        if(0 > offset || 0 > limit) {
            return users;
        }

        shared_ptr<MysqlUtil> mysql = MysqlPool::Pool()->mysql();
        if(nullptr == mysql) {
            return users;
        }

        vector<map<string,Value> > temp = mysql->Selection("SELECT * FROM user WHERE id=");
        vector<map<string,Value> >::const_iterator it = temp.begin();
        while(it != temp.end()) {
            map<string,Value> value = *it;
            map<string,Value>::const_iterator tit = value.begin();
            while(tit != value.end()) {
                User user;

                if("id" == tit->first) {
                    this->m_id = Value(tit->second).toInt();
                }
                if("account" == tit->first) {
                    user.m_account = Value(tit->second).toString();
                }
                if("name" == tit->first) {
                    user.m_name = Value(tit->second).toString();
                }
                if("password" == tit->first) {
                    user.m_pass = Value(tit->second).toString();
                }
                if("email" == tit->first) {
                    user.m_email = Value(tit->second).toString();
                }
                if("tel" == tit->first) {
                    user.m_tel = Value(tit->second).toString();
                }

                users.push_back(user);

                tit++;
            }

            it++;
        }


        return users;
    }

    bool User::GetByAccount(string account) {
        if("" == account) {
            return false;
        }

        shared_ptr<MysqlUtil> mysql = MysqlPool::Pool()->mysql();
        if(nullptr == mysql) {
            return false;
        }

        vector<map<string,Value> > temp = mysql->Selection("SELECT * FROM user WHERE account='" + account + "' limit 1");
        vector<map<string,Value> >::const_iterator it = temp.begin();
        if(it != temp.end()) {
            map<string,Value> value = *it;
            map<string,Value>::const_iterator tit = value.begin();
            while(tit != value.end()) {
                if("id" == tit->first) {
                    this->m_id = Value(tit->second).toInt();
                }
                if("account" == tit->first) {
                    this->m_account = Value(tit->second).toString();
                }
                if("name" == tit->first) {
                    this->m_name = Value(tit->second).toString();
                }
                if("pass" == tit->first) {
                    this->m_pass = Value(tit->second).toString();
                }
                if("email" == tit->first) {
                    this->m_email = Value(tit->second).toString();
                }
                if("tel" == tit->first) {
                    this->m_tel = Value(tit->second).toString();
                }

                ++tit;
            }
        }else {
            return false;
        }

        return true;
    }

    bool User::GetByName(string name) {
        if("" == name) {
            return false;
        }

        shared_ptr<MysqlUtil> mysql = MysqlPool::Pool()->mysql();
        if(nullptr == mysql) {
            return false;
        }

        vector<map<string,Value> > temp = mysql->Selection("SELECT * FROM user WHERE name='" + name + "' limit 1");
        vector<map<string,Value> >::const_iterator it = temp.begin();
        if(it != temp.end()) {
            map<string,Value> value = *it;
            map<string,Value>::const_iterator tit = value.begin();
            while(tit != value.end()) {
                if("id" == tit->first) {
                    this->m_id = Value(tit->second).toInt();
                }
                if("account" == tit->first) {
                    this->m_account = Value(tit->second).toString();
                }
                if("name" == tit->first) {
                    this->m_name = Value(tit->second).toString();
                }
                if("password" == tit->first) {
                    this->m_pass = Value(tit->second).toString();
                }
                if("email" == tit->first) {
                    this->m_email = Value(tit->second).toString();
                }
                if("tel" == tit->first) {
                    this->m_tel = Value(tit->second).toString();
                }

                ++tit;
            }
        }else {
            return false;
        }

        return true;
    }

    bool User::Update() {
        if(0 > this->m_id) {
            return false;
        }

        bool isCan = false;
        string sql("UPDATE user SET ");
        if("" != this->m_name) {
            isCan = true;
            sql += "name='" + this->m_name + "',";
        }

        if("" == this->m_pass) {
            isCan = true;
            sql += "pass='" + this->m_pass + "',";
        }

        if("" != this->m_email) {
            isCan = true;
            sql += "email='" + this->m_email + "',";
        }

        if("" != this->m_tel) {
            isCan = true;
            sql += "email='" + this->m_email + "'";
        }

        if(!isCan) {
            return false;
        }

        stringstream stream;  
        stream<<this->m_id;;  
        string temp=stream.str(); 

        sql += " WHERE id=" + temp;
        shared_ptr<MysqlUtil> mysql = MysqlPool::Pool()->mysql();
        if(nullptr == mysql) {
            return false;
        }

        mysql->Exec(sql); // return  rows

        return true;
    }

    bool User::Create() {
        if("" == this->m_account || 
           "" == this->m_name) {
            return false;
        }
        string sql("INSERT INTO user(account,name,pass,email,tel) VALUE('");
        sql += this->m_account + "','";
        sql += this->m_name + "','";
        sql += this->m_pass + "','";
        sql += this->m_email + "','";
        sql += this->m_tel + "')";

        shared_ptr<MysqlUtil> mysql = MysqlPool::Pool()->mysql();
        if(nullptr == mysql) {
            return false;
        }

        cout<<sql<<endl;
        mysql->Exec(sql);; // return  rows

        return true;
    }

    string User::toString() const{
        const string str = "Account: " + this->m_account +
            "\nName: " + this->m_name + 
            "\nPassword: " + this->m_pass +
            "\nEmail: " + this->m_email + 
            "\nTel: " + this->m_tel;

        return str;
    }

    ostream & operator<<(ostream &out,const User &user) {
        out<<"m_id: "<<user.id();
        out<<"\nm_account: " << user.account()<<"\nm_name: " << user.name();
        out<<"\nm_pass: "<<user.pass()<<"\nm_email: "<<user.email();
        out<<"\nm_tel: "<<user.tel()<<endl;

        return out;
    }
}