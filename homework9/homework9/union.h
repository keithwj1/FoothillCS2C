#pragma once
// C++ implementation of disjoint set
#include <iostream>
typedef int_least64_t OSMID;

#include <iostream>
#include <vector>
#include <unordered_map>



class NodeUnionOld {
public:
    //std::vector<OSMID> rank, parent;
    //std::set<OSMID> parent, rank;
    std::map<OSMID, OSMID> parent, rank;
    OSMID n;
    // Creates n single item sets
    void makeSet()
    {
        for (OSMID i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    OSMID find(OSMID x) {
        // Finds the representative of the set
        // that x is an element of
        if (parent[x] != x) {

            // if x is not the parent of itself
            // Then x is not the representative of
            // his set,
            parent[x] = find(parent[x]);

            // so we recursively call Find on its parent
            // and move i's node directly under the
            // representative of this set
        }

        return parent[x];
    }
    // Do union of two sets represented
    // by x and y.
    void Union(OSMID x, OSMID y)
    {
        // Find current sets of x and y
        OSMID xset = find(x);
        OSMID yset = find(y);

        // If they are already in same set
        if (xset == yset)
            return;

        // Put smaller ranked item under
        // bigger ranked item if ranks are
        // different
        if (rank[xset] < rank[yset]) {
            parent[xset] = yset;
        }
        else if (rank[xset] > rank[yset]) {
            parent[yset] = xset;
        }

        // If ranks are same, then increment
        // rank.
        else {
            parent[yset] = xset;
            rank[xset] = rank[xset] + 1;
        }
    }
};

class DisjSet {
    OSMID* rank, * parent, n;

public:
    // Constructor to create and
    // initialize sets of n items
    DisjSet(OSMID n) {
        //handle unkown size.
        //use vectors
        rank = new OSMID[n];
        parent = new OSMID[n];
        this->n = n;
        makeSet();
    }
    ~DisjSet() {
        delete rank;
        delete parent;
    }

    // Creates n single item sets
    void makeSet()
    {
        for (OSMID i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    // Finds set of given item x
    OSMID find(OSMID x)
    {
        // Finds the representative of the set
        // that x is an element of
        if (parent[x] != x) {

            // if x is not the parent of itself
            // Then x is not the representative of
            // his set,
            parent[x] = find(parent[x]);

            // so we recursively call Find on its parent
            // and move i's node directly under the
            // representative of this set
        }

        return parent[x];
    }

    // Do union of two sets represented
    // by x and y.
    void Union(OSMID x, OSMID y)
    {
        // Find current sets of x and y
        OSMID xset = find(x);
        OSMID yset = find(y);

        // If they are already in same set
        if (xset == yset)
            return;

        // Put smaller ranked item under
        // bigger ranked item if ranks are
        // different
        if (rank[xset] < rank[yset]) {
            parent[xset] = yset;
        }
        else if (rank[xset] > rank[yset]) {
            parent[yset] = xset;
        }

        // If ranks are same, then increment
        // rank.
        else {
            parent[yset] = xset;
            rank[xset] = rank[xset] + 1;
        }
    }
};