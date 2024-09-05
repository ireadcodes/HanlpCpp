#ifndef TFIDF_H
#define TFIDF_H

#include "hanlp/global.h"

class TfIdf {
public:
    static unordered_map<string, double> tfNatural(const vector<string>& document) {
        unordered_map<string, double> tf_temp;
        for (const string& term : document) {
            tf_temp[term]++;
        }
        return tf_temp;
    }

    static unordered_map<string, double> idfFromTfs(const vector<unordered_map<string, double>>& tfs) {
        vector<vector<string>> key_set;
        for (const unordered_map<string, double>& tf: tfs) {
            vector<string> temp;
            for (const auto& kv : tf) {
                temp.push_back(kv.first);
            }
            key_set.push_back(temp);
        }
        return idf(key_set, true, true);
    }

    static unordered_map<string, double> idf(const vector<vector<string>>& document_vocabs, bool smooth, bool add_one) {
        // 这里的df计算词汇在整个语料库中的词频，因为前面对单篇文章的词汇去重了，所以就得到了包含该词条的文档数量
        unordered_map<string, double> df;
        int d = smooth ? 1 : 0;
        int a = add_one ? 1 : 0;
        int n = d;
        for (const vector<string>& document_vocab : document_vocabs) {
            n++;
            for (const string& term : document_vocab) {
                int t = df.find(term) == df.end() ? d : df[term];
                df[term] = t + 1;
            }
        }

        unordered_map<string, double> idf;
        for (const auto& [term, f] : df) {
            // 计算idf值，n代表文档数量，f代表包含该词条的文档数量
            idf[term] = log((double)n / f) + a;
        }
        return idf;
    }

    static unordered_map<string, double> tfidf(const unordered_map<string, double>& tf, const unordered_map<string, double>& idf) {
        unordered_map<string, double> tf_idf;
        for (const auto& [term, tf_val] : tf) {
            double idf_value = idf.count(term) ? idf.at(term) : 1;
            double tf_value = tf_val > 0 ? tf_val : 1;
            // TF-IDF=TF*IDF
            tf_idf[term] = tf_value * idf_value;
        }
        return tf_idf;
    }
};

#endif // TFIDF_H
