#!/bin/sh

echo convert 5.4.0 release 1
echo convert test script - open systems environments
echo ================================================================================

if [ -d out ]; then
    rm -rf out
fi

mkdir out

echo
echo test case 01: convert Linux to DOS formatted text file
echo --------------------------------------------------------------------------------
echo

./ltod -i ltod-l.dat -o out/dtol-d.dat -v

echo
echo test case 02: convert DOS to Linux formatted text file
echo --------------------------------------------------------------------------------
echo

./dtol -i dtol-d.dat -o out/ltod-l.dat -v

echo
echo test case 03: convert Linux to DOS formatted text file and write over input
echo --------------------------------------------------------------------------------
echo

cp ltod-l.dat out/ltod-l-overwrite.dat

./ltod -i out/ltod-l-overwrite.dat -w -v

echo
echo test case 04: convert DOS to Linux formatted text file and write over input
echo --------------------------------------------------------------------------------
echo

cp dtol-d.dat out/dtol-d-overwrite.dat

./dtol -i out/dtol-d-overwrite.dat -w -v
