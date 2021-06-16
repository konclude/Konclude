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
DEFINES += QT_XML_LIB QT_NETWORK_LIB KONCLUDE_REDLAND_INTEGRATION KONCLUDE_FORCE_ALL_DEBUG_DEACTIVATED
INCLUDEPATH += ./generatedfiles \
    ./GeneratedFiles/Release \
    ./Source \
	.
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
QMAKE_CXXFLAGS += -ggdb

#Include file(s)
include(Konclude.pri)

unix: LIBS += $$PWD/External/librdf/Linux/x64/lib/release/librdf.a
unix: LIBS += $$PWD/External/librdf/Linux/x64/lib/release/libraptor2.a
unix: LIBS += $$PWD/External/librdf/Linux/x64/lib/release/librasqal.a
unix: LIBS += $$PWD/External/librdf/Linux/x64/lib/release/libxml2.a
unix: LIBS += $$PWD/External/jemalloc/Linux/x64/lib/release/libjemalloc.a
unix: LIBS += -lz -lltdl -lgmp -ldl -L External/jemalloc/Linux/x64/lib/release/ -ljemalloc

unix: INCLUDEPATH += $$PWD/External/librdf/Linux/x64/include
unix: DEPENDPATH += $$PWD/External/librdf/Linux/x64/include


win32: LIBS += "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\kernel32.Lib"
win32: LIBS += $$PWD/External/librdf/Windows/x64/lib/Release/librdf.lib
win32: LIBS += $$PWD/External/librdf/Windows/x64/lib/Release/raptor2.lib
win32: LIBS += $$PWD/External/librdf/Windows/x64/lib/Release/rasqal.lib
win32: LIBS += $$PWD/External/librdf/Windows/x64/lib/Release/libsv.lib

win32: INCLUDEPATH += $$PWD/External/librdf/Windows/x64/include
win32: DEPENDPATH += $$PWD/External/librdf/Windows/x64/include