#ifndef Node2_H
#define Node2_H
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Node2
{
	public:
		Node2();
		Node2(Node2* n);
		Node2(pair< int, int> temp, vector < pair < int, int> > temp2);
		Node2(pair< int, int> temp, vector < pair < int, int> > temp2, Node2* z, bool isGoal);
		~Node2();
		pair<int, int> getLoc();
		vector< pair< int, int> > getGoals();
		map<pair<int, int>, int> getMap();
		double getHeuristic();
		double getF();
		int getG();
		Node2* getParent();
		bool operator== (Node2 two);


	private:
		pair<int, int> loc;
		vector< pair< int, int> > unvisited;
		int gValue;
		double fValue;
		double heuristic;
		Node2* parent;
		map<pair<int, int>, int> Map;
		static const double scale = 1.0;

};

#endif