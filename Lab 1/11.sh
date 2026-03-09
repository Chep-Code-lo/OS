#! /bin/bash
dir="$1"
month="$2"
day="$3"

[ ! -d "$dir" ] && echo "Ko ton tai " && exit 1

echo "File co quyen nhom la read va excute"
find "$dir" -type f -perm -g=rx ! -perm -g=w 2>/dev/null

echo -e "\nFile co thang $month va ngay $day"
if [ -n "$month" ] && [ -n "$day" ]; then
    ls -l --time-style=+"%b %d" "$dir" 2>/dev/null | grep "$month $day" | awk '{print $NF}'
else
    echo "Nhap thieu tham so"
fi
find "$dir" -type f -size +500k 2>/dev/null

echo -e "\nThu muc con ko co quyen ghi ... " 
find "$dir" -type d ! -writable 2>/dev/null