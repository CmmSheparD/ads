#!/bin/sh

base="1_000_000-snails-set-"
home=`pwd`
echo $home
for sset in `ls -d */ | cut -f1 -d"/"`
do
	echo $sset
	cd $sset
	../../build/src/ads-lab2-main > result.txt &
	cd $home
done
