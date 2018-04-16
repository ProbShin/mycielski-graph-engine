/*************************************************************************
    File Name: main.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Thu 12 Apr 2018 02:16:55 PM EDT
*********************************************************************/
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <random>
#include "genMC.hpp"

using namespace std;

// ============================================================================
// read file into CSR format
// ============================================================================
void AdjGraph::reset(const string &f){
    if(f.empty()){
        fprintf(stderr, "ERR! The matrix file is empty\n");
        exit(1);
    }

    if(f.size()>4 && 
       f.substr(f.size()-4)==".mtx") {
        read_mm_struct_only(f);
    }
    else{
        fprintf(stderr,"Err! the matrix file %s cannot be read. Currently only supported .mtx format formt.\n", f.c_str());
    }
}

// ============================================================================
// read_mm_struct_only(f);
// ============================================================================
void AdjGraph::read_mm_struct_only(const string&file_name){
    G_.clear();
    G_.shrink_to_fit();
    bool b_symmetric = true;
    string line;
    istringstream ss;
    int edge_count=0;
    if(file_name.empty()) {
        fprintf(stderr,"ERR! The matrix file is empty.\n");
        exit(1);
    }
   

    ifstream in(file_name.c_str());
    if(!in) {
        fprintf(stderr,"Err! the matrix file %s cannot be open.\n", file_name.c_str());
        exit(1);
    }
    getline(in,line);
    ss.str(line);
    
    {
        string Tag1,Tag2,Tag3,Tag4,Tag5;
        ss>>Tag1>>Tag2>>Tag3>>Tag4>>Tag5;
        if(Tag1!="%%MatrixMarket" || Tag2!="matrix" ) {
            fprintf(stderr,"Err! Could not process Matrix Market file %s 's banner %s.\n",file_name.c_str(), line.c_str());
            exit(1);
        }
        if(Tag3!="coordinate") {
            fprintf(stderr,"Err! only support sparse matrix, %s 's banner %s is not.\n",file_name.c_str(), line.c_str());
            exit(1);
        }
        if(Tag5=="general") {
            fprintf(stderr,"Warning, the non-symmetric matrix %s, will be convert to symmetric by adding corresponding edge.\n", file_name.c_str());
            b_symmetric=false;
        }
    }
    
    do getline(in,line);
    while(!line.empty() && line[0]=='%');
    
    if(line.empty()) {
        fprintf(stderr, "Err! file %s Matrix Market format is wrong? Meet a blank line?\n", file_name.c_str());
        exit(1);
    }

    ss.str(line);
    int nrows, ncols, nnz;
    ss>>nrows>>ncols>>nnz;
    G_.resize(max(nrows, ncols));
    int entry_counter=0;
    if(b_symmetric) {
        do{
            getline(in,line);
            if(line.empty() || line[0]=='%') 
                continue;
            entry_counter++;
            ss.str(line);
            int row,col;
            ss>>row>>col;
            if(row<col) {
                fprintf(stderr,"Err! Find an entry (row,col)=(%d, %d) in upper part of symmtric matrix %s.\n", row, col, file_name.c_str());
                exit(1);
            }
            row--; col--;
            G_[row].push_back(col);
            G_[col].push_back(row);
            edge_count+=2;
        }while( (!in.eof()) && entry_counter<nnz);
    }  // end of if b_symmetric
    else{
        do{
            getline(in, line);
            if(line.empty() || line[0]=='%')
                continue;
            entry_counter++;
            ss.str(line);
            int row, col;
            ss>>row>>col;
            row--; col--;
            G_[row].push_back(col);
            G_[col].push_back(row);
        }while((!in.eof()) && entry_counter<nnz);
        for(auto &vec : G_){
            sort(vec.begin(), vec.end());
            vec.erase( unique(vec.begin(), vec.end()), vec.end());
            edge_count+=vec.size();
        }
        edge_count/=2;
    } // end of else b_symmetric
    
    in.close();
    if(entry_counter!=nnz) {
        fprintf(stderr,"Error! Matrix file %s size %d %d %d does not contains enough entries %d as expected\nPossibily because of file truncate! Check the files\n", file_name.c_str(), nrows, ncols, nnz, entry_counter);
        exit(1);
    }
    num_edges_=edge_count;
}

