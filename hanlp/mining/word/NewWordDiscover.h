#ifndef NEWWORDDISCOVER_H
#define NEWWORDDISCOVER_H

#include <algorithm>
#include <cassert>
#include <memory>
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "hanlp/mining/word/WordInfo.h"

struct CompareWordInfo {
    bool operator()(const std::shared_ptr<WordInfo>& a, const std::shared_ptr<WordInfo>& b) const {
        // 按照p值从大到小排序
        return a->p < b->p;
    }
};

class NewWordDiscover {
public:
    NewWordDiscover(int max_word_len, int min_freq, double min_entropy, double min_aggregation)
        : max_word_len(max_word_len),
          min_freq(min_freq),
          min_entropy(min_entropy),
          min_aggregation(min_aggregation),
          seperator(L'\u0020') {}

    std::vector<WordInfo> discover(const std::vector<std::wstring>& docs, unsigned int size) {
        std::unordered_map<std::wstring, std::shared_ptr<WordInfo>> word_candidates;
        int total_length = 0;
        for (const std::wstring& doc : docs) {
            int doc_length = doc.size();
            for (int i = 0; i < doc_length; i++) {
                int end = std::min(i + 1 + max_word_len, doc_length + 1);
                for (int j = i + 1; j < end; j++) {
                    const std::wstring& word = doc.substr(i, j - i);
                    if (word.find(seperator) != std::string::npos) {
                        continue;  // 含有分隔符的不认为是词语
                    }
                    if (word_candidates.find(word) == word_candidates.end()) {
                        // emplace直接调用WordInfo的构造函数，只要写构造参数
                        word_candidates.emplace(word, std::make_shared<WordInfo>(word));
                    }
                    // wordCandidates[word]需要WordInfo的无参构造函数
                    std::shared_ptr<WordInfo> info = word_candidates[word];
                    info->update(i == 0 ? seperator : doc[i - 1], j < doc_length ? doc[j] : seperator);
                }
            }
            total_length += doc_length;
        }
        assert(word_candidates[L"三"]->text.compare(L"三") == 0);
        assert(word_candidates[L"三国"]->text.compare(L"三国") == 0);
        assert(word_candidates[L"三国演"]->text.compare(L"三国演") == 0);
        assert(word_candidates[L"三国演义"]->text.compare(L"三国演义") == 0);
        assert(word_candidates[L"国"]->text.compare(L"国") == 0);
        assert(word_candidates[L"国演"]->text.compare(L"国演") == 0);
        assert(word_candidates[L"国演义"]->text.compare(L"国演义") == 0);
        assert(word_candidates[L"演"]->text.compare(L"演") == 0);

        // 计算信息熵
        for (auto& pair : word_candidates) {
            pair.second->computeProbobilityEntropy(total_length);
        }
        // 计算互信息
        for (auto& pair : word_candidates) {
            pair.second->computeAggregation(word_candidates);
        }

        // 过滤
        auto remove_condition = [&](const std::shared_ptr<WordInfo>& info) {
            return info->text.size() < 2 || info->p < min_freq || info->entropy < min_entropy || info->aggregation < min_aggregation;
        };

        // 使用优先队列进行排序
        std::priority_queue<std::shared_ptr<WordInfo>, std::vector<std::shared_ptr<WordInfo>>, CompareWordInfo> top_n;
        for (const auto& pair : word_candidates) {
            if (!remove_condition(pair.second)) {
                top_n.push(pair.second);
            }
        }

        // 获取结果
        std::vector<WordInfo> result;
        while (!top_n.empty() && result.size() < size) {
            result.push_back(*top_n.top());
            top_n.pop();
        }
        return result;
    }

private:
    // 词语最长长度
    int max_word_len;
    // 词语最低频率
    int min_freq;
    // 词语最低信息熵
    double min_entropy;
    // 词语最低互信息
    double min_aggregation;
    // 原本算法用的是\u0000分隔字符串，但c++中\u0000表示字符串结尾，会截断字符串，所以改用空格\u0020
    wchar_t seperator;
};

#endif  // NEWWORDDISCOVER_H
