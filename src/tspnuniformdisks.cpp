#include <tspnuniformdisks.h>

inline double dotpro(Position v, Position w) {
	return v.x*w.x + v.y*w.y;
}
inline double crossprosign(Position v, Position w) {
	if((v.x*w.y-v.y*w.x)*(-1.0) < 0 )	return 1.0;
	else	return -1.0;
}

TSPNUniformDisks::TSPNUniformDisks(
		const vector<Position> &c, double r) {
	centers = c;
	radius = r;
	createTour();
}

void TSPNUniformDisks::createTour(void) {

	// Create a tsp tour of the centers
	// of disks in MIS
	vector<Position> mis, diskIntersections;
	getMISDisks(mis);
	TSP tsp(mis);
	list<Position> order = tsp.getTourVertices();
	removeRepeated(order);
	getDiskTourIntersections(order,diskIntersections);
	assert(diskIntersections.size() > 2);
	
	// TODO To complete the tour, we should join the end to the start
	Position final = diskIntersections.back();
	diskIntersections.pop_back();
	diskIntersections.insert(diskIntersections.begin(),final);
	
	// Clockwise traversal of each disk
	path.clear();
	list<Position> waypoints;
	goRoundDisks(order,diskIntersections,true,waypoints);
	path.splice(path.end(),waypoints);
	path.push_back(final);
	goRoundDisks(order,diskIntersections,false,waypoints);
	path.splice(path.end(),waypoints);
	path.push_back(final);
	
	//path = order;
}

void TSPNUniformDisks::getDiskTourIntersections(
		const list<Position> &order,
		vector<Position> &diskIntersections) {
	// Extract where the tsp tour of centers
	// intersects the disks centered at each center
	// Each edge in the tour contributes to two intersections
	diskIntersections.clear();
	list<Position>::const_iterator iit = order.cbegin();
	list<Position>::const_iterator jit = order.cbegin();
	jit++;
	for(;iit != order.cend() && jit != order.cend(); iit++,jit++) {
		Position i = *iit;
		Position j = *jit;
		// Point between i and j at a distance r from i:
		// r/l*i + (1-r/l)*j where l is distance bet. j and i
		double t = radius/i.distance(j);
		diskIntersections.push_back(
				Position(t*i.x+(1-t)*j.x,t*i.y+(1-t)*j.y));
		diskIntersections.push_back(
				Position(t*j.x+(1-t)*i.x,t*j.y+(1-t)*i.y));
	}
}

void TSPNUniformDisks::getMISDisks(vector<Position> &mis) {
	// Find the mis of overlapping disks
	// by building a graph with edges if
	// disks overlap
	mis.clear();
	PositionGraph g(centers);
	PositionGraph::EdgeID eid;
	for(PositionGraph::VertexID i = 0; i<centers.size(); i++) {
		// Check overlap with all centers
		// Don't repeat comparisons, so start 
		// with items ahead in the list only.
		for(PositionGraph::VertexID j = i+1; j<centers.size(); j++) {
			if(centers[i].distance(centers[j]) < 2*radius) {
				g.addEdge(i,j,1,eid);
			}
		}
	}
	mis = g.getMIS();
}

void TSPNUniformDisks::goRoundDisks(
		const list<Position> &order, 
		const vector<Position> &diskIntersections,
		bool clockwise,
		list<Position> &waypoints) {
	
	waypoints.clear();
	int i = 0;
	for(list<Position>::const_iterator it = order.cbegin();
			it != order.cend() && i<(int)diskIntersections.size(); it++) {

		// Pairs
		Position c = *it;		
		Position s = diskIntersections[i];
		Position t = diskIntersections[i+1];
		double bearing = atan2(s.y-c.y,s.x-c.x);
		double dott = dotpro(t-c,s-c)/(s.distance(c)*t.distance(c));
		dott = dott >= 1.0 ? 1.0:dott;
		dott = dott <= -1.0 ? -1.0:dott;
		double alpha = crossprosign(t-c,s-c)*acos(dott);
		double arclen;
		double dir;
		
		if(clockwise) {
			if(alpha>0) {
				arclen = radius*alpha;
			} else {
				arclen = 2*M_PI*radius + radius*alpha;
			}
			dir = -1;
		} else {
			if(alpha>0) {
				arclen = 2*M_PI*radius - radius*alpha;
			} else {
				arclen = -radius*alpha;
			}
			dir = 1;
		}

		double len = 0;
		while(len < arclen) {
			Position interp;
			interp.x = c.x + radius*cos(bearing+dir*len/radius);
			interp.y = c.y + radius*sin(bearing+dir*len/radius);
			waypoints.push_back(interp);
			len = len + 0.01;
		}
		i=i+2;
	}
}
