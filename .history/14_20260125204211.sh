#! /bin/bash
echo "Dong chan - chu hoa:"
awk 'NR % 2 == 0 {print toupper($0)}' "$1"

echo -e "Dong chua 'st':"
grep "st" "$1"
echo "So dong: $(grep -c "st" "$1")"