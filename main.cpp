/*************************************************************************
    File Name: main.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Fri 13 Apr 2018 10:26:52 AM EDT
*********************************************************************/
#include <sstream>
#include <iostream>
#include "genMC.hpp"
using namespace std;


int main( int argc, const char* argv[] ) {
    MycielskiGraphEngine g;
    g.write_mm_out("fuck.mtx");
    g.std_propagate(1);
    g.write_mm_out("wocao.mtx");
    return 0;
}

