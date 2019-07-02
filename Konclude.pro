
message("Preparing source code of Konclude.")
TEMPLATE = app
TARGET = Konclude
DESTDIR = ./Release
QT += xml network
CONFIG += release console  warn_off
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
