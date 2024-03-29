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
	mu_check(Normalized == PATH_SEP "aa" PATH_SEP "bb" PATH_SEP "cc");
}

MU_TEST(Test_Fs_GetParentDir) {
	mu_check(Fs::GetParentDir(PATH_SEP "aa" PATH_SEP "bb" PATH_SEP "cc") == PATH_SEP "aa" PATH_SEP "bb");
}

MU_TEST(Test_Fs_GetBaseName) {
	mu_check(Fs::GetBaseName(PATH_SEP "aa" PATH_SEP "bb" PATH_SEP "cc") == "cc");
}

MU_TEST(Test_Fs_MakeDir) {
	mu_check(Fs::MakeDir("mainDir") == true);
	mu_check(Fs::IsDirectory("mainDir") == 1);
}

MU_TEST(Test_Fs_IsRegularX) {
	mu_check(Fs::IsDirectory(".github" PATH_SEP "workflows"));
	mu_check(Fs::IsFile(".github" PATH_SEP "workflows" PATH_SEP "ci.yml"));
}

MU_TEST(Test_Fs_MakeDirRecursive) {
	mu_assert(
		Fs::MakeDirRecursive("mainDir" PATH_SEP "first" PATH_SEP "second" PATH_SEP "third" PATH_SEP) == true,
#ifdef TEST_TARGET_WINDOWS
		GetLastErrorAsString().c_str()
#else
		strerror(errno)
#endif
	);
	mu_check(Fs::IsDirectory("mainDir" PATH_SEP "first" PATH_SEP) == 1);
	mu_check(Fs::IsDirectory("mainDir" PATH_SEP "first" PATH_SEP "second" PATH_SEP) == 1);
	mu_check(Fs::IsDirectory("mainDir" PATH_SEP "first" PATH_SEP "second" PATH_SEP "third" PATH_SEP) == 1);
}

MU_TEST(Test_Fs_ListDir) {
	bool didListWorkflowsDir = false;
	mu_check(Fs::ListDir(".github", [&](const String& baseName, bool isFile) -> bool {
		didListWorkflowsDir = baseName == "workflows" && !isFile;
		return true;
	}) == true);
	mu_check(didListWorkflowsDir);

	bool didListCIYMLFile = false;
	mu_check(Fs::ListDir(".github" PATH_SEP "workflows", [&](const String& baseName, bool isFile) -> bool {
		didListCIYMLFile = baseName == "ci.yml" && isFile;
		return true;
	}) == true);
	mu_check(didListCIYMLFile);
}

MU_TEST_SUITE(MainSuite) {
	MU_RUN_TEST(Test_Fs_NormalizePath);
	MU_RUN_TEST(Test_Fs_GetParentDir);
	MU_RUN_TEST(Test_Fs_GetBaseName);
	MU_RUN_TEST(Test_Fs_IsRegularX);
	MU_RUN_TEST(Test_Fs_MakeDir);
	MU_RUN_TEST(Test_Fs_MakeDirRecursive);
	MU_RUN_TEST(Test_Fs_ListDir);
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

