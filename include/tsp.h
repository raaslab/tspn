#ifndef __TSP_H
#define __TSP_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <sstream>
#include <tour.h>
#include <graph.h>
#include <tsp_options.h>

using namespace std;

class TSP : public Tour {

	public:

		/**
		 * Default Constructor
		 */
		TSP() : Tour() {};

		/**
		 * Create a TSP tour
		 * @param[in] vertices Vertices 
		 */
		TSP(const vector<Position> &vertices);

		
	private:
		typedef Graph<Position> PositionGraph;
		PositionGraph g;
		vector<Position> vertices;
		void create2approxTour(void);
		bool writeTSPData(string filename);
		bool readTSPOutput(string filename, list<Position> &path);
};


#endif /* __TSP_H */
