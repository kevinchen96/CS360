#include <queue>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include "problem.h"
#include "Node.cpp"
#include "Node2.cpp"

using namespace std;
/// you can add whatever helper functions/variables you need here.

class compare{
	bool comparator;
public:
  	compare(const bool& param=true){
  		comparator = param;
  	}
  	bool operator() (Node* lhs, Node* rhs) const{
    	if (comparator) return (lhs->getF()>rhs->getF());
    	else return (lhs->getF()<rhs->getF());
  	}
};
struct set_compare {
    bool operator() (Node* lhs, Node* rhs) const{
        if(lhs->getLoc() != rhs->getLoc()){
        	return lhs->getLoc() < rhs->getLoc();
        }
        else if(lhs->getGoals().size() != rhs->getGoals().size()){
        	return lhs->getGoals().size() < rhs->getGoals().size();
        }
        else{
        	vector<pair<int,int> > oneG = lhs->getGoals();
			vector<pair<int,int> > twoG = rhs->getGoals();
			for(int i = 0; i < oneG.size(); i++){
				if(oneG[i] != twoG[i]){
			//		cout <<"different goal" << endl;
					return oneG[i] < twoG[i];
				}
			}
			return false;
        } 
    }
};

class compare3{
	bool comparator;
public:
  	compare3(const bool& param=true){
  		comparator = param;
  	}
  	bool operator() (Node2* lhs, Node2* rhs) const{
    	if (comparator) return (lhs->getF()>rhs->getF());
    	else return (lhs->getF()<rhs->getF());
  	}
};
struct set_compare3 {
    bool operator() (Node2* lhs, Node2* rhs) const{
        if(lhs->getLoc() != rhs->getLoc()){
        	return lhs->getLoc() < rhs->getLoc();
        }
        else if(lhs->getGoals().size() != rhs->getGoals().size()){
        	return lhs->getGoals().size() < rhs->getGoals().size();
        }
        else{
        	vector<pair<int,int> > oneG = lhs->getGoals();
			vector<pair<int,int> > twoG = rhs->getGoals();
			for(int i = 0; i < oneG.size(); i++){
				if(oneG[i] != twoG[i]){
			//		cout <<"different goal" << endl;
					return oneG[i] < twoG[i];
				}
			}
			return false;
        } 
    }
};

/// Do not change codes below
vector<string> getPath(map<pair<int, int>, pair<int,int> > &parent, pair<int, int> goal)
{
	vector<string> path;
	pair<int, int> state = goal;
	int dx, dy;
	while (parent[state] != state)
	{
		dx = state.first - parent[state].first;
		dy = state.second - parent[state].second;
		if (dx>0)
			path.push_back("South");
		else if (dx<0)
			path.push_back("North");
		else if (dy>0)
			path.push_back("East");
		else
			path.push_back("West");
		state = parent[state];
	}
	reverse(path.begin(), path.end());
	return path;
}

vector<string> getPath2(Node* goal, Node* start)
{
	vector<string> path;
	Node* state = goal;
	int dx, dy;
	//cout << "In Function" << endl;
	while (!(*(state) == *(state->getParent())))
	{
		dx = state->getLoc().first - state->getParent()->getLoc().first;
		dy = state->getLoc().second - state->getParent()->getLoc().second;
		if (dx>0){
			path.push_back("South");
		}
		else if (dx<0){
			path.push_back("North");
		}
		else if (dy>0){
			path.push_back("East");
		}
		else{
			path.push_back("West");
		}
		state = state->getParent();
		
	}
	reverse(path.begin(), path.end());
	return path;
}

vector<string> getPath3(Node2* goal, Node2* start)
{
	vector<string> path;
	Node2* state = goal;
	int dx, dy;
	//cout << "In Function" << endl;
	while (!(*(state) == *(state->getParent())))
	{
		dx = state->getLoc().first - state->getParent()->getLoc().first;
		dy = state->getLoc().second - state->getParent()->getLoc().second;
		if (dx>0){
			path.push_back("South");
		}
		else if (dx<0){
			path.push_back("North");
		}
		else if (dy>0){
			path.push_back("East");
		}
		else{
			path.push_back("West");
		}
		state = state->getParent();
		
	}
	reverse(path.begin(), path.end());
	return path;
}

