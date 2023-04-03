purge.commands = find . -name '*~*' -exec rm -f {} \;

CONFIG		+= qt release warn_on
DEFINES		+= QT_DEPRECATED_WARNINGS
LANGUAGE        = C++
QT		+= concurrent widgets
TEMPLATE        = app

QMAKE_CLEAN	+= Dreamy

android {
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-align \
                   -Wcast-qual \
                   -Wdouble-promotion \
                   -Werror \
                   -Werror=format-security \
                   -Wextra \
                   -Wformat=2 \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstack-protector \
                   -Wstrict-overflow=1 \
                   -Wundef \
                   -Wzero-as-null-pointer-constant \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -pedantic \
                   -std=c++11
} else {
QMAKE_CXXFLAGS  += -Wall \
                   -Wcast-qual \
                   -Werror \
                   -Werror=format-security \
                   -Wextra \
                   -Wformat=2 \
                   -Woverloaded-virtual \
                   -Wpointer-arith \
                   -Wstack-protector \
                   -Wstrict-overflow=1 \
                   -Wzero-as-null-pointer-constant \
                   -fPIE \
                   -fstack-protector-all \
                   -fwrapv \
                   -pedantic \
                   -std=c++11

linux-* {
QMAKE_CXXFLAGS  += -Wformat-overflow=2 \
                   -pie
}
}

QMAKE_EXTRA_TARGETS = purge

FORMS		= UI/dreamy.ui \
                  UI/dreamy_options.ui

HEADERS		= Source/dreamy.h \
                  Source/dreamy_label.h \
                  Source/dreamy_options.h

RESOURCES       = Icons/icons.qrc

SOURCES		= Source/dreamy_main.cc

PROJECTNAME	= Dreamy
TARGET		= Dreamy

android {
DISTFILES += android/AndroidManifest.xml \
             android/build.gradle \
             android/gradle.properties \
             android/gradle/wrapper/gradle-wrapper.jar \
             android/gradle/wrapper/gradle-wrapper.properties \
             android/gradlew \
             android/gradlew.bat \
             android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}
}
