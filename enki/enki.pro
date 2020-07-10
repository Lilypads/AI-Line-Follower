TEMPLATE = lib
TARGET = enki

INCLUDEPATH += robots robots/e-puck robots/marxbot interactions objects

# Input
HEADERS = robots/e-puck/EPuckModel.h \
          robots/e-puck/EPuck.h \
          robots/marxbot/MarxbotModel.h \
          robots/marxbot/Marxbot.h \
          robots/DifferentialWheeled.h \
          objects/Objects.h \
          BluetoothBase.h Interaction.h Random.h Enki.h \
          Geometry.h PhysicalEngine.h Types.h \
          interactions/ActiveSoundSource.h \
          interactions/CircularCam.h \
          interactions/IRSensor.h \
          interactions/Bluetooth.h \
          interactions/GroundSensor.h \
          interactions/Microphone.h
SOURCES = robots/e-puck/EPuckModel.cpp \
           robots/e-puck/EPuck.cpp \
           robots/marxbot/MarxbotModel.cpp \
           robots/marxbot/Marxbot.cpp \
           robots/DifferentialWheeled.cpp \
           Enki.cpp \
           BluetoothBase.cpp Geometry.cpp PhysicalEngine.cpp Types.cpp \
           interactions/ActiveSoundSource.cpp \
           interactions/CircularCam.cpp \
           interactions/IRSensor.cpp \
           interactions/Bluetooth.cpp \
           interactions/GroundSensor.cpp \
           interactions/Microphone.cpp \
           objects/EPuckBody.cpp \
           objects/EPuckRest.cpp \
           objects/EPuckRing.cpp \
           objects/EPuckWheelLeft.cpp \
           objects/EPuckWheelRight.cpp \
           objects/MarxbotBase.cpp \
           objects/MarxbotWheel.cpp

RESOURCES += enki-viewer-textures.qrc

QT += opengl widgets

CONFIG          += qt warn_on debug staticlib

QMAKE_CXXFLAGS += -std=c++0x -march=native

headersDataFiles.path = /usr/local/include/enki
headersDataFiles.files = *.h \
                         robots/e-puck/*.h \
                         robots/marxbot/*.h \
                         robots/*.h \
                         interactions/*.h
INSTALLS += headersDataFiles

libraryFiles.path = /usr/local/lib
libraryFiles.files = libenki.a
INSTALLS += libraryFiles
