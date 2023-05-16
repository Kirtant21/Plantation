// CMSC 341 - Fall 2022 - Project 3
#include "iqueue.h"
#include <random>
// the followings are sample priority functions to be used by IQueue class
// users can define their own priority functions
// Priority functions compute an integer priority for a crop.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Crop &crop);// works with a MAXHEAP
int priorityFn2(const Crop &crop);// works with a MINHEAP

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public:

bool minHeapCheck(IQueue iq, Node* parent, bool testFailed = false);
bool maxHeapCheck(IQueue iq, Node* parent, bool testFailed = false);
void testRemove(Node* parent);
Node* getHeap(IQueue iq);

};

int main(){
    Random idGen(MINCROPID,MAXCROPID);
    Random temperatureGen(MINTEMP,MAXTEMP);
    int temperature = temperatureGen.getRandNum();
    Random moistureGen(MINMOISTURE,MAXMOISTURE);
    Random timeGen(MINTIME,MAXTIME);
    int time = timeGen.getRandNum();
    Random typeGen(MINTYPE,MAXTYPE);
    IQueue queue1(priorityFn2, MINHEAP);
    for (int i=0;i<10;i++){
        Crop aCrop(idGen.getRandNum(),
                    temperature,// it is the same in the region
                    moistureGen.getRandNum(),
                    time,       // it is the same in the region
                    typeGen.getRandNum());
        queue1.insertCrop(aCrop);
    }
    cout << "\nDump of the queue with priorityFn2 (MINHEAP):\n";
    queue1.dump();
    queue1.setPriorityFn(priorityFn1, MAXHEAP);
    cout << "\nDump of the queue with priorityFn1 (MAXHEAP):\n";
    queue1.dump();
    cout << "\nPreorder traversal of the nodes in the queue with priorityFn1 (MAXHEAP):\n";
    queue1.printCropsQueue();


    cout << "DEBUG - queue1 crop num: " << queue1.numCrops() << endl;
    
    // ****************** Unit Test Begins here *******************

    try{

    cout << endl;
    cout << "************** Unit Test *****************" << endl;
    cout << endl;
    
    Tester test1;
    int bigNumber = 300;
    // INSERTION MINHEAP - NORMAL CASE

    IQueue minHeapInsertNormalTest(priorityFn2, MINHEAP);

     for (int i=0;i<bigNumber;i++){
        Crop aCrop(idGen.getRandNum(),
                    temperature,// it is the same in the region
                    moistureGen.getRandNum(),
                    time,       // it is the same in the region
                    typeGen.getRandNum());
        minHeapInsertNormalTest.insertCrop(aCrop);
    }
  
    
    if(minHeapInsertNormalTest.numCrops() == bigNumber)
    {
        cout << "MINHEAP INSERTION NORMAL CASE TESTING (INSERTING 300 NODES) - TEST PASSED!" << endl;
    }else {
        cout << "MINHEAP INSERTION NORMAL CASE TESTING (INSERTING 300 NODES) - TEST FAILD!" << endl;

    }

    if(test1.minHeapCheck(minHeapInsertNormalTest, test1.getHeap(minHeapInsertNormalTest)))
    {
        cout<< "MINHEAP IS NOT STORED CORRECTLY!" << endl;
    }else{
        cout << "MINHEAP IS STORED CORRECTLY" << endl;
    }


    // INSERTION MAXHEAP - NORMAL CASE

    IQueue maxHeapInsertNormalTest(priorityFn2, MINHEAP);

    maxHeapInsertNormalTest = minHeapInsertNormalTest; // assigns minHeapInsertNormalTest data to maxHeapInsertNormalTest 
                                                     // maxHeapInsertNormalTest has minheap priority - priorities are subject to change later..

    cout << "TEST - CHANGING PRIORITY OF 300 NODES, RESTORING THE HEAP, AND MAKING IT MAXHEAP" << endl;

    maxHeapInsertNormalTest.setPriorityFn(priorityFn1, MAXHEAP); // changing maxHeapInsertNormalTest priorites to maxHeap

    if(maxHeapInsertNormalTest.numCrops() == bigNumber)
    {
        cout << "MAXHEAP INSERTION NORMAL CASE TESTING (INSERTING 300 NODES) - TEST PASSED!" << endl;
    }else{
        cout << "MAXHEAP INSERTION NORMAL CASE TESTING (INSERTING 300 NODES) - TEST FAILD!" << endl;
    }

    // below code checks if the priorites are set correctly, if true test fails if false test passes

    if(test1.maxHeapCheck(maxHeapInsertNormalTest, test1.getHeap(maxHeapInsertNormalTest)))
    {
        cout<< "MAXHEAP IS NOT STORED CORRECTLY!" << endl;
    }else{
        cout << "MAXHEAP IS STORED CORRECTLY" << endl;
    }

    queue1.dump();

    cout<<endl;
    cout<<endl;

    // REMOVAL TEST MINHEAP AND MAXHEAP

    cout << "TESTING REMOVAL (MINHEAP) AFTER INSERTING 300 NODES" << endl;

    for(int i = 0; i < bigNumber; i++) // removes 300 nodes
    {
        minHeapInsertNormalTest.getNextCrop(); 
    }

    cout << "TESTING REMOVAL (MAXHEAP) AFTER INSERTING 300 NODES" << endl;

    for(int i = 0; i < bigNumber; i++) // removes 300 nodes
    {
        maxHeapInsertNormalTest.getNextCrop(); 
    }

    
    cout<<endl;
    cout << "PRINTING AS REMOVEING (FOR MINHEAP) 10 NODES TO SHOW REMOVAL PROCESS WORKS!" << endl;
    cout<<endl;
    

    IQueue queue2(priorityFn2, MINHEAP);
    IQueue queue3(priorityFn2, MINHEAP); // for later use - to test assignment operator 

    queue3 = queue1;
    queue2 = queue1;

    for(int i = 0; i < 10; i++) // removes 10 nodes, just to display removal process - MINHEAP
    {
        queue1.getNextCrop(); 
        queue1.dump();
    }

    cout<<endl;
    cout << "PRINTING AS REMOVEING (FOR MAXHEAP) 10 NODES TO SHOW REMOVAL PROCESS WORKS!" << endl;
    cout<<endl;


    queue2.setPriorityFn(priorityFn1, MAXHEAP);

    
    for(int i = 0; i < 10; i++) // removes 10 nodes, just to display removal process - MAXHEAP
    {
        queue2.getNextCrop(); 
        queue2.dump();
    }
    
    // MERGE TEST - EDGE CASE

    IQueue mergeTestMINHeapQueue(priorityFn1, MAXHEAP); // Creating a maxheap queue

    for (int i=0;i<10;i++){
        Crop aCrop(idGen.getRandNum(),
                    temperature,// it is the same in the region
                    moistureGen.getRandNum(),
                    time,       // it is the same in the region
                    typeGen.getRandNum());
        mergeTestMINHeapQueue.insertCrop(aCrop);
    }

     IQueue emptyQueue(priorityFn1, MAXHEAP); // Creating an empty maxheap queue

    cout << "EDGE TEST - MERGING EMPTY QUEUE WITH A POPULATED QUEUE";

    mergeTestMINHeapQueue.mergeWithQueue(emptyQueue); 

    if(mergeTestMINHeapQueue.numCrops() == 10)
    {
        cout << " - TEST PASSED!"; 
    }
    else
    {
        cout << " - TEST FAILED!";
    }

    cout<<endl;

    // COPY CONSTRUCTOR TEST - NORMAL CASE

    IQueue copyConstQueue(mergeTestMINHeapQueue);

    if(&copyConstQueue != &mergeTestMINHeapQueue) // comparing memory locations - checking for deep copy
    {
        cout << "COPY CONSTRUCTOR NORMAL CASE - TEST PASSED!" << endl;
    }
    else
    {
        cout << "COPY CONSTRUCTOR NORMAL CASE - TEST FAILED!" << endl;
    }
    
    // COPY CONSTRUCTOR TEST - EDGE CASE

     IQueue copyConstQueue2(emptyQueue);

     if(&copyConstQueue2 != &emptyQueue)
     {
        cout << "COPY CONSTRUCTOR EDGE CASE - TEST PASSED!" << endl;
     }
     else
     {
        cout << "COPY CONSTRUCTOR EDGE CASE - TEST FAILED!" << endl;
     }

    // ASSIGNMENT OPERATOR TEST - NORMAL CASE

    IQueue assignmentQueueTest(mergeTestMINHeapQueue);

    assignmentQueueTest = queue3; // assigning queue3 to assignmentQueueTest

    if(&assignmentQueueTest != &queue3)
    {
        cout << "TESTING ASSIGNMENT OPERATOR FOR NORMAL CASE - TEST PASSED! " << endl;
    }
    else
    {
        cout << "TESTING ASSIGNMENT OPERATOR FOR NORMAL CASE - TEST FAILED! " << endl;
    }
    
    // ASSIGNMENT OPERATOR TEST - EDGE CASE

     IQueue assignmentQueueEdgeTest(mergeTestMINHeapQueue);

    assignmentQueueEdgeTest = emptyQueue; // assigning queue3 to assignmentQueueTest

    if(test1.getHeap(assignmentQueueEdgeTest) != nullptr)
    {

        cout << "TESTING ASSIGNMENT OPERATOR FOR EDGE CASE (ASSIGNING AN EMPTY QUEUE) - TEST PASSED! " << endl;
    }
    else
    {
        cout << "TESTING ASSIGNMENT OPERATOR FOR EDGE CASE (ASSIGNING AN EMPTY QUEUE) - TEST FAILED! " << endl;
    }

    // EXCEPTION ERROR CASE - MERGE

    IQueue maxHeapRangeTest(priorityFn1, MAXHEAP);
    IQueue minHeapRangeTest(priorityFn2, MINHEAP);

     for (int i=0;i<10;i++){
        Crop aCrop(idGen.getRandNum(),
                    temperature,// it is the same in the region
                    moistureGen.getRandNum(),
                    time,       // it is the same in the region
                    typeGen.getRandNum());
        maxHeapRangeTest.insertCrop(aCrop);
    }

     for (int i=0;i<10;i++){
        Crop aCrop(idGen.getRandNum(),
                    temperature,// it is the same in the region
                    moistureGen.getRandNum(),
                    time,       // it is the same in the region
                    typeGen.getRandNum());
        minHeapRangeTest.insertCrop(aCrop);
    }

    maxHeapRangeTest.mergeWithQueue(minHeapRangeTest); // trying to merge two queues with different priority - error case
    

    }catch(out_of_range const &e){
        cout << "error: Queue is out of range" << endl;
    }

    IQueue emptyQueue(priorityFn1, MAXHEAP);

    // EXCEPTION ERROR CASE - REMOVE
    
    try{
        cout << "ATTEMPTING TO DEQUEUE AN EMPTY QUEUE: ";
        cout << emptyQueue.getNextCrop() << endl; // trying to remove crop from an empty queue - error case

    }catch(std::out_of_range const &e){
        cout << "error: Queue is out of range" << endl;
    }
    return 0;

}

