#pragma once
#ifndef VERTEX_H
#define VERTEX_H

#include "edge.h"
#include <string>
#include <vector>
#include <list>

using namespace std;

class Vertex {
private:
	int id;
	vector <Edge> trains;
	//for Dijkstra's algorithm
	list <string> path;

public:
	Vertex(int, int, int, int, int, float);

	void addTrain(int, int, int, int, float);

	void printVertex();

	void clearPath();

	void addPath(string);

	void splicePath(list <string>);

	list <string> getPath();

	void popPath();

	void addBack(string);

	int getTimeWndw(double);

	Edge getTrain(unsigned int);

	int getTrainAmount();

	int getId();	

	~Vertex();
};

#endif 