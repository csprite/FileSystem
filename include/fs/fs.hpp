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

using FsPermission = std::uint32_t;

namespace FileSystem {
	String GetParentDir(const String& path); // get the "cc" part of the path "/aa/bb/cc/dd"
	String GetBaseName(const String& path); // get the "dd" part of the path "/aa/bb/cc/dd"

	// returns false on fail
	bool MakeDir(const String& path); // create a directory
	bool MakeDirRecursive(const String& path); // create a directory and any intermediate directories that don't exist

	// returns -1 on fail
	i32 GetFileSize(const String& filePath); // get a file's size, uses i32 to represent the size in bytes which limits the "detectable" size to ~4GB
}

#endif // FILESYSTEM_FS_HPP_INCLUDED_

