#! /bin/bash
echo "$1 : $(stat -c "%A" "$1" | cut -c2-4)