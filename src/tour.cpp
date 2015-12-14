#include <cassert>
#include <cfloat>
#include <algorithm>
#include <tour.h>

double Tour::pathLength(void) {
	// Iterator through the list
	// And compute pairwise distances and accumulate them
	
	assert(path.size()>0);

	if(path.size()==1) {
		return 0;
	}
	
	list<Position>::const_iterator itI = path.begin();
	list<Position>::const_iterator itJ = path.begin();
	itJ++;
	double dist = 0;
	for(;itJ!=path.end(); itI++,itJ++) {
		dist = dist + (*itI).distance((*itJ));
	}
	return dist;
}

bool Tour::savepath(string filename) {
	fstream file(filename,fstream::out);
	if(!file.good()) {
		std::cerr << "[ERROR] Could not write path to file " << filename << endl;
		return false;
	}

	for(list<Position>::const_iterator it = path.begin(); it != path.end(); it++) {
		file << (*it).x << ", " << (*it).y << endl;
	}
	return true;
}

bool Tour::GetLeftIntWithSeg(
		const Position &i, const Position &j,
		const Position &h, double r, Position &left) {

	Position nearest = getNearestPointToLine(i,j,h);
	double d = h.distance(nearest);
	if(d - r >= EPSILON) {
		return false;
	}
	if(nearest==i || (i.distance(h) - r <= EPSILON)) {
		left= i;
		return true;
	}
	double t = sqrt(r*r-d*d);
	double di = i.distance(nearest);
	left.x = (t/di)*i.x + (1-t/di)*nearest.x;
	left.y = (t/di)*i.y + (1-t/di)*nearest.y;

	// Check if it lies in between i and j
	if(((left.x-i.x) >= -EPSILON && (left.x-j.x) <= EPSILON) ||
			((left.x-i.x) <= EPSILON && (left.x-j.x) >= -EPSILON)) {
		return true;
	} else {
		return false;
	}
}

bool Tour::GetRightIntWithSeg(
		const Position &i, const Position &j,
		const Position &h, double r, 
		Position &right) {

	Position nearest = getNearestPointToLine(i,j,h);
	double d = h.distance(nearest);
	if(d - r >= EPSILON) {
		return false;
	}
	if(nearest==j || (j.distance(h) - r <= EPSILON)) {
		right = j;
		return true;
	}
	double t = sqrt(r*r-d*d);
	double dj = j.distance(nearest);
	right.x = (t/dj)*j.x + (1-t/dj)*nearest.x;
	right.y = (t/dj)*j.y + (1-t/dj)*nearest.y;

	// Check if it lies in between i and j
	if(((right.x-i.x) >= -EPSILON && (right.x-j.x) <= EPSILON) ||
			((right.x-i.x) <= EPSILON && (right.x-j.x) >= -EPSILON)) {
		return true;
	} else {
		return false;
	}
}

Position Tour::getNearestPointToSegment(
		Position start, Position end, Position point) {

	// First check if start and end are same
	double length = start.distance(end);
	if(abs(length) < EPSILON) {
		return start;
	}

	// Find point of intersection x
	// as a scalar t such that x = start + t*(end-start)
	// Also: t = (end-start).(test-start)/|end-start|^2
	double t = dot(end-start,point-start)/(length*length);
	if(t < 0) { // beyond start
		return start;
	} else if(t>1) { //beyond end
		return end;
	} else { //somewhere in between
		Position poi = start;
		poi.x = start.x+t*(end.x-start.x);
		poi.y = start.y+t*(end.y-start.y);
		return poi;
	}
}

Position Tour::getNearestPointToLine(
		Position start, Position end, Position point) {

	// First check if start and end are same
	double length = start.distance(end);
	if(abs(length) < EPSILON) {
		return start;
	}

	// Find point of intersection x
	// as a scalar t such that x = start + t*(end-start)
	// Also: t = (end-start).(test-start)/|end-start|^2
	double t = dot(end-start,point-start)/(length*length);
	Position poi = start;
	poi.x = start.x+t*(end.x-start.x);
	poi.y = start.y+t*(end.y-start.y);
	return poi;
}

inline double Tour::dot(Position v, Position w) {
	return v.x*w.x + v.y*w.y;
}

