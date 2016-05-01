#include "edge.h"

Edge::Edge(int trainId, int arvId, int depTime, int arvTime, float price)
{
	train_id = trainId;
	arv_id = arvId;
	dep_time = depTime;
	arv_time = arvTime;
	this->price = price;
}

int Edge::getStId()
{
	return arv_id;
}

int Edge::getTrainId()
{
	return train_id;
}

float Edge::getPrice()
{
	return price;
}

int Edge::getArvTime()
{
	return arv_time;
}

int Edge::getDepTime()
{
	return dep_time;
}

int Edge::getSpentTime()
{
	return (arv_time - dep_time);
}
