steps-

1. download osm from osm website
2. convert to shaplite db(in qgis -> vector -> osm -> import)
3. convert db to shapefile (in qgis -> vector -> osm -> export)
4. import shape file to network graph
	
	a.a source routing/bin/activate
	a. import networkx as nx
	b. import matplotlib.pyplot as pyplot
	b.b #import neonx
	c. G=read_osm('iiit_map/iiit_map.osm', False)
	d. nx.draw(G)
	e. pyplot.show()
	f. #results = neonx.write_to_neo("http://localhost:7474/db/data/", G, 'LINKS_TO', json.JSONEncoder())

	f. if networkx is missing intsall using pip install networkx==2.0
	g. if gdal is missing install using https://hackernoon.com/install-python-gdal-using-conda-on-mac-8f320ca36d90
	h. install qgis


possible leads
0. osm_to_networkx (https://gist.github.com/Tofull/49fbb9f3661e376d2fe08c2e9d64320e)
1. install this graph into mongodb(neo4j) using https://networkx.github.io/documentation/networkx-1.10/reference/readwrite.html
1.1 or use the import for neo4j (https://neonx.readthedocs.io/en/latest/contributing.html)
2. use neo4j for routing(https://neo4j-contrib.github.io/neo4j-graph-algorithms/)
3. or use this (https://stackoverflow.com/questions/38237237/neo4j-finding-the-shortest-path-between-two-nodes-based-on-relationship-proper)