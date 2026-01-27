#! /bin/bash
echo "So dong: $(wc -1 < "$1")"
echo "So tu: $(wc -w < "$1")"
echo "Dong dai nhat:"
awk '{print length, $0}' "$1" | sort -nr | head -1 | cut -d' ' -f2-