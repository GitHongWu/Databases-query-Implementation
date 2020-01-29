/*util.h*/

//
// Utility functions for myDB project
//
// <<Hongcheng Wu>>
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #04
//

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "avl.h"

using namespace std;

void EchoData(string tablename, int recordSize, int numColumns);

vector<string> GetRecord(string tablename, streamoff pos, int numColumns);

vector<streamoff> LinearSearch(string tablename, int recordSize, int numColumns, string matchValue, int matchColumn);

void buildTree(vector<avltree<string, streamoff>>&avl, vector<int> indexedPos, vector<string> indexedColumns, string tablename, int recordSize, int numColumns);
void printIndexedVector(vector<avltree<string, streamoff>> avl, vector<string> columns);
int getColumnsPos(vector<string> columns, string columnName);
void printRecord(vector<string> v, vector<string> vectorColumns, int selectColumnPos);
bool validQuery(vector<string> tokens, vector<string> vectorColumns, string tablename);
