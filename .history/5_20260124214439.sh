#! /bin/bash
while true; do
    echo "------------------------"
    echo "Main Menu"
    echo "------------------------"
    echo "[1] Show today date/time"
    echo "[2] Show all files in current directory"
    echo "[3] Show users"
    echo "[4] Show calendar"
    echo "[5] Exit/Stop"
    read -r -p "Choice [1-5]: " choice
    
    case "$choice" in
        1) date ;;
        2) ls -la ;;
        3) cut -d: -f1 /etc/passwd | sort ;;
        4) cal ;;
        5) echo "Bye"; exit 0 ;;
        *) echo "Chon dung "
    esac
done