#!/bin/sh

base="1_000_000-snails-set-"
home=`pwd`
echo $home
for sset in `ls -d */ | cut -f1 -d"/"`
do
	echo "$sset"
	cd $sset
	# By default, all instances are launched simultaneously,
	# which impacts execution time even on high cpu cores
	# count setups. You can launch them one after another
	# to improve individual and overall results.
	../../build/src/ads-lab2-main > result.txt &
	cd $home
done