vector<string> questionOne(Problem &problem)
{
	/// write your own code here
	//priority_queue<int, vector<int>, std::greater<int> > first;
	vector<pair<int, int> > q;
	map<pair<int, int>, pair<int, int> > parent;
	map<pair<int, int>, int > gValue;
	map<pair<int, int>, int > fValue;
	pair<int, int> start = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();
	if (goals.size() <= 0)
		return vector<string>();
	pair<int, int> goal = goals[0];
	q.push_back(start);
	parent[start] = start;
	gValue[start] = 0;
	fValue[start] = abs(start.first - goal.first) + abs(start.second - goal.second);
	while (!q.empty())
	{
		pair<int, int> thisState=q.front();
		q.erase(q.begin());
		vector<pair<int, int> > successors = problem.getSuccessors(thisState);
		for(int i=0; i<successors.size(); ++i)
		{
			if (parent.count(successors[i]) == 0)
			{
				gValue[successors[i]] = gValue[thisState] + 1;
				fValue[successors[i]] = gValue[successors[i]] + abs(successors[i].first - goal.first) + abs(successors[i].second - goal.second);
				q.push_back(successors[i]);
				parent[successors[i]] = thisState;
			}

		}
		for(int i=0; i<q.size(); ++i){
			int min = fValue[q[i]];
			int minIndex = i;
			for(int j = i+1; j < q.size(); ++j){
				if(fValue[q[j]] < min){
					min = fValue[q[j]];
					minIndex = j;
				}
			}
			pair<int, int> temp = q[i];
			q[i] = q[minIndex];
			q[minIndex] = temp;
		}
		if (parent.count(goal) != 0)
		{
			return getPath(parent, goal);
		}
		//for(int i = 0; i < successors.size(); i++){
		//	cout << heuristics[i] << endl;
		//}*/
	}
	return vector<string>();
}

vector<string> questionTwo(Problem &problem)
{
	/// write your own code here
	//vector<Node*> q;
	priority_queue<Node*,vector<Node*>,compare> q;
	//map<Node*, Node*> parent;
	set<Node*, set_compare> tracker;
	pair<int, int> start = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();
	if (goals.size() <= 0)
		return vector<string>();
	Node * startN = new Node(start, goals);
	q.push(startN);
	tracker.insert(startN);
	int count = 0;
	while (q.size()!=0)
	{
		Node* thisState = new Node(q.top());
		q.pop();
		vector< pair< int, int> > tempG = thisState->getGoals();
		vector<pair<int, int> > successors = problem.getSuccessors(thisState->getLoc());
		for(int i=0; i<successors.size(); ++i)
		{
				vector< pair< int, int> > tGoals = tempG;
				bool isGoal = false;
				for(int j = 0; j < tGoals.size(); j++){
					if(successors[i].first == tGoals[j].first && successors[i].second == tGoals[j].second){
						tGoals.erase(tGoals.begin() + j);
						isGoal = true;
					}
					if(tGoals.size() == 0){
						Node* temp = new Node(successors[i], tGoals, thisState, isGoal);
						return getPath2(temp, startN);
					}	
				}
				Node* temp = new Node(successors[i], tGoals, thisState, isGoal);
				/*int random = 0;
				for(random = 0; random < tracker.size(); random++){
					if(*(tracker[random]) == *temp){
						break;
					}
				}
				if(random == tracker.size()){*/
				int random = tracker.size();
				tracker.insert(temp);
				if(tracker.size() > random){
					q.push(temp);
					//parent[temp] = thisState;
				//	tracker.push_back(temp);	
				}

					/* for (set<Node*>:: iterator it=tracker.begin(); it!=tracker.end(); ++it){
						cout << (*it)->getLoc().first << " " << (*it)->getLoc().second << endl;
					}
					cout << endl;*/

			
		}
	//count++;
	}
	return vector<string>();
}


vector<string> questionThree(Problem &problem)
{
	/// write your own code here
	priority_queue<Node*,vector<Node*>,compare> q;
	//map<Node*, Node*> parent;
	set<Node*, set_compare> tracker;
	pair<int, int> start = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();
	if (goals.size() <= 0)
		return vector<string>();
	Node * startN = new Node(start, goals);
	q.push(startN);
	//tracker.insert(startN);
	int count = 0;
	while (q.size()!=0)
	{
		Node* thisState = q.top();
		q.pop();
		if(tracker.count(thisState) == 0){
			tracker.insert(thisState);

		vector< pair< int, int> > tempG = thisState->getGoals();
		if(tempG.size() == 0){
			return getPath2(thisState, startN);
		}	
		vector<pair<int, int> > successors = problem.getSuccessors(thisState->getLoc());
		for(int i=0; i<successors.size(); ++i)
		{
				vector< pair< int, int> > tGoals = tempG;
				bool isGoal = false;
				for(int j = 0; j < tGoals.size(); j++){
					if(successors[i].first == tGoals[j].first && successors[i].second == tGoals[j].second){
						tGoals.erase(tGoals.begin() + j);
						isGoal = true;
					}
					
				}
				Node* temp = new Node(successors[i], tGoals, thisState, isGoal);
				/*int random = 0;
				for(random = 0; random < tracker.size(); random++){
					if(*(tracker[random]) == *temp){
						break;
					}
				}
				if(random == tracker.size()){*/
				//int random = tracker.size();
				//tracker.insert(temp);
				if(tracker.count(temp) == 0 ){
					q.push(temp);
					//parent[temp] = thisState;
				//	tracker.push_back(temp);	
				}

					/* for (set<Node*>:: iterator it=tracker.begin(); it!=tracker.end(); ++it){
						cout << (*it)->getLoc().first << " " << (*it)->getLoc().second << endl;
					}
					cout << endl;*/

			
		}
	}
	//count++;
	}
	return vector<string>();
}

