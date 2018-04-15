/*************************************************************************
    File Name: main.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Thu 12 Apr 2018 02:16:55 PM EDT
*********************************************************************/

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
        fpirntf(stderr,"Err! the matrix file %s cannot be read. Currently only supported .mtx format formt.\n", f.c_str());
    }
}

// ============================================================================
// read_mm_struct_only(f);
// ============================================================================
AdjGraph::read_mm_struct_only(file_name){
    list_of_nodes_.clear(); 
    list_of_nodes_.shrink_to_fit();
    G_.clear();
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
        fprintf(stderr,"Err! the matrix file %S cannot be open.\n", file_name.c_str());
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
            fprintf(stderr,"Warning, the non-symmetric matrix %s, will be convert to symmetric by adding corresponding edge.\n");
            b_symmetric=false;
        }
    }
    
    do{
        getline(in,line);
    while(!line.empty() && line[0]=='%');
    
    if(line.empty()) {
        fprintf(stderr, "Err! file %s Matrix Market format is wrong? Meet a blank line?\n", file_name);
        exit(1);
    }

    ss.str(line);
    int nrows, ncols, nnz;
    ss>>nrows>>ncols>>nnz;
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
                fprintf(stderr,"Err! Find an entry (row,col)=(%d, %d) in upper part of symmtric matrix %s.\n", row, col, file_name);
                exit(1);
            }
            row--; col--;
            G[row].push_back(col);
            G[col].push_back(row);
            edge_count++;
        }while(!in.eof && entry_counter<nnz) 
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
            G[row].push_back(col);
            G[col].push_back(row);
        }while(!in.eof && entry_counter<nnz)
        for(auto &piv : G_){
            sort(piv.second.begin(), piv.second.end());
            piv.second.erase( unique(piv.second.begin(), piv.second.end()), piv.second.end());
            edge_count+=piv.second.size();
        }
        edge_count/=2;
    } // end of else b_symmetric
    
    in.close();
    if(entry_counter!=nnz) {
        fprintf(stderr,"Error! Matrix file %s size %d %d %d does not contains enough entries %d as expected\nPossibily because of file truncate! Check the files\n", file_name.c_str(), nrows, ncols, nnz, entry_counter);
        exit(1);
    }

    num_nodes_=max(nrows, ncols);
    num_deges_=edge_count;
    list_nzdeg_nodes_reserve(G_.size());
    for(const auto & piv : G_) {
        list_nzdeg_nodes.push_back(piv.first);
    }
    sort(list_nzdeg.begin(), list_nzdeg.end());
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
    out<<num_nodes_<<" "<<num_nodes_<<" "<<num_edges<<endl;
    for(auto x: list_nzdeg_nodes){
        for(auto y: G_[x]){
            if(x<y) continue;
            cout<<x<<" "<<y"\n";
        }
    }
    out.close();
}


// ============================================================================
//
// ============================================================================
MycielskiGraphEngine::std_propagate(INT steps){
    vector<INT>&  nzd_nodes;
    unordered_map<INT, vector<INT>>& G;
    INT& N = num_nodes_;
    INT& M = num_edges_;

    for(step_cnt=0; step_cnt<steps; step_cnt++){
        INT edge_increament=0;
        const INT ORG_NZD_SIZE = nzd_nodes.size();
        // the duplicated vertex copy the neighbers, and adding extra vertex
        for(INT i=0; i<ORG_NZD_SIZE; i++){
            INT v = nzd_nodes[i];
            INT w = v + N;
            G[w].reserve(G[v].size()+1);
            G[w].assign(G[v].begin(), G[v].end());
            G[w].push_back(2*N+1);
            edge_increament+=G[v].size();
            nzd_nodes.push_back(v);
        }
        // adding the extra vertex
        G[2*N+1].reserve(ORG_NZD_SIZE);
        G[2*N+1].assign(nzd_nodes.begin()+ORG_NZD_SIZE, nzd_nodes.end());
        edge_increament+=N;
        nzd_nodes.push_back(2*N+1);
        // original vertex doubled degrees
        for(INT i=0; i<ORG_NZD_SIZE; i++){
            INT v=nzd_nodes[i];
            vector<INT>& Gv=G[v];
            for(INT j=0,jEnd=Gv.size(); j<jEnd; j++) {
                Gv.append(Gv[j]+N);
                edge_increament++;
            }
        }
        N=2*N+1;
        M=M+edge_increament/2;
    }// end of for steps
    return;
}

// ============================================================================
//
// ============================================================================
MycielskiGraphEngine::rnd_propagate(INT steps){
    vector<INT>&  nzd_nodes;
    unordered_map<INT, vector<INT>>& G;
    INT& N = num_nodes_;
    INT& M = num_edges_;

    for(step_cnt=0; step_cnt<steps; step_cnt++) {
        INT edge_increament=0;
        const INT ORG_NZD_SIZE = nzd_nodes.size();
        // duplicate nodes


    }
    return;
}

// ============================================================================
// the default state of standard Mycielski Graph is k=2, G={0:1,1:0}
// ============================================================================
void MycielskiGraphEngine::reset(){
    list_nzdeg_nodes_.clear();
    list_nzdeg_nodes_.push_back(0);
    list_nzdeg_nodes_.push_back(1);
    list_nzdeg_nodes_.shrink_to_fit();
    G_.clear();
    G_[0].push_back(1);
    G_[1].push_back(0);
    num_nodes_=2;
    num_edges_=1;
}






