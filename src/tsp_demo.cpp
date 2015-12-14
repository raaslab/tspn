#include <iostream>
#include <list>
#include <tsp.h>

double dummy(int i, int j) {
	return 1.0;
}

using namespace std;

int main (int argc, char *argv[]) {

	vector<Position> vertices;
	for(int i = 0; i < 10; i++) {
		vertices.push_back(Position(i,0));
	}
	for(int i = 0; i < 10; i++) {
		vertices.push_back(Position(i,1));
	}
	
	TSP tsp(vertices);
	list<Position> tour = tsp.getTourVertices();

	cout << "Length of TSP: " << tsp.pathLength() << endl;
	for(list<Position>::const_iterator it = tour.cbegin();
			it != tour.cend(); it++) {
		cout << *it << " ";
	}
	tsp.savepath("tsp_demo.out");
	cout << endl;

}

