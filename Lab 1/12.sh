#! /bin/bash
[ -e "$1" ] && echo "Ton tai : $(realpath "$1")" || echo "Ko ton tai"