// CMSC 341 - Fall 2022 - Project 3
#include    "iqueue.h"
IQueue::IQueue(prifn_t priFn, HEAPTYPE heapType){
  m_heap = nullptr;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_size = 0;
}

IQueue::~IQueue(){

  if(m_size != 0)
  {
    m_size = 0;
    m_priorFunc = nullptr;
    helpDelete(m_heap);
  }
}

IQueue::IQueue(const IQueue& rhs){
  m_priorFunc = rhs.m_priorFunc;
  m_size = rhs.m_size;
  m_heap = helpCopy(rhs.m_heap);
}

IQueue& IQueue::operator=(const IQueue& rhs){

  if(this != &rhs)
  {
    if(rhs.m_heap != nullptr)
    {
      clear();
      m_size = rhs.m_size;
      m_priorFunc = rhs.m_priorFunc;
      m_heap = helpCopy(rhs.m_heap);

    }

  }
    return *this;
}

void IQueue::insertCrop(const Crop& crop) {

  if(m_heap == nullptr)
  {
    IQueue tempIQueue(m_priorFunc, m_heapType);
    Node* temp = new Node(crop);
    m_heap = temp;
    m_size = 1;

  }else{
    IQueue tempIQueue(m_priorFunc, m_heapType);
    tempIQueue.m_heap = new Node(crop);
    tempIQueue.m_size = 1;
    mergeWithQueue(tempIQueue);
  }

}

Crop IQueue::getNextCrop() {

  //this function pops a node from the data structure as it returns the value
  // like pop() in stack

  if(m_heap != nullptr)
  {

    Node *tempA = m_heap;
    Node* tempB = m_heap->m_right;
    m_heap = m_heap->m_left;

    tempA->m_left = nullptr;
    tempA->m_right = nullptr;

    m_heap = helpMerge(m_heap, tempB);
    m_size--;

    Crop crop(tempA->m_crop); // copy constructor is called

    delete tempA;
    tempA = nullptr;
    tempB = nullptr;
    return crop;
  }
  else{
    throw std::out_of_range("can't get next heap - heap is empty");
  }

}

void IQueue::mergeWithQueue(IQueue& rhs) {
 
  if(rhs.m_size != 0)
  {
    if((rhs.m_priorFunc != this->m_priorFunc) || (rhs.m_heapType != this->m_heapType))
    {
      throw std::out_of_range("unable to merge queues with various priorities");
    }else{

      Node *tempA = rhs.m_heap; 
      m_size += rhs.m_size;
      rhs.m_size = 0;
      m_heap = helpMerge(m_heap, tempA);
    }
  }
  

}

void IQueue::clear() {
  
  helpDelete(m_heap); // helper function
}

int IQueue::numCrops() const{
  return m_size;
}

void IQueue::printCropsQueue() const {

  if(m_heap != nullptr)
  {
    helpPrint(m_heap); // helper function
  }
  else{

  cout << "Error: unable to print, heap is empty!" << endl;

  }

}

prifn_t IQueue::getPriorityFn() const {
  
  return m_priorFunc;
}

void IQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
                   
  m_priorFunc = priFn;
  m_heapType = heapType;
  Node* tHeap = m_heap;
  m_heap = nullptr;
  helpSet(tHeap);

}

void IQueue::dump() const{
  if (m_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(m_heap);
    cout << endl;
  }
}

void IQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    cout << m_priorFunc(pos->m_crop) << ":" << pos->m_crop.getCropID();
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Crop& crop) {
  sout << "Crop ID: " << crop.getCropID() 
        << ", current temperature: " << crop.getTemperature()
        << ", current soil moisture: " << crop.getMoisture() << "%"
        << ", current time: " << crop.getTimeString()
        << ", plant type: " << crop.getTypeString();
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getCrop();
  return sout;
}

void IQueue:: helpDelete(Node* aNode){

  if(aNode != nullptr)
  {
    helpDelete(aNode->m_right);
    helpDelete(aNode->m_left);
    //aNode = nullptr;
    delete aNode;
  }else{

  return;
  }

}

Node* IQueue:: helpMerge(Node* aNode, Node* bNode){
  
  //Recursive function, helping to merge

  if(aNode == nullptr) // base case
  {
    return bNode;
  }
  
  if(bNode == nullptr) // base case
  {
    return aNode;
  }

  if(m_heapType == MINHEAP)
  {
    if(m_priorFunc(aNode->m_crop) > m_priorFunc(bNode->m_crop))
    {
      swap(aNode, bNode);
    }

  }
  else if(m_priorFunc(aNode->m_crop) < m_priorFunc(bNode->m_crop)){

     swap(aNode, bNode);

  }

  swap(aNode->m_left, aNode->m_right);
  aNode->m_left = helpMerge(aNode->m_left, bNode);
  return aNode;

}

void IQueue:: helpSet(Node* aNode){

  //Recursive function, helping to set priorites
  if(aNode != nullptr)
  {
    Node* tLeft = aNode->m_left;
    Node* tRight = aNode->m_right;
    aNode->m_left = nullptr;
    aNode->m_right = nullptr;
    m_heap = helpMerge(m_heap, aNode);
    helpSet(tLeft);
    helpSet(tRight);
    tLeft= nullptr;
    tRight = nullptr;
    aNode = nullptr;
    
  }
  else{

    return;
  }


}

Node* IQueue:: helpCopy(Node* aNode){
  
    //Recursive function, helping to copy Node

  if(aNode != nullptr)
  {
    Node* insert = new Node(aNode->m_crop);
    insert->m_left = helpCopy(aNode->m_left);
    insert->m_right = helpCopy(aNode->m_right);
    return insert;
  }
  else{
    return nullptr;
  }

}

void IQueue:: helpPrint(Node* aNode) const{

  //Recursive function, helping to print

  if(aNode != nullptr)
  {
    cout << "[" << m_priorFunc(aNode->m_crop) << "] " << aNode->m_crop << endl;
    helpPrint(aNode->m_left);
    helpPrint(aNode->m_right);
  }
  else{
    return;
  }

}

