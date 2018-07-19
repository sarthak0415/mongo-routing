// Program to find Dijkstra's shortest path using
// priority_queue in STL
#include<bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <chrono>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/array/view.hpp>
#include <bsoncxx/array/view.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/stdx/string_view.hpp>
#include <bsoncxx/string/to_string.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/types/value.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using namespace std;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
# define INF 0x3f3f3f3f

// iPair ==> Integer Pair
typedef pair<long int, double> iPair;

// This class represents a directed graph using
// adjacency list representation
class Graph
{
	int V; // No. of vertices

	// In a weighted graph, we need to store vertex
	// and weight pair for every edge
	list<iPair> *adj;

public:
	Graph(int V); // Constructor

	// function to add an edge to graph
	void addEdge(long int u, long int v, double w);

	// prints shortest path from s
	double shortestPath(long int s, long int d);
};

// Allocates memory for adjacency list
Graph::Graph(int V)
{
	this->V = V;
	adj = new list<iPair> [V];
}

void Graph::addEdge(long int u, long int v, double w)
{
	adj[u].push_back(make_pair(v, w));
	//adj[v].push_back(make_pair(u, w));
}

// Prints shortest paths from src to all other vertices
double Graph::shortestPath(long int src, long int dest)
{
	// Create a priority queue to store vertices that
	// are being preprocessed. This is weird syntax in C++.
	// Refer below link for details of this syntax
	// http://geeksquiz.com/implement-min-heap-using-stl/
	priority_queue< iPair, vector <iPair> , greater<iPair> > pq;

	// Create a vector for distances and initialize all
	// distances as infinite (INF)
	vector<double> dist(V, INF);

	// Insert source itself in priority queue and initialize
	// its distance as 0.
	pq.push(make_pair(0, src));
	dist[src] = 0;

	/* Looping till priority queue becomes empty (or all
	distances are not finalized) */
	while (!pq.empty())
	{
		// The first vertex in pair is the minimum distance
		// vertex, extract it from priority queue.
		// vertex label is stored in second of pair (it
		// has to be done this way to keep the vertices
		// sorted distance (distance must be first item
		// in pair)
		int u = pq.top().second;
		pq.pop();

		// 'i' is used to get all adjacent vertices of a vertex
		list<iPair>::iterator i;
		for (i = adj[u].begin(); i != adj[u].end(); ++i)
		{
			// Get vertex label and weight of current adjacent
			// of u.
			int v = (*i).first;
			int weight = (*i).second;

			// If there is shorted path to v through u.
			if (dist[v] > dist[u] + weight)
			{
				// Updating distance of v
				dist[v] = dist[u] + weight;
				pq.push(make_pair(dist[v], v));
			}
		}
	}
	return dist[dest];
}
unordered_map<long int, long int> get_nodes_map(mongocxx::collection nodes_collection){
	long long int index=0;
	mongocxx::cursor cursor = nodes_collection.find({});
	unordered_map<long int, long int> hash;
	for(auto doc : cursor){
		long int source_node_id;
		if(doc["node_id"].type() == bsoncxx::type::k_int32) {
			source_node_id = doc["node_id"].get_int32();
		}else{
			source_node_id = doc["node_id"].get_int64();
		}
		hash[source_node_id] = index++;
	}
	return hash;
}
Graph createGraph(mongocxx::collection nodes_collection, unordered_map<long int, long int> nodes_map){
	long int nodes_count = nodes_map.size();
	cout << "nodes_count ==   " <<  nodes_count << endl;
	Graph g(nodes_count);

	mongocxx::cursor cursor = nodes_collection.find({});
	for(auto doc : cursor) {
		long int source_node_id;
		if(doc["node_id"].type() == bsoncxx::type::k_int32) {
			source_node_id = doc["node_id"].get_int32();
		}else{
			source_node_id = doc["node_id"].get_int64();
		}

		bsoncxx::document::element edges_element = doc["connects_to"];
		bsoncxx::array::view edges{edges_element.get_array().value};

		for(const bsoncxx::array::element& edge : edges) {
			bsoncxx::document::view edgedoc = edge.get_document().value;
			
			double weight = edgedoc["weight"].get_double().value;
			long int temp_destination_node_id = 0;

			if(edgedoc["target"].type() == bsoncxx::type::k_int32) {
				temp_destination_node_id = edgedoc["target"].get_int32();
			}else{
				temp_destination_node_id = edgedoc["target"].get_int64();
			}

			source_node_id = nodes_map[source_node_id];
			temp_destination_node_id = nodes_map[temp_destination_node_id];

			g.addEdge(source_node_id, temp_destination_node_id, weight);			
		}
	}
	return g;
}

vector<long int> get_vertices(mongocxx::collection nodes_collection, int percentage){
	vector<long int> node_ids;
	
	mongocxx::cursor cursor = nodes_collection.find({});
	for(auto doc : cursor) {
		long int node_id;
		if(doc["node_id"].type() == bsoncxx::type::k_int32) {
			node_id = doc["node_id"].get_int32();
		}else{
			node_id = doc["node_id"].get_int64();
		}
		//cout << node_id << endl;
		node_ids.push_back(node_id);
	}
 
	long long int total_size = node_ids.size();
	long long int needed_size = (percentage)*(total_size/100);
	//cout << total_size << "\t" << needed_size << endl;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(node_ids.begin(), node_ids.end(), default_random_engine(seed));
	
	vector<long int> ans(node_ids.begin(), node_ids.begin()+needed_size);
	
	return ans;
}

void run_test(mongocxx::collection nodes_collection, int percentage){
	unordered_map<long int, long int> nodes_map = get_nodes_map(nodes_collection);
	Graph g = createGraph(nodes_collection, nodes_map);
	

	vector<long int> sources_vertices = get_vertices(nodes_collection, percentage);
	vector<long int> dest_vertices = get_vertices(nodes_collection, percentage);

	int n = sources_vertices.size();
	int m = dest_vertices.size();
	auto start = std::chrono::high_resolution_clock::now();
	for(int i=0; i<n; i++){
		for (int j = 0; j < m; ++j){
			long int source = nodes_map[sources_vertices[i]];
			long int dest = nodes_map[dest_vertices[j]];
			g.shortestPath(source, dest);
			//dijkstra(nodes_collection, source, dest);
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout << duration.count() << endl;
}

// Driver program to test methods of graph class
int main()
{
	mongocxx::instance inst{};
	mongocxx::client conn{mongocxx::uri{}};
	bsoncxx::builder::stream::document document{};
	string dataset_name = "magarpatta"; //'magarpatta'
	auto nodes_collection = conn["test"][dataset_name];
	
	//run_test(nodes_collection, 1);
	//run_test(nodes_collection, 10);
	//run_test(nodes_collection, 50);
	run_test(nodes_collection, 100);

	return 0;
}
