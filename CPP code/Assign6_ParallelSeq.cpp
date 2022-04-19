#include <iostream> 
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>  //time
#include "Customer.h"
using namespace std;

//METHOD DECLARATIONS

void RunVector(std::string fileName);
void RunArray(std::string fileName);

vector<thread> CreateThreadsVector(std::string fileName);
vector<thread> CreateThreadsArray(std::string fileName);

vector<Customer> parseTextFileVector(std::string fileName);
Customer* parseTextFileArray(std::string fileName, size_t* outSize);

void calculateRange(Customer** customers, int from, int to, int arrIndex);
void calculateRange(vector<Customer>* customers, int from, int to, int arrIndex);

void testParsingFile(std::string fileName);


//Global Variables
mutex mx; //Mutual Exclusion (like synchronize keyword in java)
int counter = 0;
int numThreadsFinished = 0;
int _summations[] = { 0, 0, 0, 0 };


//******************************************************************************//

int main(void)
{
    const std::string fileName = "accounts.txt";
    cout << "Start Program " << fileName <<  endl;
    //const std::string fileName = "accounts - Large.txt";
   
    //testParsingFile(fileName);
    //return 0;

    RunVector(fileName);
    //RunArray(fileName);


    std::system("pause");

}

void RunVector(std::string fileName)
{
    //VECTOR:
    auto start = chrono::high_resolution_clock::now();

    vector<thread> raceThreads = CreateThreadsVector(fileName);
    auto stop1 = chrono::high_resolution_clock::now();

    for (thread& t : raceThreads)
        t.join();

    auto stop2 = chrono::high_resolution_clock::now();
    auto microSecondsElapsed_parsing = chrono::duration_cast<chrono::microseconds>(stop1 - start);
    auto microSecondsElapsed_processing = chrono::duration_cast<chrono::microseconds>(stop2 - stop1);
    double miliseconds_parsingFile = microSecondsElapsed_parsing.count() / 1000.00;
    double miliseconds_parallelProcessing = microSecondsElapsed_processing.count() / 1000.00;

    for (size_t i = 0; i < 4; i++)
    {
        counter += _summations[i];
    }

    //cout << "\nparallel c++ time: " << microSecondsElapsed.count() << " microsec?" << endl;
    cout << "\nparallel c++ VECTOR time: " << miliseconds_parallelProcessing << " milisec?" << endl;
    cout << "\nThe final VECTOR counter is " << counter << endl;
}

void RunArray(std::string fileName)
{
    auto start = chrono::high_resolution_clock::now();

    vector<thread> raceThreads = CreateThreadsArray(fileName);
    auto stop1 = chrono::high_resolution_clock::now();

    for (thread& t : raceThreads)
        t.join();

    auto stop2 = chrono::high_resolution_clock::now();
    auto microSecondsElapsed_parsing = chrono::duration_cast<chrono::microseconds>(stop1 - start);
    auto microSecondsElapsed_processing = chrono::duration_cast<chrono::microseconds>(stop2 - stop1);
    double miliseconds_parsingFile = microSecondsElapsed_parsing.count() / 1000.00;
    double miliseconds_parallelProcessing = microSecondsElapsed_processing.count() / 1000.00;

    for (size_t i = 0; i < 4; i++)
    {
        counter += _summations[i];
    }

    //cout << "\nparallel c++ time: " << microSecondsElapsed.count() << " microsec?" << endl;
    cout << "\nparallel ARRAY c++ time: " << miliseconds_parallelProcessing << " milisec?" << endl;
    cout << "\nThe final counter ARRAY is " << counter << endl;
}

//******************************************************************************//
vector<thread> CreateThreadsVector(std::string fileName)
{
    auto start = chrono::high_resolution_clock::now();

    auto customers = parseTextFileVector(fileName);
    int size = customers.size();

    auto stop = chrono::high_resolution_clock::now();
    auto microSecondsElapsed = chrono::duration_cast<chrono::microseconds>(stop - start);
    double miliseconds = microSecondsElapsed.count() / 1000.00;
    cout << "[CreateThreadsVector] parsing file run took in miliseconds:  " << miliseconds << " ms" << endl;
    cout << fileName << " had " << (size * 2) << " lines resulting in " << size << " customers\n";


    //Split the work into 4 threads
    int workload = size / 4;
    int range1 = workload;
    int range2 = workload * 2;
    int range3 = workload * 3;
    int range4 = size;

    vector <thread> raceThreads;

    //putting an "&" in the enclosing body of a lamba expression, means "we're capturing all local variables by reference"
    //Then in order to pass a pointer, need to de-reference that reference?? - FALSE wording
    // "dereference means using the * operator to get the value behind a pointer"
    //Then in order to pass a pointer, use the "the addressof operator" & to get the memory address of the value - CORRECT wording 
    raceThreads.push_back(thread([&] { calculateRange(&customers, 0, range1, 0); }));
    raceThreads.push_back(thread([&] { calculateRange(&customers, range1, range2, 1); }));
    raceThreads.push_back(thread([&] { calculateRange(&customers, range2, range3, 2); }));
    raceThreads.push_back(thread([&] { calculateRange(&customers, range3, range4, 3); }));

    return raceThreads;
}


