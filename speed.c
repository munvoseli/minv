#include <stdio.h>
#include <time.h>

typedef unsigned int u32;
#define TRIALS 10000000
#define BITS 32

u32 mmi_mnv_noif(u32 n) {
	u32 s = n;
	u32 m = 1;
	for (int i = 1; i < BITS; ++i) {
		u32 a = s & (1 << i);
		s += n * a;
		m |= a;
	}
	return m;
}

u32 mmi_mnv_if(u32 n) {
	int s = n;
	int m = 1;
	for (int i = 1; i < BITS; ++i) {
		int a = s & (1 << i);
		if (a) {
			s += n << i;
			m |= a;
		}
	}
	return m;
}

u32 mmi_euler(u32 n) {
	u32 a = n;
	u32 b = n;
	//for (; a != 1;) {
	for (int i = 0; i < BITS; ++i) {
	//for (int i = 0; i < 32 && a != 1; ++i) {
		a *= a;
		b *= a;
	}
	return b;
}

// https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
u32 mmi_exteucl(u32 n) {
	u32 rowa[4], rowb[4]; // qrst
	rowa[1] = 1 << (BITS-1); rowa[2] = 1; rowa[3] = 0;
	rowb[1] =             n; rowb[2] = 0; rowb[3] = 1;
	u32* rowx = rowa, *rowy = rowb;
	while (rowx[1] && rowy[1]) {
		u32 q = rowx[1] / rowy[1];
		u32 r = rowx[1] % rowy[1];
		u32 s = rowx[2] - q * rowy[2];
		u32 t = rowx[3] - q * rowy[3];
		rowx[0] = q; rowx[1] = r; rowx[2] = s; rowx[3] = t;
		u32* h = rowx;
		rowx = rowy;
		rowy = h;
	}
	return rowx[3];
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
		TEST_FUNC(mmi_exteucl, "extended euclidian")
	}

	return 0;
}
