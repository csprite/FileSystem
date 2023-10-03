#ifndef FILESYSTEM_FS_HPP_INCLUDED_
#define FILESYSTEM_FS_HPP_INCLUDED_
#pragma once

#include "fs/defs.hpp"
#include <functional>

namespace FileSystem {
	// if callback function returns false, the function doesn't continue listing
	using ListDirCallback = std::function<bool(const String& baseName, bool isFile)>;

	void NormalizePath(String& path); // convert "/aa\bb/cc\dd/" to "\aa\bb\cc\dd\" on windows, or "/aa/bb/cc/dd/" on other platforms

	String GetParentDir(const String& path); // get "/aa/bb/cc" from the path "/aa/bb/cc/dd"
	String GetBaseName(const String& path); // get the "dd" part of the path "/aa/bb/cc/dd"

	// returns false on fail
	bool MakeDir(const String& path); // create a directory
	bool MakeDirRecursive(const String& path); // create a directory and any intermediate directories that don't exist, no error if the path exists.
	bool GetFileSize(const String& filePath, u64* SizePtr); // get size of a file at `filePath` and store it in *SizePtr
	bool ListDir(const String& dirPath, ListDirCallback cb);

	// returns 1 if regular, 0 if not, -1 on error, with errno set
	i32 IsRegularFile(const String& filePath);
	i32 IsRegularDir(const String& dirPath);
}

#endif // FILESYSTEM_FS_HPP_INCLUDED_

