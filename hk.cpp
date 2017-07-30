#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
typedef struct Dlist{
    int x;
    struct Dlist *next;
    struct Dlist *prev;
}list_node,*list_head;
list_head CreateList(int);
void josephus(list_head,int,int,int);
int InitList(list_head);
int main(int argc,char * argv[]){ 
    list_head h = CreateList(5);
    list_node * ptr=h;
    do{
        std::cout<<ptr->x<<std::endl;
        ptr = ptr->next;
    }while(ptr!=h);
    return 0;
}
list_head CreateList(int n){
    list_head head = NULL;
    list_node *s,*q;
    int i;
    for(i=0;i<n;i++){
        s = (list_node*)malloc(sizeof(list_node));
        s->x = i;
        s->next = NULL;
        if(head == NULL){
            head = s;
            s->prev = head;
            s->next = head;
        }else{
            s->next = q->next;
            q->next = s;
            s->prev = q;
            head->prev = s;
        }
        q=s;
    }
    return head;
}