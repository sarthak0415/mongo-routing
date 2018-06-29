require 'pqueue'
require 'mongo'
INFINITY = 1 << 32
def dijkstra(source_node_id, destination_node_id)
    visited = {}
    shortest_distances = {}
    previous = {}
    pq = PQueue.new() {|x,y| (shortest_distances[x] || INFINITY) < (shortest_distances[y] || INFINITY)}
    #
    pq.push(source_node_id)
    visited[source_node_id] = true
    shortest_distances[source_node_id] = 0
    #
    client = Mongo::Client.new([ '127.0.0.1:27017' ], :database => 'test')
    nodes_collection = client[:iiit_map]
    #
    while pq.size != 0
        current_node_id = pq.pop
        visited[current_node_id] = true
        current_node = nodes_collection.find( { node_id: current_node_id } ).first
        edges = current_node[:connects_to]
        if edges.size > 0
            edges.each do |edge|
                temp_destination_node_id = edge[:target]
                weight = edge[:weight]
                if !visited[temp_destination_node_id] and (shortest_distances[temp_destination_node_id] || INFINITY) > shortest_distances[current_node_id] + weight
                    shortest_distances[temp_destination_node_id] = shortest_distances[current_node_id] + weight
                    previous[temp_destination_node_id] = current_node_id
                    pq.push(temp_destination_node_id)
                end
            end
        end
    end
    return shortest_distances[destination_node_id]
    #return [shortest_distances, previous]
end