# -*- coding: utf-8 -*-
"""
Created on Thu Oct 11 10:51:20 2018

@author: sama
"""

import numpy as np
import matplotlib.pylab as plt
import matplotlib as mpl

path='/home/sama/Documents/enki-1/examples/build-playground-Desktop-Debug/'
spath='/home/sama/Documents/publications/sharedCorel/'

sizeY=3
ratioYX=4
my_dpi=600
imgSize=2

class predictor():
    
    def __init__ (self, name): #name is the number or colomn
        self.name=name
        self.data=np.loadtxt('{}predictors.tsv'.format(path))

        self.filt1=np.loadtxt('{}fp1.tsv'.format(path))
        self.filt2=np.loadtxt('{}fp2.tsv'.format(path))
        self.filt3=np.loadtxt('{}fp3.tsv'.format(path))
        self.filt4=np.loadtxt('{}fp4.tsv'.format(path))
        self.filt5=np.loadtxt('{}fp5.tsv'.format(path))
        
        self.filtdata1= self.filt1[:,self.name-1]
        self.filtdata2= self.filt2[:,self.name-1]
        self.filtdata3= self.filt3[:,self.name-1]
        self.filtdata4= self.filt4[:,self.name-1]
        self.filtdata5= self.filt5[:,self.name-1]

        self.predictor=self.data[:,self.name-1]

        
    def plotPredictor(self,error):
        
        
        fig=plt.figure('predictor%d' %self.name, figsize=(3,1), dpi=my_dpi)
        ax=fig.add_subplot(111)
        
        plt.plot(self.predictor/max(abs(self.predictor)), color='silver', linewidth=0.3)
        
        #plt.plot(self.filtdata1/max(abs(self.predictor)), color='grey', linestyle='dashed', linewidth=0.2)
        #plt.plot(self.filtdata2/max(abs(self.predictor)), color='grey', linestyle='dashed', linewidth=0.2)
        #plt.plot(self.filtdata3/max(abs(self.predictor)), color='grey', linestyle='dashed', linewidth=0.2)
        #plt.plot(self.filtdata4/max(abs(self.predictor)), color='grey', linestyle='dashed', linewidth=0.2)
        plt.plot(self.filtdata5/max(abs(self.predictor)), color='grey', linestyle='dashed', linewidth=0.2)
        
        plt.plot(error, color='black', linewidth=0.5)
        
        #plt.ylabel('Predictor %d' %self.name)
        plt.ylim(-1.1, 1.1)
        plt.yticks([-1,0,1])
        plt.xticks([0,len(self.predictor)])
        ax.set_aspect(aspect=len(self.predictor)/(7))
        plt.show()
        fig.savefig(spath+'predictor%d' %self.name, quality= 100, format='svg', bbox_inches='tight')


class neuron():
    
    def __init__ (self, name):
        self.name=name
        self.data=np.loadtxt('{}neuronWeight{}.txt'.format(path, self.name))
        self.numInputs=self.data.shape[1]
        
    def plotWeights(self):
        wfig=plt.figure('neuron%d' %self.name, figsize=(sizeY,sizeY), dpi=my_dpi)    
        axw=wfig.add_subplot(111)
        for i in range(self.numInputs):
            maxD=max((self.data[:,i]))
            minD=min(self.data[:,i])
            plt.plot((self.data[:,i]-minD)/(maxD-minD), color='black')
            plt.ylabel('Weight %d' %self.name)
            plt.ylim(-0.1, 1.1)
            plt.xticks([0,len(self.data[:,i])])
            plt.yticks([0,1])
        
        axw.set_aspect(aspect=len(self.data[:,0])/1.2)
        wfig.savefig(path+'BP_weights%d' %self.name, quality= 95, format='svg', bbox_inches='tight')
        
class layer():
    
    def __init__ (self, name):
        self.name=name
        self.data=np.loadtxt('{}layerWeight{}.txt'.format(path, self.name))
        self.data=self.data/(np.amax(abs(self.data)))
        self.numNeurons=self.data.shape[0]
        self.numInputs=self.data.shape[1]
        self.ratio=self.numInputs/self.numNeurons

    def plotLayerWeights(self):
        
        print(np.amax(self.data))
        print(np.amin(self.data))
        fig, ax = plt.subplots()
        myimage=ax.imshow(self.data,cmap='gray',interpolation='none')
        fig.colorbar(myimage,ax=ax)
        
#        imgfig=plt.figure('layer%d' %self.name, figsize=(imgSize*self.ratio,imgSize), dpi=my_dpi)
#        aximg=imgfig.add_subplot(111)
#        plt.imshow(self.data ,cmap='gray',interpolation='none')
        plt.gca().set_yticks(np.arange(0,self.numNeurons,2))
        plt.gca().set_xticks(np.arange(0,self.numInputs,5))
        ax.set_aspect(aspect='equal')
        fig.savefig(spath+'layer%d' %self.name, quality= 100, format='svg', bbox_inches='tight')
        
class finalLayer():
    
    def __init__ (self, name):
        self.name=name
        self.dataRaw=np.loadtxt('{}layerWeight{}.txt'.format(path, self.name))
        self.data=np.array([self.dataRaw,self.dataRaw])
        self.numInputs=self.data.shape[0]
        self.numNeurons=1
        self.ratio=self.numInputs/self.numNeurons

    def plotFinalLayerWeights(self):
        
        #imgfig=plt.figure('Flayer%d' %self.name, figsize=(imgSize*self.ratio/2,imgSize/2), dpi=my_dpi)
        #aximg=imgfig.add_subplot(111)
        fig, ax = plt.subplots()

        myimage=ax.imshow(self.data,cmap='gray',interpolation='none')
        fig.colorbar(myimage,ax=ax)
        #plt.gca().set_yticks(np.arange(0,self.numNeurons,1)+0.5)
        #plt.gca().set_yticklabels(['n1'])
        #plt.gca().set_xticks(np.arange(0,self.numInputs,1))
        
        #aximg.set_aspect(aspect=0.5)#'equal')
        plt.show()
        #imgfig.savefig(spath+'BP_Flayer%d' %self.name, quality= 95, format='svg', bbox_inches='tight')

#neuron11=neuron(11)
#neuron11.plotWeights()
#
#layer1=layer(1)
#layer1.plotLayerWeights()

#layer1=finalLayer(2)
#layer1.plotFinalLayerWeights()