from refactored_m4 import *
import matplotlib.pyplot as plt
filename = "/data/Aug08_CFD/DataF_Muon_CFD_6_75perc_th_8_8_9_9_Slab_8Aug_20_0.root"
E,df_grouped_energy=ProcessData(filename,[0,1,2,3,4,5,6,7])
delt_list=df_grouped_energy["TimeDiff_0_2"]

zlist=[]
p0=1.52
p1=0.0022
p2=-0.000000435
p3=0.000000000795
for delt in delt_list:
    z = p0 + p1*delt + p2*delt**2 + p3*delt**3
    zlist.append(z)

print(zlist)
binss=np.linspace(-100,100,100)
plt.hist(zlist, bins=binss, histtype ='step', color ='red', alpha = 0.7,)
plt.show()
