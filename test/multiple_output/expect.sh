#!/bin/bash
echo foo | diff --color=always foo -
if [ $? != 0 ]
then
    exit 1
fi
echo bar | diff --color=always bar -
