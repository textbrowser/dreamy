#!/usr/bin/env bash

# Alexis Megas.

if [ ! -x /usr/bin/dpkg-deb ]
then
    echo "Please install dpkg-deb."
    exit 1
fi

if [ ! -x /usr/bin/fakeroot ]
then
    echo "Please install fakeroot."
    exit 1
fi

qmake=$(which qmake)

if [ -z "$qmake" ]
then
    qmake=$(which qmake6)
fi

if [ -z "$qmake" ]
then
    echo "Please install Qt."
    exit 1
fi

make distclean 2>/dev/null
mkdir -p ./opt/dreamy
$qmake -o Makefile dreamy.pro && make -j $(nproc) 2>/dev/null
cp -p ./Dreamy ./opt/dreamy/.
cp -p ./dreamy.desktop ./opt/dreamy/.
mkdir -p dreamy-debian.d/opt
cp -pr DEBIAN dreamy-debian.d/.
cp -r ./opt/dreamy dreamy-debian.d/opt/.
fakeroot dpkg-deb -Zgzip --build dreamy-debian.d Dreamy-2025.05.14_amd64.deb
make distclean
rm -fr ./opt
rm -fr dreamy-debian.d
