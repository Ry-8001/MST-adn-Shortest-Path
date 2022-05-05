
/*
* q2.cpp
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
#include <bits/stdc++.h>
using namespace std;
#define INF 0x3f3f3f3f
typedef pair<int, int> edgePair;

//edge struct
//string source and destination
typedef struct n
{
    string source;
    string dest;
    int weight;
} node;

//edge struct
//int source and destianiton
//mapped edge
typedef struct n1
{
    int source;
    int dest;
    int weight;
} edge;

class Graph
{
    int V;

    list<pair<int, int>> *adj;

public:
    vector<int> par;
    int dis;
    Graph(int V); // Constructor
    ~Graph();
    void addEdge(int u, int v, int w);

    // prints shortest path from s
    void shortestPath(int s);
};

Graph::Graph(int V)
{
    this->V = V;
    adj = new list<edgePair>[V];
}
Graph::~Graph()
{
	delete[] adj;
}
void Graph::addEdge(int u, int v, int w)
{
    adj[u].push_back(make_pair(v, w));
    adj[v].push_back(make_pair(u, w));
}

void Graph::shortestPath(int begIndex)
{

    priority_queue<edgePair, vector<edgePair>, greater<edgePair>> priorQueue;

    vector<int> weights(V, INF);//infinite weighted vertexes
    vector<int> parent(V, -1);//this array is for the path printing
    //Give 0 to first nodes priority 
    //to pop first
    priorQueue.push(make_pair(0, begIndex));
    weights[begIndex] = 0;

    while (!priorQueue.empty())
    {
        //get the index of the poped element
        int parentIndex= priorQueue.top().second;
        priorQueue.pop();

        list<pair<int, int>>::iterator i;
        for (i = adj[parentIndex].begin(); i != adj[parentIndex].end(); ++i)
        {
            //vertex index parent
            int v = (*i).first;
            int weight = (*i).second;//current weight

            //looks for the shortest path
            if (weights[v] > weights[parentIndex] + weight)
            {
                // Update the weight  
                weights[v] = weights[parentIndex] + weight;
                //add the new pair to the queue
                priorQueue.push(make_pair(weights[v], v));
                //add the parent into parent array
                parent[v] = parentIndex;
            }
        }
    }
    //asssign the instances 
    this->par = parent;
    this->dis = weights[V - 1];
}
//same as q1.cpp
string f(int index, map<string, int> mapF)
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
//retuns if the given key in the vector array
bool Invector(vector<string> &v, string e)
{

    for (unsigned int i = 0; i < v.size(); i++)
    {
        size_t s1 = v[i].find(e);
        // size_t destcePosEn = tempEdges[i].dest.find("E");

        if (s1 == 0)
        {
            return true;
        }
    }

    return false;
}

int main()
{
    string file_name;
    cin >> file_name;
    ifstream file(file_name);
    if (!file.is_open())
    {
        cout << "File cannot be opened" << endl;
        return 1;
    }

    string line, s, d;
    int weight;
    //contains strings that has conncection 
    //less than 5 meter to the Enemy vertex
    vector<string> problemVertexs;
    vector<node> tempEdges;
    //contains mapped edges like src=0 dest =1 weight 13
    vector<edge> mappedEdges;
    while (!file.eof())
    {

        getline(file, line, ',');

        s = line;
        //get destination
        getline(file, line, ',');
        d = line;
        getline(file, line, '\n');
        stringstream cas(line);
        weight = 0;
        cas >> weight;

        size_t pos1 = s.find("E");
        size_t pos2 = d.find("E");
        //enemy spot detected
        //either source or destination
        if (pos1 == 0 && weight < 5 && pos2 != 0) 
        {
            //add to problematic vertex
            problemVertexs.push_back(s);
            //  cout<<"1problematic->"<<s<<endl;
        }
        else if (pos2 == 0 && weight < 5 && pos1 != 0)
        {
            problemVertexs.push_back(s);
            // cout<<"2problematic->"<<s<<endl;
        }
        // add to the non-mapped edge vector
        node temp;
        temp.source = s;
        temp.dest = d;
        temp.weight = weight;
        tempEdges.push_back(temp);
    }

    unsigned int counter = 1;
    //map for hashingn the vertexs
    map<string, int> mymap;
    map<string, int>::iterator it;

    for (unsigned int j = 0; j < tempEdges.size(); j++)
    {
        //looks the source and deest
        
        size_t sourcePosEn = tempEdges[j].source.find("E");
        size_t destcePosEn = tempEdges[j].dest.find("E");
        //if there is no enemy in source or dest
        //then
        if (sourcePosEn != 0 && destcePosEn != 0)
        {
            //if no direct conn to problmetic Vertex in source or dest
            //then
            if (!(Invector(problemVertexs, tempEdges[j].source) || Invector(problemVertexs, tempEdges[j].dest)))
            {
                //mapp the given source vertex
                it = mymap.find(tempEdges[j].source);

                //find from source
                if (it == mymap.end())
                {
                    //if it is Ma then corresponding value is ->0
                    size_t cont = tempEdges[j].source.find("Ma");
                    if (cont == 0)
                    {
                        mymap.insert(pair<string, int>(tempEdges[j].source, 0));
                    }
                    else
                    {
                        mymap.insert(pair<string, int>(tempEdges[j].source, counter++));
                    }
                }
                //find from dest
                it = mymap.find(tempEdges[j].dest);
                // if mankara is seen then asign it as 0th vertex
                if (it == mymap.end())
                {
                    size_t cont = tempEdges[j].dest.find("Ma");
                    if (cont == 0)
                    {
                        mymap.insert(pair<string, int>(tempEdges[j].dest, 0));
                    }
                    else
                    {
                        mymap.insert(pair<string, int>(tempEdges[j].dest, counter++));
                    }
                }

                //insert new edge  to the vector
                edge temp;
                temp.source = mymap[tempEdges[j].source];
                temp.dest = mymap[tempEdges[j].dest];
                temp.weight = tempEdges[j].weight;
                mappedEdges.push_back(temp);
            }
        }
    }
  

    //init the graph
    int V = mymap.size();
    Graph g(V);
    //create the graph by adding edges
    for (unsigned int i = 0; i < mappedEdges.size(); i++)
    {
        //cout<<f(mappedEdges[i].source,mymap)<<f(mappedEdges[i].dest,mymap)<<mappedEdges[i].weight<<endl;
        g.addEdge(mappedEdges[i].source, mappedEdges[i].dest, mappedEdges[i].weight);
    }
    g.shortestPath(mymap["Ma"]);

    vector<int> parent = g.par;

    //find the index for  the  Mo vertex
    int ind = mymap["Mo"];

    

    //add to stack 
    //because the output is reverse
    //ordered
    stack<string> mystack;
    //push Mo to stack
    mystack.push(f(ind, mymap));
    //push all the parents of Mo to the stack
    while (ind != 0)
    {
        mystack.push(f(parent[ind], mymap));
        ind = parent[ind];
    }

    //pop the Vertexes and print
    while (!mystack.empty())
    {
        cout << mystack.top() << " ";
        mystack.pop();
    }
    //print out the total dist
    cout << g.dis << endl;

    return 0;
}