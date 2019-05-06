#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <stack>
#include <functional>
#include <algorithm>
#include <unordered_set>
#include <stdlib.h>
#include <time.h>
struct point
{
	int x;
	int y;
	int animals;
	bool available;
	point(int xpos, int ypos){
		x = xpos;
		y = ypos;
		animals = 0;
		available = true;
	}
	point(int xpos, int ypos, int anims, bool free){
		x = xpos;
		y = ypos;
		animals = anims;
		available = free;
	}
};

struct forest
{
	int n;
	int cameraTotal;
	int camerasPlaced;
	int animals;
	int points;
	int totalCost;
	int pathCost;
	int estimatedCost;
	std::string key;
	std::vector<std::vector<point> > grid;
	std::vector<point> cameraPositions;
	bool operator < (const forest& b){
		return totalCost < b.totalCost;
	}
	forest(int dimensions, int cameraCount, int animalCount, std::vector<point> animalPos){
		n = dimensions;
		cameraTotal = cameraCount;
		camerasPlaced = 0;
		points = 0;
		totalCost = 0;
		pathCost = 0;
		estimatedCost = 0;
		key = "";
		animals = animalCount;
		for(int x = 0; x < n; x++){
			std::vector<point> row;
			for(int y = 0; y < n; y++){
				row.push_back(point(x,y));
			}
			grid.push_back(row);
		}
		for(int i = 0; i < animalPos.size(); i++){
			int animalX = animalPos[i].x;
			int animalY = animalPos[i].y;
			grid[animalX][animalY].animals++;
		}
	}
	forest(const forest& other){
		this->n = other.n;
		this->cameraTotal = other.cameraTotal;
		this->camerasPlaced = other.camerasPlaced;
		this->animals = other.animals;
		this->points = other.points;
		this->grid = other.grid;
		this->cameraPositions = other.cameraPositions;
		this->totalCost = other.totalCost;
		this->estimatedCost = other.estimatedCost;
		this->pathCost = other.pathCost;
		this->key = other.key;
	}
	void setCamera(point P){
		if (grid[P.x][P.y].available == false){
			std::cout << "ERROR: point not available" << std::endl;
			return;
		}
		if(camerasPlaced < cameraTotal){
			cameraPositions.push_back(P);
			camerasPlaced++;
			for(int i = 0; i < n; i++){
				grid[i][P.y].available = false;
			}
			for(int i = 0; i < n; i++){
				grid[P.x][i].available = false;
			}
			for(int i = 1; i < n; i++){
				if((P.x + i) < n && (P.y + i) < n){
					grid[P.x + i][P.y + i].available = false;
				}
				else{
					break;
				}
			}
			for(int i = -1; i > -n; i--){
				if((P.x + i) >= 0 && (P.y + i) >= 0){
					grid[P.x + i][P.y + i].available = false;
				}
				else{
					break;
				}
			}
			for(int i = 1; i < n; i++){
				if((P.x - i) >= 0 && (P.y + i) < n){
					grid[P.x - i][P.y + i].available = false;
				}
				else{
					break;
				}
			}
			for(int i = 1; i < n; i++){
				if((P.x + i) < n && (P.y - i) >= 0){
					grid[P.x + i][P.y - i].available = false;
				}
				else{
					break;
				}
			}

		}
		else{
			std::cout << "ERROR: Can't place more cameras" << std::endl;
			return;
		}
	}
	int score(){
		points = 0;
		if(camerasPlaced != cameraTotal){
			return -1;
		}
		for(int i = 0; i < cameraPositions.size(); i++){
			//std::cout << "scoring camera " << i;
			int x = cameraPositions[i].x;
			int y = cameraPositions[i].y;
			//std::cout << " at position " << x << "," << y << ". Animals found: ";
			points += grid[x][y].animals;
			//std::cout << grid[x][y].animals << std::endl;
		}
		return points;
	}
	int getPathCost(){
		pathCost = 0;
		for(int i = 0; i < cameraPositions.size(); i++){
			int x = cameraPositions[i].x;
			int y = cameraPositions[i].y;
			pathCost -= grid[x][y].animals;
		}
		return pathCost;
	}
	int estimateRemainingCost(){
		estimatedCost = 0;
		for(int x = 0; x < n; x++){
			for(int y = 0; y < n; y++){
				if(grid[x][y].available){
					estimatedCost -= grid[x][y].animals;
				}
			}
		}
		return estimatedCost;
	}
	int getTotalCost(){
		if(camerasPlaced < cameraTotal){
			totalCost = getPathCost() + estimateRemainingCost();
		}
		else{
			totalCost = getPathCost();
			estimatedCost = 0;
		}
		return totalCost;
	}
	bool goalTest(){
		return (camerasPlaced == cameraTotal);
	}
	std::string cameraPositionsToKey(){
		std::vector<int> arr;
		std::string combined = "";
		for(int i = 0; i < cameraPositions.size(); i++){
			std::string temp = "";
			char x = '0' + (cameraPositions[i].x + 1);
			char y = '0' + (cameraPositions[i].y + 1);
			temp += x;
			temp += y;
			arr.push_back(atoi(temp.c_str()));
			//std::cout << arr[i] << std::endl;
		}
		std::sort(arr.begin(), arr.end());
		for(int i = 0; i < arr.size(); i++){
			//std::cout << arr[i] << " ";
			std::stringstream ss;
			ss << arr[i];
			combined += ss.str();
		}
		key = combined;
		return key;
	}
};

