#ifndef TFIDF_H
#define TFIDF_H
#include <map>
#include <string>
#include <vector>

using namespace std;

class TfIdf {
public:
    static map<string, double> tfNatural(vector<string> &document);

    static map<string, double> idfFromTfs(vector<map<string, double> > &tfs);

    static map<string, double> idf(vector<vector<string> > &documentVocabularies, bool smooth, bool addOne);

    static map<string, double> tfidf(const map<string, double> &tf, map<string, double> &idf);
};


#endif //TFIDF_H
