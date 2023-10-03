#include "minunit/minunit.h"
#include "fs/fs.hpp"

namespace Fs = FileSystem;

MU_TEST(Test_Fs_NormalizePath) {
#ifdef TEST_TARGET_WINDOWS
	const char NormalPath[] = "\\aa\\bb\\cc";
#else
	const char NormalPath[] = "/aa/bb/cc";
#endif
	String Normalized = "/aa\\bb/cc";
	Fs::NormalizePath(Normalized);
	mu_check(Normalized == NormalPath);
}

MU_TEST_SUITE(MainSuite) {
	MU_RUN_TEST(Test_Fs_NormalizePath);
}

int main(void) {
	MU_RUN_SUITE(MainSuite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

