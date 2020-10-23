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
    virtual T removeAt(int index) = 0;// Changed T& to T
    virtual bool removeItem(const T& item) = 0;
    virtual bool empty() = 0;
    virtual int size() = 0;
    virtual void clear() = 0;
    virtual T get(int index) = 0;
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
    //Node *HeadFragmentPointers;
public:
    FragmentLinkedList(int fragmentMaxSize = 5)
    {
        this->count = 0;
        this->fragmentMaxSize = fragmentMaxSize;
        this->fragmentPointers = new Node *[2]; 
        this->fragmentPointers[0] = NULL;
        this->fragmentPointers[1] = NULL;
    }
    virtual ~FragmentLinkedList();
    virtual void add(const T& element);
    virtual void add(int index, const T& element);
    virtual T removeAt(int index);
    virtual bool removeItem(const T& item);
    virtual bool empty();
    virtual int size();
    virtual void clear();
    virtual T get(int index);
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
        int index;

    public:
        Iterator(FragmentLinkedList<T> *pList = 0, bool begin = true){
            this->pList=pList;
            if (begin==true){
                pNode=this->pList->fragmentPointers[0];
                this->index=0;
            }else{
                pNode=NULL;
                this->index=this->pList->count;
            }
        };
        Iterator(FragmentLinkedList<T> *pList = 0, int fragmentIndex = 0, bool begin = true){
            this->pList=pList;
            if (fragmentIndex==-1)
                fragmentIndex=(this->pList->count-1)/this->pList->fragmentMaxSize;
            if (begin==true){
                this->pNode=this->pList->fragmentPointers[fragmentIndex];
                this->index=fragmentIndex*this->pList->fragmentMaxSize;
            }else{
                int NumberFragment=(this->pList->count-1)/this->pList->fragmentMaxSize;
                if (fragmentIndex<NumberFragment) {
                    this->index=(fragmentIndex+1)*this->pList->fragmentMaxSize;
                    this->pNode=this->pList->fragmentPointers[fragmentIndex+1];
                }else{
                    this->index=this->pList->count;
                    this->pNode=NULL;
                }
            }
        };
        Iterator &operator=(const Iterator &iterator){
            this->pList=iterator.pList;
            this->pNode=iterator.pNode;
            this->index=iterator.index;
            return *this;
        };
        T &operator*(){
            if (this->pNode==NULL) throw std::out_of_range("Segmentation␣fault!");
            return this->pNode->data;
        };
        bool operator!=(const Iterator &iterator){
            return (this->pNode!=iterator.pNode || this->index!=iterator.index);
        };
        void remove(){
            //can be repaired
            //delete pNode;
            //delete[] pList;
            int index=0;
            Node *pTrain=this->pList->fragmentPointers[0];
            while (pTrain!=pNode){pTrain=pTrain->next;index++;}
            pNode=pNode->prev;
            this->index=this->index-1;
            this->pList->removeAt(index);
            //delete pTrain;// can be reviewed
            //reset pTrain=NULL;
            pTrain=NULL;
        };
        void set(const T& element){
            this->pNode->data=element;
        };
        Iterator &operator++(){
            if (pNode!=NULL){
                this->index++;
                pNode=pNode->next;
            } 
            else if (this->index==-1) {
                this->index=0;
                pNode=this->pList->fragmentPointers[0];
            }
            return *this;
        };
        Iterator operator++(int){
            Iterator tmp=*this;
            if (pNode!=NULL){
                this->pNode=this->pNode->next;
                this->index++;
            }
            else if (this->index==-1) {
                this->pNode=this->pList->fragmentPointers[0];
                this->index=0;
            }
            return tmp;
        };
    };
};
template <class T>
FragmentLinkedList<T>::~FragmentLinkedList(){
    FragmentLinkedList<T>::clear();
    delete [] this->fragmentPointers;
}
template <class T>
void FragmentLinkedList<T>::add(const T& element){    
    if (this->count==0){
        this->fragmentPointers[0]=new Node(element,NULL,NULL);
    }else{
        //Find Node end of List
        Node *pTrain=this->fragmentPointers[0];
        while (pTrain->next!=NULL){pTrain=pTrain->next;}
        //insert Node
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
            for (int i=0;i<=NumberFragment;i++)
                this->fragmentPointers[i]=NULL;
            this->fragmentPointers[0]=head;
            for (int i=1;i<=this->count;i++){
                head=head->next;
                if (i%this->fragmentMaxSize==0){
                    this->fragmentPointers[i/this->fragmentMaxSize]=head;
                }
            }
            //reset head=NULL
            head=NULL;
        }
        //reset pTrain=NULL
        pTrain=NULL;
    } 
    //increase number Node
    this->count++;
}
template <class T>
void FragmentLinkedList<T>::add(int index, const T& element){
    //if index is out of bound
    if (index<0 || index>this->count){
        throw std::out_of_range("The index is out of range!");
        return;
    }
    // if Node is added on tail of FragmentLinkedList
    if (index==this->count){
        FragmentLinkedList<T>::add(element);
        return;
    }

    //if index<this->count
    //find node in position index
    Node *pTrain=this->fragmentPointers[0];
    for (int i=0;i<index;i++) pTrain=pTrain->next;
    
    // Insert Node
    Node *newNode=new Node(element,pTrain,pTrain->prev);
    
    // if Node is added in front of FragmentLinkedList
    if (index==0) this->fragmentPointers[0]=newNode;
    
    //if the number of fragemt increase 
    if (this->count%this->fragmentMaxSize==0){
        Node *head=this->fragmentPointers[0];

        //delete old fragmentPointers
        int NumberFragmentBefore=max((this->count-1)/this->fragmentMaxSize,1);
        for (int i=0;i<=NumberFragmentBefore;i++){this->fragmentPointers[i]=NULL;}
        delete [] this->fragmentPointers;

        // Create New FragmentPointers and copy data from Old FragmetPointers
        int NumberFragment=max(this->count/this->fragmentMaxSize,1);
        this->fragmentPointers=new Node *[NumberFragment+1];
        for (int i=0;i<=NumberFragment;i++)
            this->fragmentPointers[i]=NULL;
        this->fragmentPointers[0]=head;
        
        //reset head=NULL
        head=NULL;
    }
    //reset head=NULL
    Node *head=this->fragmentPointers[0];
    
    //rearange 
    for (int i=1;i<=this->count;i++){
        head=head->next;
        if (i%this->fragmentMaxSize==0){
            this->fragmentPointers[i/this->fragmentMaxSize]=head;
        }
    }
    //reset head=NULL pTrain=NULL
    head=NULL;
    pTrain=NULL;

    // increase number of element
    this->count++;
}
template <class T>
T FragmentLinkedList<T>::removeAt(int index){\
    //if index is out of bound
    if (index<0 || index>=this->count){
        throw std::out_of_range("The index is out of range!");
    }

    //Find Node need delete
    Node *pTrain=this->fragmentPointers[0];
    for (int i=0;i<index;i++){pTrain=pTrain->next;}

    //save Data of Node is deleted
    //Node *tmp=pTrain;
    //T RES=(pTrain->data);
    //T& RESULT=RES;
    //delete
    T RES=pTrain->data;

    //remove Node
    if (pTrain->prev==NULL){
        this->fragmentPointers[0]=this->fragmentPointers[0]->next;
        if (this->fragmentPointers[0]!=NULL)
            this->fragmentPointers[0]->prev=NULL;
    }else{
        pTrain->prev->next=pTrain->next;
        if (pTrain->next!=NULL)
            pTrain->next->prev=pTrain->prev;
    }
    //delete node
    delete pTrain;
    //delete tmp; // can be reviewed
    Node *head=this->fragmentPointers[0];
    int NumberFragmentBefore=max((this->count-1)/this->fragmentMaxSize,1);
    for (int i=0;i<=NumberFragmentBefore;i++)this->fragmentPointers[i]=NULL;
    delete[] this->fragmentPointers;

    // Decrease this->count;
    this->count--;

    // create New FragmentPointers
    int NumberFragment=max((this->count-1)/this->fragmentMaxSize,1);
    this->fragmentPointers=new Node *[NumberFragment+1];
    for (int i=0;i<=NumberFragment;i++)
        this->fragmentPointers[i]=NULL;
    this->fragmentPointers[0]=head;
    for (int i=1;i<this->count;i++){
        head=head->next;
        if (i%this->fragmentMaxSize==0){
            this->fragmentPointers[i/this->fragmentMaxSize]=head;
        }
    }
    //reset head=NULL, pTrain=NULL
    head=NULL;
    pTrain=NULL;
    return RES;
}
template <class T>
bool FragmentLinkedList<T>::removeItem(const T& item){
    int Index=FragmentLinkedList<T>::indexOf(item);
    if (Index==-1) return false;
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

    //reset pTrain=NULL
    pTrain=NULL;
    return RESULT;
}
template <class T>
T FragmentLinkedList<T>::get(int index){
    //if index is out of bound
    if (index<0 || index>=this->count)
        throw std::out_of_range("The index is out of range!");
    Node *FindNode=this->fragmentPointers[0];
    int i=0;
    while (i<index){
        i++;
        FindNode=FindNode->next;
    }
    T RESULT= FindNode->data;

    FindNode=NULL;
    return RESULT;
}
template<class T>
bool FragmentLinkedList<T>::contains(const T& item){
    return (FragmentLinkedList<T>::indexOf(item)!=-1);
}
template <class T>
bool FragmentLinkedList<T>::empty(){
    return !(this->count>0);
}
template <class T>
void FragmentLinkedList<T>::clear(){
    Node *Current=this->fragmentPointers[0],*Next;
    while (Current!=NULL){ 
        Next=Current->next; 
        delete Current;
        Current=NULL;Current=Next;
    }
    //set Current=NULL
    Current=NULL;
    for (int i=0;i<(this->count-1)/this->fragmentMaxSize;i++){
        this->fragmentPointers[i]=NULL;
    }
    delete[] this->fragmentPointers;
    this->fragmentPointers=new Node*[2];
    this->fragmentPointers[0]=NULL;
    this->fragmentPointers[1]=NULL;
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
    pTrain=NULL;
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
    else{
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
    }
    //reset ptr=NULL
    ptr=NULL;
    

    return ss.str();
}
#endif
// END: STUDENT ANSWER

int main()
{
    FragmentLinkedList<int> pList=FragmentLinkedList<int>(15);
    for (int i=0;i<2;i++){
        pList.add(i);
    }
    auto it=FragmentLinkedList<int>::Iterator(&pList,true);
    auto it1=FragmentLinkedList<int>::Iterator(&pList,false);
    it.remove();
    cout<<*++it;

}
//when i use delete method can be reviewd