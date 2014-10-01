QT = core xml
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
} else {
    QT += gui
}
HEADERS = VVVVVVHandler.h
SOURCES = main.cpp VVVVVVHandler.cpp
CONFIG += console
