#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Sep 27 10:31:26 2024

@author: g
"""

import numpy as np
import math
import matplotlib.pyplot as plt
import matplotlib.ticker as mtick
from pathlib import Path
import glob
import os
import re
import matplotlib.cm as cm
import matplotlib.colors as mcolors
import matplotlib.animation as animation
#from mpl_toolkits.mplot3d import Axes3D


#parameters=Parameter()
# omega=float(parameters["omega_in"])
# delta=float(parameters["delta"])
# slides=int(parameters["slides"])
# epsilon=float(parameters["epsilon"])
# gamma=float(parameters["gamma"])
# res=int(parameters["res"])
# offset=float(parameters["offset"])
# dx=(math.pi-2*offset)/res
file= os.path.dirname(os.getcwd())
file1= os.path.join(file,"output")


#plt.close()

 #%%   
    # Create the plot
fig, ax = plt.subplots()
# Label axes and title
norm = mcolors.Normalize(vmin=-1, vmax=1)
cmap = plt.colormaps.get_cmap('coolwarm')
sm = cm.ScalarMappable(cmap=cmap, norm=norm)
sm.set_array([])  # Only needed for the colorbar
cbar = plt.colorbar(sm, ax=ax, orientation='horizontal')
cbar.set_label('Velocity')

# Show the plot

dirFiles = os.listdir(file1+'/10') #list of directory files
dirFiles.sort(key=lambda f: int(re.sub('\D', '', f)))
os.chdir(file1+'/10')
count=0
def manual_plot():
    for file in dirFiles:
    
        w=np.loadtxt(file,delimiter=",",dtype=float)
      #  print(file)
        
        #plt.clf()
        x=(w[:,0])
        base=(w[:,1])
        height=(w[:,2]+w[:,1])
        velocity=(w[:,3])
        ax.cla()
        ax.set_xlabel("X")
        ax.set_ylabel("Height")
        ax.set_title("Heap deformation due to sublimation")
        plt.gca().set_aspect('equal', adjustable='box')
        plt.xlim(-5,5)
        plt.ylim(0,2)
    
        
        # Plot the water height profile as a line
        ax.plot(x, height, color='black', linewidth=0.1, label="Water Height")
        
        # Color the area under the curve using fill_between and colormap
        for i in range(0,len(x)-10,10):
            ax.fill_between(x[i:i+11], base[i:i+11], height[i:i+11], color=cmap(norm(velocity[i+10])))
            ax.fill_between(x[i:i+11], 0,  base[i:i+11], color=cmap(norm(0)))
        
        # Add a color bar
        
        plt.show()
        plt.pause(0.1)  
    
        #break
    
def animate(i):
    ax.cla()  # Clear the axis
    print(f"{i}/{len(dirFiles)}")
    # Load data from the file
    file = dirFiles[i]
    w = np.loadtxt(file, delimiter=",", dtype=float)

    x = w[:, 0]
    base = w[:, 1]
    #height=np.zeros(len(base))
    #for i in range(1,len(base)-1):
        
    height = w[:, 2] + w[:, 1]
        
    velocity = w[:, 3]

    ax.set_xlabel("X")
    ax.set_ylabel("Height")
    ax.set_title("Heap deformation due to sublimation")
    plt.gca().set_aspect('equal', adjustable='box')
    plt.xlim(-5, 5)
    plt.ylim(0, 2)

    # Plot the water height profile as a line
    ax.plot(x, height, color='black', linewidth=0.1, label="Water Height")

    # Color the area under the curve using fill_between and colormap
    for j in range(0, len(x)-10, 10):
        ax.fill_between(x[j:j+11], base[j:j+11], height[j:j+11], color=cmap(norm(velocity[j+10])))
        ax.fill_between(x[j:j+11], 0, base[j:j+11], color=cmap(norm(0)))

# Create the animation
ani = animation.FuncAnimation(fig, animate, frames=len(dirFiles), repeat=False)
#os.chdir(file1)

# Show the animation
plt.show()
ani.save("animations.mp4")