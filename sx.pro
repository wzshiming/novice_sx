TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    dial.c \
    getpin.c \
    md5.c \
    postdial.c \
    libghttp-1.0.9/ghttp.c \
    libghttp-1.0.9/http_base64.c \
    libghttp-1.0.9/http_date.c \
    libghttp-1.0.9/http_hdrs.c \
    libghttp-1.0.9/http_req.c \
    libghttp-1.0.9/http_resp.c \
    libghttp-1.0.9/http_trans.c \
    libghttp-1.0.9/http_uri.c

QMAKE_LFLAGS = -static

QMAKE_LFLAGS_RELEASE = -static

INCLUDEPATH += libghttp-1.0.9

LIBS += -L. -lws2_32 -lrasapi32

HEADERS += \
    dial.h \
    getpin.h \
    global.h \
    md5.h \
    postdial.h
