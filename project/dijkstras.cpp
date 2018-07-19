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

#define INFN INT_MAX
map<long int, bool> visited;
map<long int, double> shortest_distances;
map<long int, long int> previous;

struct pq_comp{  
	bool operator()(const long int& x, const long int& y)  {  
		return (shortest_distances[x] || INFN) < (shortest_distances[y] || INFN);
	}
};

double dijkstra(mongocxx::collection nodes_collection, long int source_node_id, long int destination_node_id){

	//cout << "source_node_id  ->> " << source_node_id << "\t" << "destination_node_id --> " << destination_node_id << endl;

	priority_queue<long int, vector <long int>, pq_comp> pq;
	pq.push(source_node_id);
	visited[source_node_id] = true;
	shortest_distances[source_node_id] = 0;

	while (!pq.empty()){
		long int current_node_id = pq.top();
		//cout << current_node_id << endl;
		pq.pop();
		visited[current_node_id] = true;
		
		bsoncxx::stdx::optional<bsoncxx::document::value> current_node_doc = nodes_collection.find_one(document{} << "node_id" << current_node_id << finalize);
		if(current_node_doc) {			
			bsoncxx::document::value current_node_value{current_node_doc.value()};
			bsoncxx::document::view current_node_view{current_node_value.view()};
			//cout << bsoncxx::to_json(current_node_view) << endl;

			bsoncxx::document::element edges_element = current_node_view["connects_to"];
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
					
				double temp = shortest_distances[temp_destination_node_id];
				if(temp == 0) temp = INFN;

				//cout << temp_destination_node_id << "\t" << weight << endl;
				//cout << "!" << visited[temp_destination_node_id] << "&&(" << (temp) << " > " << shortest_distances[current_node_id] << " + " << weight << endl;
 

				if(!visited[temp_destination_node_id] && temp>(shortest_distances[current_node_id] + weight) ){
					//cout << "int there " << endl;
					shortest_distances[temp_destination_node_id] = shortest_distances[current_node_id] + weight;
					previous[temp_destination_node_id] = current_node_id;
					pq.push(temp_destination_node_id);
				}
			}
		}
	}
	return shortest_distances[destination_node_id];
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
	vector<long int> sources_vertices = get_vertices(nodes_collection, percentage);
	vector<long int> dest_vertices = get_vertices(nodes_collection, percentage);

	int n = sources_vertices.size();
	int m = dest_vertices.size();
	auto start = std::chrono::high_resolution_clock::now();
	for(int i=0; i<n; i++){
		for (int j = i; j < m; ++j){
			long int source = sources_vertices[i];
			long int dest = dest_vertices[j];
			dijkstra(nodes_collection, source, dest);
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	cout << duration.count() << endl;
}

int main()
{
	mongocxx::instance inst{};
	mongocxx::client conn{mongocxx::uri{}};
	bsoncxx::builder::stream::document document{};

	string dataset_name = "iiit";
	auto nodes_collection = conn["test"][dataset_name];
	run_test(nodes_collection, 50);
	return 0;
}