vector<string> questionFour(Problem &problem)
{
	/// write your own code here
	priority_queue<Node2*,vector<Node2*>,compare3> q;
	//map<Node*, Node*> parent;
	set<Node2*, set_compare3> tracker;
	pair<int, int> start = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();
	if (goals.size() <= 0)
		return vector<string>();
	Node2 * startN = new Node2(start, goals);
	q.push(startN);
	//tracker.insert(startN);
	int count = 0;
	while (q.size()!=0)
	{
		Node2* thisState = q.top();
		q.pop();
		if(tracker.count(thisState) == 0){
			tracker.insert(thisState);

		vector< pair< int, int> > tempG = thisState->getGoals();
		if(tempG.size() == 0){
			return getPath3(thisState, startN);
		}	
		vector<pair<int, int> > successors = problem.getSuccessors(thisState->getLoc());
		for(int i=0; i<successors.size(); ++i)
		{
				vector< pair< int, int> > tGoals = tempG;
				bool isGoal = false;
				for(int j = 0; j < tGoals.size(); j++){
					if(successors[i].first == tGoals[j].first && successors[i].second == tGoals[j].second){
						tGoals.erase(tGoals.begin() + j);
						isGoal = true;
					}
					
				}
				Node2* temp = new Node2(successors[i], tGoals, thisState, isGoal);
				/*int random = 0;
				for(random = 0; random < tracker.size(); random++){
					if(*(tracker[random]) == *temp){
						break;
					}
				}
				if(random == tracker.size()){*/
				//int random = tracker.size();
				//tracker.insert(temp);
				if(tracker.count(temp) == 0 ){
					q.push(temp);
					//parent[temp] = thisState;
				//	tracker.push_back(temp);	
				}

					/* for (set<Node*>:: iterator it=tracker.begin(); it!=tracker.end(); ++it){
						cout << (*it)->getLoc().first << " " << (*it)->getLoc().second << endl;
					}
					cout << endl;*/

			
		}
	}
	//count++;
	}
	return vector<string>();	
}


vector<string> questionZero(Problem &problem)
{
	// A simple BFS to find the path from the start to the first goal.
	queue<pair<int, int> > q;
	map<pair<int, int>, pair<int, int> > parent;
	pair<int, int> start = problem.getStartState();
	vector<pair<int, int> > goals = problem.getGoals();
	if (goals.size() <= 0)
		return vector<string>();
	pair<int, int> goal = goals[0];
	q.push(start);
	parent[start] = start;
	while (!q.empty())
	{
		pair<int, int> thisState=q.front();
		q.pop();
		vector<pair<int, int> > successors = problem.getSuccessors(thisState);
		for(int i=0; i<successors.size(); ++i)
		{
			if (parent.count(successors[i]) == 0)
			{
				q.push(successors[i]);
				parent[successors[i]] = thisState;	
			}
		}
		if (parent.count(goal) != 0)
		{
			return getPath(parent, goal);
		}
	}
	return vector<string>();
}

void error()
{
	cout <<"run \'proj1 layout_name question_number\'" <<endl;
}

int main(int argc, char**argv)
{
	if (argc<3)	
	{
		error();
		return 0;
	}
	vector<string> _board;
	_board.clear();
	string namePattern = argv[1];
	string inputName; 
#ifdef _WIN32
	inputName = "layouts\\" + namePattern + ".lay";
#else
	inputName = "layouts/" + namePattern + ".lay";
#endif
	string outputName = namePattern + ".out";
	string queryName = namePattern + "_stats.out";
	try {
		ifstream fin;
		fin.open(inputName.c_str());
		while (!fin.eof())
		{
			string str;
			getline(fin, str);
			if (str.length() > 0)
				_board.push_back(str);
		}
		fin.close();
	} catch (...) {
		cout << "Error while loading the layout file..." << endl;
		return 1;

	}
	Problem problem(_board);
	vector<string> _path;
	switch (argv[2][0]){
		case '1': _path = questionOne(problem); break;
		case '2': _path = questionTwo(problem); break;
		case '3': _path = questionThree(problem); break;
		case '4': _path = questionFour(problem); break;
		default: _path = questionZero(problem);
	}
	try {
		ofstream fout;
		fout.open(outputName.c_str());
		for(int i=0; i<_path.size(); ++i)
		{
			fout << _path[i] << endl;
		}
		fout.close();
	} catch (...){
		cout << "Error while saving the results..." << endl;
		return 1;
	}
	cout << "Number of expanded states: " << problem.getExpansionCounts() << endl;
	cout << "Results are saved in " << outputName << endl; 
	problem.dumpQueries(queryName);
	return 0;
}
