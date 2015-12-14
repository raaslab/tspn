#ifndef __TOUR_H
#define __TOUR_H

/**
 * Tour is an ordered collection of samples and positions
 * indicating the tour by the robot. Samples marks the 
 * locations where UAV and/or UGV take samples in hotspots.
 * Positions is a list of positions visited by the UAV and/or
 * UGV.
 *
 */

#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <position.h>

using namespace std;

class Tour {
	public:

		/**
		 * Default constructor
		 */
		Tour() {};

		/**
		 * Get the length of the path
		 */
		double pathLength(void);

		/**
		 * Writes the path to a CSV file
		 * @param[in] filename Path to the file to write 
		 * @param[out] true if success, false is failure
		 */
		bool savepath(string filename);
		
		/**
		 * Get the entire tour as sequence
		 * of vertices in order
		 * @param[out] tour Tour as a std::list
		 */
		list<Position> getTourVertices(void) {
			return path;
		}


	protected:

		list<Position> path;
		
		/**
		 * Removes repeated points along the tour.
		 * Repeated points are same positions that
		 * follow each other in the order specified.
		 * @param[in] path Path to remove repeated
		 * points from. The path itself is modified.
		 */
		void removeRepeated(list<Position> &path) {
			if(path.size()==0) {
				return;
			}
			list<Position>::iterator it = path.begin();
			Position prev = *it;
			it++;
			while(it!=path.end()) {
				if(prev == *it) {
					it=path.erase(it); //get next element
				} else {
					prev = *it;
					it++;
				}
			}
		}
		/**
		 * Removes repeated points along the tour.
		 * Repeated points are same positions that
		 * follow each other in the order specified.
		 * @param[in] path Path to remove repeated
		 * points from. The path itself is modified.
		 */
		void removeRepeated(vector<Position> &path) {
			if(path.size()==0) {
				return;
			}
			vector<Position>::iterator it = path.begin();
			Position prev = *it;
			it++;
			while(it!=path.end()) {
				if(prev == *it) {
					it=path.erase(it); //get next element
				} else {
					prev = *it;
					it++;
				}
			}
		}



	private:
		
		Position getNearestPointToSegment(
				Position start, Position end, Position point);
		Position getNearestPointToLine(
				Position start, Position end, Position point);
		bool GetLeftIntWithSeg(
				const Position &i,const Position &j,
				const Position &h,double r, Position &left);
		bool GetRightIntWithSeg(
				const Position &i,const Position &j,
				const Position &h,double r, Position &right);
		inline double dot(Position v, Position w);

};

#endif /* __TOUR_H */
