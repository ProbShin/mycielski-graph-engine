#ifndef GENMC_HPP
#define GENMC_HPP
/*************************************************************************
    File Name: main.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Thu 12 Apr 2018 02:16:55 PM EDT
*********************************************************************/
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <random>
using namespace std;

/*
// ============================================================================
// encapsulation of graph data structure, take care of all io operation
// ============================================================================
class AdjGraph{
    //typedef int INT
    typedef long long int INT;
public:
    AdjGraph(){}
    AdjGraph(vector<INT>& input_list, unordered_map<INT, vector<INT>>intput_G ):list_of_nodes_(input_list.begin(), input_list.end()), G_(input_G.begin(),input_G.end()) {}
    AdjGraph(const string &f){ reset(f); }
    ~AdjGraph(){}

public:
    virtual INT nodes() const { return num_nodes_; }
    virtual INT edges() const { return num_edges_; }
    void write_mm_out(const string& file_name);
    void reset(const string& file_name);


protected:
    vector<INT> list_of_nonzero_nodes_;
    unordered_map<INT, vector<INT>> G_;
    INT num_nodes_;
    INT num_edges_;
};
*/

// ============================================================================
// encapsulation of graph data structure, take care of all io operation
// ============================================================================
class AdjGraph{
public:
    //typedef int INT
    typedef long long int INT;
public:
    AdjGraph(){}
    AdjGraph(vector<vector<INT>>input_G ): G_(input_G.begin(),input_G.end()) {}
    AdjGraph(const string &f){ reset(f); }
    ~AdjGraph(){}

public:
    virtual INT nodes() const { return G_.size(); }
    virtual INT edges() const { return num_edges_; }
    void reset(const string& file_name);
    void write_mm_out(const string& file_name);
    void read_mm_struct_only(const string& file_name);

protected:
    vector<vector<INT>> G_;
    INT num_edges_;
public: void dump();
};


class MycielskiGraphEngine: public AdjGraph{
public:
    MycielskiGraphEngine(): mt_(12345) { reset(); }
    MycielskiGraphEngine(const string &f): mt_(12345) { if(f.empty()) reset(); else AdjGraph::reset(f); }
    virtual ~MycielskiGraphEngine(){};
public:
    virtual void std_propagate(INT steps);
    virtual void rnd_propagate(INT steps);
    void set_rseed(INT rseed){mt_.seed(rseed);}

    void random_pick_two_vertex_from_range(int &v1, int&v2, const int N, unordered_set<INT>&chech_list, uniform_int_distribution<INT>& dist);
private:
    void reset();
    mt19937_64 mt_;
};


#endif
