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
using namespace std;

time_t init;
time_t init1;
time_t fin;
time_t fin1;

struct hero
{
	int id = -1;
	int lastDigit = -1;
	double power = -1;
	double rMastery = -1;
	double dMastery = -1;
	int membership = -1;
	hero(int id_, double power_, double rMastery_, double dMastery_, int membership_):
		id(id_), power(power_), rMastery(rMastery_), dMastery(dMastery_), membership(membership_){
			lastDigit = id_ % 10;
		};
	hero(const hero& other){
		this->id = other.id;
		this->lastDigit = other.lastDigit;
		this->power = other.power;
		this->rMastery = other.rMastery;
		this->dMastery = other.dMastery;
		this->membership = other.membership;
	}
	void print() const{
		cout << "Hero ID: " << id /*<< ", Last Digit: " << lastDigit*/ << ", Power: " << power << ", Radiant Mastery: " << rMastery << ", Dire Mastery: " << dMastery << ", Team: " << membership << endl;
	}
	bool operator < (const hero& other) const{
		return id < other.id;
	}
};

void printlist(list<hero> List){
	for (auto it = List.begin(); it != List.end(); ++it){
		hero temp(*it);
		cout << temp.id << ", ";
	}
	cout << "END" << endl;
}

struct state
{
	list<hero> rHeros;
	list<hero> dHeros;
	list<hero> pool;
	list<hero> poolTotal;
	int heroTotal;
	int heroesPicked = -1;
	bool terminal = false;
	double advantage = -12345678987654321;
	vector<state> children;
	state* parent = nullptr;
	state(list<hero>& radiants, list<hero>& dires, list<hero>& remaining, list<hero>& ttl, int total, int picked, double advantage_, state* parent_){
		rHeros = radiants;
		dHeros = dires;
		pool = remaining;
		poolTotal = ttl;
		heroesPicked = picked;
		heroTotal = total;
		advantage = advantage_;
		parent = parent_;
		if(heroesPicked == 10){
			terminal = true;
		}
		//TODO Delete this for optimization
		if((rHeros.size() + dHeros.size() != heroesPicked)){
			cout << "Error: Heroes don't add up" << endl;
		}
		if(dHeros.size() > rHeros.size()){
			cout << "Error: Too many dires" << endl;
		}
		if(rHeros.size() >= dHeros.size() + 2){
			cout << "Error: Too many radiants" << endl;
		}
	}
	void print(){
		std::cout << "Hero Total: " << heroTotal << " | Heroes Picked: " << heroesPicked << " | Terminal: " << terminal << endl;
		if(terminal){
			cout << "Advantage: " << advantage << endl;
		}
		cout << "Radiants: " << endl;
		printlist(rHeros);
		//cout << "----------------------------------------------------------------------------" << endl;
		cout << "Dires: " << endl;
		printlist(dHeros);
		//cout << "----------------------------------------------------------------------------" << endl;
		cout << "Pool: " << endl;
		printlist(pool);
		//cout << "----------------------------------------------------------------------------" << endl;
		return;
	}
	void printchildren(){
		for(int i = 0; i < children.size(); i++){
			cout << "CHILD: " << i << endl;
			state temp = children[i];
			temp.print();
			cout << "============================================================" << endl;
		}
	}
	/*bool generateChildren(){
		if(terminal){
			return false;
		}
		//TODO can delete copying of other team vector for time saving
		if(heroesPicked % 2 == 0){
			//radiant pick
			cout << "Radiant pick" << endl;
			for(int i = 0; i < pool.size(); i++){
				list<hero> radiantCopy = rHeros;
				list<hero> direCopy = dHeros;
				list<hero> poolCopy = pool;
				list<hero>::iterator it = poolCopy.begin();
				advance(it, i);
				hero temp = *it;
				poolCopy.erase(it);
				radiantCopy.push_front(temp);
				//todo maybe need to sort ^
				int newPicked = heroesPicked + 1;
				state child(radiantCopy, direCopy, poolCopy, heroTotal, newPicked, advantage, this);
				children.push_back(child);
			}
		}
		else{
			//dire pick
			cout << "Dire pick" << endl;
			for(int i = 0; i < pool.size(); i++){
				list<hero> radiantCopy = rHeros;
				list<hero> direCopy = dHeros;
				list<hero> poolCopy = pool;
				list<hero>::iterator it = poolCopy.begin();
				advance(it, i);
				hero temp = *it;
				poolCopy.erase(it);
				direCopy.push_front(temp);
				//todo maybe need to sort ^
				int newPicked = heroesPicked + 1;
				state child(radiantCopy, direCopy, poolCopy, heroTotal, newPicked, advantage, this);
				children.push_back(child);
			}
		}
		return true;
	}
	double calculateAdvantage(){
		bool rSynergy = true;
		bool dSynergy = true;
		double radiantSum = 0;
		double direSum = 0;
		unordered_set<int> rLastDigits;
		unordered_set<int> dLastDigits;
		for(auto it = rHeros.begin(); it != rHeros.end(); ++it){
			hero temp = *it;
			radiantSum += temp.power * temp.rMastery;
			int digit = temp.lastDigit;
			if(rLastDigits.count(digit) == 0){
				rLastDigits.insert(digit);
			}
			else{
				rSynergy = false;
			}
		}
		for(auto it = dHeros.begin(); it != dHeros.end(); ++it){
			hero temp = *it;
			direSum += temp.power * temp.dMastery;
			int digit = temp.lastDigit;
			if(dLastDigits.count(digit) == 0){
				dLastDigits.insert(digit);
			}
			else{
				dSynergy = false;
			}
		}
		if(rSynergy){
			radiantSum += 120;
		}
		if(dSynergy){
			direSum += 120;
		}
		double adv = radiantSum - direSum;
		advantage = adv;
		return advantage;
	}*/
};
list<hero> a;
list<hero> b;
list<hero> c;
list<hero> d;
state Initial = state(a,b,c,d, 0, 0, 0, nullptr);
string algorithm;

