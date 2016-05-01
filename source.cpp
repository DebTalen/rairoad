#include <iostream>
#include <vector>
#include <list>
#include <string>

#include "xml/tinyxml.h"
#include "vertex.h"

#define ALL_STATIONS -3
using namespace std;

void dump_attribs_to_vector(TiXmlElement* pElement, vector<Vertex> & arr, unsigned int indent)
{
	if (!pElement) return;

	TiXmlAttribute* pAttrib = pElement->FirstAttribute();
	double attValues[6];
	string time;
	int check = -1;
	for (int i = 0; i < 4; ++i)
	{
		pAttrib->QueryDoubleValue(&attValues[i]);
		pAttrib = pAttrib->Next();
	}
	for (int i = 4; i < 6; ++i)
	{
		time = pAttrib->Value();
		attValues[i] = stoi(time.substr(0, 2)) * 3600 + stoi(time.substr(3, 5)) * 60 + stoi(time.substr(6));
		pAttrib = pAttrib->Next();
	}
	for (size_t i = 0; i < arr.size(); ++i)
	{
		if ((int)attValues[1] == arr.at(i).getId())
			check = i;
	}
	if (attValues[5] > attValues[4])
		attValues[4] += 86400;
	//If this is the new station
	if (check == -1)
		arr.push_back({ (int)attValues[1], (int)attValues[0], (int)attValues[2],\
			(int)attValues[5], (int)attValues[4], (float)attValues[3] }); //omg 	
	else
		arr.at(check).addTrain((int)attValues[0], (int)attValues[2], \
			(int)attValues[5], (int)attValues[4], (float)attValues[3]);
}

void dump_to_vector(TiXmlNode* pParent, vector<Vertex> & arr, unsigned int indent = 0)
{
	if (!pParent) return;

	TiXmlNode* pChild;
	int t = pParent->Type();

	if (t == TiXmlNode::TINYXML_ELEMENT &&	!strcmp(pParent->Value(), "TrainLeg"))
		dump_attribs_to_vector(pParent->ToElement(), arr, indent + 1);

	for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
		dump_to_vector(pChild, arr, indent + 1);
}

int minElem(vector<bool> & flags, vector<double> & weights)
{
	double min;
	size_t i = 0, indx = 0;
	while (true)
	{
		if (i == flags.size()) return -1;
		min = weights[i];
		indx = i;
		if (!flags[i]) break;
		++i;
	}

	for (i; i < flags.size(); ++i)
	{
		if (flags[i] != 1 && (weights[i] < min))
		{
			min = weights[i];
			indx = i;
		}
	}
	return indx;
}

vector <double> dijkstraComputePath(vector<Vertex> & arr, int pos, bool mode)
{
	size_t nearStId, nearStIndx;
	double tempWeight = 0;
	string pAlternative = "or "; //for similar trains
	vector <bool> isVisited(arr.size());
	vector <double> weights(arr.size());
	for (size_t i = 0; i < arr.size(); ++i)
	{
		isVisited[i] = false;
		weights[i] = INT_MAX;
	}
	weights[pos] = 0;
	arr[pos].addPath("0");
	arr[pos].addTrain(0, arr[pos].getId(), 0, 0, 0);

	while (true)
	{
		for (int i = 0; i < arr[pos].getTrainAmount(); ++i)
		{
			nearStId = arr[pos].getTrain(i).getStId();
			for (nearStIndx = 0; nearStIndx < arr.size(); ++nearStIndx)
				if (arr[nearStIndx].getId() == nearStId)
					break;
			if (isVisited[nearStIndx])
				continue;

			if (mode)
				tempWeight = weights[pos] + arr[pos].getTrain(i).getSpentTime() \
				+ arr[pos].getTimeWndw(arr[pos].getTrain(i).getDepTime());
			else
				tempWeight = weights[pos] + arr[pos].getTrain(i).getPrice();

			if (weights[nearStIndx] == tempWeight)
			{
				arr[nearStIndx].addBack(pAlternative.append(to_string(arr[pos].getTrain(i).getTrainId())));//omg
				pAlternative = "or ";
			}
			else if (weights[nearStIndx] > tempWeight)
			{//better way and weight
				weights[nearStIndx] = tempWeight;
				arr[nearStIndx].clearPath();
				arr[nearStIndx].addPath(to_string(arr[pos].getTrain(i).getTrainId()));
				arr[nearStIndx].splicePath(arr[pos].getPath());
			}
		}
		isVisited[pos] = true;
		pos = minElem(isVisited, weights);
		if (pos == -1) break;
	}
	return weights;
}

