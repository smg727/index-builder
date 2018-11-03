# Makefile for index builder

# *****************************************************
# Variables to control Makefile operation
CXX = g++
CXXFLAGS = -Wall -g -std=c++11


# ****************************************************
# Targets needed to bring the executable up to date

main: main.o utils.o helper.o listOps.o vbyte.o
	$(CXX) $(CXXFLAGS) -o main main.o utils.o helper.o listOps.o libvbyte.a vbyte.o

# The main.o target can be written more simply

main.o: main.cpp flags.h utils.h helper.h listOps.h vbyte.h
	$(CXX) $(CXXFLAGS) -c main.cpp

utils.o: flags.h utils.h

helper.o: helper.h flags.h

listOps.o: listOps.h flags.h helper.h vbyte.h

