#ifndef FONCTIONSAIDE_H
#define FONCTIONSAIDE_H

#include <string>
using namespace std;

void clearInputBuffer();
bool checkQuit(const string& input);
bool readIntOrQuit(int& value, int min, int max, const string& prompt);
bool readStringOrQuit(string& value, const string& prompt);


#endif