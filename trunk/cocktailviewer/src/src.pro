# Diese Datei wurde mit dem qmake-Manager von KDevelop erstellt. 
# ------------------------------------------- 
# Unterordner relativ zum Projektordner: ./src
# Das Target ist eine Anwendung:  ../cocktailviewer

FORMS += cocktailviewerwidgetbase.ui \
         ingredientseditorwidgetbase.ui \
         cocktaileditorwidgetbase.ui 
DISTFILES += green.xpm \
             red.xpm 
IMAGES += green.xpm \
          red.xpm 
HEADERS += cocktailviewerwidget.h \
           ingredientseditorwidget.h \
           cocktaileditorwidget.h 
SOURCES += main.cpp \
           cocktailviewerwidget.cpp \
           ingredientseditorwidget.cpp \
           cocktaileditorwidget.cpp 
LIBS += -lsqlite3
TARGET = ../cocktailviewer
CONFIG += release \
warn_on \
qt \
thread
TEMPLATE = app
