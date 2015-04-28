# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-minotaur

CONFIG += sailfishapp

SOURCES += src/harbour-minotaur.cpp \
    src/tools.cpp

OTHER_FILES += qml/harbour-minotaur.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-minotaur.changes.in \
    rpm/harbour-minotaur.spec \
    rpm/harbour-minotaur.yaml \
    translations/*.ts \
    harbour-minotaur.desktop \
    qml/pages/MenuPage.qml \
    qml/pages/GamePage.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
#TRANSLATIONS += translations/harbour-minotaur-de.ts

HEADERS += \
    src/tools.h

