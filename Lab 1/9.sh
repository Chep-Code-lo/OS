#! /bin/bash
read -r -p "Nhap : " -a str
echo "So tu : ${#str[@]}"
printf '%s\n' "${str[@]}"