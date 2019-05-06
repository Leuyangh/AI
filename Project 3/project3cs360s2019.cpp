#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <functional>
#include <algorithm>
#include <unordered_set>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iterator>
#include <float.h>
#include <utility>
#include <iomanip>
using namespace std;

double gam = 0.9;
double epsilon = 0.1;
double moveCost = -1;
double minChange = (epsilon*(1-gam))/gam;
double defaultU = -10.0; //apparently this value has the least differences between sample and real output

int greatest(double rightval, double leftval, double downval, double upval){
	//0 = right, 1 = left, 2 = down, 3 = up
	double direction = 0;
	double max = rightval;
	if(max <= leftval){
		direction = 1;
		max = leftval;
	}
	if(max <= downval){
		direction = 2;
		max = downval;
	}	
	if(max <= upval){
		direction = 3;
		max = upval;
	}
	return direction;
}

struct tile
{
	int x, y;
	double U;
	bool obstacle;
	bool goal;
	int policy = -1;
	tile(){
		U = defaultU;
		x = 0;
		y = 0;
		obstacle = false;
		goal = false;
		policy = -1;
	}
	tile(int ex, int ey, bool ob, bool go){
		x = ex;
		y = ey;
		obstacle = ob;
		goal = go;
		U = defaultU;
		policy = -1;
	};
	tile(double uwu, int ex, int ey, bool ob, bool go){
		x = ex;
		y = ey;
		obstacle = ob;
		goal = go;
		U = uwu;
		policy = -1;
	};
	tile(const tile& other){
		this->U = other.U;
		this->obstacle = other.obstacle;
		this->goal = other.goal;
		this->x = other.x;
		this->y = other.y;
		this->policy = other.policy;
	};
};

struct map{
	int dimensions;
	int obstacles;
	tile** grid;
	map(){
		dimensions = 0;
		obstacles = 0;
		grid = nullptr;
	};
	map(int dimensions_, int obstacles_, tile** grid_){
		dimensions = dimensions_;
		obstacles = obstacles_;
		grid = grid_;
	};
	void fillMap(string filename){
		ifstream file(filename);
		if(file.is_open()){
			file >> dimensions;
			file >> obstacles;
			grid = new tile*[dimensions];
			for(int i = 0; i < dimensions; i++){
				grid[i] = new tile[dimensions];
				for(int j = 0; j < dimensions; j++){
					grid[i][j] = tile(i, j, false, false);
				}
			}
			for(int i = 0; i < obstacles; i++){
				string line;
				file >> line;
				stringstream ss(line);
				string number;
				getline(ss, number, ',');
				int x = atoi(number.c_str());
				getline(ss, number, ',');
				int y = atoi(number.c_str());
				//cout << x << "," << y << endl;
				grid[y][x].obstacle = true;
				grid[y][x].U = -100;
			}
			string line;
			file >> line;
			stringstream ss(line);
			string number;
			getline(ss, number, ',');
			int x = atoi(number.c_str());
			getline(ss, number, ',');
			int y = atoi(number.c_str());
			//cout << x << "," << y << endl;
			grid[y][x].goal = true;
			grid[y][x].U = 100;
			file.close();
		}
		return;
	};
	void printInitial(){
		for(int i = 0; i < dimensions; i++){
			for(int j = 0; j < dimensions; j++){
				if(grid[i][j].obstacle){
					cout << "O";
				}
				else if(grid[i][j].goal){
					cout << "G";
				}
				else{
					cout << ".";
				}
			}
			cout << endl;
		}
	}
	tile* getTile(int x, int y){
		return &grid[x][y];
	}
	vector<tile> getNeighbors(int x, int y){
		vector<tile> neighbors;
		int tempx = min(x+1, dimensions-1);
			neighbors.push_back(grid[tempx][y]);
		tempx = max(x-1, 0);
			neighbors.push_back(grid[tempx][y]);
		int tempy = min(y+1, dimensions-1);
			neighbors.push_back(grid[x][tempy]);
		tempy = max(y-1, 0);
			neighbors.push_back(grid[x][tempy]);
		return neighbors;
	};
	double getEstimatedValue(vector<tile> neighbors, int direction){
		//0 = right, 1 = left, 2 = down, 3 = up
		double sum = 0.0;
		if(neighbors.size() != 4){
			cout << "there arent 4 neighbors. this is a bug" << endl;
		}
		for(int i = 0; i < neighbors.size(); i++){
			double chance = 0.1;
			if(i == direction){
				chance = 0.7;
			}
			sum += chance * neighbors[i].U;
		}
		return sum;
	};
	double getU(int x, int y){
		//cout << "updating (" << x << "," << y << ")" << endl;
		vector<tile> neighbors = this->getNeighbors(x, y);
		double rightVal = this->getEstimatedValue(neighbors, 0);
		double leftVal = this->getEstimatedValue(neighbors, 1);
		double downVal = this->getEstimatedValue(neighbors, 2);
		double upVal = this->getEstimatedValue(neighbors, 3);
		double maxVal = max(rightVal, leftVal);
		maxVal = max(maxVal, downVal);
		maxVal = max(maxVal, upVal);
		double U = -1 + (gam * maxVal);
		grid[x][y].U = U;
		return U;
	};
	void iterateValues(){
		bool done = false;
		while(!done){
			double d = 0;
			for(int i = 0; i < dimensions; i++){
				for(int j = 0; j < dimensions; j++){
					if(!grid[i][j].obstacle && !grid[i][j].goal){
						double initialU = grid[i][j].U;
						double newU = this->getU(i, j);
						double change = abs(newU - initialU);
						if(change > d){
							d = change;
						}
					}
				}
			}
			if(d < minChange){
				done = true;
			}
			//cout << "changed by " << d << " reiterating..." << endl;
		}
	};
	void calculatePolicy(){
		for(int i = 0; i < dimensions; i++){
			for(int j = 0; j < dimensions; j++){
				if(!grid[i][j].obstacle && !grid[i][j].goal){
					vector<tile> neighbors = this->getNeighbors(i, j);
					double rightVal = this->getEstimatedValue(neighbors, 0);
					double leftVal = this->getEstimatedValue(neighbors, 1);
					double downVal = this->getEstimatedValue(neighbors, 2);
					double upVal = this->getEstimatedValue(neighbors, 3);
					int direction = greatest(rightVal, leftVal, downVal, upVal);
					grid[i][j].policy = direction;
				}
			}
		}
	};
	void printU(){
		for(int i = 0; i < dimensions; i++){
			for(int j = 0; j < dimensions; j++){
				cout << setw(8) << grid[i][j].U << " ";
			}
			cout << endl;
		}
	};
	void printPolicy(bool andInt){
		//0 = right, 1 = left, 2 = down, 3 = up
		//this needs to be inverted because of [i][j] vs [j][i] stuff
		//right->down/left->up/down->right/up->left
		for(int i = 0; i < dimensions; i++){
			for(int j = 0; j < dimensions; j++){
				if(grid[i][j].obstacle){
					cout << "o";
				}
				else if(grid[i][j].goal){
					cout << ".";
				}
				else{
					if(grid[i][j].policy == 0){
						cout << "v";
					}
					if(grid[i][j].policy == 1){
						cout << "^";
					}
					if(grid[i][j].policy == 2){
						cout << ">";
					}
					if(grid[i][j].policy == 3){
						cout << "<";
					}
				}
			}
			cout << endl;
		}
		if(andInt){
			for(int i = 0; i < dimensions; i++){
				for(int j = 0; j < dimensions; j++){
					if(grid[i][j].policy == -1){
						cout << ".";
					}
					else{
						cout << grid[i][j].policy;
					}
				}
				cout << endl;
			}	
		}
	};
	void writeToFile(string filename){
		ofstream ofile(filename);
		for(int i = 0; i < dimensions; i++){
			for(int j = 0; j < dimensions; j++){
				if(grid[i][j].obstacle){
					ofile << "o";
				}
				else if(grid[i][j].goal){
					ofile << ".";
				}
				else{
					if(grid[i][j].policy == 0){
						ofile << "v";
					}
					if(grid[i][j].policy == 1){
						ofile << "^";
					}
					if(grid[i][j].policy == 2){
						ofile << ">";
					}
					if(grid[i][j].policy == 3){
						ofile << "<";
					}
				}
			}
			ofile << endl;
		}
	};
	const int countTies(){
		int ties = 0;
		for(int i = 0; i < dimensions; i++){
			for(int j = 0; j < dimensions; j++){
				vector<tile> neighbors = this->getNeighbors(i, j);
				unordered_set<double> values;
				for(int k = 0; k < 4; k++){
					double temp = neighbors[k].U;
					if(values.count(k) != 0){
						ties++;
					}
					values.insert(k);
				}
			}
		}
		return ties;
	};
};

