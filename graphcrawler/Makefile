LDFLAGS=-lcurl
CXXFLAGS= -I ~/prgs/rapidjson/include

graphcrawler: graphcrawler.o
	$(CXX) -o $@ $^ $(LDFLAGS)

graphcrawler.o: graphcrawler.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)