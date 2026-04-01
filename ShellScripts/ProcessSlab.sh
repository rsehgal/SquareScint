#!/bin/bash
flname="Slab_LongRun_"
for i in $(seq 0 1 3); do
    for j in $(seq 0 1 23); do

	    filename=$flname""$i"_"$j".root"
	    #echo $filename
            outfilename="output_day_"$i"_hour_"$j".root"
            #echo $outfilename
	    if [ -f $filename ]; then 
	        echo $filename
	        ./ProcessData2_Slab $filename $outfilename
            fi 
    done
    #echo $i
done
