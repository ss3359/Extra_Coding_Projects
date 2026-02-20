import math 
import random 
import numpy as np 
import pandas as pd 
import scipy as si 
import sympy as sp 

'''
We are going to recreate the orbit of John Glenn along the 
Mercury Atlas-6 space capsule 

This will be derived from Newtons Second Law and Newtons 
Law of Universal Gravitation 

F= G*M*m/r**2; 
F= m*r''. So we will have the following vector ODE 

mr'' = ((G*M*m)/r**2)r 

r'' = (G*M/r**2)(r).Also, we need to normalize the r vector, Therefore we will have the finalized 
ODE. 

rn'' = (G*M/||rn||**3)rn. 

we can transform this second order ODE into a system of first order nonlinear ODE's

Let r = rn and v= rn' => r' = rn' and v' = rn'' 

So, r' = v
    v' = (-G*M/||r||**3)r
'''

# Constants 

G = 6.6743e-11 # Gravitational Constant m^3 kg^-1 s^-2
M = 5.972e24 # Mass Of The Earth kg
R_Earth=6378 # Earths Radius, km

rp= R_Earth+159  # Perigee Altitude km
ra=R_Earth+265  # Apogee Altitude km 
theta=32.5 # The inclinaton (degrees)
period =88.5 # period of the spacecraft (minutes)

mu=398600 # This is G*M in km^3/s^2



class Orbit: 

    def __init__(self,r,v):
        self.r=r 
        self.v=v
        
 
    
    def DVEL(self,r,v): 
        return (-mu/np.linalg.norm(r)**3)*r 
    def DPOS(self,r,v): 
        return v
    

    def Radius(self,v): 
        return np.linalg.norm(v)
    def Theta(self,v): 
        return math.atan2(v[1],v[0])
    
    '''
    In order to show valid conical orbit, we need to show the Angular Momentum (H), and 
    Mechanical Energy stay constant
    '''

    # The function below defines angular momentum 
    def H(self,r,v): 
        r_cross_v=np.linalg.cross(r,v) 
        return np.linalg.norm(r_cross_v)
    
    #The function below defines mechanical energy. 
    def E(self,r,v):
        P=(1/2)*(np.linalg.norm(v)**2) 
        K=(mu)/np.linalg.norm(r)

        return P-K
    
    # Now the function below represents the conical trajectory of the spacecraft. If this 
    # is correct, this should represent an elliptical path 
    def r_conic(self,r,v):
        h=self.H(r,v) 
        p = (h**2)/(mu)
        theta=self.Theta(r)
        E=self.E(r,v)

        print("Angular Momentum: ", h)
        print("Mechanical Energy: ", E)
        print("Angle (Theta): ", theta)
        e = np.sqrt(1+((2*E*h**2)/mu**2))

        r_c=p/(1+(e*math.cos(theta)))
        print(f"New Elliptical Point: {r_c}")
       
 
    #Now the main iteration will come in with the most common iteratice method, the RK4 method

    def RK4(self,r,v): 
        h=1
        n=10 

       
        for _ in range(n): 
         
            J1=self.DPOS(r,v)
            K1=self.DVEL(r,v)

            J2=self.DPOS(r+(h/2.0)*J1,v+(h/2.0)*K1)
            K2=self.DVEL(r+(h/2.0)*J1,v+(h/2.0)*K1)

            J3=self.DPOS(r+(h/2.0)*J2,v+(h/2.0)*K2)
            K3=self.DVEL(r+(h/2.0)*J2,v+(h/2.0)*K2)

            J4=self.DPOS(r+(h*J3),v+(h*K3))
            K4=self.DVEL(r+(h*J3),v+(h*K3))

            r=r +((h/6.0)*(J1+2.0*J2+2.0*J3+J4))
            v=v +((h/6.0)*(K1+2.0*K2+2.0*K3+K4))

            print(f"New Position: {r}")
            print(f"New Velocity: {v}")

            
            self.r_conic(r,v)
            print()

# The main method    
def main(): 
    r=np.array([6537,0,0]) # Inital position vector (km)
    v=np.array([0,7.9,0]) #Initial velocity vector (km/s)
    
    O=Orbit(r,v)
    O.RK4(r,v)

main()