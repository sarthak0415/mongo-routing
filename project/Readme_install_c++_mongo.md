1. prereq - Boost 1.56 or plus.
2. install it using https://particlephysicsandcode.wordpress.com/2013/03/11/installing-boost-1-52-ubuntu-12-04-fedora/
3. then follow http://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/installation/
4. first install libmongoc using http://mongoc.org/libmongoc/current/installing.html
5. then install mongocxx dirver using http://mongodb.github.io/mongo-cxx-driver/mongocxx-v3/installation/
6. to update the boost version, use `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DBOOST_ROOT=/usr/local/boost-1.56.0 ..`
7. run `make && sudo make install`
8. to compile use `g++ -I/usr/local/boost-1.56.0/include/ -std=c++11 test.cpp -o test  $(pkg-config --cflags --libs libmongocxx)`