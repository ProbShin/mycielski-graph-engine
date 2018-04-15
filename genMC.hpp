#ifndef GENMC_HPP
#define GENMC_HPP
/*************************************************************************
    File Name: main.cpp
    Author: Xin Cheng
    Descriptions: 
    Created Time: Thu 12 Apr 2018 02:16:55 PM EDT
*********************************************************************/

#include <iostream>
#include <unordered_map>
#include <vector>
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
    //typedef int INT
    typedef long long int INT;
public:
    AdjGraph(){}
    AdjGraph(vector<INT>& input_list, unordered_map<INT, vector<INT>>intput_G ):list_of_nodes_(input_list.begin(), input_list.end()), G_(input_G.begin(),input_G.end()) {}
    AdjGraph(const string &f){ reset(f); }
    ~AdjGraph(){}

public:
    virtual INT nodes() const { return G_.size(); }
    virtual INT edges() const { return num_edges_; }
    void write_mm_out(const string& file_name);
    void reset(const string& file_name);

protected:
    vector<INT, vector<INT>> G_;
    INT num_edges_;
};


class MycielskiGraphEngine: public AdjGraph{
public:
    MycielskiGraphEngine(){ reset(); }
    MycielskiGraphEngine(const string &f){ AdjGraph::reset(f); }
    virtual ~MycielskiGraphEngine(){};
protected:
    virtual void std_propagate(INT steps);
    virtual void rnd_propagate(INT steps);
private:
    reset();
};


#endif
