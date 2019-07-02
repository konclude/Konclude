
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
QT += xml network
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

#Include file(s)
include(Konclude.pri)

unix:!macx: LIBS += $$PWD/External/librdf/Linux/x64/lib/release/librdf.a
unix:!macx: LIBS += $$PWD/External/librdf/Linux/x64/lib/release/libraptor2.a
unix:!macx: LIBS += $$PWD/External/librdf/Linux/x64/lib/release/librasqal.a
unix:!macx: LIBS += $$PWD/External/librdf/Linux/x64/lib/release/libxml2.a
unix:!macx: LIBS += -lz -lltdl -lgmp

INCLUDEPATH += $$PWD/External/librdf/Linux/x64/include
DEPENDPATH += $$PWD/External/librdf/Linux/x64/include

unix:!macx: PRE_TARGETDEPS += $$PWD/External/librdf/Linux/x64/lib/release/librdf.a
unix:!macx: PRE_TARGETDEPS += $$PWD/External/librdf/Linux/x64/lib/release/libraptor2.a
unix:!macx: PRE_TARGETDEPS += $$PWD/External/librdf/Linux/x64/lib/release/librasqal.a
unix:!macx: PRE_TARGETDEPS += $$PWD/External/librdf/Linux/x64/lib/release/libxml2.a


