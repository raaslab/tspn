#ifndef __GRAPH_H
#define __GRAPH_H

#include <cfloat>
#include <vector>
#include <set>
#include <cassert>
#include <algorithm>
#include <position.h>

using namespace std;

template <class T>
class Graph {

	public:

		typedef vector<T> Vertices;	
		typedef typename vector<T>::iterator VertexIter;	
		typedef unsigned int VertexID;	
		typedef unsigned int EdgeID;	
		typedef pair<VertexID, VertexID> EDGE;	
		typedef vector<set<EdgeID>> Neighbors;

		/**
		 * Default constructor
		 */
		Graph() {};

		/**
		 * Create a graph given vertices
		 * @param[in] vertices Set of vertices
		 */
		Graph(const Vertices &vertices) {
			this->vertices = vertices;
			neighbors = Neighbors(vertices.size());
		}	

		/**
		 * Create a graph given vertices and edges
		 * @param[in] vertices Set of vertices
		 * @param[in] edges list of edges 
		 * @param[in] edges list of weights
		 * weights[i] corrs. to edges[i]
		 */
		Graph(const Vertices &vertices, 
				const vector<EDGE > &edges,
				const vector<EDGE > &weights) {
			assert(edges.size() == weights.size());
			this->vertices = vertices;
			this->edges = edges;
			this->weight = weights;
			neighbors = Neighbors(vertices.size());
			updateNeighbors(edges);
		}
		
		/**
		 * Creates a complete graph using
		 * the vertices of the Graph
		 * @param[in] func Function pointer to 
		 * a function that takes in two vertices
		 * and returns the weight
		 */
		void createCompleteGraph(double (*func)(T,T)) {
			neighbors = Neighbors(vertices.size());
			EdgeID eid;
			for(VertexID i=0; i<vertices.size(); i++) {
				for(VertexID j=i+1; j<vertices.size(); j++) {
					addEdge(EDGE(i,j),func(vertices[i],vertices[j]),eid);
				}
			}
		}


		/**
		 * Add an edge to the graph
		 * @param[in] edge Pair of VertexID
		 * @param[in] weight Weight of the edge
		 * @param[in] edgeid The ID of the new edge 
		 * will be stored here. If edge already existed, 
		 * then it'll point to existing edge.
		 * @param[out] added True if edge was added
		 * false if already existed. 
		 */
		bool addEdge(EDGE edge, double weight, EdgeID &edgeid) {	
			if(lookupEdge(edge.first,edge.second,edgeid)) {
				cerr << "[ERROR] Adding an edge which already existed" << endl;
				return false;
			} else {
				edges.push_back(edge);	
				edgeid = edges.size()-1;
				weights.push_back(weight);	
				updateNeighbor(edgeid);
				return true;
			}
		}

		/**
		 * Add an edge to the graph
		 * between two vertices
		 * @param[in] u VertexID of endpoint 
		 * @param[in] v VertexID of endpoint 
		 * @param[in] weight Weight of the edge
		 * @param[in] edgeid The ID of the new edge 
		 * will be stored here. If edge already existed, 
		 * then it'll point to existing edge.
		 * @param[out] added True if edge was added
		 * false if already existed. 
		 */
		bool addEdge(VertexID u, VertexID v, double weight, EdgeID &edgeid) {	
			return addEdge(EDGE(u,v),weight,edgeid);	
		}

		/**
		 * Get a list of vertices
		 * @param[out] vertices Set of all vertices
		 */
		Vertices getVertices(void) {
			return vertices;
		}

		/**
		 * Get a list of edges
		 * @param[out] edges Set of all edges
		 */
		vector<EDGE > getEdges(void) {
			return edges;
		}

		/**
		 * Get MST in order using Prims
		 * @param[out] mst MST as list of VertexID 
		 */
		vector<EDGE> getMST(void) {

			int numVertices = vertices.size();
			assert(numVertices > 1);

			vector<EDGE> mst;
			vector<VertexID> vadded;
			vector<VertexID> vleft(numVertices);
			for(int i = 0; i<numVertices; i++) {
				vleft.push_back(i);
			}
			vadded.push_back(*(vleft.begin()));
			vleft.erase(vleft.begin());

			while(vleft.size() > 0) {
				// Choose an edge u,v such that u is in vadded
				// and v is in vleft with minimal weight
				double minweight = DBL_MAX;

				// assuming complete map
				vector<VertexID>::iterator u,v;
				for(vector<VertexID>::iterator uit = vadded.begin();
						uit != vadded.end(); uit++) {
					for(vector<VertexID>::iterator vit = vleft.begin();
							vit != vleft.end(); vit++) {
						EdgeID eid;
						if(lookupEdge(*uit,*vit,eid)) {
							if(weights[eid] < minweight) {
								minweight = weights[eid];
								u = uit;
								v = vit;
							}
						}
					}
				}

				// Add edge and update vleft and vadded
				mst.push_back(EDGE(*u,*v));
				vadded.push_back(*v);
				vleft.erase(v);
			}
			return mst;
		}

