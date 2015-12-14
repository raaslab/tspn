#include <cmath>
#include <stdlib.h>
#include <tsp.h>

double distance(Position p1, Position p2) {
	return sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

TSP::TSP(const vector<Position> &vertices) {

	this->vertices = vertices;

#ifdef USE_LINKERN
	writeTSPData("dumptsp.in");
	system(string(CONCORDE_PATH + "/LINKERN/linkern -o dumptsp.out dumptsp.in").c_str());
	readTSPOutput("dumptsp.out",path);
	system("rm dumptsp.*");
#else
	#ifdef USE_CONCORDE
		writeTSPData("dumptsp.in");
		system(string(CONCORDE_PATH + "/TSP/concorde -o dumptsp.out dumptsp.in").c_str());
		readTSPOutput("dumptsp.out",path);
		system("rm dumptsp.*");
	#else
		g = PositionGraph(vertices);
		g.createCompleteGraph(distance);
		create2approxTour();
	#endif
#endif
}

bool TSP::writeTSPData(string filename) {
	fstream outfile;
	outfile.open(filename,fstream::out);
	if(outfile.fail()) {
		std::cout << "[ERROR] " << filename << " could not be opened" << std::endl;
		return false;
	}

	// Write the header
	outfile << "NAME : dumptsp" << endl;
	outfile << "COMMENT : dumptsp" << endl;
	outfile << "TYPE : TSP" << endl;
	outfile << "DIMENSION : " << vertices.size() << endl;
	outfile << "EDGE_WEIGHT_TYPE : EUC_2D" << endl;
	outfile << "NODE_COORD_SECTION" << endl;

	// TODO concorde assumes integer weights, which is a problem if the points
	// are not too far from each other. So I'm scaling the points
	int scale = 100;

	for(unsigned int i = 0; i < vertices.size(); i++) {
		outfile << i << " " << scale*vertices[i].x << " " << scale*vertices[i].y << endl;
	}
	outfile << "EOF";

	return true;
}

bool TSP::readTSPOutput(string filename, list<Position> &path) {
	path.clear();
	fstream infile;
	infile.open(filename,fstream::in);
	if(infile.fail()) {
		std::cerr << "[ERROR] Could not open " << filename << endl;
		return false;
	}

	string line;
	getline(infile,line); //header line

#ifdef USE_LINKERN
	// file has format
	// i j edgeweight
	// i and j are indices in vertices
	double nums[3];
	while(getline(infile,line)) {
		// Tokenize
		stringstream ss(line);
		string value;
		int cnt = 3; //we expect each line to have 5 numbers
		while(getline(ss,value,' ')) {
			if(cnt==0) {
				std::cerr << "[ERROR] More than 3 values found in " << filename << endl;
				return false;
			}

			//TODO check atof for valid numbers
			nums[3-cnt] = atof(value.c_str());
			cnt--;
		}
		if(cnt>0) {
			std::cerr << "[ERROR] Less than 3 values found in one line in " << filename << endl;
			return false;
		}
		path.push_back(vertices[(int)nums[0]]);
	}
	path.push_back(vertices[(int)nums[1]]);
#else
	// concorde
	// file has format
	// i j k ...
	// i j k are indices in vertices
	while(getline(infile,line)) {
		// Tokenize
		stringstream ss(line);
		string value;
		while(getline(ss,value,' ')) {
			//TODO check atof for valid numbers
			path.push_back(vertices[(int)atof(value.c_str())]);
		}
	}
#endif
	return true;
}

void TSP::create2approxTour(void) {

	vector<PositionGraph::EDGE> mst = g.getMST();

	// Now create tour
	// get rid of visited vertices
	vector<bool> visited(mst.size(),false);
	path.clear();
	path.push_back(vertices[(*mst.begin()).first]);
	visited[0] = true;
	for(vector<PositionGraph::EDGE>::const_iterator eid = mst.begin();
			eid != mst.end(); eid++) {
		if(visited[(*eid).second]==false) {
			path.push_back(vertices[(*eid).second]);
			visited[(*eid).second] = true;
		}
	}
	path.push_back(vertices[(*mst.begin()).first]);
}
