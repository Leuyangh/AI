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

void generateTests(int cases){
	for(int i = 0; i < cases; i++){
		stringstream ofname;
		ofname << "randomInput" << i << ".txt";
		ofstream outfile(ofname.str());
		bool toolarge = true;
		int heroTotal = (rand()%90) + 10;
		int heroesPicked;
		unordered_set<int> ids;
		if(heroTotal <= 20){
			heroesPicked = (rand()%5 * 2);
		}
		else if(heroTotal <= 40){
			heroesPicked = (rand()%4 * 2) + 2;
		}
		else if(heroTotal <= 60){
			heroesPicked = (rand()%3 * 2) + 4;
		}
		else{
			heroesPicked = (rand()%2 * 2) + 6;
		}
		int rheroesleft = heroesPicked/2;
		int dheroesleft = heroesPicked/2;
		outfile << heroTotal << endl << "ab" << endl;
		for(int j = 0; j < heroTotal; j++){
			bool taken = true;
			while(taken){
				int id = rand()%89999 + 10000;
				if(ids.count(id) == 0){
					ids.insert(id);
					outfile << id << ",";
					taken = false;
				}
			}
			double power = (double)(rand()%20000) / 100;
			double rmastery = (double)(rand()%100) / 100;
			double dmastery = (double)(rand()%100) / 100;
			int membership = 0;
			if(rheroesleft > 0){
				membership = 1;
				rheroesleft--;
			}
			else if(dheroesleft > 0){
				membership = 2;
				dheroesleft--;
			}
			outfile << power << "," << rmastery << "," << dmastery << "," << membership << endl;
		}
	}
}

int main(){
	time_t timer;
	srand(time(&timer));
	int cases;
	cout << endl << "Cases: ";
	cin >> cases;
	generateTests(cases);
	return 1;
}