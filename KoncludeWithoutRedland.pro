
message("Updating Konclude version from Git Revision.")

# Create our custom gitbuild target.
win32:gitbuild.commands = $${PWD}/WinGitBuildScript.bat
else:gitbuild.commands = $${PWD}/UnixGitBuildScript.sh
QMAKE_EXTRA_TARGETS += gitbuild

PRE_TARGETDEPS = gitbuild

message("Preparing source code of Konclude.")
TEMPLATE = app
TARGET = Konclude
DESTDIR = ./Release
QT += xml network concurrent
CONFIG += release console warn_off c++11
DEFINES += QT_XML_LIB QT_NETWORK_LIB KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
INCLUDEPATH += ./generatedfiles \
    ./GeneratedFiles/Release \
    ./Source \
	.
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

#Include file(s)
include(Konclude.pri)



# jemalloc in place of the malloc of the C library, as the release packages of Konclude.pro link
# it: 'qmake CONFIG+=jemalloc' links -ljemalloc, and JEMALLOC_LIB=<file> a particular library,
# e.g. a static libjemalloc.a or the shared library of a distribution without its development
# package. On Linux with glibc this shortens the classification of SNOMED CT by a third, see
# 'THE MEMORY ALLOCATOR ON LINUX' in Java/Readme.md.
unix:jemalloc {
	isEmpty(JEMALLOC_LIB): LIBS += -ljemalloc
	else: LIBS += $$JEMALLOC_LIB
	message("Linking jemalloc: $$JEMALLOC_LIB")
}
