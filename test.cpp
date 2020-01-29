/*test.cpp*/

//
// Lab week 09 / project #04
// 
// Testing for util.cpp?
//

#include <iostream>
#include <vector>

#include "avl.h"
#include "util.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

TEST_CASE("(1) GetRecord") 
{
	vector<string> v = GetRecord("students", 0, 5);

	REQUIRE(v.size() == 5);
	REQUIRE(v[0] == "123456");
}

TEST_CASE("(2) LinearSearch") 
{
	vector<streamoff> m = LinearSearch("students", 82, 5, "kim", 3);

	REQUIRE(m.size() == 2);
	REQUIRE(m[0] == 164);
	REQUIRE(m[1] == 410);
}

TEST_CASE("(3) EchoData") 
{
	EchoData("students", 82, 5);
}
