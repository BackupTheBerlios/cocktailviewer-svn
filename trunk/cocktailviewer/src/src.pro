# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../cocktailviewer

FORMS += cocktailviewerwidgetbase.ui \
         ingredientseditorwidgetbase.ui 
DISTFILES += green.xpm \
             red.xpm 
IMAGES += green.xpm \
          red.xpm 
HEADERS += cocktailviewer.h \
           cocktailviewerwidget.h \
           ingredientseditorwidget.h 
SOURCES += cocktailviewer.cpp \
           main.cpp \
           cocktailviewerwidget.cpp \
           ingredientseditorwidget.cpp 
LIBS += -lsqlite3
TARGET = ../cocktailviewer
CONFIG += release \
warn_on \
qt \
thread
TEMPLATE = app
