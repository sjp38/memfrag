#!/bin/bash

if [ ! `pidof memfrag` ]
then
	exit 0
fi

if grep --silent "running" /proc/`pidof memfrag`/status
then
	exit 1
else
	exit 0
fi
