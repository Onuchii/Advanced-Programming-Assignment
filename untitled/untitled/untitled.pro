TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        board.cpp \
        characters.cpp \
        main.cpp

HEADERS += \
    ItemsDB.h \
    board.hpp \
    characters.hpp \
    items.hpp

DISTFILES += \
    Class Design \
    report
