/*
* q1.cpp
* 
* Created on: April 22th, 2021
*     Author: Ramazan Yetismis 150190708
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
using namespace std;
#include <bits/stdc++.h>
using namespace std;
#define INF 0x3f3f3f3f

// iPair ==>  Integer Pair
typedef pair<int, int> iPair;
//nodes for edge
typedef struct node
{
	string source;
	string dest;
	int weig;
} edge;
//helps the stl sort function 
//sort the given edges
bool compareByLength(const edge &a, const edge &b)
{
	return a.weig < b.weig;
}

class Graph
{
	int V;
	list<pair<int, int>> *adj;
	map<string, int> mapF;

public:
	vector<edge> last;
	int total;
	Graph(int V, map<string, int> m);
	~Graph();
	void addEdge(int u, int v, int w);
	void primMST();
	string f(int index);
	bool containKey(vector<int> &arr, int key);
};

// Allocates memory for adjacency list
Graph::Graph(int V, map<string, int> m)
{
	this->V = V;
	adj = new list<iPair>[V];
	this->mapF = m;
}
Graph::~Graph()
{
	delete[] adj;
}
//adds edges to adj array
void Graph::addEdge(int u, int v, int w)
{
	adj[u].push_back(make_pair(v, w));
	adj[v].push_back(make_pair(u, w));
}

//converts the given int value
//corresponding mapped  String value 
// for instance 0->GP.
string Graph::f(int index)
{

	for (map<string, int>::iterator it = mapF.begin(); it != mapF.end(); ++it)
	{

		if (it->second == index)
		{
			return it->first;
		}
	}
	return NULL;
}

//it looks  the given vector and 
//returns false if contains key
//returns true if not
bool Graph::containKey(vector<int> &arr, int key)
{

	for (unsigned int i = 0; i < arr.size(); i++)
	{
		if (arr[i] == key)
		{
			return false;
		}
	}
	return true;
}

void Graph::primMST()
{

	vector<bool> inMST(V, false);
	vector<int> checkStack;//add the vertex if its in MST
	
	vector<edge> sol;//the last printed array

	checkStack.push_back(mapF["GP"]);//add GP to the stack
	inMST[mapF["GP"]] = true;//add GP to the MST

	//add church firstly
	int minV = INF, hipV;//İndexes for the chırch
	int minW = INF, hipW;//indexes for the hipp
	int mstCounter = 3, tot = 0;

	//this part
	//finds the min Church and Hipp
	for (list<pair<int, int>>::iterator j = adj[0].begin(); j != adj[0].end(); ++j)
	{

		int v = (*j).first;		  //get vertex from adj list
		int weight = (*j).second; //get the corresponding weight

		string isChurch = f(v); //turn it into string correspondance.
		size_t pos2 = isChurch.find("Ch");

		//if the currrent vertex is curch and it's weight is less then prev
		//then assign it as min
		if (pos2 == 0 && weight < minW)
		{
			minV = v;
			minW = weight;
		}
		//if curr vertex is Hipp then
		//do the same procces above
		size_t pos3 = isChurch.find("Hipp");
		if (pos3 == 0)
		{
			hipV = v;
			hipW = weight;
			//cout<<"hiipp<<"<<v<<" ->"<<weight<<endl;
		}
	}

	inMST[minV] = true; //add the church into min span tree
	checkStack.push_back(minV);
	//put the church into  the edge vector
	edge church;
	church.source = f(0);  //make it' source as GP
	church.dest = f(minV); //make destionation itself
	church.weig = minW;	   //add the weight
	sol.push_back(church);
	tot += minW;
	//add hipp
	//and same goes for the HIPP
	inMST[hipV] = true;
	checkStack.push_back(hipV); //add hipp to the stack
	//add hipp to the solution vectro
	edge hipp;
	hipp.source = f(0);
	hipp.dest = f(hipV);
	hipp.weig = hipW;
	sol.push_back(hipp);
	tot += hipW;

	//termination condition for the loop
	//if there is no vertex to add it terminates
	//even the it do not fulfill the creterias
	bool terminate = false;

	while (terminate == false && mstCounter < V)
	{

		int minWeig = INF, parentIndex, minInd;
		bool isBeAdded = false;

		//this part gives us the min weight
		//it looks all element in the vector and compares the adjs and returns the min edge
		//that are currently inside the checkStack(vector)

		for (unsigned int j = 0; j < checkStack.size(); j++)
		{
			//takes  the index from the vector for the first iteration
			//stack=[GP,CH2,Hipp]
			int ind = checkStack[j];

			//cout<<"for i ="<<f(ind)<<endl;
			list<pair<int, int>>::iterator i;
			for (i = adj[ind].begin(); i != adj[ind].end(); ++i)
			{

				int v = (*i).first;
				int weight = (*i).second;

				if (inMST[v] == false && minWeig > weight && containKey(checkStack, v))
				{

					minWeig = weight;
					minInd = v;
					parentIndex = checkStack[j];
					isBeAdded = true;
					//cout<<f(parentIndex)<<"->"<<f(v)<<"-"<<weight<<endl;
				}
			}
		}
		//if there is min edge that will meets the adding conditions
		//it will add it to the checkStack & MST.
		if (isBeAdded)
		{
			//cout<<"addded to stack "<<f(minInd)<<endl;
			checkStack.push_back(minInd);
			inMST[minInd] = true;
			mstCounter++;
			//cout<<"mst "<<mstCounter<<"v "<<V<<endl;

			//add to will be printed array.
			edge temp;
			temp.source = f(parentIndex);
			temp.dest = f(minInd);
			temp.weig = minWeig;
			sol.push_back(temp);
			tot += minWeig; //increase total distance
		}
		else
		{
			//if no more edge then finish the algorithm
			terminate = true;
		}
	}
	//sort the solution vector
	sort(sol.begin(), sol.end(), compareByLength);
	//return vector & total
	this->last = sol;
	this->total = tot;
}

int main()
{
	//take file name 
	string file_name;
	cin >> file_name;
	//open the file
	ifstream file(file_name);
	if (!file.is_open())
	{
		cout << "File cannot be opened" << endl;
		return 1;
	}
	//list for the edges pair is=> <inti,int> 
	list<iPair> edges;
	vector<int> weight;

	map<string, int> mymap;

	map<string, int>::iterator it;

	if (!file)
	{
		cerr << "File cannot be opened!";
		exit(1);
	}
	string line, s, d;
	int castWeight, source, desti, counter = 1;
	while (!file.eof())
	{

		getline(file, line, ',');

		s = line;
		it = mymap.find(line);
		//if GP is in the source
		//make GP corresponance ->0
		if (it == mymap.end())
		{
			size_t pos1 = line.find("GP");
			if (pos1 == 0)
			{
				mymap.insert(pair<string, int>(line, 0));
			}
			else
			{
				mymap.insert(pair<string, int>(line, counter++));
			}
		}

		//take destination
		getline(file, line, ',');
		it = mymap.find(line);
		d = line;
		//check whether new vertex already exist
		if (it == mymap.end())
		{
			size_t pos1 = line.find("GP");
			//if the vertex GP then make its correspondace ->0
			if (pos1 == 0)
			{
				mymap.insert(pair<string, int>(line, 0));
			}
			else
			{
				mymap.insert(pair<string, int>(line, counter++));
			}
		}

		//take weight
		getline(file, line, '\n');
		stringstream cas(line);
		castWeight = 0;
		cas >> castWeight;
		//find the index from map
		it = mymap.find(s);
		source = it->second;
		it = mymap.find(d);
		desti = it->second;

		//add to list
		//here Eliminate the creterias

		size_t pos1 = s.find("Hp");
		size_t pos2 = d.find("Hp");
		//2 HPs can not be connecter with each other
		if (pos1 > 0 || pos2 > 0)
		{
			size_t pos3 = s.find("Hipp");
			size_t pos4 = d.find("Bas");

			size_t pos5 = d.find("Bas");
			size_t pos6 = s.find("Hipp");
			//Hipp and Bas can not be connected with each other
			if ((pos3 == 0 && pos4 == 0) || (pos5 == 0 && pos6 == 0))
			{
				//do not add
			}

			else
			{
				//cout<<s<<"-"<<d<<endl;
				//if the edge meet all the creterias then add it to the graph
				edges.push_back(make_pair(source, desti));
				weight.push_back(castWeight);
			}
		}
	}
	//take the unique vertexs
	int V = mymap.size();
	Graph g(V, mymap);

	list<pair<int, int>>::iterator i;
	int index = 0;

	//add the edges to the adj matrix
	for (i = edges.begin(); i != edges.end(); ++i)
	{

		g.addEdge(i->first, i->second, weight[index]);
		index++;
	}
	g.primMST(); //call prim function
	vector<edge> control = g.last;

	//prints the min path.
	//Also this part ensures that the format is same with the input.txt

	for (unsigned int j = 0; j < control.size(); j++)
	{

		for (i = edges.begin(); i != edges.end(); ++i)
		{
			//if source or destiantoin is valid in the input.txt
			//then print line in input txt.
			if ((control[j].source == g.f(i->first) && control[j].dest == g.f(i->second)) || (control[j].dest == g.f(i->first) && control[j].source == g.f(i->second)))
			{
				cout << g.f(i->first) << " " << g.f(i->second) << " " << control[j].weig << endl;
			}
		}
	}
	cout << g.total << endl; //print total distance

	return 0;
}