//Idk why but array lamba reference seems to be falling out of scope, so make global...Wtf
int globalWorkLoad;
int globalRange1;
int globalRange2;
int globalRange3;
int globalRange4;


vector<thread> CreateThreadsArray(std::string fileName)
{
    auto start = chrono::high_resolution_clock::now();

    //our out-parameter
    size_t size;
    auto customers = parseTextFileArray(fileName, &size);

    auto stop = chrono::high_resolution_clock::now();
    auto microSecondsElapsed = chrono::duration_cast<chrono::microseconds>(stop - start);
    double miliseconds = microSecondsElapsed.count() / 1000.00;
    cout << "[CreateThreadsArray] parsing file run took in miliseconds:  " << miliseconds << " ms" << endl;
    cout << fileName << " had " << (size * 2) << " lines resulting in " << size << " customers\n";


    //Split the work into 4 threads
    //int size = customers.size();
    //int workload = size / 4;
    //int range1 = workload;
    //int range2 = workload * 2;
    //int range3 = workload * 3;
    //int range4 = size;  
    // 
    //Idk why but array lamba reference seems to be falling out of scope, so make global...Wtf
    globalWorkLoad = size / 4;
    globalRange1 = globalWorkLoad;
    globalRange2 = globalWorkLoad * 2;
    globalRange3 = globalWorkLoad * 3;
    globalRange4 = size;


    vector <thread> raceThreads;

    //putting an "&" in the enclosing body of a lamba expression, means "we're capturing all local variables by reference"
    //Then in order to pass a pointer, need to de-reference that reference?? - FALSE wording
    // "dereference means using the * operator to get the value behind a pointer"
    //Then in order to pass a pointer, use the "the addressof operator" & to get the memory address of the value - CORRECT wording 
    raceThreads.push_back(thread([&] { calculateRange(&customers,   0, globalRange1, 0); }));
    raceThreads.push_back(thread([&] { calculateRange(&customers, globalRange1, globalRange2, 1); }));
    raceThreads.push_back(thread([&] { calculateRange(&customers, globalRange2, globalRange3, 2); }));
    raceThreads.push_back(thread([&] { calculateRange(&customers, globalRange3, globalRange4, 3); }));

    return raceThreads;
}

//******************************************************************************//

/*
Entries = 400,000
VECTOR TIME: 4597.44 ms
ARR TIME: 3984.78 ms
*/
void testParsingFile(std::string fileName)
{
    //VECTOR
    auto start1 = chrono::high_resolution_clock::now();
    parseTextFileVector(fileName);
    auto stop1 = chrono::high_resolution_clock::now();
    auto microSecondsElapsed1 = chrono::duration_cast<chrono::microseconds>(stop1 - start1);
    double miliseconds1 = microSecondsElapsed1.count() / 1000.00;
    cout << "VECTOR TIME: " << miliseconds1 << " ms" << endl;

    //PREMEM_ALLOC
    auto start2 = chrono::high_resolution_clock::now();
    size_t size = 0;
    parseTextFileArray(fileName, &size);
    auto stop2 = chrono::high_resolution_clock::now();
    auto microSecondsElapsed2 = chrono::duration_cast<chrono::microseconds>(stop2 - start2);
    double miliseconds2 = microSecondsElapsed2.count() / 1000.00;
    cout << "ARR TIME: " << miliseconds2 << " ms" << endl;
}