struct gameState
{
	list<int> radiantHeroes;
	list<int> direHeroes;
	list<int> heroPool;
	int poolTotal;
	int currentlyPicked;
	bool terminal = false;
	gameState(list<int> r, list<int> d, list<int> p, int pT, int cP){
		radiantHeroes = r;
		direHeroes = d;
		heroPool = p;
		poolTotal = pT;
		currentlyPicked = cP;
		if(currentlyPicked == 10){
			terminal = true;
		}
	}
	gameState(state initial){
		for(auto a = initial.rHeros.begin(); a != initial.rHeros.end(); ++a){
			hero temp = *a;
			int tmp = temp.id;
			radiantHeroes.push_front(tmp);
		}
		for(auto a = initial.dHeros.begin(); a != initial.dHeros.end(); ++a){
			hero temp = *a;
			int tmp = temp.id;
			direHeroes.push_front(tmp);
		}
		for(auto a = initial.pool.begin(); a != initial.pool.end(); ++a){
			hero temp = *a;
			int tmp = temp.id;
			heroPool.push_front(tmp);
		}
		poolTotal = initial.heroTotal;
		currentlyPicked = initial.heroesPicked;
		terminal = initial.terminal;
		//todo delete this?
		heroPool.sort();
	}
	double score(){
		if (radiantHeroes.size() != 5 || direHeroes.size() != 5){
			cout << "Invalid score called" << endl;
		}
		double advantage = 0;
		bool rSynergy = true;
		bool dSynergy = true;
		double radiantSum = 0;
		double direSum = 0;
		unordered_set<int> rLastDigits;
		unordered_set<int> dLastDigits;
		for(auto a = radiantHeroes.begin(); a != radiantHeroes.end(); ++a){
			int idNum = *a;
			for(auto it = Initial.poolTotal.begin(); it != Initial.poolTotal.end(); ++it){
				hero temp = *it;
				if(temp.id == idNum){
					radiantSum += temp.power * temp.rMastery;
					int digit = temp.lastDigit;
					if(rLastDigits.count(digit) == 0){
						rLastDigits.insert(digit);
					}
					else{
						rSynergy = false;
					}
				}
			}
		}
		for(auto a = direHeroes.begin(); a != direHeroes.end(); ++a){
			int idNum = *a;
			for(auto it = Initial.poolTotal.begin(); it != Initial.poolTotal.end(); ++it){
				hero temp = *it;
				if(temp.id == idNum){
					direSum += temp.power * temp.dMastery;
					int digit = temp.lastDigit;
					if(dLastDigits.count(digit) == 0){
						dLastDigits.insert(digit);
					}
					else{
						dSynergy = false;
					}
				}
			}
		}
		if(rSynergy){
			radiantSum += 120;
		}
		if(dSynergy){
			direSum += 120;
		}
		double adv = radiantSum - direSum;
		advantage = adv;
		return advantage;
	}
};

