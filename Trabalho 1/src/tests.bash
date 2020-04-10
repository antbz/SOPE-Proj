#!/bin/bash
for (( i = 1; i <= 5120; i++ ))
do
    (./simpledu ../test_dir -b -B $i -a --max-depth=2|| echo $?) | sort -k2 > testeT_simpledu.txt
    (du ../test_dir -b -B $i -l -a --max-depth=2 || echo $?) | sort -k2 > testeT_du.txt
    echo $i
    diff -q testeT_simpledu.txt testeT_du.txt > /dev/null 2>&1 && echo OK || echo FAILED
done