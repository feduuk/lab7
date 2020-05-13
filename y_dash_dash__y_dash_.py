import numpy as np
import matplotlib.pyplot as plt
data1=np.loadtxt("file_y2.txt")
plt.plot(data1[:,0],data1[:,1])
plt.grid(True)
plt.show()

