/*
 *		Copyright (C) 2013-2015, 2026 by the Konclude Developer Team.
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is free software: you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License
 *		(LGPLv3) as published by the Free Software Foundation.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *		GNU (Lesser) General Public License for more details.
 *
 *		You should have received a copy of the GNU (Lesser) General Public
 *		License along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 */

// The shared library links jemalloc on Linux (KoncludeLIB.pro, CONFIG+=jemalloc), hidden, so that
// every malloc and free of Konclude, of the static Qt and of the static C++ runtime inside the
// library goes to jemalloc, while the program the library is loaded into keeps its own malloc.
// The functions of the C library below return memory that the C library allocates with its malloc
// and that the caller frees; the linker renames the calls of the library to them (--wrap) to the
// functions here, which return the same in memory of jemalloc. No other function the library
// imports passes the ownership of memory across, see 'THE MEMORY ALLOCATOR ON LINUX' in
// Java/Readme.md, which also says how the list was made.

#if defined(KONCLUDE_JEMALLOC_IN_LIBRARY)

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

extern "C" {

	char* __real_realpath(const char* path, char* resolved);
	char* __real_getcwd(char* buffer, size_t size);
	char** __real_backtrace_symbols(void* const* buffer, int size);
	// the free of the C library, which the library's own free, jemalloc's, is not
	void __libc_free(void* pointer);


	char* __wrap_strdup(const char* string) {
		size_t length = strlen(string) + 1;
		char* copy = (char*)malloc(length);
		if (copy) {
			memcpy(copy, string, length);
		}
		return copy;
	}


	char* __wrap_realpath(const char* path, char* resolved) {
		if (resolved) {
			return __real_realpath(path, resolved);
		}
		char buffer[PATH_MAX];
		if (!__real_realpath(path, buffer)) {
			return nullptr;
		}
		return __wrap_strdup(buffer);
	}


	// a null buffer asks for one of at least size bytes, or of the needed size if size is 0
	char* __wrap_getcwd(char* buffer, size_t size) {
		if (buffer) {
			return __real_getcwd(buffer, size);
		}
		size_t allocated = size > 0 ? size : PATH_MAX;
		while (true) {
			char* path = (char*)malloc(allocated);
			if (!path) {
				errno = ENOMEM;
				return nullptr;
			}
			if (__real_getcwd(path, allocated)) {
				return path;
			}
			int error = errno;
			free(path);
			if (error != ERANGE || size > 0) {
				errno = error;
				return nullptr;
			}
			allocated *= 2;
		}
	}


	// the array and its strings are one block, which the caller frees at once
	char** __wrap_backtrace_symbols(void* const* buffer, int size) {
		char** symbols = __real_backtrace_symbols(buffer, size);
		if (!symbols) {
			return nullptr;
		}
		size_t total = size * sizeof(char*);
		for (int i = 0; i < size; ++i) {
			total += strlen(symbols[i]) + 1;
		}
		char** copy = (char**)malloc(total);
		if (copy) {
			char* strings = (char*)(copy + size);
			for (int i = 0; i < size; ++i) {
				size_t length = strlen(symbols[i]) + 1;
				memcpy(strings, symbols[i], length);
				copy[i] = strings;
				strings += length;
			}
		}
		__libc_free(symbols);
		return copy;
	}

}

#endif
