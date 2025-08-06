#!/bin/bash

echo "Menu Test Program"

stop=0
while test $stop -eq 0
do
    cat << ENDOFMENU
    
1: Print the date
2,3: Print the current working directory
4: Exit
ENDOFMENU

    echo
    echo "Your choice? "
    read reply
    echo

    case $reply in
        "1")
            date
            ;;
        "2" | "3")
            pwd
            ;;
        "4")
            stop=1
            ;;
        *)
            echo "Illegal choice"
            ;;
    esac
done
