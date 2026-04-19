#!
#rm -rf MacDir/*
#rm -rf OutDir/*
#for x in $(seq -20 5 20); do
for x in 0; do
  for y in $(seq -20 2 20); do
    echo "$x $y"
    outfilename="OutDir/Muon_output_${x}_${y}_data.root"
    ./NonSegmented skel.mac $outfilename $x $y 
    #echo $outfilename
    #rm -rf $filename
  done
done

#x=-23;
#y=23
#filename="muon_${x}_${y}.mac"
#sed "s/xpos/${x}/g" skel.mac > $filename 
#sed "s/ypos/${y}/g" -i $filename 
#./NonSegmented $filename
#rm -rf $filename
