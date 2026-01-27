#!/usr/bin/env bash
# Usage: ./q2_calc.sh 21 / 3

if [ $# -ne 3 ]; then
  echo "Usage: $0 <a> <op> <b>   (op: + - x /)"
  exit 1
fi

a="$1"; op="$2"; b="$3"

if ! [[ "$a" =~ ^-?[0-9]+$ && "$b" =~ ^-?[0-9]+$ ]]; then
  echo "Error: a, b must be integers."
  exit 1
fi

case "$op" in
  "+") echo $((a + b)) ;;
  "-") echo $((a - b)) ;;
  "x"|"*") echo $((a * b)) ;;
  "/")
    if [ "$b" -eq 0 ]; then echo "Error: division by zero"; exit 1; fi
    echo $((a / b))
    ;;
  *) echo "Error: invalid operator. Use + - x /"; exit 1 ;;
esac
