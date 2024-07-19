#include "TfIdfCounter.h"

#include <algorithm>
#include <iterator>

#include "TfIdf.h"

void TfIdfCounter::add(const string &id, vector<string> words) {
    // 这里是计算每篇文章的中的词频
    auto tf = TfIdf::tfNatural(words);
    tfMap[id] = tf;
    idf.clear();
}

map<string, map<string, double>> TfIdfCounter::compute() {
    vector<map<string, double>> values;
    for (const auto & item : tfMap) {
        values.push_back(item.second);
    }
    idf = TfIdf::idfFromTfs(values);
    for (const auto & item : tfMap) {
        auto tfidf = TfIdf::tfidf(item.second, idf);
        tfidfMap[item.first] = tfidf;
    }
    return tfidfMap;
}

set<string> TfIdfCounter::documents() {
    set<string> keySet;
    for (const auto & item : tfMap) {
        keySet.insert(item.first);
    }
    return keySet;
}

vector<pair<string, double>> TfIdfCounter::getKeywordOf(const string &id, int size) {
    auto tfidfs = tfidfMap[id];
    return topN(tfidfs, size);
}

vector<pair<string, double>> TfIdfCounter::topN(map<string, double> tfidfs, int size) {
    vector<pair<string, double>> sortedVec;
    for (const auto & item : tfidfs) {
        sortedVec.emplace_back(item);
    }
    sort(sortedVec.begin(), sortedVec.end(), [](const pair<string, double> &a, const pair<string, double> &b){ return a.second > b.second; });
    vector<pair<string, double>> result(sortedVec.begin(), sortedVec.begin() + size);
    return result;
}

vector<pair<string, double>> TfIdfCounter::getKeywordsWithTfIdf(vector<string> document, int size) {
    if (idf.empty()) compute();
    auto tfidfs = TfIdf::tfidf(TfIdf::tfNatural(document), idf);
    return topN(tfidfs, size);
}
