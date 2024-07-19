#include "TfIdf.h"

#include <cmath>
#include <iterator>

map<string, double> TfIdf::tfNatural(vector<string> &document) {
    map<string, double> tf;
    for (const auto &term: document) {
        double f = tf[term];
        tf[term] = f + 1.0;
    }
    return tf;
}

map<string, double> TfIdf::idfFromTfs(vector<map<string, double> > &tfs) {
    vector<vector<string> > keySet;
    for (const auto & tf : tfs) {
        vector<string> temp;
        for (const auto & item : tf) {
            temp.push_back(item.first);
        }
        keySet.push_back(temp);
    }
    return idf(keySet, true, true);
}

map<string, double> TfIdf::idf(vector<vector<string> > &documentVocabularies, bool smooth, bool addOne) {
    // 这里的df是计算词语在整个语料库中的词频
    map<string, int> df;
    int d = smooth ? 1 : 0;
    int a = addOne ? 1 : 0;
    // n代表文档数量
    int n = d;
    for (const auto &documentVocabulary: documentVocabularies) {
        n += 1;
        for (const auto & term : documentVocabulary) {
            int t = df[term];
            if (t == 0) t = d;
            df[term] = t + 1;
        }
    }
    map<string, double> idf;
    for (const auto & e : df) {
        string term = e.first;
        double f = e.second;
        // 计算idf值
        idf[term] = std::log(n / f) + a;
    }
    return idf;
}

map<string, double> TfIdf::tfidf(const map<string, double> &tf, map<string, double> &idf) {
    map<string, double> tfIdf;
    for (const auto & pair : tf) {
        const string & term = pair.first;
        double tfVal = pair.second;
        if (tfVal == 0) tfVal = 1.0;
        double idfVal = idf[term];
        if (idfVal == 0) idfVal = 1.0;
        // TF-IDF=TF*IDF
        tfIdf[term] = tfVal * idfVal;
    }
    return tfIdf;
}
