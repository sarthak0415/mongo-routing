import networkx as nx
from OSMParser import read_osm
from networkx.readwrite import json_graph
import json

def get_adjacency_list(networkx_graph, networkx_graph_json_data):

	node_schema = {}

	for row in nx.generate_edgelist(networkx_graph,data=['weight']):
		print(row)


if __name__ == "__main__":
	networkx_graph = read_osm('gachibowli.osm', False)		##read graph from osm file
	networkx_graph_json_data = json_graph.node_link_data(networkx_graph)	#convert graph to json data 
	with open('networkx_graph_json_data_hyderabad_india.txt', 'w') as outfile:			#store in a file
		json.dump(networkx_graph_json_data, outfile)

	#adjacency_list = get_adjacency_list(networkx_graph, networkx_graph_json_data)
