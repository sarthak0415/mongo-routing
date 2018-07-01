require 'pqueue'
require 'mongo'
INFINITY = 1 << 32

def dijkstra(nodes_collection, source_node_id, destination_node_id)
    visited = {}
    shortest_distances = {}
    previous = {}
    pq = PQueue.new() {|x,y| (shortest_distances[x] || INFINITY) < (shortest_distances[y] || INFINITY)}
    pq.push(source_node_id)
    visited[source_node_id] = true
    shortest_distances[source_node_id] = 0

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
end


def get_vertices(nodes_collection, percentage)
    node_ids = []
    nodes_collection.find.each{|doc| node_ids << (doc["node_id"])}

    total_size = node_ids.size
    needed_size = percentage*total_size/100

    return node_ids.shuffle.first(needed_size.to_i)
end

def run_tests(percentage)
    dataset_name = 'iiit'
    
    client = Mongo::Client.new([ '127.0.0.1:27017' ], :database => 'test')
    nodes_collection = client[dataset_name]

    sources_vertices = get_vertices(nodes_collection, percentage)
    dest_vertices = get_vertices(nodes_collection, percentage)

    time = 0
    sources_vertices.each do |source|
        dest_vertices.each do |dest|
            #puts source, dest
            time += Benchmark.realtime do
                dijkstra(nodes_collection, source, dest)
            end
        end
    end
    return time
end