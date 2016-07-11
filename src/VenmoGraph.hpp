//
//  VenmoGraph.hpp
//  InsightDataCodeChallenge
//
//  Created by Umesh Shukla on 7/9/16.
//  Copyright © 2016 Umesh Shukla. All rights reserved.
//

#ifndef VenmoGraph_hpp
#define VenmoGraph_hpp

#include <iostream>
#include <map>
#include <unordered_map>
#include<set>
#include <vector>

using namespace std;

// An edge represents a transaction between two users: target and actor
struct edge {
    string target;
    string actor;
    edge(string a, string b){target = a; actor = b;}     // constructor
};

// VenmoGraph maintains the graph and associated data structures to compute median at any instance
class VenmoGraph {
    
    // Data structures & properties
    
    // Multimap to hold all the edges in order sorted by time. Insert/Delete complexity O(logN)
    multimap<long, edge> graph;
    
    // <User name, Associated multiset iterator pointing to user's degree value>. Insert/Delete/Find complexity: O(1)
    unordered_map <string, multiset<long>::iterator> nodes;
    
    // nodeDegreeVals contains degree of each node in sorted order. Insert/Delete complexity: O(logN)
    multiset<long> nodeDegreeVals;
    
    // Sliding Window duration can be configured. Default value of 60 secs
    long slidingWindowTimeVal = 60;
    
    // Internal functions
    void removeEdge(multimap<long, edge>::iterator it);
    void insertEdge(std::pair<long, edge> e);
    void decrementNodeDegree(string nodeKey);
    void incrementNodeDegree(string nodeKey);
    bool AreEdgesEqual(edge e1, edge e2);
    
public:
    // Constructors
    VenmoGraph();
    
    // Call this constructor if sliding Window time length needs to be set other than 60 s
    VenmoGraph(long maxTimeWindowVal);
    
    // processEdge: Take a new edge as input and modify current graph accordingly to CodingChallenge rules
    // Complexity: O(N)
    void processEdge(long time, edge newEdge);
    
    // computeDegreeMedian: Computes median of graph degree. Since nodeDegreeVals is sorted, it either picks the
    // mid item or the average of (mid, mid-1) items based on set size.
    // Complexity: O(N)
    double computeDegreeMedian();
};

#endif /* VenmoGraph_hpp */
