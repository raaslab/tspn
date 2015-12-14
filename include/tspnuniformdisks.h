#ifndef __TSPN_UNIFORM_DISKS_H
#define __TSPN_UNIFORM_DISKS_H

#include <cstdlib>
#include <vector>
#include <tsp.h>
#include <tour.h>
#include <graph.h>

using namespace std;

class TSPNUniformDisks : public Tour {

	public:

		/**
		 * Create a TSPN tour where
		 * neighborhoods are uniform disks
		 * @param[in] centers Center positions 
		 * of neighborhoods 
		 * @param[in] radius Radius of each 
		 * neighborhoods 
		 */
		TSPNUniformDisks(const vector<Position> &centers, double radius);

		/**
		 * Gets the disks corresponding to the MIS
		 * @param[in] mis Vector that will be filled in
		 */
		void getMISDisks(vector<Position> &mis);
	
	private:
		typedef Graph<Position> PositionGraph;
		vector<Position> centers;
		double radius;
		
		void createTour(void);
		void goRoundDisks(
				const list<Position> &order, 
				const vector<Position> &diskIntersections,
				bool clockwise,
				list<Position> &waypoints);

		/**
		 * Gets the intersection between the tour
		 * and disks in the MIS.
		 * @param[in] order Order of disks to visit
		 * @param[in] intersection Positions where the tour
		 * intersects the disks in order.
		 */
		void getDiskTourIntersections(
				const list<Position> &order,
				vector<Position> &diskIntersections);
};


#endif /* __TSPN_UNIFORM_DISKS_H */
