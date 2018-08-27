#ifndef H_IMODULE_H_
#define H_IMODULE_H_

#include <iostream>

using namespace std;

namespace util {

	class Imodule {
	public:
		virtual void onInit(void) = 0;
		virtual void onStart(void) = 0;
		virtual void onStop(void) = 0;
		virtual void Run(void) = 0;

		virtual void onLoadConfig(const string file){}

	private:
	};

}
#endif