#! /bin/bash
read -r -p "Nhap thu muc: " dir
[ ! -d "$dir" ] && echo "Ko ton tai " && exit 1
echo "File co quyen nhom la read va excute"
find "$dir" -type f -perm -g=rx ! -perm -g=w 2>/dev/null

echo -e "Ngay thang tao lap file"
read -r -p "Nhap ten file: " file
[ -f "$dir/$file" ] && stat -c "%y" "$dir/$file" | cut -d' ' -f1 || echo "Ko ton tai file"

echo -e "file co kich thuoc >= 500KB"
file "$dir" -type f -size +500k 2>/dev/null

echo -e "Thu muc con ko co quyen ghi ... " 
find "$dir" -type d ! -writable 2>/dev/null