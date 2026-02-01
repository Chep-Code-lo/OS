#! /bin/bash
n=$1
sum=0
while [ "${#n}" -gt 0 ]; do
    d="${n:0:1}"
    sum=$((sum + d))
    n="${n:1}"
done
echo "$sum"