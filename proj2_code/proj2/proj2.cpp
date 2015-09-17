#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>
#include <map>

using namespace std;

enum Status {UNSOLVED, SOLVED, CONTRADICTION};
string _status[3] = {"Unsolved", "Solved", "Contradiction"};

struct Cell {
	int i, j, k;
	Cell(): i(0), j(0), k(0){}
	Cell(int i1, int j1, int k1): i(i1), j(j1), k(k1){}
};

struct Puzzle {
	vector<vector<Cell> > disjunction;
	vector<Cell> evidence;
	Status status;
	void clearAll()
	{
		disjunction.clear();
		evidence.clear();
		status=UNSOLVED;
	}
};

struct set_compare {
    bool operator() (Cell lhs, Cell rhs) const{
        if(lhs.i != rhs.i){
        	return lhs.i < rhs.i;
        }
        else if(lhs.j != rhs.j){
        	return lhs.j < rhs.j;
        }
        else{
        	if(lhs.k != rhs.k){
        		return lhs.k < rhs.k;
        	}
        	return false;
        }
    }
};

struct set_compare2 {
    bool operator() (Cell lhs, Cell rhs) const{
        if(lhs.i != rhs.i){
        	return lhs.i < rhs.i;
        }
        else{
        	if(lhs.j != rhs.j){
        		return lhs.j < rhs.j;
        	}
        	return false;
        }
    }
};

Cell Guess(Puzzle puzzle, int x){
	set<Cell, set_compare> temp;
	for(int i = 0; i < puzzle.evidence.size(); i++){
		temp.insert(puzzle.evidence[i]);
	}
	int count = 0;
	int i, j;
	bool broken = false;
	for(i = 0; i < puzzle.disjunction.size(); i++){
		for(j = 0; j < puzzle.disjunction[i].size(); j++){
			if(count == x){
				broken = true;
				break;
			}
			count++;
		}
		if(broken) break;

	}
	if(broken){
		Cell y = puzzle.disjunction[i][j];
		/*cout << y.i << y.j << y.k << endl;
		cout << "This size is " << puzzle.disjunction[i].size() << endl;*/
		return y;
	}
	else{
		Cell y = Cell(-1,-1,-1);
		return y;
	}
}
// This is for Problem 1
Puzzle resolveStrimko(Puzzle &puzzle) {
	vector<Cell> oldCell = puzzle.evidence;
	vector<set<Cell, set_compare> > A;
	int size = (int) sqrt(puzzle.disjunction.size()/4);
	vector<vector<Cell> > chains;
	map<pair<int, int>, int> findChain;
	set<Cell, set_compare> tempB;
	for(int i = 0; i < puzzle.evidence.size(); i++){
		tempB.insert(puzzle.evidence[i]);
	}
	int count = 0;
	for(int i = puzzle.disjunction.size()-size; i < puzzle.disjunction.size(); i++){
		vector<Cell> temp;
		for(int j = 0; j < size; j++){
			temp.push_back(puzzle.disjunction[i][j]);
			findChain[make_pair(puzzle.disjunction[i][j].i, puzzle.disjunction[i][j].j)] = count;
		}
		chains.push_back(temp);
		count ++;
	}
	for(int i = 0; i < puzzle.disjunction.size(); i++){
		set<Cell, set_compare> temp;
		for(int j = 0; j < size; j++){
			temp.insert(puzzle.disjunction[i][j]);
		}
		A.push_back(temp);
	}
	while(puzzle.evidence.size() < puzzle.disjunction.size()/4){
		set<Cell, set_compare2> newCell;
		for(int i = 0; i < oldCell.size(); i++){
			set<Cell, set_compare> C;
			for(int j = 1; j <= size; j++){
				if(oldCell[i].j != j && tempB.count(Cell(oldCell[i].i, j, oldCell[i].k)) == 0){
					C.insert(Cell(oldCell[i].i, j, oldCell[i].k));
				}
				if(oldCell[i].i != j && tempB.count(Cell(j, oldCell[i].j, oldCell[i].k)) == 0){
					C.insert(Cell(j, oldCell[i].j, oldCell[i].k));
				}
				if(oldCell[i].i != chains[findChain[make_pair(oldCell[i].i, oldCell[i].j)]][j-1].i && oldCell[i].j != chains[findChain[make_pair(oldCell[i].i, oldCell[i].j)]][j-1].j && tempB.count(Cell(chains[findChain[make_pair(oldCell[i].i, oldCell[i].j)]][j-1].i, chains[findChain[make_pair(oldCell[i].i, oldCell[i].j)]][j-1].j, oldCell[i].k)) == 0){
					C.insert(Cell(chains[findChain[make_pair(oldCell[i].i, oldCell[i].j)]][j-1].i, chains[findChain[make_pair(oldCell[i].i, oldCell[i].j)]][j-1].j, oldCell[i].k));
				}
				if(oldCell[i].k != j && tempB.count(Cell(oldCell[i].i, oldCell[i].j, j)) == 0){
					C.insert(Cell(oldCell[i].i, oldCell[i].j, j));
				}
			}
			for(int j = 0; j < A.size(); j++){
				for(set<Cell, set_compare>::iterator it = C.begin(); it != C.end(); it++){
					if(A[j].count(*it) > 0){
						A[j].erase(*it);
					}
				}
				if(A[j].size() == 0){
					vector<vector<Cell> > result;
					for(int k = 0; k < A.size(); k++){
						vector<Cell> temp;
						for(set<Cell, set_compare>::iterator it = A[k].begin(); it != A[k].end(); it++){
							temp.push_back(*it);
						}
						result.push_back(temp);
					}
					puzzle.disjunction = result;
					puzzle.status = CONTRADICTION;
					return puzzle;
				}
			}

			for(int j = 0; j < A.size(); j++){
				if(A[j].size() == 1){
					bool countTrue = true;
					for(int k = 0; k < puzzle.evidence.size(); k++){
						if(puzzle.evidence[k].i == A[j].begin()->i && puzzle.evidence[k].j == A[j].begin()->j){
							countTrue = false;
						}
					}
					if(countTrue){
						newCell.insert(*(A[j].begin()));
						A.erase(A.begin()+j);
					}
					
				}
			}

		}
		if(newCell.size() == 0){
			vector<vector<Cell> > result;
			for(int k = 0; k < A.size(); k++){
				vector<Cell> temp;
				for(set<Cell, set_compare>::iterator it = A[k].begin(); it != A[k].end(); it++){
					temp.push_back(*it);
				}
				result.push_back(temp);
			}
			puzzle.disjunction = result;
			puzzle.status = UNSOLVED;
			return puzzle;
		}
		oldCell.clear();
		for(set<Cell, set_compare>::iterator it = newCell.begin(); it != newCell.end(); it++){
			puzzle.evidence.push_back(*it);
			oldCell.push_back(*it);
			tempB.insert(*it);
		}
	}
	vector<vector<Cell> > result;
	for(int k = 0; k < A.size(); k++){
		vector<Cell> temp;
		for(set<Cell, set_compare>::iterator it = A[k].begin(); it != A[k].end(); it++){
			temp.push_back(*it);
		}
		result.push_back(temp);
	}
	puzzle.disjunction = result;
	puzzle.status = SOLVED;
	return puzzle;
}

