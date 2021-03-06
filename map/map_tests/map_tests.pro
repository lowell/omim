# Map library tests.

TARGET = map_tests
CONFIG += console warn_on
CONFIG -= app_bundle
TEMPLATE = app

ROOT_DIR = ../..
DEPENDENCIES = map render gui routing search storage graphics indexer platform anim geometry coding base \
               freetype fribidi expat protobuf tomcrypt jansson osrm stats_client minizip succinct

!linux* {
  DEPENDENCIES *= opening_hours
}

drape {
  DEPENDENCIES *= drape_frontend drape
}

include($$ROOT_DIR/common.pri)

QT *= core opengl

linux*|win* {
  QT *= network
}

win32*: LIBS *= -lOpengl32
macx-*: LIBS *= "-framework IOKit" "-framework SystemConfiguration"

SOURCES += \
  ../../testing/testingmain.cpp \
  bookmarks_test.cpp \
  ge0_parser_tests.cpp  \
  geourl_test.cpp \
  kmz_unarchive_test.cpp \
  mwm_url_tests.cpp \
  navigator_test.cpp \
  mwm_set_test.cpp \

!linux* {
  SOURCES += working_time_tests.cpp \
}
