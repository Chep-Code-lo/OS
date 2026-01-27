#! /bin/bash
[ ! -d "SYS" ] && mkdir SYS
find . -maxdepth 1 -type f -printf "%TY-%Tm %p\n" | awk '$1 ~ /-07$/ {system("cp "$2" SYS/")}'
echo "Hoan thanh"