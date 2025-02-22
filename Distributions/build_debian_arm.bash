#!/usr/bin/env bash

# Alexis Megas.

if [ ! -x /usr/bin/dpkg ]
then
    echo "Please install dpkg."
    exit 1
fi

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

make distclean 2>/dev/null
mkdir -p ./opt/dreamy
qmake -o Makefile dreamy.pro && make -j $(nproc)
cp -p ./Dreamy ./opt/dreamy/.
mkdir -p dreamy-debian.d/opt

architecture="$(dpkg --print-architecture)"

if [ "$architecture" = "armhf" ]
then
    cp -pr DEBIAN-ARM32 dreamy-debian.d/DEBIAN
else
    cp -pr DEBIAN-ARM64 dreamy-debian.d/DEBIAN
fi

cp -r ./opt/dreamy dreamy-debian.d/opt/.
fakeroot dpkg-deb \
	 --build dreamy-debian.d Dreamy-2024.08.15_$(architecture).deb
make distclean
rm -fr ./opt
rm -fr dreamy-debian.d
