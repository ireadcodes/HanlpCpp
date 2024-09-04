#ifndef TEXTRANKSENTENCE_H
#define TEXTRANKSENTENCE_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
#include <memory>
#include "hanlp/summary/BM25.h"

class TextRankSentence {
public:
    TextRankSentence(const std::vector<std::vector<std::string>>& docs)
        : docs(docs) {
        d = 0.85;
        max_iter = 200;
        min_diff = 0.001;
        bm25 = std::make_unique<BM25>(docs);
        // 文档句子的个数
        D = docs.size();
        // 句子和其他句子的相关程度
        weight.resize(D, std::vector<double>(D, 0.0));
        // 该句子和其他句子相关程度之和
        weight_sum.resize(D, 0.0);
        // 迭代之后收敛的权重，下标表示句子序号，元素表示权重值
        vertex.resize(D, 0.0);
        solve();
    }

    void solve() {
        int cnt = 0;
        for (const std::vector<std::string>& sentence : docs) {
            std::vector<double> scores = bm25->simAll(sentence);
            weight[cnt] = scores;
            weight_sum[cnt] = std::accumulate(scores.begin(), scores.end(), -scores[cnt]);
            vertex[cnt] = 1.0;
            cnt++;
        }

        // TextRank核心公式计算
        for (int iter = 0; iter < max_iter; ++iter) {
            std::vector<double> m(D, 0.0);
            double max_diff = 0.0;
            for (int i = 0; i < D; ++i) {
                m[i] = 1 - d;
                for (int j = 0; j < D; ++j) {
                    if (j == i || weight_sum[j] == 0) {
                        continue;
                    }
                    m[i] += (d * weight[j][i] / weight_sum[j]) * vertex[j];
                }
                double diff = std::abs(m[i] - vertex[i]);
                max_diff = std::max(max_diff, diff);
            }
            vertex = m;
            if (max_diff < min_diff) {
                break;
            }
        }

        // 排序
        for (int i = 0; i < D; ++i) {
            top[vertex[i]] = i;
        }
    }

    std::vector<int> getTopSentence(int size) {
        std::vector<int> values;
        for (const auto& item : top) {
            values.push_back(item.second);
        }
        size = std::min(size, static_cast<int>(values.size()));
        return std::vector<int>(values.rbegin(), values.rbegin() + size);
    }

private:
    const std::vector<std::vector<std::string>>& docs;
    double d;
    int max_iter;
    double min_diff;
    int D;
    std::unique_ptr<BM25> bm25;
    std::vector<std::vector<double>> weight;
    std::vector<double> weight_sum;
    std::vector<double> vertex;
    // 排序后的最终结果 score -> index
    std::map<double, int> top;
};

#endif  // TEXTRANKSENTENCE_H