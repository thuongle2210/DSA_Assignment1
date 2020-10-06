#include <sstream>
#include <string>
#include <iostream>
#include <type_traits>
using namespace std;

#ifndef ILIST_H
#define ILIST_H

template <class T>
class IList
{
public:
    
    virtual void add(const T& element) = 0;
    virtual void add(int index, const T& element) = 0;
    virtual T& removeAt(int index) = 0;
    virtual bool removeItem(const T& item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T& get(int index) = 0;
    virtual void set(int index, const T& element) = 0;
    virtual int indexOf(const T& item) = 0;
    virtual bool contains(const T& item) = 0;
    virtual string toString() = 0;
};

#endif

// STUDENT ANSWER
#ifndef FRAGMENT_LINKED_LIST
#define FRAGMENT_LINKED_LIST
template <class T>
class FragmentLinkedList : IList<T>
{
public:
    class Node;
    class Iterator;
protected:
    Node **fragmentPointers;
    int fragmentMaxSize;
    int count;

public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->count = 0;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node *[2]; // change range
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
    }
    virtual ~FragmentLinkedList();
    virtual void add(const T& element);
    virtual void add(int index, const T& element);
    virtual T& removeAt(int index);
    virtual bool removeItem(const T& item);
    virtual bool empty();
    virtual int size();
    virtual void clear();
    virtual T& get(int index);
    virtual void set(int index, const T& element);
    virtual int indexOf(const T& item);
    
    virtual bool contains(const T& item);
    virtual string toString();
    Iterator begin(int index = 0);
    Iterator end(int index = -1);
    

public:
    class Node
    {
    private:
        T data;
        Node *next;
        Node *prev;
        friend class FragmentLinkedList<T>;