// ============================================================================
// write matrix out 
// ============================================================================
void AdjGraph::write_mm_out(const string& file_name){
    if(file_name.empty()) {
        fprintf(stderr, "Err, output file name is empty\n");
        exit(1);
    }
    ofstream out(file_name.c_str());
    if(!out) {
        fprintf(stderr, "Err, output file cannot be creat\n");
        exit(1);
    }

    out<<"\%\%MatrixMarket matrix coordinate pattern symmetric\n";
    out<<"\% nothing\n";
    
    out<<G_.size()<<" "<<G_.size()<<" "<<num_edges_<<endl;
    for(INT y=0, yEnd=G_.size(); y<yEnd; y++) {
        for(auto x: G_[y]){
            if(x<=y) continue;
            out<<x+1<<" "<<y+1<<"\n";
        }
    }
    out.close();
}


// ============================================================================
//
// ============================================================================
void MycielskiGraphEngine::std_propagate(INT steps){
    vector<vector<INT>>& G=G_;
    INT& M = num_edges_;
    

    for(INT step_cnt=0; step_cnt<steps; step_cnt++){
        const INT N = G_.size();
        INT edge_increament=0;
        // the duplicated vertex copy the neighbers, and adding extra vertex
        G.resize(2*N+1);
        for(INT v=0; v<N; v++){
            INT w = v + N;
            G[w].reserve(G[v].size()+1);
            G[w].assign(G[v].begin(), G[v].end());
            G[w].push_back(2*N+1-1);
            edge_increament+=G[w].size();
        }
        // adding the extra vertex
        G[2*N+1-1].reserve(N);
        for(INT v=0; v<N; v++)
            G[2*N+1-1].push_back(v+N);
        edge_increament+=N;
        // original vertex doubled degrees
        for(INT v=0; v<N; v++){
            vector<INT>& Gv=G[v];
            INT Nloc = Gv.size();
            Gv.resize(2*Nloc);
            edge_increament+=Nloc;
            for(INT w=0; w<Nloc; w++) {
                Gv.push_back(Gv[w]+N);
            }
        }
        M=M+edge_increament/2;
    }// end of for steps
    return;
}

// ============================================================================
//
// ============================================================================
void MycielskiGraphEngine::rnd_propagate(INT steps){
    vector<vector<INT>>& G = G_;
    INT& M = num_edges_;

    for(INT step_cnt=0; step_cnt<steps; step_cnt++) {
        const INT N = G_.size();
        INT edge_increament=0;
        G.resize(2*N+1);
        // duplicate nodes
        unordered_map<INT,unordered_set<INT>> check_list;
        for(INT i=0; i<M; i++) {
            INT v1(0),v2(0),w1,w2;
            random_pick_two_vertex_from_range(v1, v2, N, check_list);
            w1=v1+N;
            w2=v2+N;
            G[v1].push_back(w2);
            G[v2].push_back(w1);
            G[w2].push_back(v1);
            G[w1].push_back(v2);
            edge_increament+=2;
        }
        for(INT v=0; v<N; v++) 
            G[v+N].push_back(2*N+1-1);
        // extra nodes
        G[2*N+1-1].reserve(N);
        for(int v=0; v<N; v++)
            G[2*N+1-1].push_back(v+N);
        edge_increament += N;
        M=M+edge_increament;
    }//end of steps
    return;
}

// ============================================================================
// the default state of standard Mycielski Graph is k=2, G={0:1,1:0}
// ============================================================================
void MycielskiGraphEngine::reset(){
    G_.clear();
    G_.resize(2);
    G_.shrink_to_fit();
    G_[0].push_back(1);
    G_[1].push_back(0);
    num_edges_=1;
}

// ============================================================================
//
// ============================================================================
void MycielskiGraphEngine::random_pick_two_vertex_from_range(INT &v1, INT&v2, const INT N, unordered_map<INT,unordered_set<INT>>&check_list){
    mt19937_64 mt(11234);
    uniform_int_distribution<INT> dist(0, N-1);
    bool bQuit=false;
    do{    
        v1=dist(mt);
        do{
            v2=dist(mt);
        }while(v1==v2);

        if(v1>v2) swap(v1,v2);
        if(check_list.count(v1)==0 || check_list[v1].count(v2)==0){
            check_list[v1].insert(v2);
            bQuit=true;
        }
    }while(bQuit==false);
    return;
}






