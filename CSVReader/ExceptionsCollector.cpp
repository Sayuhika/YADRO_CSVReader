#include "ExceptionsCollector.h"
#include <iostream>

using namespace std;

ExceptionsCollector* ExceptionsCollector::instance;

ExceptionsCollector::ExceptionsCollector() {};

ExceptionsCollector* ExceptionsCollector::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ExceptionsCollector;
    }

    return instance;
};

void ExceptionsCollector::add(string message)
{
    exceptionsContainer.push_back(message);
};

void ExceptionsCollector::print() 
{
    if (exceptionsContainer.empty()) 
    {
        return;
    }

    cout << endl;

    for (int i = 0; i < exceptionsContainer.size(); i++)
    {
        cout << endl << i << ") " << exceptionsContainer[i];
    }
};

ExceptionsCollector::~ExceptionsCollector() {};