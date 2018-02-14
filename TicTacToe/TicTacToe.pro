TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
            BPNeuralNetwork.cpp \
    Table.cpp \
    Test.cpp
            
HEADERS += BPNeuralNetwork.hpp \
            Matrix.hpp \
    Table.hpp \
    Test.hpp
