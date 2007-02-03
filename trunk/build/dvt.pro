TEMPLATE = app

CONFIG += qt release static
CONFIG -= debug

!win32 {
	TARGET = ../dist/dvt
}

win32 {
	TARGET = ../../dist/dvt
}

include(../src/core/qmake.in)
include(../src/gui/qmake.in)
include(../src/utils/debug/qmake.in)
include(../src/utils/xmlNode/qmake.in)

LIBS += 
	
INCLUDEPATH += \
	../src/core \
	../src/core/lang \
	../src/core/trainer \
	../src/core/simpleTrainer \
	../src/core/flashCardTrainer \
	../src/gui \
	../src/gui/editExt \
	../src/gui/editDetail \
	../src/gui/simpleTrainer \
	../src/gui/flashCardTrainer \
	../src/utils/debug \
	../src/utils/xmlNode

