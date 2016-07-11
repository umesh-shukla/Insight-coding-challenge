//
//  main.cpp
//  InsightDataCodeChallenge
//
//  Created by Umesh Shukla on 7/9/16.
//  Copyright © 2016 Umesh Shukla. All rights reserved.
//
// Note: This file takes input and output file names.
// It processes input log file and calls VenmoGraph APIs to add a transaction edge in the existing Graph.
// Then it computes median of the updated graph and prints median's value in provided output file.

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <iomanip>
#include "VenmoGraph.hpp"

using namespace std;

// getMatchedPatternInString: Generic pattern matching function which returns a vector of
// strings matching with pattern provided in logFilePattern

vector<string> getMatchedPatternInString(const string &str, const string &logFilePattern)
{
    regex e1(logFilePattern);
    sregex_iterator iter(str.begin(), str.end(), e1);
    sregex_iterator end;
    vector<string> matchedStrings;
    
    while(iter != end)
    {
        for(unsigned i = 1; i < iter->size(); ++i)
        {
            matchedStrings.push_back((*iter)[i]);
        }
        ++iter;
    }
    return matchedStrings;
}

// Time format converter function: ISO8061 Format --> unix epoch time in secs

long convertTimeToSec(istringstream ss)
{
    std::tm timeStampInSec = {};
    if (!(ss >> get_time(&timeStampInSec, "%Y-%m-%dT%H:%M:%S"))){
        std::cout << "Parse failed\n";
    }
    return std::mktime(&timeStampInSec);
}

// main function: It takes input and output filename as command line args
// and process the input file to build the graph, compute median and print
// the value in output filename.

int main(int argc, char *argv[])
{
    if (argc < 3){
        throw invalid_argument("Enter correct no. of arguments");
    }

    ifstream inStream(argv[1]);
    ofstream outStream(argv[2]);
    
    if (!inStream.is_open()){
        throw invalid_argument("Could not open Input file");
    }
    
    if (!outStream.is_open()){
        throw invalid_argument("Could not open Output file");
    }
    
    // Grpah object: Maintains <timestamp, edge> sorted by timestamp
    VenmoGraph myGraph = VenmoGraph();
    
    // Regex for pattern matching and num of matches we expect in a line
    // If transaction log format changes in future, modify this regex
    string logFilePattern = "\\w+\":\\s*\"([a-zA-Z\\-0-9\\:]+)";
    long maxNumOfPatternsPerLine = 3;
    
    while(!inStream.eof()){
        string str;
        getline(inStream, str);
        
        if (str.empty())
            continue;
        
        // Step1: Find matched patterns from the line
        vector<string> matchedPatternsInString = getMatchedPatternInString(str, logFilePattern);
        
        // If no. of pattern matches in a line are not equal to maxNumOfPatternsPerLine, ignore the line
        if (matchedPatternsInString.size() == maxNumOfPatternsPerLine){
            
            // Step2: Construct new edge and ask the object to process this new edge
            long time = convertTimeToSec(istringstream(matchedPatternsInString[0]));
            edge newEdge = edge(matchedPatternsInString[1], matchedPatternsInString[2]); // (target, actor)
            myGraph.processEdge(time, newEdge);
            
            // Step3: Compute median and print in output file
            double median = myGraph.computeDegreeMedian();
//            cout << fixed << setprecision(2)<< median << endl;
            outStream << fixed << setprecision(2)<< median << endl;
        }
    }
    
    inStream.close();
    outStream.close();
    
}

