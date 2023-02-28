QT += quick

SOURCES += \
        authmanager.cpp \
        listcontroller.cpp \
        main.cpp \
        mymodel.cpp \
        securemanager.cpp

resources.files = main.qml
resources.prefix = /$${TARGET}


RESOURCES += resources \
    qml.qrc

OTHER_FILES += \
    main.qml \
    Data.qml \
    Login.qml


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data.json

HEADERS += \
    authmanager.h \
    listcontroller.h \
    mymodel.h \
    securemanager.h
