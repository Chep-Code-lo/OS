#! /bin/bash
max="$1"
[ "$2" -gt "$max" ] && max="$2"
[ "$3" -gt "$max" ] && max="$3"
echo $max