#ifndef H_UUID4_H
#define H_UUID4_H

#include <iostream>

#define UUID4_VERSION "0.1.0"
#define UUID4_LEN 37

using namespace std;

class UUID {
public:
	enum {
		UUID4_ESUCCESS =  0,
		UUID4_EFAILURE = -1
	};

	static int init_seed(void);
	static uint64_t xorshift128plus(uint64_t *s);
	static int uuid4_generate(char *dst);

	static int seeded;
	static uint64_t seed[2];
};

#endif
