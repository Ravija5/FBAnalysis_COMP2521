/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by 
  COMP2521 2019 T1
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "LanceWilliamsHAC.h"
#include "Graph.h"
#define numVertices numVerticies

// #define ALPHA1 0.5
// #define ALPHA2 0.5
// #define BETA 0

typedef int* cluster;

//Struct containing pairs of index with minimum distance b/w them
typedef struct indexpair{
  int index1;
  int index2;
}indexpair;

//Static functions 
static Dendrogram newDendogram(int vertex);
static double** newDistMatrix(Graph g);
static double edgeWeightValue(Graph g,int row,int col);
static indexpair getVerticeWithMinDist(int size,double **dist);
//static Dendrogram *updateDendogram(int size,Dendrogram *dendRep,indexpair ip);
static double **updateDist(int size,int method,double **dist,indexpair ip);
static void printMatrix(double** matrix, int size);

/* 
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs) 
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods. 

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 * 
 */

Dendrogram LanceWilliamsHAC(Graph g, int method) {

  double **dist = newDistMatrix(g);
  printMatrix(dist, numVertices(g));

  //Initialising an array of Dendogram structs
  Dendrogram *dendRep = malloc(sizeof(DNode) * numVertices(g));
  for(int i = 0; i < numVertices(g) ; i++){
    dendRep[i] = newDendogram(i);
  }

  //Iterating through number of times the clusters need to be merged 
  for(int i = 0; i < numVertices(g) ; i++){
    indexpair ip;
    ip = getVerticeWithMinDist((numVertices(g)-i),dist);
    printf("Min dist vertices found in run %d are %d and %d\n", i, ip.index1, ip.index2);
    //dendRep = updateDendogram((numVertices(g)-i),dendRep,ip);
    dist = updateDist( (numVertices(g)-i), method, dist, ip);
    printMatrix(dist, numVertices(g));
    //printf("row:%d col:%d\n",ip.index1,ip.index2);
  }
  // printf("%d\n"*dendRep->vertex);
 
  return dendRep[0];
}

//Function to initialise a Dendogram struct
static Dendrogram newDendogram(int vertex){
  Dendrogram newD = malloc(sizeof(DNode));
  newD->vertex = vertex;
  newD->left = NULL;
  newD->right = NULL;
  return newD;
}

//Function to derive the distance matrix
static double** newDistMatrix(Graph g){
  double** dist =  malloc(sizeof( double* ) * numVertices(g));
  for(int i = 0;i<numVertices(g);i++){
    dist[i] = malloc(sizeof(double)*numVertices(g));
  }
  
  for(int row = 0;row<numVertices(g);row++){
    for(int col = 0;col<numVertices(g);col++){
     
      if(row == col){
        //Diagonals are initialised to 0
        dist[row][col] = 0;
      }else{
        //Values of distance initialised based on graph
        dist[row][col] = 1/edgeWeightValue(g,row,col);
        //printf("row :%d col:%d %f\n",row,col,dist[row][col]);
      }
    }
  }
  return dist;
}


//Function to calculate edge weight between 2 vertices
static double edgeWeightValue(Graph g,int row,int col){
  for(AdjList temp = outIncident(g,row) ; temp != NULL; temp = temp->next){
    if(temp->w == col){
      return temp->weight;  
    }
  }
  return 0;
}

//function to find clusters with minimum distances
static indexpair getVerticeWithMinDist(int size ,double **dist){

  indexpair ip;
  //Initialise indexpair struct
  ip.index1 = ip.index2 = 0;

  double min = dist[0][1];

  for(int row = 0;row < size; row++){
    for(int col = 0;col < size; col++){
      if(row == col){
        //Reached a diagonal element
        continue;
      }

      //Updating when a minimum distance is found
      if(dist[row][col] < min){
        min = dist[row][col];
        ip.index1 = row;
        ip.index2 = col;
      }
    }
  }
  return ip;
}