// This is for Problem 2
Puzzle solveStrimko(Puzzle &puzzle) {
	Puzzle temp = puzzle;
	temp = resolveStrimko(temp);
	if(temp.status == SOLVED){
		return temp;
	}
	else if(temp.status == UNSOLVED){
		int count = 0;
		while(true){
			Puzzle temp2 = puzzle;
			temp2.evidence = temp.evidence;
			Cell x = Guess(temp, count);
			if(x.i == -1 && x.j == -1 && x.k == -1) break;
			temp2.evidence.push_back(x);
			Puzzle temp3 = solveStrimko(temp2);
			if(temp3.status == SOLVED) return temp3;
			count++;
		}
	}
	return puzzle;
}


/// Do not change codes below

void error() {
	cout << "run \'proj1 problem_number\' or \'proj1 problem_number size_id\'" <<endl;
	cout << "\tproblem_number: 1 or 2" <<endl;
	cout << "\tsize_id: 4_1~4_5, 5_1~5_10, 6_1~6_5, 7_1~7_3" <<endl;
}

int main(int argc, char**argv) {
	if (argc < 2){
		error(); return 0;
	}
	vector<string> puzzleNameList;
	if (argc >= 3) {
		puzzleNameList.push_back(argv[2]);
	} else {
		string puzzleNameList_arr[] = {"4_1","4_2","4_3","4_4","4_5","5_1","5_2","5_3","5_4","5_5","5_6","5_7","5_8","5_9","5_10","6_1","6_2","6_3","6_4","6_5","7_1","7_2","7_3"};
		for (int i=0; i<23; ++i)
			puzzleNameList.push_back(puzzleNameList_arr[i]);
	}
	string inputName, outputName, puzzleName; 
	for (int puzzleId=0; puzzleId<puzzleNameList.size(); ++puzzleId){
		puzzleName = puzzleNameList[puzzleId];
#ifdef _WIN32
		inputName = "puzzles\\" + puzzleName + ".txt";
		outputName = "puzzles\\" + puzzleName + ".out";
#else
		inputName = "puzzles/" + puzzleName + ".txt";
		outputName = "puzzles/" + puzzleName + ".out";
#endif
		Puzzle puzzle;
		puzzle.clearAll();
		try {
			ifstream fin;
			fin.open(inputName.c_str());
			if (!fin.is_open())
				throw -1;
			int N, nDis, nEvi;
			fin >> N >> nDis >> nEvi;
			int loc_i, loc_j, num_k;
			for (int i=0; i<nDis; ++i){
				vector<Cell> this_dist;
				this_dist.clear();
				for( int j=0; j<N; ++j) {
					fin >> loc_i >> loc_j >> num_k;
					this_dist.push_back(Cell(loc_i, loc_j, num_k));
				}
				puzzle.disjunction.push_back(this_dist);
			}
			for (int i=0; i<nEvi; ++i)
			{
				fin >> loc_i >> loc_j >> num_k;
				puzzle.evidence.push_back(Cell(loc_i, loc_j, num_k));
			}
			fin.close();
		} catch (...) {
			cout << "Error while loading the puzzle file..." << endl;
			return 1;
		}
		cout << "Puzzle is loaded from " << inputName << endl;	
		Puzzle solution;
		
		switch (argv[1][0]){
			case '1': solution = resolveStrimko(puzzle); break;
			case '2': solution = solveStrimko(puzzle); break;
			case '0': solution = puzzle; break;
			default: error(); return 0;
		}
		try {
			ofstream fout;
			fout.open(outputName.c_str());
			fout << _status[solution.status] << endl;
			fout << solution.evidence.size() << endl;
			for(int i=0; i<solution.evidence.size(); ++i)
			{
				fout << solution.evidence[i].i << " " << solution.evidence[i].j << " " << solution.evidence[i].k << endl;
			}
			fout.close();
		} catch (...){
			cout << "Error while saving the results..." << endl;
			return 1;
		}
		cout << "Solution is saved in " << outputName << endl;	
	}
	return 0;
}
