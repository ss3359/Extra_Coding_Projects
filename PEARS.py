import math 
import numpy as np 
import pandas as pd
import random 
import scipy as si
import sympy as sy

class NeuralNetwork: 
    def __init__(self,input_size,hidden_size,output_size):

        self.x_val=np.linspace(0,2*math.pi,4)
        self.y_target=np.sin(self.x_val)
        self.W=np.random.randn(4,4)*0.01
        self.W_h=np.random.randn(hidden_size,input_size)*0.01
        self.W_o=np.random.randn(output_size,hidden_size)*0.01

        self.O=np.zeros((4,1))
        self.b_w=np.zeros((4,1))
        self.b_o=np.zeros((1,1))
        self.H=np.zeros((4,1))
        self.learning_rate=0.01

    def sigmoid(self,x): 
        return 1/(1+np.exp(-x)) 
    def sigmoid_derivative(self,x):
        s= self.sigmoid(x)
        return s*(1-s)


def main(): 
    # Approximate the value of y = sin(x)
    NN= NeuralNetwork(1,4,1)
    W= NN.W
    H=NN.H
    O=NN.O
    b_w= NN.b_w
    b_o=NN.b_o
    L_Rate=NN.learning_rate

    X=np.linspace(0,2*math.pi,4).reshape(1,-1)
    Y=np.sin(X)

    for epoch in range(0,10):
        new_H=NN.sigmoid((NN.W_h.dot(X))+b_w)
        new_O=NN.sigmoid((NN.W_o.dot(new_H))+b_o)
       
        
        error=Y-new_O
        dO=error*NN.sigmoid_derivative(new_O)
        dH=(NN.W_o.T@dO)*NN.sigmoid_derivative(new_H)

        #Update Weights: 
        NN.W_o+=L_Rate*(dO@new_H.T)
        NN.W_h+=L_Rate*(dH@X.T)

        
        print("dO: ",dO.shape)
        print("new_H:",new_H.shape)
        print("W_o: ",NN.W_o.shape)
        
        print(f"Epoch {epoch}: mean error = {np.mean(np.abs(error)):.4f}")

    

main()