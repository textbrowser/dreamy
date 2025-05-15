#!/usr/bin/env bash

# Alexis Megas.

VERSION=$1

if [ -z "$VERSION" ]
then
    echo "Please specify the version: $0 <VERSION>."
    exit 1
fi

# Debian release packages.

for FILE in */control
do
    sed -i "s/Version: .*/Version: $VERSION/" $FILE
done

for file in Distributions/build*
do
    sed -i \
    "s/Dreamy-[[:digit:]]\+\.[[:digit:]]\+\.[[:digit:]]\+/Dreamy-$VERSION/" \
    $file
done

# Version configuration.

FILE="Source/dreamy_version.h"

sed -i 's/\(DREAMY_VERSION "\)[0-9]\+\(\.[0-9]\+\)*"/\1'"$VERSION"'"/' \
    $FILE

# Android manifest file.

FILE="Android/AndroidManifest.xml"

sed -i \
    's/\(android:versionCode="\)[0-9]\+\([0-9]\+\)*"/\1'"${VERSION//./}"'"/' \
    $FILE
sed -i \
    's/\(android:versionName="\)[0-9]\+\(\.[0-9]\+\)*"/\1'"$VERSION"'"/' \
    $FILE
