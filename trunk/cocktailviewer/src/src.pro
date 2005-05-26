# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: ./src
# Das Target ist eine Anwendung:  ../cocktailviewer

LIBS += -lsqlite3 
TARGET = ../cocktailviewer 
CONFIG += release \
          warn_on \
          qt \
          thread 
TEMPLATE = app 
FORMS += cocktailviewerwidgetbase.ui 
HEADERS += cocktailviewer.h \
           cocktailviewerwidget.h 
SOURCES += cocktailviewer.cpp \
           main.cpp \
           cocktailviewerwidget.cpp 
