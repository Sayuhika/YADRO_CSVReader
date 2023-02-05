#pragma once
#include <memory>
#include <string>
#include <vector>

using namespace std;

class ExceptionsCollector
{
private:
	ExceptionsCollector();
	static ExceptionsCollector* instance;
	vector<string> exceptionsContainer;
public:
	static ExceptionsCollector* getInstance();
	void add(string message);
	void print();
	~ExceptionsCollector();
};