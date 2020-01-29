/*util.cpp*/

//
// Utility functions for myDB project
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

#include "util.h"

using namespace std;


//
// EchoData
//
// Reads the contents of a table's .data file, and outputs the
// values for each record (one record per line).  Pass the table
// name, the record size, and the # of columns per record.
//
// Example: EchoData("students", 82, 5) would output the contents
// of "students.data".
//
void EchoData(string tablename, int recordSize, int numColumns)
{
  string   filename = tablename + ".data";
  ifstream data(filename, ios::in | ios::binary);

  if (!data.good())
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return;
  }

  //
  // Okay, read file record by record, and output each record of values:
  //
  data.seekg(0, data.end);  // move to the end to get length of file:
  streamoff length = data.tellg();

  streamoff pos = 0;  // first record at offset 0:
  string    value;

  while (pos < length)
  {
    data.seekg(pos, data.beg);  // move to start of record:

    for (int i = 0; i < numColumns; ++i)  // read values, one per column:
    {
      data >> value;
      cout << value << " ";
    }

    cout << endl;
    pos += recordSize;  // move offset to start of next record:
  }
}


//
// GetRecord
//
// Reads a record of data values and returns these values in a vector.
// Pass the table name, the file position (a stream offset), and the # 
// of columns per record.
//
// Example: GetRecord(�students�, 0
// , 5) would read the first student
// record in �students.data�.
// 
vector<string> GetRecord(string tablename, streamoff pos, int numColumns)
{
  vector<string>  values;
  //
  // open the file, make sure it opened, seekg to the given position,
  // loop and input values using >>, store into vector, return vector
  //
  string   filename = tablename + ".data";
  ifstream data(filename, ios::in | ios::binary);

  if (!data.good())
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return values;
  }
  string    value;
	data.seekg(pos, data.beg);  

	for (int i = 0; i < numColumns; ++i)  // read values, one per column:
	{
		data >> value;
		values.push_back(value);
	}
  return values;
}


//
// LinearSearch
//
// Searches the contents of a .data file record by record; the first 
// three parameters are the tablename, record size, and # of columns.
// The 4th parameter specifies the value to search for: matchValue.
// The 5th parameter is the record column to match against --- pass
// 1 for the first column, 2 for the 2nd column, and so on.  All matches
// are exact matches.
//
// Example: LinearSearch(�students�, 82, 5, "kim", 3) would search the 
// "students.data" file for all records whose 3rd column --- lastname ---
// matches "kim".  There are 2 matches (records 3 and 6), so their file 
// positions of 164 and 410 would be returned in the vector.
// 
vector<streamoff> LinearSearch(string tablename, int recordSize, int numColumns, string matchValue, int matchColumn)
{
  vector<streamoff>  matches;

  //
  // open the file, make sure it opened, loop through record by 
  // record looking for matches.  For each match found, add file
  // position to vector.
  //
  string   filename = tablename + ".data";
  ifstream data(filename, ios::in | ios::binary);

  if (!data.good())
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return matches;
  }
	
  data.seekg(0, data.end);  // move to the end to get length of file:
  streamoff length = data.tellg();

  streamoff pos = 0;  // first record at offset 0:
  string    value;

  while (pos < length)
  {
    data.seekg(pos, data.beg);  // move to start of record:
		vector<string> v = GetRecord(tablename, pos, numColumns);
		if(v[matchColumn - 1] == matchValue)
		{
			matches.push_back(pos);
		}
		pos += recordSize;
	}

  return matches;
}

void buildTree(vector< avltree<string, streamoff> >&avl, vector<int> indexedPos, vector<string> indexedColumns, string tablename, int recordSize, int numColumns)
{
	string filename = tablename + ".data";
  ifstream data(filename, ios::in | ios::binary);
  if (!data.good())
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return;
  }

  string    value;
  data.seekg(0, data.end);  // move to the end to get length of file:
  streamoff length = data.tellg();
	for(unsigned int i = 0; i < indexedColumns.size(); ++i)
  {
    streamoff pos = 0;  // first record at offset 0:
    while (pos < length)
    {
      data.seekg(pos, data.beg);  // move to start of record:
      for (int j = 0; j < numColumns; ++j)  // read values, one per column:
      {
        data >> value;
        if (indexedPos[i] == j)
        {
          avl[i].insert(value, pos);
          break;
        }
      }
      pos += recordSize;  // move offset to start of next record:
    }
  }
}

//print index vector
void printIndexedVector(vector<avltree<string, streamoff>> avl, vector<string> columns)
{
	for(unsigned int i = 0; i < avl.size(); i++)
	{
		cout << "Index column: " << columns[i] << endl;
    cout << "  Tree size: " << avl[i].size() << endl;
    cout << "  Tree height: " << avl[i].height() << endl;
	}
	cout << endl;
}

//return the pos by given columns, if not return -1
int getColumnsPos(vector<string> columns, string columnName)
{
  for (unsigned int i = 0; i < columns.size(); ++i)
  {
    if (columns[i] == columnName)
      return i;
  }
  return -1;
}

//print record
void printRecord(vector<string> v, vector<string> vectorColumns, int selectColumnPos)
{
  if(v.size() == 0){
    cout << "Not found..." << endl;
    return;
  }

  if(selectColumnPos == -1)
  {
    for(unsigned int i = 0; i < vectorColumns.size(); ++i)
    {
      cout << vectorColumns[i] << ": " << v[i] << endl;
    }
  }
  else
  {
    for(unsigned int i = 0; i < vectorColumns.size(); ++i)
    {
      if(selectColumnPos == i)
      {
        cout << vectorColumns[i] << ": " << v[i] << endl;
      }
    }
  }
}

bool validQuery(vector<string> tokens, vector<string> vectorColumns, string tablename)
{
  if(tokens.size() == 8)
  {
    if(getColumnsPos(vectorColumns, tokens[1]) == -1 && tokens[1] != "*")
    {
      cout << "Invalid select column, ignored..." << endl;
      return false;
    }
    if(tokens[3] != tablename)
    {
      cout << "Invalid table name, ignored..." << endl;
      return false;
    }
    if(getColumnsPos(vectorColumns, tokens[5]) == -1)
    {
      cout << "Invalid where column, ignored..." << endl;
      return false;
    }
    return true;
  }
  else
  {
    if(tokens[0] == "select")
    {
      cout <<"Invalid select query, ignored..." << endl;
      return false;
    }
    else
    {
      cout <<"Unknown query, ignored..." << endl;
      return false;
    }
  }
  return false;
}