state readInput(string filename){
	list<hero> radiants;
	list<hero> dires;
	list<hero> pool;
	list<hero> totalPool;
	int heroTotal = 0;
	int heroesPicked = 0;
	double advantage = -12345678987654321;
	ifstream file(filename);
	if(file.is_open()){
		file >> heroTotal;
		file >> algorithm;
		for(int i = 0; i < heroTotal; i++){
			int id, membership;
			double rMastery, dMastery, power;
			string line;
			file >> line;
			stringstream ss(line);
			string number;
			getline(ss, number, ',');
			id = atoi(number.c_str());
			getline(ss, number, ',');
			power = stod(number);
			getline(ss, number, ',');
			rMastery = stod(number);
			getline(ss, number, ',');
			dMastery = stod(number);
			getline(ss, number, ',');
			membership = atoi(number.c_str());
			//cout << line << endl;
			hero temp(id, power, rMastery, dMastery, membership);
			//temp.print();
			if(membership == 0){
				totalPool.push_front(temp);
				pool.push_front(temp);
			}
			else if(membership == 1){
				totalPool.push_front(temp);
				radiants.push_front(temp);
				heroesPicked++;
			}
			else{
				totalPool.push_front(temp);
				dires.push_front(temp);
				heroesPicked++;
			}
		}
		radiants.sort();
		dires.sort();
		pool.sort();
		state initial(radiants, dires, pool, totalPool, heroTotal, heroesPicked, advantage, nullptr);
		//cout << filename << " | Heroes: " << heroTotal << " | Depth: " << (10-heroesPicked) << endl;
		return initial;
	}
	else{
		cout << "Error opening file" << endl;
		return state(radiants, dires, pool, totalPool, 0, 0, advantage, nullptr);
	}
}

double MIN(gameState game);

double MAX(gameState game){
	if(game.terminal){
		return game.score();
	}
	double v = -DBL_MAX;
	for(int i = 0; i < game.heroPool.size(); i++){
		list<int> rCopy = game.radiantHeroes;
		list<int> dCopy = game.direHeroes;
		list<int> pCopy = game.heroPool;
		list<int>::iterator it = pCopy.begin();
		advance(it, i);
		int temp = *it;
		rCopy.push_front(temp);
		pCopy.erase(it);
		gameState child(rCopy, dCopy, pCopy, game.poolTotal, game.currentlyPicked + 1);
		v = max(v, MIN(child));
	}
	return v;
}

double MIN(gameState game){
	if(game.terminal){
		return game.score();
	}
	double v = DBL_MAX;
	for(int i = 0; i < game.heroPool.size(); i++){
		list<int> rCopy = game.radiantHeroes;
		list<int> dCopy = game.direHeroes;
		list<int> pCopy = game.heroPool;
		list<int>::iterator it = pCopy.begin();
		advance(it, i);
		int temp = *it;
		dCopy.push_front(temp);
		pCopy.erase(it);
		gameState child(rCopy, dCopy, pCopy, game.poolTotal, game.currentlyPicked + 1);
		v = min(v, MAX(child));
	}
	return v;
}

int minimax(gameState initialGame){
	double maxAdv = -DBL_MAX;
	int bestHero;
	//int currentHero;
	//double currentAdv;
	for(int i = 0; i < initialGame.heroPool.size(); i++){
		list<int> rCopy = initialGame.radiantHeroes;
		list<int> dCopy = initialGame.direHeroes;
		list<int> pCopy = initialGame.heroPool;
		list<int>::iterator it = pCopy.begin();
		advance(it, i);
		int temp = *it;
		rCopy.push_front(temp);
		pCopy.erase(it);
		gameState child(rCopy, dCopy, pCopy, initialGame.poolTotal, initialGame.currentlyPicked + 1);
		double childscore = MIN(child);
		if(childscore == 12345678987654321){
			return 00000;
		}
		if(childscore > maxAdv){
			bestHero = temp;
			maxAdv = childscore;
		}
	}
	return bestHero;
}

