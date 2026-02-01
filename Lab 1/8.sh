#! /bin/bash
read -r -p "Nhap : " -a arr
printf '%s\n' "${arr[@]}" | sort -n