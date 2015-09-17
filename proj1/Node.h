#ifndef NODE_H
#define NODE_H
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class Node
{
	public:
		Node();
		Node(Node* n);
		Node(pair< int, int> temp, vector < pair < int, int> > temp2);
		Node(pair< int, int> temp, vector < pair < int, int> > temp2, Node* z, bool isGoal);
		~Node();
		pair<int, int> getLoc();
		vector< pair< int, int> > getGoals();
		map<pair<int, int>, int> getMap();
		int getHeuristic();
		int getF();
		int getG();
		Node* getParent();
		bool operator== (Node two);
	private:
		pair<int, int> loc;
		vector< pair< int, int> > unvisited;
		int gValue;
		int fValue;
		int heuristic;
		Node* parent;
		map<pair<int, int>, int> Map;
};


#endif