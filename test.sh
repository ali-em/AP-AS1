#!/bin/bash
g++ A1-810197459.cpp;
for i in {1..30};do ./a.out < ./description/testcases/${i}.in > test.out ;
diff test.out ./description/testcases/${i}.sol 
done
