#! /usr/bin/python3
#### File Name: mGenMC.py
#### Author: Xin Cheng
#### Descriptions: 
#### Created Time: Mon 30 Jan 2017 10:29:03 AM EST
#### -------------------------------------------------
#### 
#### Mycielski Graphs Generator
#### The graph G as stored as List of List
#### w = G[v][i]
#### represent vertex w is the i-th neightbor of vertex v
#### zeor-based
#### -----------------------------------------------------


import sys


def genMC(k):
    if k<1:
        print("err! invalid input %d"%k)
        exit()
    if k==1:
        return [[]]
    if k>20:
        print("warning! MC%d could be very large"%k)
    G=[[1],[0]]  # MC2
    for k in range(3,k+1):
        siz = len(G)   # current number of nodes 
        Gnew = [[w for w in nbs]+[2*siz] for nbs in G]               # new nodes
        Gnew.append([i for i in range(siz,2*siz)])                   # extral nodes
        for i in range(siz):                                        # previous nodes 
            G[i] += [w+siz for w in G[i]]
        G+=Gnew
    return G

def mm_write(G,gname,k):
    n   =len(G);
    nnz =sum([len(g) for g in G])/2 
    with open(gname,"w") as f:
        print("%%MatrixMarket matrix coordinate pattern symmetric",file=f)
        print("%%Mycielski graph with k=%d \n%d %d %d"%(k,n,n,nnz),file=f)
        for i in range(n):
            for j in G[i]:
                if(i>j):
                    print("%d %d"%(i+1,j+1), file=f)
    return


def usage():
    print("$ %s <k>\nThe output graph are stored as MC<k>.mtx");
    return;

def main(argv):
    if len(argv)!=2:
        usage();
        return;
    k = int(argv[1])
    G = genMC(k)
    mm_write(G,"mc%d.mtx"%k,k)
    return

if __name__ == "__main__":
    main(sys.argv)

