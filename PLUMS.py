import math 
import random
import numpy as np 
import pandas as pd 
import scipy as sp 
import sympy as sp 
from PEACHES import Matrix
from sklearn.model_selection import train_test_split



class MusicNN: 
    def __init__(self,inputs,hidden1, hidden2, hidden3 ,outputs):
        self.inputs=inputs
        self.hidden1=hidden1
        self.hidden2=hidden2
        self.hidden3=hidden3
        self.outputs=outputs
        self.nu=0.01 # learning rate
        
    def InputAndWeightMatrices(self): 
        A1=Matrix(self.hidden1,self.inputs)
        A2=Matrix(self.hidden2,self.hidden1)
        A3=Matrix(self.hidden3,self.hidden2)
        A4=Matrix(self.outputs,self.hidden3)
        A1=A1.Fill()
        A2=A2.Fill()
        A3=A3.Fill()
        A4=A4.Fill()

        return [A1,A2,A3,A4]
    
    def InputVector(self): 
        x=Matrix(self.inputs,1)
        x=x.Fill()
        return x
    
    def Biases(self): 
        b_1=Matrix(self.hidden1,1)
        b_2=Matrix(self.hidden2,1)
        b_3=Matrix(self.hidden3,1)
        b_o=Matrix(self.outputs,1)
        
        b_1=b_1.Fill()
        b_2=b_2.Fill()
        b_3=b_3.Fill()
        b_o=b_o.Fill()
        return [b_1,b_2,b_3,b_o]
    
    def sigma(self,x): 
        return (1+np.exp(x))**-1
    
    def d_sigma(self,x): 
        return -((1+np.exp(x))**-2)*(np.exp(x))
    
    def CalulateForEpoch(self): 
        total=10
        W=self.InputAndWeightMatrices()
        b=self.Biases()
        x=self.InputVector()
        W1=W[0]
        W2=W[1]
        W3=W[2]
        W4=W[3]

        b_1=b[0]
        b_2=b[1]
        b_3=b[2]
        b_o=b[3]

        y_hat=Matrix(self.outputs,1)
        y_hat=y_hat.Fill()
        for eopch in range(total): 
            Z1=(W1@x)+b_1
            A1=self.sigma(Z1)

            Z2=(W2@A1)+b_2
            A2=self.sigma(Z2)

            Z3=(W3@A2)+b_3
            A3=self.sigma(Z3)

            Z4=(W4@A3)+b_o
            O=self.sigma(Z4)
            #Error Function 
            J=np.linalg.norm(O-y_hat)

            #Backpropgation 
            # print(f"W4: {W4}")
            # print(f"d_sigma: {self.d_sigma(Z4)}")
            δ4=(O-y_hat)@self.d_sigma(Z4)@W4.T

            # δ3=(A4_or_O-y_hat)*self.d_sigma(Z2)@W2
            # δ2=(A4_or_O-y_hat)*self.d_sigma(Z1)@W1



def main(): 
    M_nn= MusicNN(3,4,5,4,3)
    M_nn.CalulateForEpoch()
    pass

main()