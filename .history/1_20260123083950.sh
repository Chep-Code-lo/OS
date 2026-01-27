#! /bin/bash

if [ $# -ne 2 ]; then
    echo "Usage: $0 <a> <b>"
    exit 1
fi

a="$1"; b="$2"
if ! [[ "$a" =~ ^-?[0-9]+$ && "$b" =~ ^-?[0-9]+$ ]]; then
    echo "Error :...."
    exit 1
fi

echo $((a + b))