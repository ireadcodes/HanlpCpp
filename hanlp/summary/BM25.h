#ifndef BM25_H
#define BM25_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <cassert>

class BM25 {
public:
    BM25(const std::vector<std::vector<std::string>>& docs) 
        : k1(1.5), b(0.75), docs(docs), D(docs.size()), avgdl(0) {

        // 计算文档句子的平均长度
        for (const auto& sentence : docs) {
            avgdl += sentence.size();
        }
        avgdl /= D;

        // 初始化频率和文档频率
        f.resize(D);
        for (std::size_t index = 0; index < D; ++index) {
            const auto& sentence = docs[index];
            std::unordered_map<std::string, int> tf;
            for (const auto& word : sentence) {
                tf[word]++;
            }
            f[index] = tf;

            for (const auto& [word, count] : tf) {
                df[word]++;
            }
        }

        // 计算IDF
        for (const auto& [word, freq] : df) {
            idf[word] = log(D - freq + 0.5) - log(freq + 0.5);
        }
    }

    double sim(const std::vector<std::string>& sentence, int index) {
        double score = 0.0;
        for (const auto& word : sentence) {
            if (f[index].find(word) == f[index].end()) {
                continue;
            }
            int d = docs[index].size();
            int tf = f[index][word];

            // BM25核心公式
            score += (idf[word] * tf * (k1 + 1) / (tf + k1 * (1 - b + b * d / avgdl)));
        }
        return score;
    }

    std::vector<double> simAll(const std::vector<std::string>& sentence) {
        std::vector<double> scores(D, 0.0);
        for (int i = 0; i < D; ++i) {
            scores[i] = sim(sentence, i);
        }
        return scores;
    }

private:
    double k1;
    double b;
    std::vector<std::vector<std::string>> docs;
    int D;
    double avgdl;
    std::vector<std::unordered_map<std::string, int>> f;
    std::unordered_map<std::string, int> df;
    std::unordered_map<std::string, double> idf;
};

#endif  // BM25_H