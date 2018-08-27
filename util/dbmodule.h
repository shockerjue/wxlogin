#ifndef H_DBMODULE_H_
#define H_DBMODULE_H_

#include <map>
#include <vector>
#include <iostream>

using namespace std;

namespace util {
	
	namespace db {
		class DBModule {
		public:
			/**
			* 设置数据库相关的信息
			* 主机、端口、用户、密码、数据库名字
			*/
			void SetPort(const int);
			void SetUser(const string);
			void SetPass(const string);
			void SetHost(const string);
			void SetDbName(const string);

			/**
			* 初始化连接
			*/
			virtual bool InitConnection(void) = 0;

			/**
			* 获取数据库想的信息
			* 主机、端口、用户、密码、数据库名字
 			*/
			int 	GetPort(void);
			string	GetHost(void);
			string	GetUser(void);
			string	GetPass(void);
			string 	GetDbName(void);

			virtual string	toString(void);
			virtual bool can_connect(void);

		protected:
			bool 			m_isopen;

		private:
			int 			m_port;
			string 			m_host;
			string 			m_dbname;
			string			m_user;
			string			m_pass;
		};
	}

}
#endif