//start of debug functions
int compareFiles(string filename1, string filename2){
	ifstream file1(filename1);
	ifstream file2(filename2);
	int line = 0;
	int differences = 0;
	if(file1.is_open() && file2.is_open()){
		while(file1.good() && file2.good()){
			char line1, line2;
			file1 >> line1;
			file2 >> line2;
			if(line1 != line2){
				differences++;
			}
			line++;
		}
	}
	file1.close();
	file2.close();
	return differences;
};

void test(int count){
	string inputfile, outputfile;
	for(int i = 0; i < count; i++){
		clock_t stime, etime;
		stringstream ss1, ss2;
		ss1 << "input-" << i << ".txt";
		ss2 << "output-" << i << ".txt";
		inputfile = ss1.str();
		outputfile = ss2.str();
		//cout << inputfile << " " << outputfile << endl;
		stime = clock();
		map mars;
		mars.fillMap(inputfile);
		mars.iterateValues();
		mars.calculatePolicy();
		mars.writeToFile("output.txt");
		etime = clock();
		double seconds = (double)(etime-stime)/CLOCKS_PER_SEC;
		int differences = compareFiles("output.txt", outputfile);
		cout << "Test case " << i << " finished in " << seconds << " seconds with " << differences << " differences" << endl;
		cout << "	Found " << mars.countTies() << " ties." << endl;
	}
};

int main(){
	map mars;
	mars.fillMap("input.txt");
	//mars.printInitial();
	mars.iterateValues();
	//mars.printU();
	mars.calculatePolicy();
	//mars.printPolicy(false);
	mars.writeToFile("output.txt");
	//if(compareFiles("output.txt", "output-1.txt")){
	//	cout << "All good!" << endl;
	//}
	
	//test(6);
	return 0;
}