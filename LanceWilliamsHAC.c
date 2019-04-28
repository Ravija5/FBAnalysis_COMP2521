/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by 
  COMP2521 2019 T1
*/
#include <stdlib.h>
#include <stdio.h>
#include "LanceWilliamsHAC.h"
#include "Graph.h"
#define numVertices numVerticies
typedef int* cluster;
//structure to hold the positions of distances with minimum distances
typedef struct indexpair{
  int index1;
  int index2;
}indexpair;
static double edgeWeightvalue(Graph g,int row,int col);
static indexpair getVerticeWithMinDist(int size,double **dist);
static Dendrogram *updateDendogram(int size,Dendrogram *dendA,indexpair vp);
static double **updateDist(int size,int method,double **dist,indexpair ip);
//function to create a new dendogram
static Dendrogram newDendogram(int vertex){
  Dendrogram newD = malloc(sizeof(DNode));
  newD->vertex = vertex;
  newD->left = NULL;
  newD->right = NULL;
  return newD;
}
/* 
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs) 
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods. 

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 * 
 */

Dendrogram LanceWilliamsHAC(Graph g, int method) {
  //intialising a matrix of doubles to store the distance
  double **dist = malloc(sizeof(double *)*numVertices(g));
  for(int i = 0;i<numVertices(g);i++)
  {
    dist[i] = malloc(sizeof(double)*numVertices(g));
  }
  //intialising the distance array with the distances between vertices
  for(int row = 0;row<numVertices(g);row++)
  {
    for(int col = 0;col<numVertices(g);col++)
    {
      //dealing with the diagnol in the matrix
      if(row == col)
      {
        dist[row][col] = 0;
      }
      //alloting distances based on graph
      else
      {
        dist[row][col] = 1/edgeWeightvalue(g,row,col);
        //printf("row :%d col:%d %f\n",row,col,dist[row][col]);
      }
    }
  }
  //initiliasing the Dendogram array
  Dendrogram *dendA = malloc(sizeof(DNode)*numVertices(g));
  for(int i = 0;i<numVertices(g);i++)
  {
    dendA[i] = newDendogram(i);
  }
  //iterating number of times the clustered need to be merged 
  for(int count = 0;count < numVertices(g);count++)
  {
    indexpair ip;
    //printf("%d\n",count);
    ip = getVerticeWithMinDist((numVertices(g)-count),dist);
    dendA = updateDendogram((numVertices(g)-count),dendA,ip);
    dist = updateDist((numVertices(g)-count),method,dist,ip);
    //printf("row:%d col:%d\n",vp.index1,vp.index2);
  }
  //printf("toooocool\n");
  // printf("%d\n"*dendA->vertex);
 
  return dendA[0];
}

//function to calculate edge weight betwenn 2 vertices
static double edgeWeightvalue(Graph g,int row,int col){
  for(AdjList temp = outIncident(g,row);temp != NULL;temp = temp->next)
  {
    if(temp->w == col)
    {
      return temp->weight;  
    }
  }
  return 0;
}
//function to find clusters with minimum distances
static indexpair getVerticeWithMinDist(int size,double **dist){  indexpair VP;
  //setting a default value 
  VP.index1 = 0;
  VP.index2 = 0;
  double min = dist[0][1];
  for(int row = 0;row<size;row++)
  {
    for(int col = 0;col<size;col++)
    {
      if(row == col)
      {
        continue;
      }
      //updating when a minimum distance found
      if(dist[row][col] < min)
      {
        min = dist[row][col];
        VP.index1 = row;
        VP.index2 = col;
      }
    }
  }
  return VP;
}
//function to update the dendogram after merging clusters
static Dendrogram *updateDendogram(int size,Dendrogram *dendA,indexpair vp){
  //IF THE SIZE OF THE DENDOGRAM ARRAY IS ONE THEN NOTHING NEEDS TO BE DONE
  if(size == 1)
  {
    return dendA;
  }
  //printf("size:%d\n",size);
  //creating a new dendogram array with size one less then the previous one
  Dendrogram *newDend = malloc(sizeof(DNode)*(size - 1));
  int j = 0;
  //setting the last node of the dendogram array to be a root node for other clusters
  newDend[size - 2] = newDendogram(-1);
  //calculating which pair of index appears first
  if(vp.index1 < vp.index2)
  {
    //iterating through the new dendogram array to update it with accordance to merged clusters
    for(int i = 0;i<(size-1);i++)
    {
      //if j is equal to one of the vertices to be merged ,then adding the new node to heirarchy
      if(j == vp.index1)
      {
        newDend[size-2]->left = dendA[j];
        //incrementing j
        j++;
      }
      if(j == vp.index2)
      {
        newDend[size-2]->right = dendA[j];
        j++;
      }
      //added condition to check the newDend[size-2] always holds the new heirarchy
      if(i != (size-2))
      {
        newDend[i] = dendA[j];
      }
      //printf("cool\n");
      j++;
    }
  }
  else
  {
    for(int i = 0;i<(size-1);i++)
    {
      if(j == vp.index2)
      {
        newDend[size-2]->left = dendA[j];
        j++;
      }
      if(j == vp.index1)
      {
        newDend[size-2]->right = dendA[j];
        j++;
      }
      if(i != (size-2))
      {
        newDend[i] = dendA[j];
      }
     // printf("cool\n");
      j++;
    }
  }
  
  return newDend; 
}
static double **updateDist(int size,int method,double **dist,indexpair ip){
    //defining constants to be used in formulas based on methods
  double alpha1,alpha2,beta,gamma;
  if(method == 1)
  {
    alpha1 = 0.5;
    alpha2 = 0.5;
    beta = 0;
    gamma = -0.5;
  }
  else if(method == 2)
  {
    alpha1 = 0.5;
    alpha2 = 0.5;
    beta = 0;
    gamma = 0.5; 
  }
  double **newdist = malloc(sizeof(double *)*(size-1));
  for(int i = 0;i<size;i++)
  {
    newdist[i] = malloc(sizeof(double)*size-1);
  }
  // for(int d_col = 0;d_col<size-1;d_col++)
  // {
  //   new
  // }
  int o_row = 0;
  for(int row = 0;row<size-1;row++)
  {
    if(o_row == ip.index1 || o_row ==ip.index2)
    {
      o_row++;
    }
    int o_col = 0;
    for(int col = 0;col<size-1;col++)
    {
      // if(row == col)
      // {
      //   continue;
      // }
      if(o_col == ip.index1 || o_col ==ip.index2)
      {
         o_col++;
      }  
      if(row != size-2 && col != size-2)
      {
        newdist[row][col] = dist[o_row][o_col];
      }
      //updating when a minimum distance found
      o_col++;
    }
    o_row++;
  }
  return newdist ;
}
void freeDendrogram(Dendrogram d) {
	return;
}
