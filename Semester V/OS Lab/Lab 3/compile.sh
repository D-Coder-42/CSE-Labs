#!/bin/bash

case "$1" in
    *.c)
        cc "$1"
        ;;
    *.h | *.sh)
        # do nothing
        ;;
    *)
        echo "Unsupported file type"
        ;;
esac
