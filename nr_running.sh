#!/bin/bash

pids=`pidof memfrag`

if [ pids == "" ]
then
	echo "0"
fi

NR_INSTANCE=0
for pid in $pids
do
	if [ `grep "running" /proc/$pid/status | wc -l` -eq 1 ]
	then
		NR_INSTANCE=$(( $NR_INSTANCE + 1 ))
	fi
done

echo $NR_INSTANCE