Customer* parseTextFileArray(std::string fileName, size_t* outSize)
{
    std::ifstream myfile;
    myfile.open(fileName);
    std::string mylineId;
    std::string mylineMoney;
    size_t customerCount = 0;
    //if (myfile.is_open())
    //{
    // while (myfile)
    // {
    // std::getline(myfile, mylineId);
    // if (mylineId == "")
    // break;
    // //I guess this is how to adv the line in cpp ?
    // myfile.tellg();
    // std::getline(myfile, mylineMoney);
    // //I guess this is how to adv the line in cpp ?
    // myfile.tellg();
    // double num_double = std::stod(mylineMoney);
    // ++customerCount;

    // if (customerCount % 3000 == 0)
    // cout << ".";
    // }
    //}

    while (getline(myfile, mylineId))
    {
        if (mylineId == "")
            break;
        ++customerCount;
        if (customerCount % 30000 == 0)
            cout << ".";
    }
    //we read everyline, but know an entry is 2 lines, so div/2 to get customer count
    customerCount /= 2;
    cout << endl;
    myfile.close();

    Customer* dynArr = new Customer[customerCount];
    customerCount = 0;
    myfile.open(fileName);
    if (myfile.is_open())
    {
        double dblMoney = 0;
        std::string dummy;
        while (std::getline(myfile, mylineId))
        {

            myfile >> dblMoney; //somehow C++ can convert it from string to double without a fuck ton of work on programmer part...? UNUSUAL
            //myfile.tellg(); //Whatever the fuck this is- its SLOW AS BALLS
            std::getline(myfile, dummy);  // remove \n
            //myfile.ignore(1000, '\n'); 
            dynArr[customerCount] = Customer{ mylineId, dblMoney };
            ++customerCount;

            if (customerCount % 30000 == 0)
                cout << ".";
        }
    }
    else
        cout << "couldn't find file: " + fileName << endl;

    *outSize = customerCount;
    //cout << "outSize=" << *outSize << "  customerCount=" << customerCount << endl;
    // clean up the memory you allocated when you're done with it
    //delete[] dynArr; 
    cout << "\nDone Reading File" << endl;
    return dynArr;

}

vector<Customer> parseTextFileVector(std::string fileName)
{
    vector<Customer> customers;

    std::ifstream myfile;
    myfile.open(fileName);
    std::string mylineId;
    std::string mylineMoney;
    if (myfile.is_open())
    {
        while (myfile)
        {
            std::getline(myfile, mylineId);
            if (mylineId == "")
                break;
            //I guess this is how to adv the line in cpp ?
            //myfile.tellg();
            std::getline(myfile, mylineMoney);
            //I guess this is how to adv the line in cpp ?
            //myfile.tellg();
            double num_double = std::stod(mylineMoney);
            customers.push_back(Customer{ mylineId, num_double });
            //cout << "Made Entry: " << mylineId << " , " << num_double << " \n";
            if (customers.size() % 30000 == 0)
                cout << ".";
        }
    }
    else
        cout << "couldn't find file: " + fileName << endl;

    myfile.close();

    cout << "\nDone Reading File" << endl;
    return customers;
}

//******************************************************************************//

void calculateRange(vector<Customer>* customers, int from, int to, int arrIndex)
{
    int localCount = 0;
    std::thread::id tid = this_thread::get_id();
    //this_thread::yield(); // im pretty sure this just yields 1 tick..?

    //Pass by reference via a pointer is working, all are sharing the same memory address
    //mx.lock();
    //cout << customThreadID << " MEM ADDR= " << customers << endl;
    //mx.unlock();

    for (int i = from; i < to; i++)
    {
        if ((*customers)[i].GetBalance() < 1000)
            //localCount++; // doing var ++ is slower, creates extra item in RAM
            ++localCount;
    }

    //MUTEX IS GARBAGE SLOW:
    //mx.lock(); //blocks other threads from using global var while we use it
    //counter += localCount;
    ////cout << "\nThread " << customThreadID <<" #"<< tid << " [from: " << from << " to: " << to<< "] calculated " << localCount << endl;
    //mx.unlock();

    _summations[arrIndex] = localCount;
}

void calculateRange(Customer** customers, int from, int to, int arrIndex)
{
    int localCount = 0;
    std::thread::id tid = this_thread::get_id();
    this_thread::yield();

    //Pass by reference via a pointer is working, all are sharing the same memory address
    //mx.lock();
    //cout << customThreadID << " MEM ADDR= " << customers << endl;
    //mx.unlock();
    
    //mx.lock();
    //cout << "Run Thread [" << arrIndex <<"] from : " << from << " to : " << to << " MEM ADDR= " << customers << endl;
    //mx.unlock();

    //NOTE: if running in RELEASE will error when trying to reference (*customers)[i] , but not in debug. No idea why???? fucking C++..

    for (int i = from; i < to; i++)
    {
        if ((*customers)[i].GetBalance() < 1000)
            //localCount++; // doing var ++ is slower, creates extra item in RAM
            ++localCount;
    }

    //MUTEX IS GARBAGE SLOW:

    //mx.lock(); //blocks other threads from using global var while we use it
    //counter += localCount;
    //++numThreadsFinished;
    ////cout << "\nThread " << customThreadID << " #" << tid << " [from: " << from << " to: " << to << "] calculated " << localCount << endl;
    //mx.unlock();

    _summations[arrIndex] = localCount;

    //Clean up our own array hack
    if (numThreadsFinished >= 4)
    {
        //If using Array have to clean up memory manually
        delete[] * customers;
    }
}


//******************************************************************************//