void printForest(forest F){
	std::cout << "Animals per point: " << std::endl;
	for(int i = 0; i < F.n; i++){
		for(int j = 0; j < F.n; j++){
			std::cout << F.grid[i][j].animals << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void printForestAvailability(forest F){
	std::cout  << "Availability remaining: " << std::endl;
	for(int i = 0; i < F.n; i++){
		for(int j = 0; j < F.n; j++){
			//std::cout << F.grid[i][j].available << " ";
			if(F.grid[i][j].available){
				std::cout << ". ";
			}
			else{
				std::cout << "X ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	printForest(F);
}

void printForestCameras(forest F){
	std::cout << std::endl << F.camerasPlaced << " cameras placed" << std::endl;
	std::cout << "Images captured: " << F.points << std::endl;
	for(int k = 0; k < 3; k++){
		for(int i = 0; i < F.n; i++){
			for(int j = 0; j < F.n; j++){
				bool cameraOnPoint = false;
				for(int k = 0; k < F.cameraPositions.size(); k++){
					int x = F.cameraPositions[k].x;
					int y = F.cameraPositions[k].y;
					if(i == x && j == y){
						cameraOnPoint = true;
						break;
					}
				}
				if(cameraOnPoint){
					std::cout << "C ";
				}
				else{
					if(k == 0){
						std::cout << ". ";	
					}
					if(k == 1){
						if(F.grid[i][j].animals > 0){
							std::cout << F.grid[i][j].animals << " ";
						}
						else{
							std::cout << ". ";	
						}
					}
					if(k == 2){
						if(F.grid[i][j].available){
								if(F.grid[i][j].animals > 0){
								std::cout << F.grid[i][j].animals << " ";
							}
							else{
								std::cout << ". ";	
							}
						}
						else{
							std::cout << "x ";
						}	
					}
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	printForest(F);
}

point stringToPoint(std::string position){
	std::stringstream ss(position);
	std::string number;
	int x, y;
	getline(ss, number, ',');
	x = atoi(number.c_str());
	getline(ss, number, ',');
	y = atoi(number.c_str());
	return point(x, y);
}

bool generateAllSuccessors(std::stack<forest>& queue, forest Forest){
	if(Forest.camerasPlaced >= Forest.cameraTotal){
		return false;
	}
	for(int x = 0; x < Forest.n; x++){
		for(int y = 0; y < Forest.n; y++){
			if(Forest.grid[x][y].available){
				forest temp(Forest);
				temp.setCamera(point(x,y));
				queue.push(temp);
			}
		}
	}
	return true;
}

bool compareCost(const forest& A, const forest& B){
	return A.totalCost > B.totalCost;
}

void sortFrontier(std::vector<forest>& frontier){
	std::sort(frontier.begin(), frontier.end(), compareCost);
}

void printFrontier(std::vector<forest>& frontier){
	for(int i = 0; i < frontier.size(); i++){
		std::cout << "Frontier " << i << ": key = " << frontier[i].key << " | Cost = " << frontier[i].totalCost;
		std::cout << " (Path cost: " << frontier[i].pathCost << " | Estimated cost: " << frontier[i].estimatedCost << ")" << std::endl;
	}
}

bool stateInFrontier(const std::vector<forest>& frontier, forest Forest){
	bool isin = false;
	std::string thiskey = Forest.cameraPositionsToKey();
	for(int i = 0; i < frontier.size(); i++){
		if(thiskey == frontier[i].key){
			isin = true;
			break;
		}
	}
	return isin;
}

bool insertIntoFrontier(std::vector<forest>& frontier, forest Forest){
	if(frontier.empty()){
		frontier.push_back(Forest);
		return true;
	}
	if(Forest.totalCost <= frontier.back().totalCost){
		frontier.push_back(Forest);
		return true;
	}
	if(Forest.totalCost >= frontier.front().totalCost){
		frontier.insert(frontier.begin(), Forest);
		return true;
	}
	int l = 0;
	int r = frontier.size();
	int mid = 0;
	while (l <= r){
		mid = l + (r - l)/2;
		if(frontier[mid].totalCost == Forest.totalCost){
			std::vector<forest>::iterator it = frontier.begin() + mid;
			frontier.insert(it, Forest);
			return true;
		}
		if(frontier[mid].totalCost < Forest.totalCost){
			r = mid - 1;
		}
		if(frontier[mid].totalCost > Forest.totalCost){
			l = mid + 1;
		}
	}
	if(frontier[mid].totalCost > Forest.totalCost && frontier[mid + 1].totalCost <= Forest.totalCost){
		std::vector<forest>::iterator it = frontier.begin() + (mid + 1);
		frontier.insert(it, Forest);
		return true;
	}
	if(frontier[mid].totalCost < Forest.totalCost && frontier[mid - 1].totalCost >= Forest.totalCost){
		std::vector<forest>::iterator it = frontier.begin() + (mid);
		frontier.insert(it, Forest);
		return true;
	}
	printFrontier(frontier);
	std::cout << l << " " << r << " " << mid << std::endl;
	std::cout << "ERROR inserting into frontier" << std::endl;
	return false;
}

bool generateAllASTARSuccessors(std::vector<forest>& frontier, forest Forest, std::unordered_set<std::string>& explored){
	if(Forest.camerasPlaced >= Forest.cameraTotal){
		return false;
	}
	int childnum = 0;
	for(int x = 0; x < Forest.n; x++){
		for(int y = 0; y < Forest.n; y++){
			if(Forest.grid[x][y].available){
				forest temp(Forest);
				temp.setCamera(point(x,y));
				if(explored.count(temp.cameraPositionsToKey()) == 0){ 
					if(!stateInFrontier(frontier, temp)){
						childnum++;
						temp.getTotalCost();
						//std::cout << "inserting child: " << childnum << " Key: " << temp.key << " Cost: "<< temp.totalCost << std::endl;
						if(!insertIntoFrontier(frontier, temp)){
							return false;
						}
						//frontier.push_back(temp);
					}
				}
			}
		}
	}
	return true;
}

void printStack(std::stack<forest> queue){
	int i = queue.size();
	while (!queue.empty()){
		forest temp(queue.top());
		queue.pop();
		std::cout << "-------------------------------------------------" << std::endl;
		std::cout << "FOREST: " << i << std::endl;
		std::cout << "-------------------------------------------------" << std::endl;
		printForestAvailability(temp);
		std::cout << "-------------------------------------------------" << std::endl;
		i--;
	}
}

int DFS(forest Forest){
	int maxImages = 0;
	std::stack<forest> dfsQueue;
	forest* best;
	int states = 0;
	std::unordered_set<std::string> explored;
	if(generateAllSuccessors(dfsQueue, Forest)){
		while(!dfsQueue.empty()){
			states++;
			forest temp = dfsQueue.top();
			dfsQueue.pop();
			std::string key = temp.cameraPositionsToKey();
			if(explored.count(key) == 0){
				explored.insert(key);
				if(!(generateAllSuccessors(dfsQueue, temp))){//returns false if state has max cameras
					temp.score();
					if(temp.points > maxImages){
						maxImages = temp.points;
						best = new forest(temp);
					}
				}
			}
		}
		std::cout << "Algorithm used: DFS" << std::endl;
		std::cout << "States generated: " << states << std::endl;
		printForestCameras(*best);
		return maxImages;	
	}
	else{
		std::cout << "ERROR: Generating initial successors failed" << std::endl;
		return -1;
	}
}

int ASTAR(forest Forest){
	forest node(Forest);
	node.getTotalCost();
	std::vector<forest> frontier;
	std::unordered_set<std::string> explored;
	frontier.push_back(node);
	int states = 0;
	while(!frontier.empty()){
		states++;
		forest currentNode(frontier.back());
		frontier.pop_back();
		if(currentNode.goalTest()){
			std::cout << "Algorithm used: A* || Solution key: " << currentNode.key << std::endl;
			std::cout << "States generated: " << states << std::endl;
			currentNode.score();
			printForestCameras(currentNode);
			return -currentNode.totalCost;
		}
		std::string key = currentNode.cameraPositionsToKey();
		if(explored.count(key) == 0){
			explored.insert(key);
			//std::cout << "Exploring key: " << key << std::endl;
			if(generateAllASTARSuccessors(frontier, currentNode, explored)){
				//everything is fine, sort new frontier
				//std::cout << "-------------Loop " << states << " frontier------------" << std::endl;
				//printFrontier(frontier);
				//sortFrontier(frontier);
				//std::cout << "--------Loop " << states << " frontier after sort-------" << std::endl;
				//printFrontier(frontier);
				//std::cout << std::endl;
			}
			else{
				std::cout << "ERROR: something went wrong generating successors for ASTAR" << std::endl;
				return -1;
			}
		}
	}
	return -1;
}

int main() {
	int n, a, c;
	std::string algorthimToUse;
	std::ifstream file("input.txt");
	std::vector<std::string> animalPositions;
	std::vector<point> animalPoints;
	time_t sTimer;
	time(&sTimer);
	if (file.is_open()) {
		file >> n;
		file >> c;
		file >> a;
		file >> algorthimToUse;
		for (int i = 0; i < a; i++) {
			std::string line;
			file >> line;
			animalPositions.push_back(line);
		}
		for(int i = 0; i < animalPositions.size(); i++){
			point A = stringToPoint(animalPositions[i]);
			animalPoints.push_back(A);
		}
		forest Forest(n, c, a, animalPoints);
		std::ofstream outfile("output.txt");
		if(outfile.is_open()){
			if(algorthimToUse == "dfs"){
				outfile << DFS(Forest);
				time_t eTime;
				double seconds = difftime(time(&eTime), sTimer);
				std::cout << "Time passed : " << seconds << " seconds" << std::endl;
			}
			else if(algorthimToUse == "astar"){
				outfile << ASTAR(Forest);
				//printForest(Forest);
				time_t eTime;
				double seconds = difftime(time(&eTime), sTimer);
				std::cout << "Time passed : " << seconds << " seconds" << std::endl;
			}
			else{
				std::cout << "ERROR: algorthim to use is invalid" << std::endl;
			}
			outfile.close();
		}
		else{
			std::cout << "ERROR: opening outfile" << std::endl;
		}
	}
	return 0;
}