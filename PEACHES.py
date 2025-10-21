import math
import random 
import numpy as np
import pandas as pd 

class Matrix: 
    def __init__(self,r,c):
        self.rows=r
        self.columns=c
        self.M=np.zeros((r,c))
    def Fill(self):
        for i in range(0,self.rows): 
            for j in range(0,self.columns): 
                self.M[i,j]= random.uniform(0.0,1.0) 
        return self.M
        
class NN:
    def __init__(self,input_layer,hidden_layer_one,hidden_layer_two,outputs):
        self.input_layer=input_layer
        self.hidden_layer_one=hidden_layer_one
        self.hidden_layer_two=hidden_layer_two
        self.outputs=Matrix(outputs,1)
        
        self.learning_rate=0.01

        self.X=Matrix(input_layer,1)
        self.Y=Matrix(outputs,1)
        self.b_1 = Matrix(hidden_layer_one,1)
        self.b_2=Matrix(hidden_layer_two,1)
        self.b_o=Matrix(outputs,1)

        self.W1=Matrix(hidden_layer_one,input_layer)
        self.W2=Matrix(hidden_layer_two,hidden_layer_one)
        self.W3=Matrix(outputs,hidden_layer_two)
    def sigma(self,x): 
        return np.tanh(x)
    def d_sigma(self,x): 
        return 1-(np.tanh(x))**2
    def Calculate_NN(self,Yhat): 
        #learning rate 
        nu=self.learning_rate

        #input vector
        x=self.X.Fill()

        #Weight Matrices 
        W1=self.W1.Fill()
        W2=self.W2.Fill()
        W3=self.W3.Fill()

        #Bias Vectors 
        b_1=self.b_1.Fill()
        b_2=self.b_2.Fill()
        b_o=self.b_o.Fill()

        for epoch in range(0,10): 
            Z1=(W1@x)+b_1
            A1=self.sigma(Z1)

            Z2=(W2@A1)+b_2
            A2=self.sigma(Z2)

            Z3=(W3@A2)+b_o
            O=self.sigma(Z3)

          
            print(f"Output: {O}")
            

            print(f"Error: {np.linalg.norm(O-Yhat)}")

            δ3=(O-Yhat)*self.d_sigma(Z3)
            δ2=(W3.T@δ3)*self.d_sigma(Z2)
            δ1=(W2.T@δ2)*self.d_sigma(Z1)

            dW3=δ3@A2.T
            dW2=δ2@A1.T
            dW1=δ1@x.T

            W3-=nu*dW3
            W2-=nu*dW2
            W1-=nu*dW1

            b_o-=nu*δ3
            b_2-=nu*δ2
            b_1-=nu*δ1


            '''
            dL/dW2= (DL/dA2)(dA2/dZ2)(dZ2/dW2)

            '''

            

def main(): 
    
    N_Network=NN(3,4,5,3) 
    Y=N_Network.Y
  
    true_outputs=Y.Fill()
    print(true_outputs)
    print()


    N_Network.Calculate_NN(true_outputs)
main()