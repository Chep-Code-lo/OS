#! /bin/bash
temp=$(stat -c "%A" "$1")
owner="${temp:1:3}"
group="${temp:4:3}"
other="${temp:7:3}"
echo "$1 : owner=$owner, group=$group, other=$other"