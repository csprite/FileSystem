#include "minunit/minunit.h"
#include "fs/fs.hpp"

namespace Fs = FileSystem;

MU_TEST(Test_Fs_NormalizePath) {
	String Normalized = "/aa\\bb/cc";
	Fs::NormalizePath(Normalized);
#ifdef TEST_TARGET_WINDOWS
	mu_check(Normalized == "\\aa\\bb\\cc");
#else
	mu_check(Normalized == "/aa/bb/cc");
#endif
}

MU_TEST(Test_Fs_GetParentDir) {
#ifdef TEST_TARGET_WINDOWS
	mu_check(Fs::GetParentDir("\\aa\\bb\\cc") == "\\aa\\bb");
#else
	mu_check(Fs::GetParentDir("/aa/bb/cc") == "/aa/bb");
#endif
}

MU_TEST(Test_Fs_GetBaseName) {
#ifdef TEST_TARGET_WINDOWS
	mu_check(Fs::GetBaseName("\\aa\\bb\\cc") == "cc");
#else
	mu_check(Fs::GetBaseName("/aa/bb/cc") == "cc");
#endif
}

MU_TEST(Test_Fs_MakeDir) {
	mu_check(Fs::MakeDir("mainDir") == true);
	mu_check(Fs::IsRegularDir("mainDir") == 1);
}

MU_TEST(Test_Fs_MakeDirRecursive) {
	mu_check(Fs::MakeDirRecursive("mainDir/first/second/third/") == true);
	mu_check(Fs::IsRegularDir("mainDir/first/") == 1);
	mu_check(Fs::IsRegularDir("mainDir/first/second/") == 1);
	mu_check(Fs::IsRegularDir("mainDir/first/second/third/") == 1);
}

MU_TEST_SUITE(MainSuite) {
	MU_RUN_TEST(Test_Fs_NormalizePath);
	MU_RUN_TEST(Test_Fs_GetParentDir);
	MU_RUN_TEST(Test_Fs_GetBaseName);
	MU_RUN_TEST(Test_Fs_MakeDir);
	MU_RUN_TEST(Test_Fs_MakeDirRecursive);
}

int main(void) {
	MU_RUN_SUITE(MainSuite);
	MU_REPORT();
	return MU_EXIT_CODE;
}

