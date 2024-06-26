QT       += core gui widgets printsupport xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/fileprocessor.cpp \
	src/data_parser.cpp \
	src/main.cpp \
	src/charts.cpp \
        src/qcustomplot.cpp \

HEADERS += \
        include/fileprocessor.h \
	include/data_parser.h \
	include/charts.h \
        qcustomplot.h \

FORMS += \
	ui/charts.ui

RESOURCES += \ 
	figures.qrc

RC_ICONS += icon.ico

TARGET = Ussurvescom_figures
DESTDIR = $$PWD/dist

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
