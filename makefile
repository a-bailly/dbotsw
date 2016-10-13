CXX = g++
LOADER = g++
OPT = -L/usr/lib -L/usr/lib/boost/lib

CXXFLAGS = -O2 -Wall -std=c++0x
FLAGS = -O2 -std=c++0x
OPCVFLAGS = -lopencv_core -lopencv_highgui
BOOSTFLAGS = -lboost_filesystem -lboost_system -lboost_unit_test_framework
X11FLAGS = -lsvm

DIR_SRC = ./src/
DIR_BIN = ./bin
DIR_INC = ./inc
DIR_OBJ = ./obj
DIR_PRG = ./prog

dbotsw: $(DIR_OBJ)/dbotsw.o $(DIR_OBJ)/botsw.o $(DIR_OBJ)/utils.o $(DIR_OBJ)/cross_validation.o $(DIR_OBJ)/svm.o $(DIR_OBJ)/sift.o $(DIR_OBJ)/norm.o $(DIR_OBJ)/param.o
	$(LOADER) $(OPT) -o $(DIR_BIN)/$@ $^ $(CXXFLAGS) $(OPCVFLAGS) $(BOOSTFLAGS)
	
get_feature_vectors: $(DIR_OBJ)/get_feature_vectors.o $(DIR_OBJ)/botsw.o $(DIR_OBJ)/utils.o $(DIR_OBJ)/cross_validation.o $(DIR_OBJ)/svm.o $(DIR_OBJ)/sift.o $(DIR_OBJ)/norm.o $(DIR_OBJ)/param.o
	$(LOADER) $(OPT) -o $(DIR_BIN)/$@ $^ $(CXXFLAGS) $(OPCVFLAGS) $(BOOSTFLAGS)
	
## ----- ##

$(DIR_OBJ)/dbotsw.o: $(DIR_PRG)/dbotsw.cpp $(DIR_SRC)/botsw.cpp $(DIR_SRC)/utils.cpp $(DIR_SRC)/cross_validation.cpp $(DIR_SRC)/svm.cpp $(DIR_SRC)/sift.cpp $(DIR_SRC)/norm.cpp $(DIR_SRC)/param.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
	
$(DIR_OBJ)/get_feature_vectors.o: $(DIR_PRG)/get_feature_vectors.cpp $(DIR_SRC)/botsw.cpp $(DIR_SRC)/utils.cpp $(DIR_SRC)/cross_validation.cpp $(DIR_SRC)/svm.cpp $(DIR_SRC)/sift.cpp $(DIR_SRC)/norm.cpp $(DIR_SRC)/param.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)
	
$(DIR_OBJ)/botsw.o: $(DIR_SRC)/botsw.cpp $(DIR_SRC)/utils.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(DIR_OBJ)/param.o: $(DIR_SRC)/param.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(DIR_OBJ)/svm.o: $(DIR_SRC)/svm.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(DIR_OBJ)/cross_validation.o: $(DIR_SRC)/cross_validation.cpp $(DIR_SRC)/norm.cpp  $(DIR_SRC)/sift.cpp $(DIR_SRC)/utils.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(DIR_OBJ)/norm.o: $(DIR_SRC)/norm.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(DIR_OBJ)/sift.o: $(DIR_SRC)/sift.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

$(DIR_OBJ)/utils.o: $(DIR_SRC)/utils.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

## ----- ##

clean:
	-rm -rf $(DIR_OBJ)/*.o *~ */*~ */*/*~

