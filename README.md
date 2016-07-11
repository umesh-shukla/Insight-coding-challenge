# Insight-coding-challenge

# Build Instructions
Just run `./run.sh`. run.sh contains following commands:
1. g++ -std=c++11 ./src/main.cpp ./src/VenmoGraph.cpp -o VenmoGraph
2. ./VenmoGraph ./venmo_input/venmo-trans.txt ./venmo_output/output.txt

# Requirements to run this code
1. GCC compiler with C++11, STL support which is fairly standard 

# Design 
main.cpp: This contains main function. This file also deals with reading and parsing input file, and writing the median values into output file.

VenmoGraph.cpp: This file is the brain of this project which maintains Venmo transaction graph. We use following data structures in our design: 
1. Graph: We use STL multimap to build the graph as <timestamp, edge>. This means the edges of the graph are sorted based on time. Due to sliding window, graph data is quite dynamic in nature. That’s why we chose multimap as most operations have O(logN) complexity where N is total number of items present in sliding window at a given time. 

2. Node Degree and median computation: We use combination of map and multiset to maintain node degrees and compute median in efficient way. Map represents <nodeID, iterator-to-node-degree> and multiset contains <node-degree>. Map has constant time complexity so that insertion/deletion and finding a node value is faster. Multiset maintains sorted node degree values to compute median faster and has logarithmic complexity for main operations. 

3. Overall algorithm to maintain graph and compute median is linear time i.e. O(N) which is decent, given the problem statement. 
