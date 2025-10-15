//
//  MEKONG.hpp
//  RIVERS_XCODE
//
//  Created by Owner on 10/8/25.
//

#ifndef MEKONG_hpp
#define MEKONG_hpp

#include <stdio.h>
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

using namespace std;

const double PI=3.141592653589793238;

class WaveTwo{
    /*
     Wave Equation:
     
     utt = 16uxx
     u(0,t)=u(π,t)=0
     u(x,0)=sin^2(x) 0<x<π
     ut(x,0)=1-cos(x), 0<x<π
     
     Let u1 = u
         u2 = ut
     
     Then,
           u1_t = ut
           u2_t = utt
     So,
           u1_t = u2
           u2_t = 16*uxx
     */
    
    
 public:
    
    double x_low=0.0,x_high=PI;
    double t_low=0.0,t_high=5.0;
    double dt=0.01,dx=0.01;
    double rows=10,columns=10;
    double epsilon=1e-4;
    vector<vector<double>> u_grid;
    

    
    
    WaveTwo();
    vector<vector<double>> InitalizeGrid();
    double uxx(vector<vector<double>> u,int i, int j);
    double ut(vector<vector<double>> u,int i, int j);
    
    vector<vector<double>> FillUtGrid();
    void RK4();
    

};



#endif /* MEKONG_hpp */
