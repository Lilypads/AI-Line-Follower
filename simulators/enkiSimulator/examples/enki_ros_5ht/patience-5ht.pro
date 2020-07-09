TEMPLATE	= app
CONFIG		+= qt warn_on debug
HEADERS		= bandp.h merge2quicktime.h robot.h \
                  defs.h worldpoint.h world.h \
                  patience-5ht.h direction.h \
                  limbic-system-model\filter.h limbic-system-model\limbic-system-model.h
SOURCES		= bandp.cpp robot.cpp world.cpp merge2quicktime.cpp \
                  patience-5ht.cpp worldpoint.cpp direction.cpp \
                  limbic-system-model\filter.cpp limbic-system-model\limbic-system-model.cpp
TARGET		= patience-5ht
LIBS            += -lquicktime -L/usr/local/lib
INCLUDEPATH     += limbic-system-model
INCLUDEPATH     += /usr/include/lqt/
SUBDIRS		= limbic-system-model
QT              += widgets
QMAKE_CXXFLAGS += -std=c++0x -march=native
