#ifndef FILESYSTEM_FILE_HPP_INCLUDED_
#define FILESYSTEM_FILE_HPP_INCLUDED_
#pragma once

#include "fs/defs.hpp"

namespace FileSystem {
	enum Mode {
		READ, // tries to open a file for reading, if it doesn't exist, returns false
		WRITE // tries to open a file for writing, if it doesn't exist, it is created
	};
	enum Endian {
		AUTO,   // use the host's endianness
		LITTLE, // force use little endian
		BIG     // force use big endian
	};

	struct File {
		bool Open(const String& filePath, Mode mode);
		bool Close();

		size_t Read(Endian e = Endian::AUTO);
		size_t Write(
			const void* src,  // source buffer to write data from
			size_t memb_size, // size of each element in the buffer (For Ex: 4 bytes for a int array)
			size_t nmemb,     // number of elements in the buffer   (For Ex: 100 for a int[100])
			Endian e = Endian::AUTO
		);

	private:
		void* _handle = nullptr;
	};
}

#endif // FILESYSTEM_FILE_HPP_INCLUDED_

