#ifndef TFIDFCOUNTER_H
#define TFIDFCOUNTER_H

#include "hanlp/mining/word/TfIdf.h"

class TfIdfCounter {
public:
    TfIdfCounter() {}

    void add(const string& id, const vector<string>& words) {
        // 这里是计算每篇文章的中的词频
        unordered_map<string, double> tf = TfIdf::tfNatural(words);
        // 这里使用tf_map的键存储词条相当于对同一篇文章中的相同词汇去重了
        tf_map[id] = tf;
        idf.clear();
    }

    void compute() {
        vector<unordered_map<string, double>> tf_map_values;
        for (const auto& entry : tf_map) {
            tf_map_values.push_back(entry.second);
        }
        idf = TfIdf::idfFromTfs(tf_map_values);
        for (const auto& [k, v] : tf_map) {
            unordered_map<string, double> tfidf = TfIdf::tfidf(v, idf);
            tfidf_map[k] = tfidf;
        }
    }

    unordered_set<string> documents() {
        unordered_set<string> doc_set;
        for (const auto& entry : tf_map) {
            doc_set.insert(entry.first);
        }
        return doc_set;
    }

    vector<pair<string, double>> topN(const unordered_map<string, double>& tfidfs, int size) {
        using Pair = pair<string, double>;
        auto compare = [](const Pair& a, const Pair& b) {
            return a.second < b.second;
        };
        priority_queue<Pair, vector<Pair>, decltype(compare)> pq(compare);
        for (const auto& entry : tfidfs) {
            pq.push(entry);
        }
        vector<Pair> result;
        while (!pq.empty() && result.size() < size) {
            result.push_back(pq.top());
            pq.pop();
        }
        return result;
    }

    vector<pair<string, double>> getKeywordsOf(const string& id, int size) {
        unordered_map<string, double> tfidfs = tfidf_map[id];
        return topN(tfidfs, size);
    }

    vector<pair<string, double>> getKeywordsWithTfidf(const vector<string>& document, int size) {
        if (idf.empty()) {
            compute();
        }
        unordered_map<string, double> tf_map = TfIdf::tfNatural(document);
        unordered_map<string, double> tfidfs = TfIdf::tfidf(tf_map, idf);
        return topN(tfidfs, size);
    }

private:
    unordered_map<string, unordered_map<string, double>> tf_map;
    unordered_map<string, unordered_map<string, double>> tfidf_map;
    unordered_map<string, double> idf;
};

#endif  // TFIDFCOUNTER_H