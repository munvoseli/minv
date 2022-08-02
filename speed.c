#include <stdio.h>
#include <time.h>

#define TRIALS 10000000
#define B64

#ifdef B64
typedef unsigned long u32;
#define BITS 64
#endif
#ifdef B32
typedef unsigned int u32;
#define BITS 32
#endif

u32 mmi_mnv_boi(u32 n) {
	u32 m = 1;
	u32 j = 1;
	for (int i = 0; i < BITS; ++i) {
		j <<= 1;
		if (((m * n) & (2 * j - 1)) != 1) {
			m |= j;
		}
	}
	return m;
}

u32 mmi_mnv_noif(u32 n) {
#ifdef B64
	// i think the original code was faster,
	// but after various attempts to optimize
	// and fix things for 64 bits
	// things happened
	u32 s = n;
	u32 h[2] = {0, n};
	unsigned int m0 = 1;
	for (unsigned int i = 1; i < 32; ++i) {
		s >>= 1;
		unsigned int a = s & 1;
		s += h[a];
		m0 |= a << i;
	}
	unsigned int m1 = 0;
	for (unsigned int i = 0; i < 32; ++i) {
		s >>= 1;
		unsigned int a = s & 1;
		s += h[a];
		m1 |= a << i;
	}
	return (((u32) m1) << 32) | m0;
#endif
#ifdef B32
	u32 s = n;
	u32 m = 1;
	for (int i = 1; i < BITS; ++i) {
		u32 a = s & (1 << i);
		s += n * a;
		m |= a;
	}
	return m;
#endif
}

u32 mmi_mnv_if(u32 n) {
	u32 s = n;
	u32 m = 1;
	for (int i = 1; i < BITS; ++i) {
		u32 a = s & (((u32) 1) << i);
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
	u32 rowa[2], rowb[2]; // qrst
	rowa[0] = ((u32) 1) << (BITS-1); rowa[1] = 0;
	rowb[0] =             n; rowb[1] = 1;
	u32* rowx = rowa, *rowy = rowb;
	while (rowy[0]) {
		u32 r = rowx[0] % rowy[0];
		rowx[1] -= rowx[0] / rowy[0] * rowy[1];
		rowx[0] = r;
		u32* h = rowx;
		rowx = rowy;
		rowy = h;
	}
	return rowx[1];
}

// https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Simple_algebraic_field_extensions
u32 mmi_eucl2(u32 n) {
	u32 t2 = 0; u32 t1 = 1;
	u32 r2 = ((u32) 1) << (BITS-1);
	u32 r1 = n;
	while (r1) {
		u32 q = r2 / r1;
		u32 r0 = r2 - q * r1;
		r2 = r1; r1 = r0;
		u32 t0 = t2 - q * t1;
		t2 = t1; t1 = t0;
	}
	return t2;
}

void test_val(u32 n) {
#ifdef B64
	printf("%16lx mnv noif\n", mmi_mnv_noif(n));
	printf("%16lx mnv if\n", mmi_mnv_if(n));
	printf("%16lx mnv boi\n", mmi_mnv_boi(n));
	printf("%16lx extended euclidian\n", mmi_exteucl(n));
	printf("%16lx extended euclidian 2\n", mmi_eucl2(n));
	printf("%16lx euler\n", mmi_euler(n));
#endif
#ifdef B32
	printf("%8x mnv noif\n", mmi_mnv_noif(n));
	printf("%8x mnv if\n", mmi_mnv_if(n));
	printf("%8x mnv boi\n", mmi_mnv_boi(n));
	printf("%8x extended euclidian\n", mmi_exteucl(n));
	printf("%8x extended euclidian 2\n", mmi_eucl2(n));
	printf("%8x euler\n", mmi_euler(n));
#endif
}

#define TEST_FUNC(func, str) start = clock();\
for (int i = 0; i < TRIALS; ++i) *pog = func(2 * i + 1);\
printf("%d ms " str "\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);


int main(int argc, char** argv) {
	clock_t start;

	printf("%d bits\n", sizeof(u32) * 8);

	test_val(13);
	test_val(15);
	test_val(4095);
	test_val(4097);
	test_val(0xc4ec4ec5);
	u32 gers;
	volatile u32 *pog = &gers;

	for (int j = 0; j < 1; ++j) {
		TEST_FUNC(mmi_mnv_if, "mnv if")
		TEST_FUNC(mmi_mnv_noif, "mnv noif")
		TEST_FUNC(mmi_mnv_boi, "mnv boi")
		TEST_FUNC(mmi_exteucl, "extended euclidian")
		TEST_FUNC(mmi_eucl2, "extended euclidian 2")
		TEST_FUNC(mmi_euler, "euler")
	}

	return 0;
}
