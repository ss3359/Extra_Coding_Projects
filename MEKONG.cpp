//
//  MEKONG.cpp
//  RIVERS_XCODE
//
//  Created by Owner on 10/8/25.
//
#include <cmath>
#include <vector>
#include <thread>
#include <iostream>
#include <random>
#include <array>
#include <cassert>
#include <memory>
#include "TIGRIS.hpp"
#include "EUPHRATES.hpp"
#include "PISHON.hpp"
#include "GIHON.hpp"
#include "MEKONG.hpp"
using namespace std;



vector<vector<double>> WaveTwo::InitalizeGrid(){
    for(int i=0; i<rows-1; i++){
        double x=x_low+(i*dx);
        for(int j=0; j<columns-1; j++){
            if(j==0)
                u_grid[i][j]=sin(x)*sin(x);
            else
                u_grid[i][j]=0.0;
        }
    }
    return u_grid;
}

WaveTwo::WaveTwo():u_grid(rows,vector<double>(columns,0.0)){
}

double WaveTwo::uxx(vector<vector<double>> u, int i, int j){
    
    if(j<=0 || j>=columns-1){
        return u[i][j];
    }
    double result=(u[i][j-1]-2*u[i][j]+u[i][j+1])/(dx*dx);
    if(abs(result) < epsilon)
        return u[i][j];
    
    return result;
}


double WaveTwo:: ut(vector<vector<double>> u,int i, int j){
    
    if(i<=0 ||i>=rows-1){
        return 0.0;
    }
    double result=(u[i+1][j]-u[i-1][j])/(2.0*dt);
    if(abs(result)<epsilon)
        return u[i][j];
    return result;
}

vector<vector<double>> WaveTwo:: FillUtGrid(){
    
    vector<vector<double>> result(rows, vector<double>(columns, 0.0));

    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++){
            result[i][j]=ut(u_grid,i,j);
        }
    }
    return result;
}

void WaveTwo::RK4(){
    
    int iterations=10,n=0;
    
    vector<vector<double>> u1=InitalizeGrid();
    vector<vector<double>> u2=FillUtGrid();
    
    
    while(n<iterations){
        
        

        double K1,K2,K3,K4;
        double L1,L2,L3,L4;
       
        vector<vector<double>> new_u1=u1, new_u2=u2;

        for(int i=1; i<rows-1;i++){
            for(int j=1; j<columns-1;j++){
                K1=16*uxx(u1,i,j);
                L1=u2[i][j];
                
                double u1_1=u1[i][j]+0.5*dt*K1;
                double u2_1=u2[i][j]+0.5*dt*L1;
                
                
                K2=u2_1;
                L2=16*uxx(u1,i,j);
                
                double u1_2=u1[i][j]+0.5*dt*K2;
                double u2_2=u2[i][j]+0.5*dt*L2;
                
                K3=u2_2;
                L3=16*uxx(u1,i,j);
                
                double u1_3=u1[i][j]+0.5*dt*K3;
                double u2_3=u2[i][j]+0.5*dt*L3;
                
                K4=u2_3;
                L4=16*uxx(u1,i,j);
                
                
                u1[i][j]+=(dt/6.0)*(K1+2.0*K2+2.0*K3+K4);
                u2[i][j]+=(dt/6.0)*(L1+2.0*L2+2.0*L3+L4);
                
            }
        }
        
        cout<<"Iteration "<<n<<":"<<endl;
        for(int i=0; i<rows; i++){
            for(int j=0; j<columns; j++){
                cout<<u_grid[i][j]<<" ";
            }
            cout<<endl;
        }
        n++;
    }
}

