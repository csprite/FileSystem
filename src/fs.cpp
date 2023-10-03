#ifdef FS_TARGET_WINDOWS
	#include <windows.h>
#else
	#include <sys/stat.h>
	#include <dirent.h>
#endif

#include "fs/fs.hpp"
#include "helper.hpp"

namespace Fs = FileSystem;

String Fs::GetParentDir(const String& path) {
	// using - 2 since it will filter out any trailing path separator
	for (auto i = path.length() - 2; i >= 0; --i) {
		if (path[i] == PATH_SEP_CHAR) {
			return path.substr(0, i);
		}
	}

	return "";
}

String Fs::GetBaseName(const String& path) {
	// using - 2 since it will filter out any trailing path separator
	for (auto i = path.length() - 2; i >= 0; --i) {
		if (path[i] == PATH_SEP_CHAR) {
			return path.substr(i + 1, path.length() - 1);
		}
	}

	return "";
}

void Fs::NormalizePath(String& path) {
	std::wstring pathWide = UTF8_To_WideString(path);

	for (wchar_t& c : pathWide) {
#ifdef FS_TARGET_WINDOWS
		if (c == L'/') c = wchar_t(PATH_SEP_CHAR);
#else
		if (c == L'\\') c = wchar_t(PATH_SEP_CHAR);
#endif
	}

	path = WideString_To_UTF8(pathWide);
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

	if (path.back() == PATH_SEP_CHAR) path.pop_back();

	for (u32 i = 0; i < path.length(); ++i) {
		if (path[i] == PATH_SEP_CHAR) {
			if (!Fs::MakeDir(path.substr(0, i + 1))) {
				return false;
			}
		}
	}

	return Fs::MakeDir(path);
}

bool Fs::GetFileSize(const String& filePath, u64* SizePtr) {
#ifdef FS_TARGET_WINDOWS
	std::wstring fPathWide = UTF8_To_WideString(filePath);
	HANDLE fH = CreateFileW(fPathWide.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fH == INVALID_HANDLE_VALUE) {
		return false;
	}

	LARGE_INTEGER size;
	if (!GetFileSizeEx(fH, &size)) {
		CloseHandle(fH);
		return false;
	}
	if (SizePtr) {
		*SizePtr = size.QuadPart;
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

#if !defined(S_IFMT)
	#define S_IFMT _S_IFMT
#endif
#if !defined(S_ISDIR)
	#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif
#if !defined(S_ISREG)
	#define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
#endif

i32 Fs::IsRegularFile(const String& filePath) {
	struct stat st;

	if (stat(filePath.c_str(), &st) < 0)
		return -1;

	return S_ISREG(st.st_mode);
}

i32 Fs::IsRegularDir(const String& dirPath) {
	struct stat st;

	if (stat(dirPath.c_str(), &st) < 0)
		return -1;

	return S_ISDIR(st.st_mode);
}

bool Fs::ListDir(const String& _dP, ListDirCallback cb) {
#ifdef FS_TARGET_WINDOWS
	String dirPath = _dP + PATH_SEP "*.*";
	std::wstring dirPathWide = UTF8_To_WideString(dirPath);
	WIN32_FIND_DATAW fdFile;

	HANDLE hFind = FindFirstFileW(dirPathWide.c_str(), &fdFile);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}

	do {
		if (wcscmp(fdFile.cFileName, L".") != 0 && wcscmp(fdFile.cFileName, L"..") != 0) {
			bool isDir = fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
			cb(WideString_To_UTF8(fdFile.cFileName), !isDir);
		}
	} while(FindNextFileW(hFind, &fdFile));

	FindClose(hFind);

	return true;
#else
	DIR* dir = opendir(_dP.c_str());
	if (dir == NULL)
		return false;

	while (true) {
		errno = 0;
		struct dirent* ent = readdir(dir);
		bool isDir = ent->d_type == DT_DIR;
		if (ent == NULL || errno != 0) {
			closedir(dir);
			return false;
		} else if (ent->d_name[0] == '.') {
			continue; // skip "." & ".." entries
		} else if (cb(ent->d_name, !isDir)) {
			break;
		}
	}

	closedir(dir);
	return true;
#endif
}

