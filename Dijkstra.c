// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>


static ItemPQ newPQItem(int key, int value){
    ItemPQ newItem;
    newItem.key = key;
    newItem.value = value;
    return newItem;
}

static PredNode *newPredNode(Vertex v){
    PredNode *new = malloc(sizeof(PredNode));
    new->v = v;
    new->next = NULL;
    return new;
}

//Function to initialise the ShortestPath struct
static ShortestPaths newShortestPath(Graph g ,  Vertex v){
    ShortestPaths sp;
    sp.noNodes = numVerticies(g);
    sp.src = v;
    sp.dist = malloc(sp.noNodes * sizeof(int));
    
    //Initialising distance array to INT_MAX
    for(int i = 0 ; i < sp.noNodes; i++){
        sp.dist[i] = INT_MAX;
    }
    
    sp.pred = malloc(sp.noNodes * sizeof(int *));
    for(int i = 0; i < sp.noNodes ; i++){
        sp.pred[i] = calloc(sp.noNodes, sizeof(int));
        sp.pred[i] = NULL;
    }
    
    return sp;
}


ShortestPaths dijkstra(Graph g, Vertex v) {
    ShortestPaths sp = newShortestPath(g, v);
    
    PQ pq = newPQ();
    ItemPQ src = newPQItem(v,0);
    addPQ(pq, src);
    sp.dist[v] = 0;
    
    while(!PQEmpty(pq)){
        //Dequee from Queue
        ItemPQ temp = dequeuePQ(pq);
        //Get adjacent vertices for temp
        AdjList adjVerticesList = outIncident(g, temp.key);
        
        while(adjVerticesList != NULL){
            int adjWeight = adjVerticesList->weight;
            
            if(sp.dist[adjVerticesList->w] > (adjWeight + sp.dist[temp.key])){
                
                //Update the distance
                sp.dist[adjVerticesList->w] = adjWeight + sp.dist[temp.key];
                ItemPQ newItem = newPQItem(adjVerticesList->w, sp.dist[adjVerticesList->w]);
                addPQ(pq, newItem);
                
                //update the predecessor array
                if(adjVerticesList->w == v){
                    sp.pred[adjVerticesList->w] = NULL;
                }else{
                    sp.pred[adjVerticesList->w] = newPredNode(temp.key);
                }
            }else if(sp.dist[adjVerticesList->w] == (adjWeight + sp.dist[temp.key])){
                
                //Add a node to the pre existing pred list
                PredNode *temper = newPredNode(temp.key);
                temper->next = sp.pred[adjVerticesList->w]->next;
                sp.pred[adjVerticesList->w]->next = temper;
            }
            adjVerticesList = adjVerticesList->next;
        }
    }
    
    //Updating all the INT_MAX distances to 0 to match the output in test file
    for(int i = 0;i<numVerticies(g);i++){
        if(sp.dist[i] == INT_MAX){
            sp.dist[i] = 0;
        }
    }
    return sp;
}


void showShortestPaths(ShortestPaths paths) {
    
    printf("Number of nodes = %d\n", paths.noNodes);
    printf("Source vertex = %d\n", paths.src);
    printf("  Distance\n");
    for (int i = 0; i < paths.noNodes; i++) {
        if(i == paths.src)
        printf("    %d : X\n",i);
        else
        printf("    %d : %d\n",i,paths.dist[i]);
    }
    
    printf("  Preds\n");
    for (int i = 0; i < paths.noNodes; i++) {
        printf("    %d : ",i);
        PredNode* curr = paths.pred[i];
        while(curr!=NULL) {
            printf("[%d]->",curr->v);
            curr = curr->next;
        }
        printf("NULL\n");
    }
}


void  freeShortestPaths(ShortestPaths paths) {
    free(paths.dist);
    for(int i = 0; i < paths.noNodes; i++){
        free(paths.pred[i]);
    }
}

