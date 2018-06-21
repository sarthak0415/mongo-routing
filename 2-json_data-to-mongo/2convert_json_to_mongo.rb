require 'json'
require 'pp'
require 'mongo'
file = File.read('networkx_graph_json_data_iiit.txt')
graph_json = JSON.parse(file)

nodes_array = graph_json["nodes"]
nodes_array = nodes_array.map{ |node| 
	node.map{|key, value| 
		if(key == "lat" || key == "lon")
			[key.to_sym, value.to_f]
		elsif(key == "id")
			[:node_id, value.to_i]
		else
			[key.to_sym, value.to_i]
		end
	}.to_h 
}
nodes_hash  = nodes_array.map{ |node| [node[:node_id], node]}.to_h

edges_array = graph_json["links"]
edges_array = edges_array.map{ |edge| 
	edge.map{|key, value| 
		if(key == "weight")
			[key.to_sym, value.to_f]
		elsif(key == "id")
			[:edge_id, value.to_i]
		else
			[key.to_sym, value.to_i]
		end
	}.to_h 
}
edges_hash = Hash.new{|hsh,key| hsh[key] = [] }
edges_array.each{ |edge|
	source = edge[:source].to_i
	edges_hash[source].push(edge)
	
}

nodes_hash.each{|node_id, node|
	node[:connects_to] = edges_hash[node_id]
}

nodes = nodes_hash.values
#pp nodes;0

#connect
client = Mongo::Client.new([ '127.0.0.1:27017' ], :database => 'test')
db = client.database
new_collection_number = Time.now.to_s.split(' ').join('-')
new_collection_name = "nodes" + '_' + new_collection_number
nodes_collection = client[new_collection_name]

result = nodes_collection.delete_many()
result = nodes_collection.insert_many(nodes)