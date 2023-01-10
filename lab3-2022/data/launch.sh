#!/bin/sh

home=`pwd`

for sset in `ls -d */ | cut -f1 -d"/"`
do
	sset="set-$n"
	echo "$sset"
	cd $sset
	uptime
	# By default, all instances are launched simultaneously,
	# which impacts execution time even on high cpu cores
	# count setups. You can launch them one after another
	# to improve individual and overall results.
	../../build/src/ads-lab3-measure
	uptime
	cd $home
done
