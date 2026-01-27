#! /bin/bash
read -r -p "Nhap : " -a str
echo "So tu : ${#($str[@])}"
print '%s\n' "{$str[@]}"