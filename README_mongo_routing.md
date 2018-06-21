 Part 1 Download OSM data online

 Part 2 Convert OSM data to JSON
 1. source routing/bin/activate
 2. python convert_network_graph_to_json.py
 	this file take `iiit_map.osm` as input and stores the json at `networkx_graph_json_data_iiit.txt`

 Part 3 convert JSON data to mongo db
 1. ruby convert json_to_mongo.rb 
 #this will put the nodes data(containing lat, long and connected nodes into a collection called nodes_datatime)

 Part 4 run the code from dijiskstras_mongo.rb to 
 get the connected nodes and their shortest distance from a given node.


