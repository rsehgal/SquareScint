#!/bin/bash
#CFD_Square_Bar1_Bar2_BG_915_890_926_938_1635_1555_1617_1582_0_0_12Mar_30min

#flname="CFD_Square_915_890_926_938_Cs_"
#flname="DataF_CFD_Square_Bar1_Bar2_BG_915_890_926_938_1635_1555_1617_1582_" #"0_0_12Mar_30min"
#tgtFolder="Mu_Slab/"
flname="DataF_CFD_Square_Cylinder_Stilbene_"
for i in $(seq -20 10 20); do
    for j in $(seq -20 10 20); do

	    filename=$flname""$i"_"$j"_23Mar_30min.root"
            #outfilename="output_"$filename
            outfilename="output_"$i"_"$j".root"
	    if [ -f $filename ]; then 
	        echo $filename
	        ./ProcessData2_SlabCylinder $filename $outfilename
            fi 
    done
    #echo $i
done
