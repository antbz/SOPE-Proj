#!/bin/bash
for (( i = 1; i <= 1024; i++ ))
do
    (./simpledu ~/FEUP/1_ano -B $i -a || echo $?) | sort -k2 > testeT_simpledu.txt
    (du ~/FEUP/1_ano -B $i -l -a || echo $?) | sort -k2 > testeT_du.txt
    echo $i
    diff -q testeT_simpledu.txt testeT_du.txt > /dev/null 2>&1 && echo OK || echo FAILED
done