#ifndef __POSITION_H
#define __POSITION_H
/**
 * Implements a position represented as (x,y)
 * Methods to compare two positions for relative
 * positions.
 */

#define EPSILON 2e-4

#include <cmath>
#include <fstream>
using namespace std;

class Position {

	friend ostream& operator<<(ostream& output, const Position& p) {
		output << "[" << p.x << ", " << p.y << "]";
		return output;
	}
	/**
	 * Add two positions (c = a + b)
	 * @param[in] a Position to add 
	 * @param[in] b Position to add 
	 * @param[out] c =a+b Position result
	 */
	friend Position operator+(const Position &a, const Position &b) {
		return Position(a.x+b.x,a.y+b.y);
	}

	/**
	 * Subtracts two positions (c = a - b)
	 * @param[in] a Position to subtract 
	 * @param[in] b Position to subtract 
	 * @param[out] c =a-b Position result
	 */
	friend Position operator-(const Position &a, const Position &b) {
		return Position(a.x-b.x,a.y-b.y);
	}


	public:
	/**
	 * Default constructor
	 */
	Position() {};

	/**
	 * @param[in] x X-coordinate
	 * @param[in] y Y-coordinate
	 */
	Position(double x, double y) {this->x = x; this->y = y;}

	/**
	 * Create a position by translating
	 * a given position.
	 * @param[in] origin Position from which to create a new one
	 * @param[in] x X-offset applied to origin
	 * @param[in] y Y-offset applied to origin
	 */
	Position(const Position &origin, double x, double y) {
		this->x = origin.x+x; this->y = origin.y+y;
	}


	/**
	 * Checks if this position is to the 
	 * left of other position
	 * @param[in] other Other position
	 */
	bool isToLeftOf(const Position &other) const {
		if(this->x-other.x < 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Checks if this position is below
	 * the other position
	 * @param[in] other Other position
	 */
	bool isBelow(const Position &other) const {
		if(this->y-other.y < 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Find Euclidean distance to another hotspot
	 * @param[in] other Hotspot to find the distance to
	 * @param[out] distance Distance between this and to
	 */
	double distance(const Position &other) const {
		return sqrt(pow(x-other.x,2)+pow(y-other.y,2));
	}

	/**
	 * Compares two Positions elements-wise
	 * using EPSILON as the threshold
	 * @param[in] other Position to compare to.
	 */
	bool operator==(const Position &other) const {
		if(abs(this->x-other.x)<EPSILON && 
				abs(this->y-other.y)<EPSILON)
			return true;
		else
			return false;
	}

	/**
	 * Compare two positions. We just use
	 * lexicographical ordering, first compare
	 * X-coords then Y-coords.
	 * @param[in] other Position to compare to
	 */
	bool operator<(const Position &other) const {
		if(this->x != other.x) {
			return (this->x < other.x);
		} else {
			return (this->y < other.y);
		};
	}

	double x, y;
};

#endif /* __POSITION_H */