    public:
        Node()
        {
            next = 0;
            prev = 0;
        }
        Node(Node *next, Node *prev)
        {
            this->next = next;
            this->prev = prev;
            //additional Node
            if (next) next->prev=this;
            if (prev) prev->next=this;
        }
        Node(T data, Node *next, Node *prev)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
            //additional Node
            if (next) next->prev=this;
            if (prev) prev->next=this;
        }
    };

    class Iterator
    {
    private:
        FragmentLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(FragmentLinkedList<T> *pList = 0, bool begin = true){
            this->pList=pList;
            if (begin==true) pNode=this->pList->fragmentLinkedList[0];
            else{
                //identify last fragment
                int Division=(this->pList->count-1)/this->pList->fragmentMaxSize;
                Node *pTrain=this->pList->fragmentPointers[Division];
                //identify last element
                while (pTrain->next!=NULL){
                    pTrain=pTrain->next;            
                }
                pNode=pTrain->next; 
            }
        };
        Iterator(FragmentLinkedList<T> *pList = 0, int fragmentIndex = 0, bool begin = true){
            this->pList=pList;
            if (fragmentIndex==-1)
                fragmentIndex=(this->pList->count-1)/this->pList->fragmentMaxSize;
            if (begin==true){
                pNode=this->pList->fragmentPointers[fragmentIndex];
            }else{
                Node *pTrain=this->pList->fragmentPointers[fragmentIndex];
                int Index=0;
                while (pTrain->next!=NULL && Index<this->pList->fragmentMaxSize){
                    pTrain=pTrain->next;
                    Index++;
                }
                pNode=pTrain->next;
            }
        };
        Iterator &operator=(const Iterator &iterator){
            this->pList=iterator->pList;
            this->pNode=iterator.Node;
        };
        T &operator*(){
            return this->pNode->data;
        };
        bool operator!=(const Iterator &iterator){
            return (this->pNode!=iterator.pNode);
        };
        void remove(){
            //can be repaired
            //delete pNode;
            //delete[] pList;
            delete pNode;
        };
        void set(const T& element){
            this->pNode->data=element;
        };
        Iterator &operator++(){
            if (pNode!=NULL) pNode=pNode->next;
            return *this;
        };
        Iterator operator++(int){
            Iterator tmp=*this;
            this->pNode=this->pNode->next;
            return tmp;
        };
    };
    
};
template <class T>
FragmentLinkedList<T>::~FragmentLinkedList(){
    delete [] this->fragmentPointers;
}
template <class T>
void FragmentLinkedList<T>::add(const T& element){    
    if (this->count==0){
        this->fragmentPointers[0]=new Node(element,NULL,NULL);
    }else{
        Node *pTrain=this->fragmentPointers[0];
        //pTrain=this->fragmentPointers[0];
        while (pTrain->next!=NULL){pTrain=pTrain->next;}
        pTrain->next=new Node(element,NULL,pTrain);
        //if the number of fragemt increase 
        if (this->count%this->fragmentMaxSize==0){
            Node *head=this->fragmentPointers[0];
            //delete old fragmentPointers
            int NumberFragmentBefore=max((this->count-1)/this->fragmentMaxSize,1);
            for (int i=0;i<NumberFragmentBefore;i++){this->fragmentPointers[i]=NULL;}
            delete [] this->fragmentPointers;
            // Create New FragmentPointers and copy data from Old FragmetPointers
            int NumberFragment=max((this->count)/this->fragmentMaxSize,1);
            this->fragmentPointers=new Node *[NumberFragment+1];
            this->fragmentPointers[0]=head;
            for (int i=1;i<=this->count;i++){
                head=head->next;
                if (i%this->fragmentMaxSize==0){
                    this->fragmentPointers[i/this->fragmentMaxSize]=head;
                }
            }
        }
        
    } 
    //increase number Node
    this->count++;
}
template <class T>
void FragmentLinkedList<T>::add(int index, const T& element){
    // if Node is added on tail of FragmentLinkedList
    if (index==this->count){
        FragmentLinkedList<T>::add(element);
        return;
    }
    //index<this->count
    Node *pTrain=this->fragmentPointers[0];
    for (int i=0;i<index;i++) pTrain=pTrain->next;
    Node *newNode=new Node(element,pTrain,pTrain->prev);
    // if Node is added in front of FragmentLinkedList
    if (index==0) this->fragmentPointers[0]=newNode;
    //if the number of fragemt increase 
    if (this->count%this->fragmentMaxSize==0){
        Node *head=this->fragmentPointers[0];
        //delete old fragmentPointers
        int NumberFragmentBefore=max((this->count-1)/this->fragmentMaxSize,1);
        for (int i=0;i<NumberFragmentBefore;i++){this->fragmentPointers[i]=NULL;}
        delete [] this->fragmentPointers;
        // Create New FragmentPointers and copy data from Old FragmetPointers
        int NumberFragment=max(this->count/this->fragmentMaxSize,1);
        this->fragmentPointers=new Node *[NumberFragment+1];
        this->fragmentPointers[0]=head;
    }
    Node *head=this->fragmentPointers[0];
    //rearange 
    for (int i=1;i<=this->count;i++){
        head=head->next;
        if (i%this->fragmentMaxSize==0){
            this->fragmentPointers[i/this->fragmentMaxSize]=head;
        }
    }
    // increase number of element
    this->count++;
}
template <class T>
T& FragmentLinkedList<T>::removeAt(int index){
    //Find Node need delete
    Node *pTrain=this->fragmentPointers[0];
    for (int i=0;i<index;i++){
        pTrain=pTrain->next;
    }
    
    //save Data of Node is deleted
    Node *tmp=pTrain;
    T RES=(pTrain->data);
    T& RESULT=RES;
    //delete
    if (pTrain->prev==NULL){
        this->fragmentPointers[0]=this->fragmentPointers[0]->next;
        this->fragmentPointers[0]->prev=NULL;
    }else{
        pTrain->prev->next=pTrain->next;
        if (pTrain->next!=NULL)
            pTrain->next->prev=pTrain->prev;
    }
    //delete tmp; // can be reviewed
    Node *head=this->fragmentPointers[0];
    int NumberFragmentBefore=max((this->count-1)/this->fragmentMaxSize,2);
    for (int i=0;i<NumberFragmentBefore;i++)
        this->fragmentPointers[i]=NULL;
    delete[] this->fragmentPointers;
    // Decrease this->count;
    this->count--;
    // create New FragmentPointers
    int NumberFragment=max((this->count-1)/this->fragmentMaxSize,2);
    this->fragmentPointers=new Node *[NumberFragment];
    this->fragmentPointers[0]=head;
    for (int i=1;i<this->count;i++){
        head=head->next;
        if (i%this->fragmentMaxSize==0){
            this->fragmentPointers[i/this->fragmentMaxSize]=head;
        }
    }
    return RESULT;
}
template <class T>
bool FragmentLinkedList<T>::removeItem(const T& item){
    int Index=FragmentLinkedList<T>::indexOf(item);
    if (Index==false) return false;
    FragmentLinkedList<T>::removeAt(Index);
    return true;
}
template <class T>
int FragmentLinkedList<T>::indexOf(const T& item){
    Node *pTrain=this->fragmentPointers[0];
    int RESULT=-1;
    int i=0;
    while (pTrain!=NULL){
        if (pTrain->data==item) {
            RESULT=i;
            break;
        }  
        pTrain=pTrain->next;
        i++;
    }
    return RESULT;
}
template <class T>
T& FragmentLinkedList<T>::get(int index){
    Node *FindNode=this->fragmentPointers[0];
    int i=0;
    while (i<index){
        i++;
        FindNode=FindNode->next;
    }
    return FindNode->data;
}
template<class T>
bool FragmentLinkedList<T>::contains(const T& item){
    return (FragmentLinkedList<T>::indexOf(item)!=-1);
}
template <class T>
bool FragmentLinkedList<T>::empty(){
    return (this->count>0);
}
template <class T>
void FragmentLinkedList<T>::clear(){
    Node *Current=this->fragmentPointers[0],*Next;
    while (Current!=NULL){
        Next=Current->next;
        free(Current);
        Current=NULL;
        Current=Next;
    }
    for (int i=0;i<this->count/this->fragmentMaxSize;i++){
        this->fragmentPointers[i]=NULL;
    }
    this->count=0;
}
template <class T>
void FragmentLinkedList<T>::set(int index,const T& element){
    int Division=index/this->fragmentMaxSize;
    int IndexDivision=index-Division*this->fragmentMaxSize;
    Node *pTrain=this->fragmentPointers[Division];
    while (IndexDivision>0){
        IndexDivision--;
        pTrain=pTrain->next;
    }
    pTrain->data=element;
}
template <class T>
int FragmentLinkedList<T>::size(){
    return this->count;
}
template <class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::begin(int index){
    return FragmentLinkedList<T>::Iterator(this,index,true);
}
template <class T>
typename FragmentLinkedList<T>::Iterator FragmentLinkedList<T>::end(int index){
    return FragmentLinkedList<T>::Iterator(this,index,false);
} 

template <class T>
string FragmentLinkedList<T>::toString()
{
    stringstream ss;
    ss << "[";
    Node *ptr = this->fragmentPointers[0];

    if (this->count == 0)
        ss << "]";

    // TODO
    ss<<ptr->data;
    int i=this->count;
    while (i>1){
        i--;
        ptr=ptr->next;
        ss<<","<<ptr->data;    
    }
    ss<<"]";
    // END: TODO

    return ss.str();
}
#endif
// END: STUDENT ANSWER

int main()
{
    // TESTCASE INPUT
    // === Example
    FragmentLinkedList<int> fList(3);
    for (int i=0;i<10000;i++) fList.add(i,i+1);
    cout<<fList.toString();
    //cout<<fList.removeAt(0)<<endl;
    //cout<<fList.removeAt(0)<<endl;`
    return 0;
}