require 'mongo'
#connect
client = Mongo::Client.new([ '127.0.0.1:27017' ], :database => 'test')
db = client.database
db.collections # returns a list of collection objects

#to access a collection
nodes_collection = client[:nodes]

#insert #docs a basically json objects
doc = { name: 'Steve', hobbies: [ 'hiking', 'tennis', 'fly fishing' ] }
result = nodes_collection.insert_one(doc)
result = nodes_collection.insert_many(docs)
result.inserted_count

#print all documents
nodes_collection.find.each{|doc| pp doc}

#delete all documents
result = nodes_collection.delete_many()

#creat index on node_id
nodes_collection.indexes.create_one({ node_id: 1 }, unique: true)

#find a node with node_id
node =  nodes_collection.find( { node_id: 1 } ).first


#disable mongo logging
Mongo::Logger.logger.level = Logger::FATAL