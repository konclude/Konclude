

JAVAHOME = $$(JAVA_HOME)
# jni.h includes the platform specific jni_md.h, which is located in a subdirectory
# that is named after the platform, so that subdirectory has to be an include
# directory as well.
macx: JAVAPLATFORMINCLUDE = $$JAVAHOME/include/darwin
else:win32: JAVAPLATFORMINCLUDE = $$JAVAHOME/include/win32
else:unix: JAVAPLATFORMINCLUDE = $$JAVAHOME/include/linux
message("Using $$JAVAHOME/include and $$JAVAPLATFORMINCLUDE from enviroment variable 'JAVA_HOME' as include directories for Java/JNI.")


message("Updating Konclude version from Git Revision.")

# Create our custom gitbuild target.
win32:gitbuild.commands = $${PWD}/WinGitBuildScript.bat
else:gitbuild.commands = $${PWD}/UnixGitBuildScript.sh
QMAKE_EXTRA_TARGETS += gitbuild

PRE_TARGETDEPS = gitbuild

message("Preparing source code of Konclude.")
TEMPLATE = lib
TARGET = Konclude
DESTDIR = ./Release
QT += xml network concurrent
CONFIG += release warn_off c++11
DEFINES += QT_XML_LIB QT_NETWORK_LIB KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED KONCLUDE_COMPILE_JNI_INTERFACE
INCLUDEPATH += ./generatedfiles \
    ./GeneratedFiles/Release \
    ./Source \
	$$JAVAHOME/include \
	$$JAVAPLATFORMINCLUDE \
	.
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(Konclude.pri)

# jemalloc on Linux, with CONFIG+=jemalloc JEMALLOC_LIB=<libjemalloc_pic.a> (built by
# .github/scripts/build-jemalloc-library.sh). It is linked in whole and hidden with every other
# static library (--exclude-libs), so that the library's malloc and free, Konclude's, the static
# Qt's and the static C++ runtime's, are jemalloc's while the program the library is loaded into
# keeps its own. The C library functions that return memory for the caller to free are wrapped,
# see Source/Utilities/Memory/CLibraryAllocatorWrappers.cpp. Without the option the library uses
# the program's malloc and tunes glibc's, see 'THE MEMORY ALLOCATOR ON LINUX' in Java/Readme.md.
# -Bsymbolic binds the library's calls to its own functions: it exports the C++ template
# instances it contains, and without it the program's libstdc++.so, which the Java virtual
# machine loads, would supply the ones it has as well (std::wstring's _M_construct among them),
# which allocate with the program's malloc what the library later frees with jemalloc.
linux:jemalloc {
	isEmpty(JEMALLOC_LIB): error("CONFIG+=jemalloc needs JEMALLOC_LIB, the path of libjemalloc_pic.a")
	message("Linking jemalloc from $$JEMALLOC_LIB into the library.")
	DEFINES += KONCLUDE_JEMALLOC_IN_LIBRARY
	SOURCES += ./Source/Utilities/Memory/CLibraryAllocatorWrappers.cpp
	LIBS += -Wl,--whole-archive $$JEMALLOC_LIB -Wl,--no-whole-archive -lpthread -ldl
	QMAKE_LFLAGS += -Wl,--exclude-libs,ALL -Wl,-Bsymbolic \
		-Wl,--wrap=realpath -Wl,--wrap=getcwd -Wl,--wrap=strdup -Wl,--wrap=backtrace_symbols
}
