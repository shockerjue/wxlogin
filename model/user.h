#ifndef H_USER_H_
#define H_USER_H_

#include <vector>
#include <iostream>
#include "util/mysqlutil.h"

using namespace std;
using namespace util;
using namespace util::db;

namespace model {
    class User final{
    public:
        User();
        ~User();

        bool Get(int64_t id);
        bool GetByName(string name);
        bool GetByAccount(string account);
        vector<User> Get(int offset,int limit);

        /**
         * 插入新的用户
         */
        bool Create();

        /**
         * 更新对应的用户
         */
        bool Update();

        inline void set_account(const string& account) {
            if("" == account) {
                return ;
            }

            this->m_account = account;
        }

        inline void set_name(const string& name) {
            if("" == name) {
                return ;
            }

            this->m_name = name;
        }

        inline void set_pass(const string& pass) {
            if("" == pass) {
                return ;
            }

            this->m_pass = pass;
        }

        inline void set_email(const string& email) {
            if("" == email) {
                return ;
            }

            this->m_email = email;
        }

        inline void set_tel(const string& tel) {
            if("" == tel) {
                return ;
            }

            this->m_tel = tel;
        }

        inline int64_t id() const {
            return this->m_id;
        }

        inline string account() const{
            return this->m_account;
        }

        inline string name() const {
            return this->m_name;
        }

        inline string pass() const {
            return this->m_pass;
        }

        inline string email() const {
            return this->m_email;
        }

        inline string tel() const {
            return this->m_tel;
        }

        string toString() const;
        friend ostream & operator<<(ostream &,const User &);
    private:
        int64_t m_id;
        string m_account;
        string m_name;
        string m_pass;
        string m_email;
        string m_tel;
    };
}

#endif