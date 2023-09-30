// for converting utf-8 encoded strings to utf-16
#include <locale>
#include <codecvt>
#include <utility> // for std::move

#ifdef FS_TARGET_WINDOWS
	#include <windows.h>
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

static std::wstring UTF8_To_WideString(const String& utf8_str) {
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return std::move(converter.from_bytes(utf8_str));
}

bool Fs::MakeDir(const String& path_utf8) {
#if defined(FS_TARGET_WINDOWS)
	std::wstring pathWide = UTF8_To_WideString(path_utf8);
	if (!CreateDirectoryW(pathWide.c_str(), NULL)) {
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

bool Fs::GetFileSize(const String& filePath, u64* SizePtr) {
#ifdef FS_TARGET_WINDOWS
	auto fPathWide = UTF8_To_WideString(filePath);
	HANDLE fH = CreateFileW(fPathWide.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE) {
		return false;
	}

	DWORD fSize = 0;
	DWORD err = GetFileSize(fH, &fSize);
	if (err == INVALID_FILE_SIZE) {
		CloseHandle(fH);
		return false;
	}
	if (SizePtr) {
		*SizePtr = fSize;
	}
#else
	FILE* f = fopen(filePath.c_str(), "r");
	if (f == NULL) return false;
	if (fseek(f, 0, SEEK_END) < 0) {
		fclose(f);
		return false;
	}

	long size = ftell(f); // on error ftell returns -1 which can be directly returned.
	if (size < 0) {
		fclose(f);
		return false;
	} else if (SizePtr) {
		*SizePtr = size;
	}

	fclose(f);
#endif

	return true;
}
