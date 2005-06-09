# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../cocktailviewer

FORMS += cocktailviewerwidgetbase.ui \
         ingredientseditorwidgetbase.ui 
HEADERS += cocktailviewer.h \
           cocktailviewerwidget.h 
SOURCES += cocktailviewer.cpp \
           main.cpp \
           cocktailviewerwidget.cpp 
LIBS += -lsqlite3
TARGET = ../cocktailviewer
CONFIG += release \
warn_on \
qt \
thread
TEMPLATE = app
