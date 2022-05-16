#!/usr/bin/bash

mkdir -p ./opt/dreamy
qmake -o Makefile dreamy.pro && make -j $(nproc)
cp -p ./Dreamy ./opt/dreamy/.
mkdir -p dreamy-debian.d/opt
cp -pr DEBIAN dreamy-debian.d/.
cp -r ./opt/dreamy dreamy-debian.d/opt/.
fakeroot dpkg-deb -Zgzip --build dreamy-debian.d Dreamy-2022.05.15_amd64.deb
make distclean
rm -fr ./opt
rm -fr dreamy-debian.d
