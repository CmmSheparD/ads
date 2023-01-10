#!/bin/sh

base="set-"
home=`pwd`
echo $home
for n in `seq 1 10`
do
	d="$base$n"
	echo "$d"
	mkdir $d
	cd $d
	../../build/src/ads-lab3-generate
	cd $home
done
