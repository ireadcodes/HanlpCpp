#ifndef TFIDFCOUNTER_H
#define TFIDFCOUNTER_H
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class TfIdfCounter {
public:
    void add(const string& id, vector<string> words);
    map<string, map<string, double>> compute();
    set<string> documents();
    vector<pair<string, double>> getKeywordOf(const string &id, int size);

    map<string, map<string, double>> tfMap;
    map<string, map<string, double>> tfidfMap;
    map<string, double> idf;
};



#endif //TFIDFCOUNTER_H
