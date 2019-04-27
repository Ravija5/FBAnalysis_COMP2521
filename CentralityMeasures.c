// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sysexits.h>
#include <err.h>
#include<limits.h>

static NodeValues newNodeValues(Graph g);
static double getNoOutNodes(Graph g, Vertex v);
static double getNoInNodes(Graph g, Vertex v);
static double getSumOfDistance(Graph g,ShortestPaths sp,Vertex v);
static double getConnected(Graph g,ShortestPaths sp,Vertex v);
static double getBetweennessValue(Graph g,Vertex v);
static double getNumShortestPath(Graph g,ShortestPaths sp,Vertex s,Vertex t,Vertex v);
static double getOccurenceOfV(Graph g,ShortestPaths sp,Vertex s,Vertex t,Vertex v);

NodeValues outDegreeCentrality(Graph g){

    NodeValues nv = newNodeValues(g);
    
    //Initialising nv struct with number of outIncident edges from i
    for(int i = 0;i < nv.noNodes; i++){
        nv.values[i] = getNoOutNodes(g,i);
    }
    return nv;
}

NodeValues inDegreeCentrality(Graph g){
    NodeValues nv = newNodeValues(g);
    
    //Initialising nv struct with number of outIncident edges from i
    for(int i = 0;i<nv.noNodes;i++){
        nv.values[i] = getNoInNodes(g,i);
    }
    return nv;
}

NodeValues degreeCentrality(Graph g) {
    NodeValues nv = newNodeValues(g);
    
    //Initialising nv struct with sum of inIncident and outIncident edges
    for(int i = 0; i < nv.noNodes ; i++) {
        double countInNodes = getNoInNodes(g,i);
        double countOutNodes = getNoOutNodes(g,i);
        nv.values[i] = countInNodes + countOutNodes;
    }
    return nv;
}

NodeValues closenessCentrality(Graph g){
    NodeValues nv = newNodeValues(g);
    double total_distance = 0;
    double connected_nodes = 0;
    double CC;
    
    for(int v = 0 ; v < nv.noNodes ;v++){
        
        ShortestPaths sp = dijkstra(g,v);
        connected_nodes = getConnected(g,sp,v);
        total_distance = getSumOfDistance(g,sp,v);
        if(total_distance <= 0){
            CC = 0;
        }else{
            //Applying the formula 
            CC =((connected_nodes)/(numVerticies(g)-1))*((connected_nodes)/total_distance);
        }
        nv.values[v] = CC;
    }
    return nv;
}

NodeValues betweennessCentrality(Graph g){
    NodeValues nv = newNodeValues(g);
    
    for(int v = 0;v < nv.noNodes;v++){
        //Get the value for betweeness for each vertex in graph
        nv.values[v] = getBetweennessValue(g,v);
    }
    return nv;
}

NodeValues betweennessCentralityNormalised(Graph g){
    NodeValues nv = newNodeValues(g);
    double aggregator = 0;
   
    for(int v = 0;v < nv.noNodes;v++){
        aggregator = getBetweennessValue(g,v);
        nv.values[v] = (1/((double)nv.noNodes-1))*(1/((double)nv.noNodes-2))*aggregator;
    }
    return nv;
}


//Function to print a NodeValues struct
void showNodeValues(NodeValues values){
    for(int count = 0;count < values.noNodes;count++){
        printf("%d: %f\n",count,values.values[count]);
    }
}

//Function to free a NodeValues struct
void freeNodeValues(NodeValues values){
    free(values.values);
}

//Static Helper Functions
//Function to get a sum of all the distances in sp.dist[] array
static double getSumOfDistance(Graph g, ShortestPaths sp, Vertex v){
    double distance = 0;
    for(int i = 0; i < sp.noNodes ;i++)    {
        distance = distance + sp.dist[i];
    }
    return distance;
}

//Function to return number of connected components in a graph g
static double getConnected(Graph g, ShortestPaths sp, Vertex v){
    double num = 0;
    for(int i = 0;i<sp.noNodes;i++){
        if(sp.dist[i] != 0){
            num++;
        }
    }
    return num;
}

//Function to calculate the between value of a given Vertex v
static double getBetweennessValue(Graph g,Vertex v){
    double shortestPathCount = 0;
    double vCount = 0;
    double aggregator = 0;
    ShortestPaths vt = dijkstra(g,v);

    // s is for all the possible sources
    for(int s = 0; s<numVerticies(g);s++){
        //ignoring the vertex whose betweeness value is to be found
        if(s == v){
            //vertex s is the vertex whose betweeness value is to be found
            continue;
        }

        ShortestPaths sp = dijkstra(g,s);

        //t is for all the destination vertices
        for(int t=0; t<numVerticies(g) ;t++){
            
            if(t == s || t == v){
                //vertex t is the vertex whose betweeness value is to be found or it is the source vertex
                continue;
            }
          
            shortestPathCount = getNumShortestPath(g,sp,s,t,v);

            //Get the number of paths where v makes an occurance
            if(sp.dist[t] == sp.dist[v] + vt.dist[t]){
                vCount = getNumShortestPath(g,sp,s,v,t) * getNumShortestPath(g,vt,v,t,s);
            }else{
                vCount = getOccurenceOfV(g,sp,s,t,v);
            }

            if(vCount != 0 && (vCount <= shortestPathCount)){
                aggregator = aggregator + (vCount/shortestPathCount);
            }
        }
    }
    return aggregator;
}


static double getNumShortestPath(Graph g,ShortestPaths sp,Vertex s,Vertex t,Vertex v){

    if(sp.pred[t] == NULL){
        return 0;
    }
    
    PredNode *p = sp.pred[t];
    double counter = 0;

    if(p->v == s){
        counter++;
        p = p->next;
    }

    while(p!= NULL){
        t = p->v;
        if(t == s){
            counter++;
            break;
        }

        if((p->next != NULL) && p->next->v == s){
            counter++;
            break;
        }
        counter = getNumShortestPath(g,sp,s,t,v) + counter;
        p = p->next;
    }
    return counter;
}


//Function to return the number of paths between s and t where v occurs.
//For example a path such as:  s - x - y -v -t
static double getOccurenceOfV(Graph g,ShortestPaths sp,Vertex s,Vertex t,Vertex v){
    double counter = 0;
    if(sp.pred[t] != NULL){
        PredNode *p = sp.pred[t];
        
        if(p->v == s){
            p = p->next;
            if(t == v){
                return counter++;
            }
        }

        while(p != NULL){
            t = p->v;
            if(p->v == v){
                counter++;
            }
            p = p->next;
            counter = getOccurenceOfV(g,sp,s,t,v) + counter;
        }
        return counter;
    }
    return counter;
}

//Function to initialise a NodeValues struct
static NodeValues newNodeValues(Graph g){
    NodeValues nv;
    nv.values = malloc(sizeof(double)*numVerticies(g));
    
    if(nv.values == NULL) {
        err (EX_OSERR, "couldn't allocate values");
    }
    
    nv.noNodes = numVerticies(g);
    return nv;
}

//Returns a count of adjacent vertices on incoming edges from a given vertex.
static double getNoInNodes(Graph g, Vertex v){
    AdjList out;
    double count = 0;
    for(out = inIncident(g,v);out != NULL;out = out->next){
        count++;
    }
    return count;
}

//Returns a count of adjacent vertices on outgoing edges from a given vertex.
static double getNoOutNodes(Graph g, Vertex v){
    AdjList out;
    double count = 0;
    for(out = outIncident(g,v); out != NULL; out = out->next){
        count++;
    }
    return count;
}

