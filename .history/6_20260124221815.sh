#! /bin/bash
is_prime(){
    n=$1
    [[ ! "$n" =~ ^[0-9]+$ || "$n" -lt 2 ]] && return 1
    [ "$n" -eq 2 ] && return 0
    [ $((n % 2)) -eq 0 ] && return 1
    for ((i=3; i*i<=n; i+=2)); do
        [ $((n % i)) -eq 0 ] && return 1
    done
    return 0
}

for x in "$@"; do is_prime "$x" && echo "$x"; done