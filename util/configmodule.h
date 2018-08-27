#ifndef H_CONFIGMODULE_H_
#define H_CONFIGMODULE_H_

#ifdef _cplusplus
    extern "C"{
#endif
	#include <string.h>
#ifdef _cplusplus
    }
#endif

#include <map>
#include <sstream>
#include <fstream>
#include <string>
#include "value.h"
#include "imodule.h"

#include "log.h"

using namespace std;
using namespace util;

namespace util{

	namespace config{
		
		class ConfigModule : Imodule {
		public:
			ConfigModule(void);
			~ConfigModule(void);

			ConfigModule(const ConfigModule&);
			ConfigModule& operator=(const ConfigModule&);

			static ConfigModule* 	getConfigModule();

			virtual void onInit(void){};
			virtual void onStart(void){};
			virtual void onStop(void){};
			virtual void Run(void){}

			/**
			* 加载配置文件以及进行解析
			*/
			bool onLoad(const string filename) ;

			/**
			* 获取key对应的值
			* 
			* @param key
			* @return string
			*/
			Value 		getValue(const string key);

			/**
			* 通过key获取配置文件中的int值
			*
			* @param key
			*/
			int  		getIntByKey(const string key);

			/**
			* 
			*
			* @param key
			*/
			bool 		getBoolByKey(const string key);

			/**
			* 通过key获取配置文件中的char值
			*
			* @param key
			*/
			char 		getCharByKey(const string key);

			/**
			* 通过key获取配置文件中的float值
			*
			* @param key
			*/
			float 		getFloatByKey(const string key);
			/**
			* 获取配置文件中对应key的配置值
			*
			* @parma key
			* @return string
			*/
			string  	getStringByKey(const string key);

		private:
			map<string,Value> 	m_config;

			static ConfigModule* 	s_pConfigModule;
		};

	}

}

#endif