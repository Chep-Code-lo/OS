#! /bin/bash
read -r -p "Nhap chuoi: " str
arr=($str)
echo "So tu: ${#arr[@]}"
printf '%s\n' "${arr[@]}"
