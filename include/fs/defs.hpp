#ifndef FILESYSTEM_DEFS_HPP_INCLUDED_
#define FILESYSTEM_DEFS_HPP_INCLUDED_
#pragma once

#include <string>
#include <cstdint>

using String  = std::string;
using WString = std::wstring;
using i32     = std::int32_t;
using u32     = std::uint32_t;
using u64     = std::uint64_t;

#ifdef FS_TARGET_WINDOWS
	#define SYS_PATH_SEP "\\"
	#define SYS_PATH_SEP_CHAR '\\'
#else
	#define SYS_PATH_SEP "/"
	#define SYS_PATH_SEP_CHAR '/'
#endif

#endif // FILESYSTEM_DEFS_HPP_INCLUDED_

