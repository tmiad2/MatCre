QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++2a

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Expression.cpp \
    Parser.cpp \
    main.cpp \
    mainwindow.cpp \
    lexer.cpp

HEADERS += \
    Expression.h \
    Parser.h \
    lexer.h \
    mainwindow.h\
    lexer.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += /usr/lib/gcc/x86_64-redhat-linux/10/include


LexerRe2c.target = lexer.cpp
LexerRe2c.commands = re2c -x --input custom --tags -g -W -o $$PWD/lexer.cpp   $$PWD/lexer.re
LexerRe2c.depends = $$PWD/lexer.re
LexerRe2c.output = lexer.cpp

QMAKE_EXTRA_TARGETS += LexerRe2c

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += lexer.re
