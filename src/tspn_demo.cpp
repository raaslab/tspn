#include <iostream>
#include <vector>

#include <tspnuniformdisks.h>

using namespace std;

int main(int argc, char* argv[]) {

	double radius = 1;
	int numPts = 20;
	double side = 10;
	vector<Position> centers;

	// Randomly generate disk centers in a box of side X
	for(int i = 0; i < numPts; i++) {
		centers.push_back(Position(side/100*(rand()%100), side/100*(rand()%100)));
	}

	TSPNUniformDisks tspn(centers,radius);
	tspn.savepath("tspn.out");
	cout << "Length of TSPN tour " << tspn.pathLength() << endl;

	return 0;
}
