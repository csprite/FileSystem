#ifndef FILESYSTEM_FS_HPP_INCLUDED_
#define FILESYSTEM_FS_HPP_INCLUDED_
#pragma once

#include <string>

#ifdef FS_TARGET_WINDOWS
	#define SYS_PATH_SEP "\\"
	#define SYS_PATH_SEP_CHAR '\\'
#else
	#define SYS_PATH_SEP "/"
	#define SYS_PATH_SEP_CHAR '/'
#endif

using String = std::string;
using i32 = std::int32_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using FsPermission = std::uint32_t;

namespace FileSystem {
	String GetParentDir(const String& path); // get the "cc" part of the path "/aa/bb/cc/dd"
	String GetBaseName(const String& path); // get the "dd" part of the path "/aa/bb/cc/dd"

	// returns false on fail
	bool MakeDir(const String& path); // create a directory
	bool MakeDirRecursive(const String& path); // create a directory and any intermediate directories that don't exist, no error if the path exists.
	bool GetFileSize(const String& filePath, u64* SizePtr); // get size of a file at `filePath` and store it in *SizePtr

	// returns 1 if regular, 0 if not, -1 on error, with errno set
	i32 IsRegularFile(const String& filePath);
	i32 IsRegularDir(const String& dirPath);
}

#endif // FILESYSTEM_FS_HPP_INCLUDED_

