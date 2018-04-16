/*************************************************************************
    File Name: main.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Fri 13 Apr 2018 10:26:52 AM EDT
*********************************************************************/
#include <sstream>
#include <iostream>
#include <cstring>   //strcmp
#include "genMC.hpp"
using namespace std;


int main( int argc, const char* argv[] ) {
        
    int s=-1;
    string seed_file="";
    string out_file="";
    bool bRnd=false;

    for(int i=1; i<argc; i++){
        if(strcmp(argv[i],"-s")==0) { s=atoi(argv[++i]); }
        else if(strcmp(argv[i], "-f")==0) { seed_file = argv[++i]; }
        else if(strcmp(argv[i], "-o")==0) { out_file  = argv[++i]; }
        else if(strcmp(argv[i], "-r")==0) { bRnd=true; }
        else {
            fprintf(stdout, "Waringing! Unused input argument '%s'\n",argv[i]);
        }
    }

    if(s<0){
        fprintf(stderr, "\nUsage: ./MCEngin.out -s <propagate steps> [-f <seeds_files>] [-r] [-o <output_files>]\n"
                        "\n"
                        "### Readme\n"
                        "     This program 'MCEngine.out' a.k.a 'Mycielski Graph Engine' would generate Mycielski graphs accoridng different policies/algorihtm/heuristics, such as *standard Mycielski graphs with/without specified seed* (each steps duplicates nodes, connectes corresponding nodes's neigbhors, then adding extral node connecting all the duplicated nodes.); *random Mycielski graph* with/without specified seed (each step duplicate nodes, randomly connected corresponding nodes's neighbors. then adding extral node connecting all the duplicated nodes.). The initial graph is the seed graph, default graph is K2. In case of problems, email cheng172@purdue.edu\n"
                        "\n"
                        "     -s <s>       : The graph will be propagate s steps.\n"
                        "     -f <in_file> : The input seed file. \n"
                        "     -r           : With this flag, the random Mycielski rules will replace standard Mycielski rules.\n"
                        "     -o <out_file>: The output seed file. \n"
                        "\n"
                        "### Examples:\n"
                        "# standard mycielski M5 (K2 propagate 3 steps) graphs:\n"
                        "     $./MCEngin.out -s 3\n"
                        "\n"
                        "# standard mycielski graph propagate 2 steps using seed graphs:\n"
                        "     $./MCEngin.out -s 2 -f bcsstk01.mtx\n"
                        "\n"
                        "# random mycielski graph propagate 1 steps using seed graphs, and the specify output filename:\n"
                        "     $./MCEngin.out -s 1 -o rnd_bcsstk01_mc1.mtx -f bcsstk01.mtx -r\n"
                        "\n"
               );
        fprintf(stderr, "'MCEngin.out' aka '%s' on this computer.\n",argv[0]);
        return 0;
    }

    if(out_file.empty()){
        stringstream ss;
        if(bRnd) ss<<"rnd_";
        if(!seed_file.empty()){ 
            auto tmp = seed_file.substr(seed_file.find_last_of("/\\")+1);
            ss<<tmp.substr(0, tmp.find_last_of("."));
        }
        ss<<"mc_s"<<s<<".mtx";
        out_file=ss.str();
    }

    MycielskiGraphEngine *g=new MycielskiGraphEngine(seed_file);
    if(bRnd) g->rnd_propagate(s);
    else     g->std_propagate(s);
    //g->dump();
    g->write_mm_out(out_file);
    delete g;
    return 0;
}

