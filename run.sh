#!/usr/bin/env bash

# example of the run script for running the rolling_median calculation with a python file, 
# but could be replaced with similar files from any major language

# I'll execute my programs, with the input directory venmo_input and output the files in the directory venmo_output
g++ -std=c++11 ./src/main.cpp ./src/VenmoGraph.cpp -o VenmoGraph
./VenmoGraph ./venmo_input/venmo-trans.txt ./venmo_output/output.txt


