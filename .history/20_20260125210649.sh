#! /bin/bash
dir="/usr/log/tmp"
size=$(du -sm "$dir" 2>/dev/null | cut -f1)

[ "$size" -gt 0.1 ] 2>/dev/null && { 
    mkdir -p /backup 
    cp -r "$dir"/* /backup/
    echo "Da copy $size MB sang /backup"
} || echo "Thu muc khong du lon ($size MB)"