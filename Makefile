CC=cc
CXX=c++
CCFLAGS=-Wall -W
CXXFLAGS=-Wall -W

test:
	$(CC)  $(CCFLAGS) -o t/01_parse.t t/01_parse.c
	$(CXX) $(CXXFLAGS) -o t/02_escape.t t/02_escape.cc
	$(CXX) $(CXXFLAGS) -o t/03_class.t t/03_class.cc
	$(CXX) $(CXXFLAGS) -o t/04_unescape.t t/04_unescape.cc
	prove --source Executable t/*.t

