/*main.cpp*/

//
// myDB project using AVL trees
//
// <<Hongcheng Wu>>
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #04
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

#include "avl.h"
#include "util.h"

using namespace std;


//
// tokenize
//
// Given a string, breaks the string into individual tokens (words) based
// on spaces between the tokens.  Returns the tokens back in a vector.
//
// Example: "select * from students" would be tokenized into a vector
// containing 4 strings:  "select", "*", "from", "students".
//
vector<string> tokenize(string line)
{
  vector<string> tokens;
  stringstream  stream(line);
  string token;

  while (getline(stream, token, ' '))
  {
    tokens.push_back(token);
  }

  return tokens;
}

int main()
{
  string tablename;// = "students";

  cout << "Welcome to myDB, please enter tablename> ";
  getline(cin, tablename);

  cout << "Reading meta-data..." << endl;
  string filename = tablename + ".meta";
  ifstream meta(filename, ios::in | ios::binary);
  if (!meta.good())
  {
    cout << "**Error: couldn't open meta file '" << filename << "'." << endl;
    return -1;
  }
	
	int recordSize, numColumns, numIndexed;
	string value;
	vector<string> vectorColumns, indexedColumns;
  vector<int> indexedPos;

	meta >> recordSize >> numColumns;
	for(int i = 0; i < numColumns; i++)
	{
		meta >> value >> numIndexed;
		vectorColumns.push_back(value);
		if(numIndexed == 1)
		{
      indexedColumns.push_back(value);
      indexedPos.push_back(i);
		}
	}

  cout << "Building index tree(s)..." << endl;
  vector< avltree<string, streamoff> > vectorAVL(indexedColumns.size());  //store avltree
	buildTree(vectorAVL, indexedPos, indexedColumns, tablename, recordSize, numColumns);
	printIndexedVector(vectorAVL, indexedColumns);

  //
  // Main loop to input and execute queries from the user:
  //
  string query;// = "select netid from students where lastname = kim";
  cout << "Enter query> ";
  getline(cin, query);
  while (query != "exit")
  {
    vector<string> tokens = tokenize(query);

    //valid query
    if(validQuery(tokens, vectorColumns, tablename))
    {
      string selectColumn = tokens[1];
      string fromColumn = tokens[5];
      string inputData = tokens[7];

      int selectColumnPos, fromColumnPos;
      vector<string> record;
      if(selectColumn == "*") //print all info
      {
        fromColumnPos = getColumnsPos(indexedColumns, fromColumn);
        if(fromColumnPos != -1)  //from column can be search by avl tree
        {
          streamoff *pos = vectorAVL[fromColumnPos].search(inputData);
          if(pos != nullptr)
          {
            record = GetRecord(tablename, *pos, numColumns);
            printRecord(record, vectorColumns, -1); //if select *, pass -1
          }
          else
          {
            cout << "Not found..." << endl;
          }
        }
        else  //from column use linear search
        {
          fromColumnPos = getColumnsPos(vectorColumns, fromColumn);
          vector<streamoff> streamVec = LinearSearch(tablename, recordSize, numColumns, inputData, fromColumnPos + 1);
          // cout << "fromColumnPos: " << fromColumnPos << endl;
          // cout << "streamVec size: " << streamVec.size() << endl;
          // cout << "input: " << inputData << endl;
          if(streamVec.size() == 0)
          {
            cout << "Not found..." << endl;
          }
          else
          {
            for (unsigned int i = 0; i < streamVec.size(); ++i)
            {
              record = GetRecord(tablename, streamVec[i], numColumns);
              printRecord(record, vectorColumns, -1); //if select *, pass -1
            }
          }
        }
      }
      else  //print from select column name
      {
        selectColumnPos = getColumnsPos(vectorColumns, selectColumn);
        fromColumnPos = getColumnsPos(indexedColumns, fromColumn);
        if(fromColumnPos != -1) //can use tree to search
        {
          streamoff *pos = vectorAVL[fromColumnPos].search(inputData);
          if(pos != nullptr)
          {
            record = GetRecord(tablename, *pos, numColumns);
            printRecord(record, vectorColumns, selectColumnPos); //if select *, pass -1
          }
          else
          {
            cout << "Not found..." << endl;
          }
        }
        else  //linear search
        {
          fromColumnPos = getColumnsPos(vectorColumns, fromColumn);
          vector<streamoff> streamVec = LinearSearch(tablename, recordSize, numColumns, inputData, fromColumnPos + 1);
//           cout << "fromColumnPos: " << fromColumnPos << endl;
//           cout << "streamVec size: " << streamVec.size() << endl;
//           cout << "input: " << inputData << endl;
          if(streamVec.size() == 0)
          {
            cout << "Not found..." << endl;
          }
          else
          {
            for (unsigned int i = 0; i < streamVec.size(); ++i)
            {
              record = GetRecord(tablename, streamVec[i], numColumns);
              printRecord(record, vectorColumns, selectColumnPos); //if select *, pass -1
            }
          }
        }
      }
    }
    cout << endl;
    cout << "Enter query> ";
    getline(cin, query);
  }

  //
  // done:
  //
  return 0;
}
