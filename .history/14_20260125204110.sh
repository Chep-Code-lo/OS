#! /bin/bash
file="$1"
[ ! -f "$file" ] && echo "File khong ton tai" && exit 1

echo "Dong chan - chu hoa:"
awk 'NR % 2 == 0 {print toupper($0)}' "$file"

echo -e "\nDong chua 'st':"
grep "st" "$file"
echo "So dong: $(grep -c "st" "$file")"