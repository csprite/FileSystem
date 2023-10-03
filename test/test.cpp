#include "fs/fs.hpp"

#ifdef TEST_TARGET_WINDOWS
	#include <windows.h>

	String GetLastErrorAsString();
#endif

#include "minunit/minunit.h"

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
	bool result = Fs::MakeDirRecursive("mainDir" SYS_PATH_SEP "first" SYS_PATH_SEP "second" SYS_PATH_SEP "third" SYS_PATH_SEP) == true;
	if (result != true) {
#ifdef TEST_TARGET_WINDOWS
		mu_fail(GetLastErrorAsString().c_str());
#else
		mu_fail(strerror(errno));
#endif
	}
	mu_check(Fs::IsRegularDir("mainDir" SYS_PATH_SEP "first" SYS_PATH_SEP) == 1);
	mu_check(Fs::IsRegularDir("mainDir" SYS_PATH_SEP "first" SYS_PATH_SEP "second" SYS_PATH_SEP) == 1);
	mu_check(Fs::IsRegularDir("mainDir" SYS_PATH_SEP "first" SYS_PATH_SEP "second" SYS_PATH_SEP "third" SYS_PATH_SEP) == 1);
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

#ifdef TEST_TARGET_WINDOWS
String GetLastErrorAsString() {
	DWORD errorMessageID = ::GetLastError();
	if(errorMessageID == 0) {
		return "";
	}

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL
	);

	String message(messageBuffer, size);
	LocalFree(messageBuffer);

	return message;
}
#endif

