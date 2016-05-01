#include "vertex.h"

Vertex::Vertex(int srcId, int trainId, int detId, int depTime, int arvTime, float price)
{
	id = srcId;
	trains.push_back({ trainId, detId, depTime,  arvTime, price });
}

void Vertex::addTrain(int trainId, int detId, int depTime, int arvTime, float price)
{
	trains.push_back({ trainId, detId, depTime,  arvTime, price });
}

void Vertex::printVertex()
{
	printf("Station number: %d\n Trains: %d", id, trains.size());
	for (int i = 0; i < trains.size(); ++i)
	{
		printf("\n\    TrainId %d: DepStId %d ArvStId  %d DepTime %d ArvTime %d Price %1.1f", \
			trains.at(i).getTrainId(), id, trains.at(i).getStId(), \
			trains.at(i).getDepTime(), trains.at(i).getArvTime(), trains.at(i).getPrice());
	}
	putchar('\n');
}

void Vertex::clearPath()
{
	path.clear();
}

void Vertex::addPath(string val)
{
	path.push_front(val);
}

void Vertex::splicePath(list <string> list)
{
	path.splice(path.begin(), list);
}

list <string> Vertex::getPath()
{
	return path;
}

void Vertex::popPath()
{
	path.pop_front();
}

void Vertex::addBack(string val)
{
	path.push_back(val);
}

int Vertex::getTimeWndw(double val)
{
	val = double(trains[stoi(path.front())].getArvTime() - val);
	if (val < 0)
		val += 86400; //amount of seconds in twenty-four hours
	return val;
}

Edge Vertex::getTrain(unsigned int val)
{
	return trains.at(val);
}

int Vertex::getTrainAmount()
{
	return trains.size();
}

int Vertex::getId()
{
	return id;
}

Vertex::~Vertex()
{
	trains.clear();
}