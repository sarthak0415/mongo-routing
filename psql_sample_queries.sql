#Postgres start server  
brew services start postgresql

#Login
psql <database_name>

#create new db and connect
CREATE DATABASE routing1;
\connect routing1

#create extensions
create extension postgis;
create extension pgrouting;
CREATE EXTENSION hstore;

osm2pgrouting --f iiit_map.osm --conf mapconfig.xml --dbname routing --username sarthak --clean --addnodes

