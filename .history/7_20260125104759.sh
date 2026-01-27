#! /bin/bash
gcd(){
    a="$1"
    b="$2"
    r
    while [ "$b" -ne 0 ]; do
        r=$((a%b))
        a="$b"
        b="$r"
    done
    echo "$a"
}

ans="$1"
for n in "$@"; do
    ans=$(gcd "$ans" "$n")
done
echo "UCLN : $ans" 