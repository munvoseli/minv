#include <stdio.h>
#include <time.h>

typedef unsigned int u32;
#define TRIALS 10000000

u32 mmi_mnv_noif(u32 n) {
	int s = n;
	int m = 1;
	for (int i = 1; i < 32; ++i) {
		int a = s & (1 << i);
		s += n * a;
		m |= a;
	}
	return m;
}

u32 mmi_mnv_if(u32 n) {
	int s = n;
	int m = 1;
	for (int i = 1; i < 32; ++i) {
		int a = s & (1 << i);
		if (a) {
			s += n << i;
			m |= a;
		}
	}
	return m;
}

u32 mmi_euler(u32 n) {
	int a = n;
	int b = n;
	for (int i = 0; a != 1 && i < 32; i++) {
		a *= a;
		b *= a;
	}
	return b;
}

u32 mmi_exteucl(u32 n) {
	return 0;
}

void test_val(u32 n) {
	printf("%8x mnv noif\n", mmi_mnv_noif(n));
	printf("%8x mnv if\n", mmi_mnv_if(n));
	printf("%8x euler\n", mmi_euler(n));
	printf("%8x extended euclidian\n", mmi_exteucl(n));
}

#define TEST_FUNC(func, str) start = clock();\
for (int i = 0; i < TRIALS; ++i) *pog = func(2 * i + 1);\
printf("%d ms " str "\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);


int main(int argc, char** argv) {
	clock_t start;
	test_val(13);
	test_val(15);
	test_val(4095);
	test_val(4097);
	test_val(0xc4ec4ec5);
	u32 gers;
	volatile u32 *pog = &gers;

	for (int j = 0; j < 3; ++j) {
		TEST_FUNC(mmi_mnv_noif, "mnv noif")
		TEST_FUNC(mmi_mnv_if, "mnv if")
		TEST_FUNC(mmi_euler, "euler")
	}

	return 0;
}
