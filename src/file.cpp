#include <cassert>

#ifdef FS_TARGET_WINDOWS
	#include <windows.h>
#else
	#include <cstdio>
#endif

#include "helper.hpp"
#include "fs/file.hpp"

namespace Fs = FileSystem;

bool Fs::File::Open(const String& filePath, Mode mode) {
#ifdef FS_TARGET_WINDOWS
	DWORD dwDesiredAccess = 0;
	DWORD dwShareMode = 0;
	DWORD dwCreationDisposition = 0;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

	if (mode == Mode::READ) {
		dwDesiredAccess = GENERIC_READ;
		dwShareMode = FILE_SHARE_READ;
		dwCreationDisposition = OPEN_EXISTING;
	} else if (mode == Mode::WRITE) {
		dwDesiredAccess = GENERIC_WRITE;
		dwShareMode = 0;
		dwCreationDisposition = CREATE_NEW;
	} else {
		assert(false && "Unreachable section reached");
	}

	WString filePathWide = UTF8_To_WideString(filePath);
	HANDLE fileHandle = CreateFileW(
		filePathWide.c_str(), dwDesiredAccess, dwShareMode, NULL,
		dwCreationDisposition, dwFlagsAndAttributes, NULL
	);

	if (fileHandle == INVALID_HANDLE_VALUE) {
		return false;
	}

	_handle = (void*)fileHandle;

	return true;
#else
	const char* fMode = "";
	if (mode == Mode::READ) {
		fMode = "rb";
	} else if (mode == Mode::WRITE) {
		fMode = "wb";
	} else {
		assert(false && "Unreachable section reached");
	}

	FILE* fPtr = fopen(filePath.c_str(), fMode);
	if (fPtr == NULL) return false;

	_handle = (void*)fPtr;
	return true;
#endif
}

bool Fs::File::Close() {
#ifdef FS_TARGET_WINDOWS
	HANDLE fileHandle = (HANDLE)_handle;
	_handle = NULL;
	return CloseHandle(fileHandle);
#else
	FILE* fPtr = (FILE*)_handle;
	_handle = NULL;
	return fclose(fPtr) == 0;
#endif
}

size_t Fs::File::Write(const void* src, size_t memb_size, size_t nmemb, Fs::Endian e) {

	return 0;
}

