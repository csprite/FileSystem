#ifndef __FILESYSTEM_HELPER_HPP_INCLUDED
#define __FILESYSTEM_HELPER_HPP_INCLUDED
#pragma once

#include "fs/defs.hpp"

WString UTF8_To_WideString(const String& utf8_str);
String WideString_To_UTF8(const WString& wide_str);

#endif // __FILESYSTEM_HELPER_HPP_INCLUDED

