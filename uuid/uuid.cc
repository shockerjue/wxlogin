#include "uuid.h"

int UUID::seeded = 0;
uint64_t UUID::seed[2];

uint64_t UUID::xorshift128plus(uint64_t *s) {
	uint64_t s1 = s[0];
	const uint64_t s0 = s[1];
	s[0] = s0;
	s1 ^= s1 << 23;
	s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
	
	return s[1] + s0;
}


int UUID::init_seed(void) {
	int res;
	FILE *fp = fopen("/dev/urandom", "rb");
	if (!fp) {
		return UUID::UUID4_EFAILURE;
	}
	res = fread(seed, 1, sizeof(seed), fp);
	fclose(fp);
	if ( res != sizeof(seed) ) {
		return UUID::UUID4_EFAILURE;
	}

	return UUID::UUID4_ESUCCESS;
}


int UUID::uuid4_generate(char *dst) {
	static const char *templ = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
	static const char *chars = "0123456789abcdef";
	union { unsigned char b[16]; uint64_t word[2]; } s;
	const char *p;
	int i, n;
	if (!seeded) {
	do {
		int err = init_seed();
		if (err != UUID::UUID4_ESUCCESS) {
		return err;
		}
	} while (seed[0] == 0 && seed[1] == 0);
	seeded = 1;
	}

	s.word[0] = xorshift128plus(seed);
	s.word[1] = xorshift128plus(seed);
	p = templ;
	i = 0;

	while (*p) {
		n = s.b[i >> 1];
		n = (i & 1) ? (n >> 4) : (n & 0xf);
		switch (*p) {
			case 'x'  : *dst = chars[n];              i++;  break;
			case 'y'  : *dst = chars[(n & 0x3) + 8];  i++;  break;
			default   : *dst = *p;
		}
		dst++, p++;
	}
	*dst = '\0';

	return UUID::UUID4_ESUCCESS;
}