		/**
		 * Gets the Maximal Independent Set for the graph
		 * @param[out] mis Set of vertices forming the MIS
		 */
		Vertices getMIS() {
			Vertices mis;
			vector<bool> covered(vertices.size(),false);

			// Pick an uncovered vertex
			// Add to MIS. Delete all neighbors of said vertex
			// from uncovered. Instead of having to actually 
			// delete vertices and neighbors, we'll just use the 
			// flag
			for(VertexID i=0; i<vertices.size(); i++) {
				if(!covered[i]) {		// Add to mis, cover all neighbors
					mis.push_back(vertices[i]);
					set<EdgeID> n = neighbors[i];
					set<EdgeID>::iterator it = n.begin();
					for(; it!=n.end(); it++) {
						covered[edges[*it].first] = true;
						covered[edges[*it].second] = true;
					}
				}
			}

			return mis;
		}

		void print(void) {
			for(VertexID i=0; i < vertices.size(); i++) {
				cout << " Neighbors of " << vertices[i] << ": ";
				set<VertexID>::iterator it = neighbors[i].begin();
				for(; it!=neighbors[i].end(); it++) {
					cout << "[" << edges[*it].first << " " << edges[*it].second << "] ";
				}
				cout << endl;
			}
		}

	private:
		Vertices vertices;
		Neighbors neighbors;
		vector<EDGE> edges;
		vector<double> weights;

		/**
		 * Updates the list of neighbors for both 
		 * vertices of an edge.
		 * @param[in] edgeid Edge whose addpoints will
		 * be added to mutual neighbors.
		 */
		void updateNeighbor(EdgeID edgeid) {
			VertexID u = edges[edgeid].first;
			VertexID v = edges[edgeid].second;
			assert(neighbors.size() > u);
			assert(neighbors.size() > v);
			neighbors[u].insert(edgeid);
			neighbors[v].insert(edgeid);
		}

		/**
		 * Updates the list of neighbors
		 * for all the neighbors, given a set of 
		 * edges.
		 * @param[in] edges List of edges
		 */
		void updateNeighbors(const vector<EDGE> &edges) {
			for(vector<EDGE>::const_iterator eid = edges.cbegin();
					eid != edges.cend(); eid++) {
				updateNeighbor(eid);
			}
		}

		/**
		 * Lookup a vertexid in the graph 
		 * for a given key 
		 * @param[in] vertex Vertex to lookup
		 * @param[in] vertexid VertexID for 
		 * the given key in the graph will be
		 * stored at this location
		 * @param[out] found True if found, false
		 * otherwise
		 */
		bool lookupVertex(
				const T &key, 
				VertexID &vertexid) {
			VertexIter it = vertices.find(key);
			if(it!=vertices.end()) {
				vertexid = distance(vertices.begin(),it);
				return true;
			} else {
				return false;
			}
		}

		/**
		 * Lookup an edge in the graph 
		 * for a given vertexids. For undirected graph,
		 * vertices can be specified in any order.
		 * @param[in] v1 VertexID for one endpoint
		 * @param[in] v2 VertexID for other endpoint
		 * @param[in] edgeid EdgeID of the edge if found
		 * @param[out] found True if found, false
		 * otherwise
		 */
		bool lookupEdge(
				VertexID v1,
				VertexID v2,
				EdgeID &edgeid) {
			assert(edges.size() == weights.size());

			//TODO make faster using neighbor lookups
			//set<VertexID>::const_iterator it;
			//set<VertexID> n1 = neighbors[v1];
			//it = find(n1.cbegin(),n1.cend(),v2); //assuming neighbors store both directions
			//if(it!=n1.cend()) {
			//	edgeid = *it;
			//	return true;
			//} else {
			//	return false;
			//}

			vector<EDGE>::iterator eid = edges.begin();	
			vector<double>::iterator wid = weights.begin();	
			for(; wid != weights.end() && eid != edges.end(); wid++,eid++) {
				VertexID i = (*eid).first;
				VertexID j = (*eid).second;
				if((i==v1 && j==v2) || (i==v2 && j==v1)) {
					//weight = *wid;
					edgeid = distance(edges.begin(),eid);
					return true;
				}
			}
			return false;
		}

};


#endif /* __GRAPH_H */
