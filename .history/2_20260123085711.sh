#! /bin/bash

if [ $# -ne 3 ]; then
    echo "Nhap dung vo "
    exit 1
fi

a="$1"; op="$2"; b="$3"

if ! [[ "$a" =~ ^-?[0-9]+$ && "$b" =~ ^-?[0-9]+$ ]]; then
    echo "Nhap vao la so"
    exit 1
fi

case "$op" in
    "+") echo $((a + b)) ;;
    "-") echo $((a - b)) ;;
    "x") echo $((a * b)) ;;
    "/" )
        if [ "$b" -eq 0 ]; then echo "Chia thi b khac ko"; exit 1; fi
        echo $((a / b))
        ;;
    *) echo "Dung dung phep toan"; exit 1 ;;
esac