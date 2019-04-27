// PQ ADT interface for Ass2 (COMP2521)
#include "PQ.h"
#include <stdlib.h>
#include <err.h>
#include <stdio.h>
#include <assert.h>
#include <sysexits.h>

typedef struct PQN *node;

struct PQN{
    ItemPQ item;
    node next;
    node prev;
};

struct PQRep {
    node first;
    node last;
    int nNodes;
};

//Create a new priority queue
PQ newPQ(){
    PQ priorityQ = malloc(sizeof(struct PQRep));
    if(priorityQ == NULL) err (EX_OSERR, "couldn't allocate DLList");
    priorityQ->first = NULL;
    priorityQ->last = NULL;
    priorityQ->nNodes = 0;
    return priorityQ;
}

//Function to create a new PQNode
static node newNode(ItemPQ element){
    node newnode = malloc(sizeof(struct PQN));
    if (newnode == NULL) err (EX_OSERR, "couldn't allocate DLList");
    newnode->item = element;
    newnode->next = NULL;
    newnode->prev = NULL;
    return newnode;
}

//Function to check if a PQ is empty
int PQEmpty(PQ p){
    if((p->first == NULL)&&(p->last == NULL))
    return 1;
    return 0;
}

//Function to add an ItemPQ to PQ
void addPQ(PQ pq, ItemPQ Element) {
    
    //Create a new ItemPQ
    node newnode = newNode(Element);
    
    if((pq->first == NULL) && (pq->last == NULL)){
        //First item to be inserted
        pq->first = newnode;
        pq->last = newnode;
        pq->nNodes++;
        return;
    }
    
    for(node updateNode = pq->first ; updateNode != NULL; updateNode = updateNode->next){
        if(updateNode->item.key == Element.key){
            //Item with Element's key already exists
            //Update the value of pre-existing item in PQ
            updateNode->item.value = Element.value;
            return;
        }
    }
    
    if(Element.value < pq->first->item.value){
        //Element to be inserted at the start of PQ
        newnode->next = pq->first;
        pq->first->prev = newnode;
        pq->first = newnode;
        pq->nNodes++;
    }else if(Element.value > pq->last->item.value){
        //Element to be inserted at the end of PQ
        pq->last->next = newnode;
        newnode->prev = pq->last;
        pq->last = newnode;
        pq->nNodes++;
    }else{
        //Element to be inserted in the middle of PQ
        node temp;
        for(temp = pq->first; temp != NULL; temp = temp->next){
            if (Element.value < temp->item.value) {
                //Insert newnode after temp
                newnode->next = temp;
                newnode->prev = temp->prev;
                temp->prev->next = newnode;
                temp->prev = newnode;
                break;
                
            } else if(Element.value == temp->item.value){
                //Values are equal. Observe FIFO
                newnode->next = temp->next;
                newnode->prev = temp;
                if(temp != pq->last){
                    temp->next->prev = newnode;
                }else{
                    pq->last = newnode;
                }
                temp->next = newnode;
                break;
            }
        }
        pq->nNodes++;
    }
    
}

//Function to dequeue ItemPQ from the PQ
ItemPQ dequeuePQ(PQ pq) {
    
    ItemPQ removed = pq->first->item;
    if(pq->first->next != NULL){
        pq->first = pq->first->next;
    }else{
        pq->first = NULL;
        pq->last = NULL;
    }
    return removed;
}



//Helper function to delete node n from PQ
static void removeElement(PQ list , node n){
    if( (list->first == n) && (list->last == n) ){
        //PQ has a single element
        list->first = list->last = NULL;
    }else if(list->first == n){
        //Item to be removed is the first element in PQ
        list->first = list->first->next;
    }else if(list->last == n){
        //Item to be removed is the last element in PQ
        list->last = list->last->prev;
    }else{
        //Item is removed from the middle
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
    
    list->nNodes--;
    free(n);
}

//Function to Update item with a given 'key' value, by updating that item's value to the given 'value'.
//If item with 'key' does not exist in the queue, no action is taken
void updatePQ(PQ pq, ItemPQ element) {
    for(node temp = pq->first ; temp != NULL; temp = temp->next){
        if(temp->item.key == element.key){
            removeElement(pq, temp);
            break;
        }
    }
    addPQ(pq,element);
}

//Functon to display PQ
void  showPQ(PQ pq) {
    for(node temp = pq->first;temp != NULL;temp = temp->next)
    {
        printf("Value: %d, Key:%d\n",temp->item.value,temp->item.key);
    }
}