pair<double, int> abMIN(gameState game, double alpha, double beta);

pair<double, int> abMAX(gameState game, double alpha, double beta){
	if(game.terminal){
		double score = game.score();
		double id = game.direHeroes.back();
		return make_pair(score, id);
	}
	double v = -DBL_MAX;
	int best = 0;
	for(int i = 0; i < game.heroPool.size(); i++){
		list<int> rCopy = game.radiantHeroes;
		list<int> dCopy = game.direHeroes;
		list<int> pCopy = game.heroPool;
		list<int>::iterator it = pCopy.begin();
		advance(it, i);
		int temp = *it;
		rCopy.push_front(temp);
		pCopy.erase(it);
		gameState child(rCopy, dCopy, pCopy, game.poolTotal, game.currentlyPicked + 1);
		pair<double, int> result = abMIN(child, alpha, beta);
		if(result.first > v){
			v = result.first;
			best = temp;
		}
		if(v >= beta){
			return make_pair(v, best);
		}
		alpha = max(alpha, v);
	}
	return make_pair(v, best);
}

pair<double, int> abMIN(gameState game, double alpha, double beta){
	if(game.terminal){
		double score = game.score();
		double id = game.direHeroes.back();
		return make_pair(score, id);
	}
	double v = DBL_MAX;
	int best = 0;
	for(int i = 0; i < game.heroPool.size(); i++){
		list<int> rCopy = game.radiantHeroes;
		list<int> dCopy = game.direHeroes;
		list<int> pCopy = game.heroPool;
		list<int>::iterator it = pCopy.begin();
		advance(it, i);
		int temp = *it;
		dCopy.push_front(temp);
		pCopy.erase(it);
		gameState child(rCopy, dCopy, pCopy, game.poolTotal, game.currentlyPicked + 1);
		pair<double, int> result = abMAX(child, alpha, beta);
		if(result.first < v){
			v = result.first;
			best = temp;
		}
		if(v <= alpha){
			return make_pair(v, best);
		}
		beta = min(beta, v);
	}
	return make_pair(v, best);
}

int alphabeta(gameState initialGame){
	pair<double, int> best;
	best = abMAX(initialGame, -DBL_MAX, DBL_MAX);
	return best.second;
}

void runtests(int num, string filename){
	ofstream outfile(filename);
	for (int i = 0; i < num; i++){
		stringstream infile;
		infile << "randomInput" << i << ".txt";
		Initial = readInput(infile.str());
		gameState initialGame(Initial);
		time(&init);
		int mmResult = minimax(initialGame);
		time(&fin);
		double seconds = difftime(fin, init);
		outfile << "Input: " << i << endl;
		outfile << "MM: " << mmResult << "| Time: " << seconds << " seconds" << endl; 
		time(&init1);
		int abresult = alphabeta(initialGame);
		time(&fin1);
		double seconds1 = difftime(fin1, init1);
		outfile << "AB: " << abresult << "| Time: " << seconds1 << " seconds";
		if(seconds < seconds1){
			outfile << " WARNING: LONGER THAN MINIMAX";
		}
		outfile << endl;
		if(abresult != mmResult){
			outfile << "ERROR: RESULTS DIFFERENT" << endl;
		}
		outfile << "-------------------------------------------------------------------" << endl;
	}
}

int main(){
	/*cout << endl << "Test mode?";
	int testing;
	cin >> testing;
	if(testing != 0){
		cout << endl << "Test cases: ";
		int cases;
		cin >> cases;
		cout << endl << "Result file: ";
		string outfile = "output.txt";
		cin >> outfile;
		runtests(cases, outfile);
		return 1;
	}*/
	Initial = readInput("input.txt");
	gameState initialGame(Initial);
	int result;
	if(algorithm == "ab"){
		result = alphabeta(initialGame);
	}
	else{
		result = minimax(initialGame);
	}
	ofstream ofile("output.txt");
	ofile << result << endl;
	ofile.close();
	//initial.generateChildren();
	//initial.printchildren();
	return 1;
}