#!/bin/bash

if [ -e foo ]
then
    echo Expected foo to be removed after rm.
    exit 1
fi
