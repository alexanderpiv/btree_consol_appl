#pragma once
#include <queue>
using namespace std;

class vertex {
public:
	vertex() {
		next = nullptr;
	}
	vertex(int Val, int Arr_loc, int Weight = -1) { //ignore weight for now
		val = Val;
		weight = Weight;
		next = nullptr;
		arr_loc = Arr_loc;

		color = 0;
		dist = INT_MAX;
		pred = -1;
	}
	vertex* getNext() {
		return next;
	}
	void setNext(vertex* Next) {
		next = Next;
	}
	int getArrLoc() {
		return arr_loc;
	}
	int getVal() {
		return val;
	}
	//not making below private just for convininece for now
	int color; //0=white, 1=gray, 2=black
	int dist; //distance
	int pred; //predecessor
	vertex* pred2;
private:
	int val;
	int weight;
	vertex* next;
	int arr_loc;

};
class graph {
public:
	//constructing graph with inputted number of vertices
	graph(int v_size) {
		adj_list = new vertex*[v_size];
		for (int i = 0; i < v_size; i++) {
			adj_list[i] = nullptr;
		}
		//curr_fill = 0;
		size = v_size;
		visited = new bool[size];
		for (int i = 0; i < size; i++) {
			visited[i] = false;
		}
	}
	void addVertex(vertex* V) {
		//adj_list[0] points to v0 and v0.next points to v00, etc
		//so if vertex* adj_list[]
		if (adj_list[V->getArrLoc()] == nullptr) {
			adj_list[V->getArrLoc()] = V;
		}
		else {
			vertex* v_traverse = adj_list[V->getArrLoc()];
			while (v_traverse->getNext() != nullptr) {
				v_traverse = v_traverse->getNext();
			}
			v_traverse->setNext(V);
		}
	}
	int getSize() {
		return size;
	}
	vertex** getAdjList() {
		return adj_list;
	}
	queue<vertex*> q; //not private just for convinience
	bool* visited;
	queue<vertex*> v_q;
private:
	vertex** adj_list; //a list of vertex pointers
	int size;
};

//color=1 not used anywjere, except fo rocndition color!=0/color==0
//pred and dist are not used either..
void bfs(graph g, int s) {
	if (!g.visited[s]) {
		printf("visiting first %d ", s);
		g.visited[s] = true;
	}
	//queue all nodes of s.arrloc
	vertex* traverse = g.getAdjList()[s];
	if (traverse->color == 0) {
		g.q.push(traverse);
		traverse->color = 1; //grey - means already in queue but not visited yet
		traverse->pred = s;
		traverse->dist = 1;
	}
	while (traverse->getNext() != nullptr) {
		if (traverse->getNext()->color == 0) {
			g.q.push(traverse->getNext());
			traverse->getNext()->color = 1;
			traverse->pred = s;
			traverse->dist = 1;
		}
		traverse = traverse->getNext();
	}
	while (!g.q.empty() && g.q.front()->color != 2) {
		int nextVal = g.q.front()->getVal();
		g.q.front()->color = 2; //black - already visited node
		if (!g.visited[g.q.front()->getVal()]) {
			printf("popping %d ", g.q.front()->getVal());
			g.visited[g.q.front()->getVal()] = true;
		}
		g.q.pop();

		//New start
		traverse = g.getAdjList()[nextVal];
		if (traverse->color == 0) {
			g.q.push(traverse);
			traverse->color = 1; //grey - means already in queue but not visited yet
			traverse->pred = nextVal;
			traverse->dist = 1;
		}
		while (traverse->getNext() != nullptr) {
			if (traverse->getNext()->color == 0) {
				g.q.push(traverse->getNext());
				traverse->getNext()->color = 1;
				traverse->pred = nextVal;
				traverse->dist = 1;
			}
			traverse = traverse->getNext();
		}
		//New end

		//recurisve - also works; except the first visited will print more than needed..bfs(g, nextVal);
	}
}

void print_adjList(graph g, int s) {
	vertex* traverse = g.getAdjList()[s];
	printf("ADj list of vertex %d = %d, ", s, traverse->getVal());
	while (traverse->getNext() != nullptr) {
		traverse = traverse->getNext();
		printf("%d ", traverse->getVal());
	}
}



void bfs2(graph g, vertex* s) {
	//go through all vertices and set their properties..
	for (int i = 0; i < g.getSize(); i++) {
		vertex* v = g.getAdjList()[i];
		while (v != nullptr) {
			v->color = 0;
			v->dist = INT_MAX;
			v->pred2 = nullptr;
			v = v->getNext();
		}
	}
	s->color = 1; //gray
	s->dist = 0;
	s->pred2 = nullptr;

	g.v_q.push(s);
	while (!empty(g.v_q)) {
		vertex* u = g.v_q.front();
		g.v_q.pop();
		vertex* v = g.getAdjList()[u->getVal()];
		while (v != nullptr) {
			if (v->color == 0) {
				v->color = 1; //gray
				v->dist = u->dist + 1;
				v->pred2 = u;
				g.v_q.push(v);
			}
			v = v->getNext();
		}
		u->color = 2;
	}
}

//shortest path from s to v (obtained autoamtically from BFS)
void print_shortest_path(graph G, vertex* s, vertex* v) {
	if (s == v) {
		printf("%d(%d) ", s->getVal(),s->dist);
	}
	else if (v->pred2 == nullptr) {
		printf("no path from %d to %d\n", s->getVal(), v->getVal());
	}
	else {
		print_shortest_path(G, s, v->pred2);
		printf("%d(%d)  ", v->getVal(), v->dist);
	}
}
/*
vertex(val, arrloc);
vertex* v0 = new vertex(0, 0); //nothing reachable from this one say
vertex* v1 = new vertex(1, 1); //undirected here assumed so from 1 reach 
vertex* v11 = new vertex(2, 1); //2
vertex* v12 = new vertex(5, 1); //and 5
vertex* v2 = new vertex(2, 2);
vertex* v22 = new vertex(1, 2); //and vice versa, i.e. from 2 reach 1
vertex* v23 = new vertex(5, 2);
vertex* v24 = new vertex(3, 2);
vertex* v25 = new vertex(4, 2);
vertex* v3 = new vertex(3, 3);
vertex* v33 = new vertex(2, 3);
vertex* v34 = new vertex(4, 3);
vertex* v4 = new vertex(4, 4);
vertex* v44 = new vertex(2, 4);
vertex* v45 = new vertex(5, 4);
vertex* v46 = new vertex(3, 4);
vertex* v5 = new vertex(5, 5);
vertex* v55 = new vertex(4, 5);
vertex* v56 = new vertex(1, 5); //and from 5 reach 1
vertex* v57 = new vertex(2, 5);
*/

