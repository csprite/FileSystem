#ifdef FS_TARGET_WINDOWS
	// for converting utf-8 encoded strings to utf-16
	#include <locale>
	#include <codecvt>
#else
	#include <sys/stat.h>
#endif

#include "fs/fs.hpp"

namespace Fs = FileSystem;

String Fs::GetParentDir(const String& path) {
	// using - 2 since it will filter out any trailing path separator
	for (auto i = path.length() - 2; i >= 0; --i) {
		if (path[i] == SYS_PATH_SEP[0]) {
			return path.substr(0, i);
		}
	}

	return "";
}

String Fs::GetBaseName(const String& path) {
	// using - 2 since it will filter out any trailing path separator
	for (auto i = path.length() - 2; i >= 0; --i) {
		if (path[i] == SYS_PATH_SEP[0]) {
			return path.substr(i + 1, path.length() - 1);
		}
	}

	return "";
}

bool Fs::MakeDir(const String& path_utf8) {
#if defined(FS_TARGET_WINDOWS)
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>> converter;
	std::wstring path_utf16 = converter.from_bytes(path_utf8);
	if (!CreateDirectoryW(path_utf16.c_str())) {
		u32 err = GetLastError();
		if (err != ERROR_ALREADY_EXISTS) {
			return false;
		}
	}
	return true;
#else
	i32 res = mkdir(path_utf8.c_str(), S_IRWXU);
	if (res != 0 && errno != EEXIST) {
		return false;
	}
	return true;
#endif
}

bool Fs::MakeDirRecursive(const String& _p) {
	String path = _p;

	if (path.back() == SYS_PATH_SEP_CHAR) path.pop_back();

	for (u32 i = 0; i < path.length(); ++i) {
		if (path[i] == SYS_PATH_SEP_CHAR) {
			if (!Fs::MakeDir(path.substr(0, i + 1))) {
				return false;
			}
		}
	}

	return Fs::MakeDir(path);
}

i32 Fs::GetFileSize(const String& filePath) {
	FILE* f = fopen(filePath.c_str(), "r");
	if (f == NULL) return -1;
	if (fseek(f, 0, SEEK_END) < 0) {
		fclose(f);
		return -1;
	}

	i32 size = ftell(f); // on error ftell returns -1 which can be directly returned.
	fclose(f);

	return size;
}

