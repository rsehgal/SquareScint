#!/bin/sh
fileIni="Muon_output"
for (( i=-20; i<=20; i+=5 ))
do
  for (( j=-20; j<=20; j+=5 ))
  do
    #echo "Current value: $i : $j"
    filename=$fileIni"_"$i"_"$j"_data.root"
    #echo $filename
    ./NonSegmented muon.mac $filename $i $j
  done
done

   
