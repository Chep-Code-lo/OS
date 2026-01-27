#! /bin/bash

a="$1"; op="$2"; b="$3"
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