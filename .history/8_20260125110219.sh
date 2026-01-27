#! /bin/bash
read -r -p "Nhap : " -a arr
echo "$("${arr[@]}" | sort -n)