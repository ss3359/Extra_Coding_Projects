//
//  main.cpp
//  RIVERS_XCODE
//
//  Created by Owner on 5/2/25.
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





int main(){
    
    WaveTwo w2;
    w2.RK4();
    
    
    
    return 0;
}








/*
 int add(int arr[], int pos, int size){
     if(pos>=size){
         return 0;
     }
     return arr[pos]+add(arr,pos+1,size);
 }

 int main(){

     int size=5;

     int numbers[size];
     
     for(int i=0; i<size; i++){
         numbers[i]= i+1;
     }
     
     cout<<"Sum of entries is "<<add(numbers,0, size)<<endl;
     
 */

/*
 | body |  m |           x |           y |  z |            vx |          vy| vz | | ---- | -: | ----------: | ----------: | -: | ------------: | ------------: | -: |
 | 1    |  1 | -0.97000436 |  0.24308753 |  0 |  0.4662036850 |  0.4323657300 |  0 |
 | 2    |  1 |  0.97000436 | -0.24308753 |  0 |  0.4662036850 |  0.4323657300 |  0 |
 | 3    |  1 |  0.00000000 |  0.00000000 |  0 | -0.9324073700 | -0.8647314600 |  0 |

 */
//    double s0 = 69.21, i0=10;
//    SIR s(s0, i0);
//    s.UpdateSIR();
/*
 Body b1({-9.7,2.4,0.0},{ 4.6,4.3,0.0},1.0);
 Body b2({9.7,-2.4,0.0},{4.6,4.3,0.0},1.0);
 Body b3({0.0,0.0 ,0}, {-9.3,-8.6,0.0},1.0);
 
 vector<Body> Bodies={b1,b2,b3};
 
 
 RK45_Method(Bodies);
 */

/*
 template<class S>


 S EXP(S num, S power){
     
     if(power<=0){
         return 1;
     }
     return num*EXP(num,power-1);
 }

 int main(){
     int base=3,power=4;
     
     cout<<base<<"^"<<power<<"="<<EXP(base, power)<<endl;
     
     assert(EXP(2,4)==16);
     assert(EXP(4,3)==64);
     cout<<"The Tests Are Passing!"<<endl;
     return 0;
 }

 */
