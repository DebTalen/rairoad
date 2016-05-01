#pragma once
#ifndef EDGE_H
#define EDGE_H

class Edge
{
private:
	int train_id, arv_id, dep_time, arv_time;
	float price;
public:
	Edge(int, int, int, int, float);

	int getStId();
	int getTrainId();
	float getPrice();

	int getArvTime();
	int getDepTime();
	int getSpentTime();	
};

#endif