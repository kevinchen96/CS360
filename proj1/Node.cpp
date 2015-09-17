#include "Node.h"

class compare2{
	bool comparator;
public:
  	compare2(const bool& param=false){
  		comparator = param;
  	}
    bool operator() (const int& lhs, const int&rhs) const{
    	if (comparator) return (lhs>rhs);
    	else return (lhs<rhs);
  	}
};

Node::Node(){

}
Node::Node(Node* n){
	loc = n -> getLoc();
	unvisited = n->getGoals();
	parent = n->getParent();
	gValue = n->getG();
	fValue = n->getF();
	heuristic = n->getHeuristic();
}
Node::Node(pair< int, int> temp, vector < pair < int, int> > temp2){
	loc = temp;
	unvisited = temp2;
	priority_queue<int,vector<int>,compare2> tempH;
	parent = this;
	gValue = 0;
	for(int i = 0; i <unvisited.size(); i++){
		tempH.push(abs(loc.first - unvisited[i].first) + abs(loc.second - unvisited[i].second));
	}
	heuristic = tempH.top();
	fValue = heuristic + gValue;

}
Node::Node(pair<int, int> temp, vector < pair < int, int> > temp2, Node* z, bool isGoal){
	loc = temp;
	unvisited = temp2;
	priority_queue<int,vector<int>,compare2> tempH;
	parent = z;
	gValue = parent->getG() + 1;
	for(int i = 0; i <unvisited.size(); i++){
		tempH.push(abs(loc.first - unvisited[i].first) + abs(loc.second - unvisited[i].second));
	}
	if(temp2.size() != 0){
		//int min = tempH[0];
		//int minIndex = 0;
		/*for(int i=1; i<tempH.size(); ++i){
			if(tempH[i] < min){
				min = tempH[i];
				minIndex = i;
			}
		}*/		
		heuristic = tempH.top();
		fValue = heuristic + gValue;
	}
	if(isGoal){
		fValue = gValue + 0;
		heuristic = 0;
	}

}
Node::~Node(){

}
pair<int, int> Node::getLoc(){
	return loc;
}
vector< pair< int, int> > Node::getGoals(){
	return unvisited;
}
map<pair <int, int> , int> Node::getMap(){
	return Map;
}
int Node::getHeuristic(){
	return heuristic;
}
int Node::getG(){
	return gValue;
}
int Node::getF(){
	return fValue;
}

bool Node::operator== (Node two){
	//cout << " hi" << endl;
	if(this->getLoc()==two.getLoc()){
		//cout <<"same locations" << endl;
		if(this->getGoals().size() == two.getGoals().size()){
			//cout << "Same goal size" << endl;
			vector<pair<int,int> > oneG = this->getGoals();
			vector<pair<int,int> > twoG = two.getGoals();
			for(int i = 0; i < getGoals().size(); i++){
				if(oneG[i] != twoG[i]){
			//		cout <<"different goal" << endl;
					return false;
				}
			}
			//cout << "same goals" << endl;
			return true;
		}
	}
	return false;
}
Node* Node::getParent(){
	return this->parent;
}