//function to update the dendogram after merging clusters
// static Dendrogram *updateDendogram(int size,Dendrogram *dendRep,indexpair ip){
//   //IF THE SIZE OF THE DENDOGRAM ARRAY IS ONE THEN NOTHING NEEDS TO BE DONE
//   if(size == 1)
//   {
//     return dendRep;
//   }
//   //printf("size:%d\n",size);
//   //creating a new dendogram array with size one less then the previous one
//   Dendrogram *newDend = malloc(sizeof(DNode)*(size - 1));
//   int j = 0;
//   //setting the last node of the dendogram array to be a root node for other clusters
//   newDend[size - 2] = newDendogram(-1);
//   //calculating which pair of index appears first
//   if(ip.index1 < ip.index2)
//   {
//     //iterating through the new dendogram array to update it with accordance to merged clusters
//     for(int i = 0;i<(size-1);i++)
//     {
//       //if j is equal to one of the vertices to be merged ,then adding the new node to heirarchy
//       if(j == ip.index1)
//       {
//         newDend[size-2]->left = dendRep[j];
//         //incrementing j
//         j++;
//       }
//       if(j == ip.index2)
//       {
//         newDend[size-2]->right = dendRep[j];
//         j++;
//       }
//       //added condition to check the newDend[size-2] always holds the new heirarchy
//       if(i != (size-2))
//       {
//         newDend[i] = dendRep[j];
//       }
//       //printf("cool\n");
//       j++;
//     }
//   }
//   else
//   {
//     for(int i = 0;i<(size-1);i++)
//     {
//       if(j == ip.index2)
//       {
//         newDend[size-2]->left = dendRep[j];
//         j++;
//       }
//       if(j == ip.index1)
//       {
//         newDend[size-2]->right = dendRep[j];
//         j++;
//       }
//       if(i != (size-2))
//       {
//         newDend[i] = dendRep[j];
//       }
//      // printf("cool\n");
//       j++;
//     }
//   }
  
  //return newDend; 
//}
static double **updateDist(int size,int method,double **dist,indexpair ip){
    //defining constants to be used in formulas based on methods
  if(size ==1)
  {
    return 0;
  }
  double alpha1,alpha2,beta,gamma;
  if(method == 1){
    alpha1 = 0.5;
    alpha2 = 0.5;
    beta = 0;
    gamma = -0.5;
  }else if(method == 2){
    alpha1 = 0.5;
    alpha2 = 0.5;
    beta = 0;
    gamma = 0.5; 
  }
  double **newdist = malloc(sizeof(double *)*(size-1));
  for(int i = 0;i<size-1;i++)
  {
    newdist[i] = malloc(sizeof(double)*size-1);
  }
  for(int d_col = 0;d_col<size-1;d_col++)
  {
    if(d_col == ip.index1 || d_col == ip.index2)
    {
      continue;
    }
    newdist[size-2][d_col] = alpha1*dist[ip.index1][d_col] + alpha2*dist[ip.index2][d_col] + gamma*fabs(dist[ip.index1][d_col]-dist[ip.index2][d_col]);
  }
  for(int d_row = 0;d_row<size-1;d_row++)
  {
    if(d_row == ip.index1 || d_row == ip.index2)
    {
      continue;
    }
    newdist[d_row][size-2] = alpha1*dist[d_row][ip.index1] + alpha2*dist[d_row][ip.index2] + gamma*fabs(dist[d_row][ip.index1]-dist[d_row][ip.index2]);
  }
  newdist[size - 2][size - 2] = 0;
 // printf("hi\n");
  if(ip.index1 > ip.index2)
  {
  int o_row = 0;
  for(int row = 0;row<size-1;row++)
  {
    if(o_row ==ip.index1)
    {
      o_row++;
    }
    if(o_row ==ip.index2)
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
      if(o_col == ip.index1)
      {
        o_col++;
      }  
      if(o_col == ip.index2)
      {
        o_col++;
      }
      if(row != size-2 && col != size-2)
      {
        newdist[row][col] = dist[o_row][o_col];
      }
      //Updating when a minimum distance found
      o_col++;
    }
    o_row++;
  }
  }
else if(ip.index2 > ip.index1)
{
  int o_row = 0;
  for(int row = 0;row<size-1;row++)
  {
    if(o_row ==ip.index2)
    {
      o_row++;
    }
    if(o_row ==ip.index1)
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
      if(o_col == ip.index2)
      {
        o_col++;
      }  
      if(o_col == ip.index1)
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
}













    printf("\n\n");
    for(int i = 0;i<size;i++)
    {
      for(int j= 0;j<size;j++)
      {
        printf("row:%d column:%d value:%f\t",i,j,dist[i][j]);
      }
      printf("\n");
    }
    printf("\n\n");
    for(int i = 0;i<size-1;i++)
    {
      for(int j= 0;j<size-1;j++)
      {
        printf("row:%d column:%d value:%f\t",i,j,newdist[i][j]);
      }
      printf("\n");
    }
  return newdist ;
}



void freeDendrogram(Dendrogram d) {
	return;
}

static void printMatrix(double** matrix, int size){
  for(int i = 0; i < size; i++){
    for(int j = 0; j < size; j++){
      printf("%f ", matrix[i][j]);
    }
    printf("\n");
  }
}