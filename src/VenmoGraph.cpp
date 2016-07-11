//
//  VenmoGraph.cpp
//  InsightDataCodeChallenge
//
//  Created by Umesh Shukla on 7/9/16.
//  Copyright © 2016 Umesh Shukla. All rights reserved.
//

#include "VenmoGraph.hpp"

// Default Constructor

VenmoGraph::VenmoGraph()
{
}

// Call this constructor if sliding Window time length needs to be set other than 60 s

VenmoGraph::VenmoGraph(long maxTimeVal)
{
    slidingWindowTimeVal = maxTimeVal;
}

// processEdge: Take a new edge as input and modify current graph accordingly to CodingChallenge rules
// Complexity: O(N)

void VenmoGraph::processEdge(long time, edge newEdge)
{
    // Insert this item only if newEdge's timestamp is within sliding window of known max time in graph.
    // Since graph is sorted based on timestamps, the last entry in current graph has max time
    if (graph.empty()){
        insertEdge(std::pair<long, edge>(time, newEdge));
    }
    else if ((graph.rbegin())->first < time + slidingWindowTimeVal){
        
        // Check if there's already an edge with same nodes in graph. Remove old edge to update with new one
        // Removal and insertion will have complexity of O(logN) as sorted map is implemented using BST.
        for (auto it = graph.begin(); it != graph.end(); it++){
            if (AreEdgesEqual(newEdge, it->second)){
                removeEdge(it);
                break;
            }
        }
        
        insertEdge(std::pair<long, edge>(time, newEdge));
        
        // Delete edges with older timestamps which fall out of sliding window after insertion of new edge
        long minTimeAllowed = (graph.rbegin())->first - slidingWindowTimeVal;
        for (auto it = graph.begin(); it != graph.end(); ) {
            
            // Sliding Window is exclusive and hence the use of <= below as deletion criteria
            if (it->first <= minTimeAllowed){
                auto deleteEdgeItr = it;
                it++;
                removeEdge(deleteEdgeItr);
            }
            else{
                break;
            }
        }
    }
}

// removeEdge: Removes input edge from the graph and update nodes and nodeDegreeVals data structure
// Complexity: O(logN)

void VenmoGraph::removeEdge(multimap<long, edge>::iterator it)
{
    // Delete this entry and add new entry
    decrementNodeDegree(it->second.actor);
    decrementNodeDegree(it->second.target);
    graph.erase(it);
}

// insertEdge: Inserts input edge from the graph and update nodes and nodeDegreeVals data structure
// Complexity: O(logN)

void VenmoGraph::insertEdge(std::pair<long, edge> e)
{
    incrementNodeDegree(e.second.actor);
    incrementNodeDegree(e.second.target);
    graph.insert(e);
}

// computeDegreeMedian: Computes median of graph degree. Since nodeDegreeVals is sorted, it either picks the
// mid item or the average of (mid, mid-1) items based on set size.
// Complexity: O(N)

double VenmoGraph::computeDegreeMedian()
{
    double median;
    auto it = nodeDegreeVals.begin();
    std::advance(it, nodeDegreeVals.size()/2); // move the iterator to middle
    
    if (nodeDegreeVals.size()%2){
        median = *it;
    }
    else{
        auto itCurr = it;
        it--;
        median = (double(*itCurr) + double(*it))/2;
    }
    
    return median;
}

// decrementNodeDegree: Called usually during removal of an edge, it updates nodes and nodeDegreeVals
// data structures. If a node's degree becomes zero after decrement, that node is removed from both data structures.
void VenmoGraph::decrementNodeDegree(string nodeKey)
{
    auto it = nodes[nodeKey];
    long newDegreeVal = *it - 1;
    nodeDegreeVals.erase(it);
    
    if (!newDegreeVal){
        nodes.erase(nodeKey);
    }
    else{
        nodes[nodeKey] = nodeDegreeVals.insert(newDegreeVal);
    }
    
}

// incrementNodeDegree:  Called usually during insertion of an edge, it updates nodes and nodeDegreeVals
// Data structures.
void VenmoGraph::incrementNodeDegree(string nodeKey)
{
    // If a node is not present in nodes data structure, then mark it's degree as 1. Otherwise, just increment.
    if (nodes.count(nodeKey)){
        multiset<long>::iterator itr = nodes[nodeKey];
        long newVal = *itr + 1;
        nodeDegreeVals.erase(itr);
        nodes[nodeKey] = nodeDegreeVals.insert(newVal);
    }
    else{
        nodes[nodeKey] = nodeDegreeVals.insert(1);
    }
}

bool VenmoGraph::AreEdgesEqual(edge e1, edge e2)
{
    return (e1.target == e2.target && e1.actor == e2.actor) || (e1.target == e2.actor && e1.actor == e2.target);
}