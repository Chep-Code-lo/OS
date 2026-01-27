#! /bin/bash
getent gruop "$1" | cut -d: -f4 | tr ',' '\n' | sort