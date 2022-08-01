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
	while (a != 1) {
		a *= a;
		b *= a;
	}
	return b;
}

u32 mmi_exteucl(u32 n) {
	return 0;
}

void test_val(u32 n) {
	printf("%x mnv noif\n", mmi_mnv_noif(n));
	printf("%x mnv if\n", mmi_mnv_if(n));
	printf("%x euler\n", mmi_euler(n));
	printf("%x extended euclidian\n", mmi_exteucl(n));
}

int main(int argc, char** argv) {
	clock_t start;
	for (int j = 0; j < 3; ++j) {
	start = clock();
	for (int i = 0; i < TRIALS; ++i) mmi_mnv_noif(2 * i + 1);
	printf("%d ms mnv noif\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);

	start = clock();
	for (int i = 0; i < TRIALS; ++i) mmi_mnv_if(2 * i + 1);
	printf("%d ms mnv if\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);

	start = clock();
	for (int i = 0; i < TRIALS; ++i) mmi_euler(2 * i + 1);
	printf("%d ms eul\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);

	}

	test_val(15);
	return 0;
}
