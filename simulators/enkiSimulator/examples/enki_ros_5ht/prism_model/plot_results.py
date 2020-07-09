#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

exp = ['normal','normal_less_wait','drn_suppress','drn_suppress_less_wait','drn_suppress_ssri','drn_suppress_ssri_less_wait','drn_suppress_5ht2up','drn_suppress_5ht2up_less_wait']


pvals = np.loadtxt('pval_results.txt')

index = np.arange(len(pvals))
height = 0.35
fig, ax = plt.subplots()
rects = ax.barh(index+height*1.1,pvals,height,align='edge')
ax.set_xlabel('r')
ax.set_title('Reward probability')
ax.set_yticks(index + height)
ax.set_yticklabels(exp)
plt.show()
