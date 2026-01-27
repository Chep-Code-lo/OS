#! /bin/bash
dir="$1"
month="$2"
day="$3"

[ ! -d "$dir" ] && echo "Ko ton tai " && exit 1

echo "File co quyen nhom la read va excute"
find "$dir" -type f -perm -g=rx ! -perm -g=w 2>/dev/null

echo -e "\nFile co thang $month va ngay $day"
[ -n "$month" ] && [ -n "$day" ] && find "$dir" -type f -exec stat -c "%y %n" {} \; 2>/dev/null | awk -v m="$month" -v d="$day" '$1 ~ "-"m"-"d {print $NF}' || echo "Nhap thieu tham so"

echo -e "\nfile co kich thuoc >= 500KB"
find "$dir" -type f -size +500k 2>/dev/null

echo -e "\nThu muc con ko co quyen ghi ... " 
find "$dir" -type d ! -writable 2>/dev/null