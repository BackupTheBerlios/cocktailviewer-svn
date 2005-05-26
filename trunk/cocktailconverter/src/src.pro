# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterverzeichnis relativ zum Projektverzeichnis: ./src
# Das Target ist eine Anwendung:  ../bin/cocktailconverter

LIBS += -lsqlite3 
TARGET = ../bin/cocktailconverter 
CONFIG += release \
          warn_on 
TEMPLATE = app 
HEADERS += cocktailconverter.h 
SOURCES += cocktailconverter.cpp 
