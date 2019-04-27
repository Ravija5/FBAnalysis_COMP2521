// Graph ADT interface for Ass2 (COMP2521)
#include <stdlib.h>
#include <stdio.h>
#include<assert.h>

#include "Graph.h"


struct GraphRep{
    AdjList *edges;  // array of lists
    int   nV;     // #vertices
    int   nE;     // #edges
};
static void showLL(AdjList L);
//Function to create a new graph
Graph newGraph(int nV) {
    assert(nV >= 0);
    int i;
    
    Graph g = malloc(sizeof(struct GraphRep));
    assert(g != NULL);
    g->nV = nV;
    g->nE = 0;
    
    // allocate memory for each row
    g->edges = malloc(nV * sizeof(int *));
    assert(g->edges != NULL);
    // allocate memory for each column and initialise with 0
    for (i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(int));
        assert(g->edges[i] != NULL);
    }
    return g;
}

int numVerticies(Graph g) {
    return g->nV;
}

//Function to insert a new adjListNode
static AdjList newAdjListNode( Vertex dest){
    AdjList newNode = malloc(sizeof(newNode));
    newNode->next = NULL;
    newNode->weight = 0;
    newNode->w = dest;
    return newNode;
}

//Function to insert a single edge
void  insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
    for(AdjList test = g->edges[src];test != NULL;test = test->next)
    {
        if(test->w == dest && test->weight == weight)
        {
            return;
        }
    }
    AdjList newNode = newAdjListNode(dest);
    AdjList temp = g->edges[src]->next;
    g->edges[src]->next = newNode;
    newNode->weight = weight;
    newNode->next = temp;
    g->nE++;
}

//Function to free an AdjListNode
static void freeAdjListNode(AdjList node){
    if (node == NULL) return;
    free (node);
}


//Function to delete a single edge
void removeEdge(Graph g, Vertex src, Vertex dest) {
    AdjList temp = g->edges[src]->next;
    AdjList prev = g->edges[src];
    while(temp != NULL){
        if(temp->w == dest){
            //Found edge
            //remove
            prev->next = temp->next;
            freeAdjListNode(temp);
            break;
        }
        temp = temp->next;
        prev= prev->next;
    }
    g->nE++;
}

//Function to check if two vertices are adjacent to each other
bool adjacent(Graph g, Vertex src, Vertex dest) {
    AdjList temp = g->edges[src]->next;
    while(temp != NULL)
    {
        if(temp->w == dest)
        {
            return true;
            break;
        }
        temp = temp->next;
    }
    return false;
}

//Function to return a list of outIncident edges at a vertex
AdjList outIncident(Graph g, Vertex v) {
    return g->edges[v]->next;
}

//Fucntion to return a list of inIncident edges at a vertex
AdjList inIncident(Graph g, Vertex v) {
    AdjList list = NULL;
    AdjList start = NULL;
    for(int vertice = 0;vertice < g->nV;vertice++)
    {
        if(vertice == v)
        {
            continue;
        }
        AdjList temp = g->edges[vertice]->next;
        while(temp != NULL)
        {
            if(temp->w == v)
            {
                if(list == NULL)
                {
                    list = newAdjListNode(vertice);
                    start = list;
                }
                else
                {
                    while(list->next!= NULL)
                    {
                        list = list->next;
                    }
                    list->next =newAdjListNode(vertice);
                }
                break;
            }
            temp = temp->next;
        }
    }
    return start;
}

//Function to show a list
static void showLL(AdjList L) {
    if (L == NULL)
    putchar('\n');
    else {
        printf(" %d(v)  %d(w)  ->  ", L->w, L->weight);
        showLL(L->next);
    }
}

//Function to display a graph
void  showGraph(Graph g) {
    assert(g != NULL);
    int i;
    
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (i = 0; i < g->nV; i++) {
        printf("%d - ", i);
        if(g->edges[i]->next != NULL){
            showLL(g->edges[i]->next);
        }else{
            printf("\n");
        }
    }
}

//Functin to free memory allocated to a list
static void freeLL(AdjList L) {
    if (L != NULL) {
        freeLL(L->next);
        free(L);
    }
}

//Function to free memory allocated to a graph
void  freeGraph(Graph g) {
    assert(g != NULL);
    int i;
    
    for (i = 0; i < g->nV; i++)
    freeLL(g->edges[i]);
    
    free(g);
}
// static void addtolist(AdjList start,AdjList new_node)
// {
//     if(start == NULL)
//     {
//         start = new_node;
//     }
//     else
//     {
//         while(start->next!= NULL)
//         {
//             start = start->next;
//         }
//         start->next = new_node;
//     }
// }