void help(const char* name)
{
	printf("\nCommand syntax: %s (Departure Station Id) (Arrival Station Id) (Priority)\n", name);
	printf("\tPriority can only be \"price\" or \"time\" without quotes\n");
	printf("\tArrival Station Id can also be \"all\" without quotes\n");
}

int init(char* argv[], bool &mode, int &depId, int &arvId)
{
	string temp = argv[1];
	if (!(depId = stoi(temp)))
		return 1;


	temp = argv[2];
	if (!temp.compare("all"))
		arvId = ALL_STATIONS;
	else
		arvId = stoi(temp);

	temp = argv[3];
	if (!temp.compare("price"))
		mode = false;
	else if (!temp.compare("time"))
		mode = true;
	else
		return 1;
	return 0;
}

int main(int argc, char* argv[])
{	
	if (argc != 4) { help();  return 1; }

	bool priority = 1;
	int depStationId, arvlStationId, depStationInx = -1, arvlStationInx = -1;
	if (init(argv, priority, depStationId, arvlStationId))
	{
		puts("Invalid input");
		return 2;
	}

	vector <Vertex> stations;
	TiXmlDocument doc("D:\\data.xml");
	if (!doc.LoadFile())
	{
		printf("Failed to load file \"D:\\data.xml\"\n");
		return 3;
	}
	dump_to_vector(&doc, stations);

	for (size_t i = 0; i < stations.size(); ++i)
		if (stations[i].getId() == depStationId)
		{
			depStationInx = i;
			break;
		}
	if (depStationInx == -1)
	{
		puts("No such departure station");
		return -1;
	}
	if (arvlStationId != ALL_STATIONS)
	{
		for (size_t i = 0; i < stations.size(); ++i)
			if (stations[i].getId() == arvlStationId)
			{
				arvlStationInx = i;
				break;
			}
		if (arvlStationInx == -1)
		{
			puts("No such arrival station");
			return -1;
		}
	}

	vector <double> weights = dijkstraComputePath(stations, depStationInx, priority);
	list<string> out;
	string parse = "0", parse2 = "or 0";

	if (arvlStationId == ALL_STATIONS)
	{
		for (size_t i = 0; i < stations.size(); ++i)
		{
			printf("From %d to %d: ", depStationId, stations.at(i).getId());
			out = stations[i].getPath();
			for (auto it = out.begin(); it != out.end(); ++it)
			{
				if (!parse.compare(*it) || !parse2.compare(*it))
					continue;
				cout << *it << ' ';
			}
			if (priority)
				cout << "\t:total time: " << weights[i] << endl;
			else
				cout << "\t:total price: " << weights[i] << endl;
		}
	}
	else
	{
		printf("From %d to %d: ", depStationId, arvlStationId);
		out = stations[arvlStationInx].getPath();
		for (auto it = out.begin(); it != out.end(); ++it)
		{
			if (!parse.compare(*it) || !parse2.compare(*it))
				continue;
			cout << *it << ' ';
		}
		if (priority)
			cout << "\t:total time: " << weights[arvlStationInx] << endl;
		else
			cout << "\t:total price: " << weights[arvlStationInx] << endl;
	}
	return 0;
}