int priorityFn1(const Crop &crop) {
    //needs MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [30-116]
    //the highest priority would be 110+6 = 116
    //the lowest priority would be 30+0 = 30
    //the larger value means the higher priority
    int priority = crop.getTemperature() + crop.getType();
    return priority;
}

int priorityFn2(const Crop &crop) {
    //needs MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-103]
    //the highest priority would be 0+0 = 0
    //the lowest priority would be 100+3 = 103
    //the smaller value means the higher priority
    int priority = crop.getMoisture() + crop.getTime();
    return priority;
}

bool Tester::minHeapCheck(IQueue iq, Node* parent, bool testFailed){


    if(parent == nullptr)
    {
        return testFailed;
    }

   if(parent->m_left && iq.m_priorFunc(parent->m_left->getCrop()) < iq.m_priorFunc(parent->getCrop()))
    {
        testFailed = true; // was set to false by default in prototype 
    }

    if(parent->m_right && iq.m_priorFunc(parent->m_right->getCrop()) < iq.m_priorFunc(parent->getCrop()))
    {
        testFailed = true; // was set to false by default in prototype 
    }
    
    minHeapCheck(iq, parent->m_left, testFailed);
    minHeapCheck(iq, parent->m_right, testFailed);

    return testFailed;


}

bool Tester::maxHeapCheck(IQueue iq, Node* parent, bool testFailed){


    if(parent == nullptr)
    {
        return testFailed;
    }

   if(parent->m_left && iq.m_priorFunc(parent->m_left->getCrop()) > iq.m_priorFunc(parent->getCrop()))
    {
        testFailed = true; // was set to false by default in prototype 
    }

    if(parent->m_right && iq.m_priorFunc(parent->m_right->getCrop()) > iq.m_priorFunc(parent->getCrop()))
    {
        testFailed = true; // was set to false by default in prototype 
    }
    
    maxHeapCheck(iq, parent->m_left, testFailed);
    maxHeapCheck(iq, parent->m_right, testFailed);

    return testFailed;


}

void Tester:: testRemove(Node* parent){

    if(parent == nullptr)
    {
     return;
    }


    testRemove(parent->m_left);
    testRemove(parent->m_right);

    //cout << parent->getNextCrop() << endl;

}

Node* Tester:: getHeap(IQueue iq){

    if(iq.m_heap == nullptr){
        return nullptr;
    }else{
        return iq.m_heap;
    }


}

