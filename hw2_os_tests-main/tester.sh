#!/bin/bash
g++ -std=c++11 helper/helper.cxx hw2_test.cxx -o helper.out
make
echo "starting test..."
./test1.exe > tmp.txt
res=$(cat tmp.txt | grep "assert")
if [[ -z "$res" ]];then
	echo "TEST PASS"
else
	echo "TEST FAIL"
fi
make clean
