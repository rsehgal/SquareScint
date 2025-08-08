from refactored_m4 import *
import matplotlib.pyplot as plt
filename = "/data/Aug08_CFD/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_8Aug_0_20.root"
E,df_grouped_energy=ProcessData(filename,[0,1,2,3,4,5,6,7])
delt_list=df_grouped_energy["TimeDiff_0_2"]

zlist=[]
p0=2.86526
p1=0.005
p2=-0.000000347
p3=0.000000000795
for delt in delt_list:
    z = p0+p1*delt+p2*pow(delt,2)#p3*pow(delt,3)+
    z = 8.*delt
    zlist.append(z)

print(zlist)
binss=np.linspace(-15,15,200)
plt.hist(zlist, bins=binss, histtype ='step', color ='red', alpha = 0.7,)
plt.show()
