#! /bin/bash
getent group "$1" | cut -d: -f4 | tr ',' '\n' | sort