#pragma once
#include <queue>
#include <list>
#include <stack>

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
	int dTime;
	int fTime;
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
	int dfs_time;
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


void dfs_visit(graph g, vertex* u) {
	g.dfs_time = g.dfs_time + 1;
	u->dTime = g.dfs_time;
	u->color = 1;
	vertex* v = g.getAdjList()[u->getVal()];
	while (v != nullptr) {
		if (v->color == 0) {
			//v->color = 1; //gray
			//v->dist = u->dist + 1;
			v->pred2 = u;
			//g.v_q.push(v);
			dfs_visit(g, v);
		}
		v = v->getNext();
	}
	u->color = 2;
	g.dfs_time = g.dfs_time + 1;
	u->fTime = g.dfs_time;
}

void dfs(graph g) {
	//go through all vertices and set their properties..
	for (int i = 0; i < g.getSize(); i++) {
		vertex* u = g.getAdjList()[i];
		while (u != nullptr) {
			u->color = 0;
			//v->dist = INT_MAX;
			u->pred2 = nullptr;
			u = u->getNext();
		}
	}

	g.dfs_time = 0;
	for (int i = 0; i < g.getSize(); i++) {
		vertex* v = g.getAdjList()[i];
		while (v != nullptr) {
			if (v->color == 0) {
				dfs_visit(g, v);
			}
			//v->dist = INT_MAX;
			//v->pred2 = nullptr;
			v = v->getNext();
		}
	}
}

void dfs_printAll(graph g) {
	//go through all vertices and set their properties..
	for (int i = 0; i < g.getSize(); i++) {
		vertex* u = g.getAdjList()[i];
		while (u != nullptr) {
			printf("%d,%d d=%d f=%d\n", u->getVal(), i, u->dTime, u->fTime);
			u = u->getNext();
		}
	}
}





class Graph_t
{
private:
	int V;
	list<int>* adj;
	void DFSUtil(int v, bool visited[]); //for DFS; not needed for BFS
	//Below is for finding strongly connected components (SCC) - finding  SCC used DFS
	// Fills Stack with vertices (in increasing order of finishing times)
	// The top element of stack has the maximum finishing time
	void fillOrder(int v, bool visited[], stack<int>& Stack);
public:
	Graph_t(int V)
	{
		this->V = V;
		adj = new list<int>[V];
		connection = V;
		print_scc = 0;
		islands = 0;
	}
	~Graph_t()
	{
		delete [] adj;
	}
	void addEdge(int v, int w, bool undirected = true);
	void BFS(int s, bool visited[]);
	void getTranspose(Graph_t& g);
	bool isConnected();

	// The main function that finds and prints strongly connected components (SCC)
	int printSCCs();

	int connection; //me added to tell if graph is strongly connected or not
	int print_scc;
	int islands;
	vector<vector<int>> sccs;
	vector<int> scc;
};

/*
 * Add Edge to connect v and w - below is for undirected graph
 */
//Default is undirected = true - see in module definiton in class.
void Graph_t::addEdge(int v, int w, bool undirected)
{
	adj[v].push_back(w);
	if (undirected) {
		adj[w].push_back(v); //commenting for SCC work - actually not true; can have undirected graph too, then it's a lot mor elikely for graph to be strongly connected..
	}
	
}

/*
 *  A recursive function to print BFS starting from s
 */
void Graph_t::BFS(int s, bool visited[])
{
	list<int> q;
	list<int>::iterator i;
	visited[s] = true;
	q.push_back(s);
	while (!q.empty())
	{
		s = q.front();
		q.pop_front();
		for (i = adj[s].begin(); i != adj[s].end(); ++i)
		{
			if (!visited[*i])
			{
				visited[*i] = true;
				q.push_back(*i);
			}
		}
	}
}

/*
 *  A recursive function to print DFS starting from v
 */
void Graph_t::DFSUtil(int v, bool visited[])
{
	visited[v] = true;
	cout << v << " "; // For SCC printing
	print_scc++;
	scc.push_back(v);
	// Recur for all the vertices adjacent to this vertex
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			DFSUtil(*i, visited);
}

/*
 * Function that returns reverse (or transpose) of this graph
 */
void Graph_t::getTranspose(Graph_t& g)
{
	//Graph_t g(V);
	for (int v = 0; v < V; v++)
	{
		// Recur for all the vertices adjacent to this vertex
		list<int>::iterator i;
		for (i = adj[v].begin(); i != adj[v].end(); ++i)
		{
			g.adj[*i].push_back(v);
		}
	}
	//return g;
}
/*
 * Check if Graph is Connected
 * IF want BFS traversal, uncomments the two BFS's and comment the two DFSutils. Currently it's doing DFS.
 */
bool Graph_t::isConnected()
{
	bool* visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;
	//BFS(0, visited);
	DFSUtil(0, visited);
	for (int i = 0; i < V; i++)
		if (visited[i] == false)
			return false;
	Graph_t gr(V);
	getTranspose(gr);
	for (int i = 0; i < V; i++)
		visited[i] = false;
	//gr.BFS(0, visited);
	gr.DFSUtil(0, visited);
	for (int i = 0; i < V; i++)
		if (visited[i] == false)
			return false;
	return true;
}

void Graph_t::fillOrder(int v, bool visited[], stack<int>& Stack)
{
	// Mark the current node as visited and print it
	visited[v] = true;

	// Recur for all the vertices adjacent to this vertex
	list<int>::iterator i;
	for (i = adj[v].begin(); i != adj[v].end(); ++i)
		if (!visited[*i])
			fillOrder(*i, visited, Stack);

	// All vertices reachable from v are processed by now, push v to Stack
	Stack.push(v);
}

// The main function that finds and prints all strongly connected components
int Graph_t::printSCCs()
{
	stack<int> Stack;

	// Mark all the vertices as not visited (For first DFS)
	bool* visited = new bool[V];
	for (int i = 0; i < V; i++)
		visited[i] = false;

	// Fill vertices in stack according to their finishing times
	for (int i = 0; i < V; i++)
		if (visited[i] == false)
			fillOrder(i, visited, Stack);

	// Create a reversed graph
	Graph_t gr(V);
	getTranspose(gr);

	// Mark all the vertices as not visited (For second DFS)
	for (int i = 0; i < V; i++)
		visited[i] = false;
	int count = 0;
	// Now process all vertices in order defined by Stack
	while (Stack.empty() == false)
	{
		// Pop a vertex from stack
		int v = Stack.top();
		Stack.pop();

		// Print Strongly connected component of the popped vertex
		if (visited[v] == false)
		{
			gr.DFSUtil(v, visited);
			cout << endl;
			connection--; //fine to update for main graph

			sccs.push_back(gr.scc);
			gr.scc.clear(); //needed since dealign with same gr every DFSUtil invocation

			if (gr.print_scc > 1) islands++; //DFS is called from gr rather than the main graph 
			gr.print_scc = 0;

		}
		count++;
	}

	if (connection == V - 1) cout << "Graph is strongly connected" << endl;
	else cout << "Grahp IS NOT strongly connected; it has " << V - connection << " strongly complenents " << endl;

	return count;
}