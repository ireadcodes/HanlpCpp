#ifndef NEWWORDDISCOVER_H
#define NEWWORDDISCOVER_H

#include <algorithm>
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
        return a->p > b->p;
    }
};

class NewWordDiscover {
public:
    NewWordDiscover(int max_word_len, int min_freq, double min_entropy, double min_aggregation)
        : max_word_len(max_word_len),
          min_freq(min_freq),
          min_entropy(min_entropy),
          min_aggregation(min_aggregation) {}

    std::vector<WordInfo> discover(const std::vector<std::wstring>& docs, unsigned int size) {
        std::unordered_map<std::wstring, std::shared_ptr<WordInfo>> word_candidates;
        int total_length = 0;
        const std::wregex delimiter(L"[\\s\\d,.<>/?:;'\"\\[\\]{}()|~!@#$%^&*\\-_=+，。《》、？：；“”‘’｛｝【】（）…￥！—┄－]+");
        for (const std::wstring& doc : docs) {
            // 原本算法用的是\u0000分隔字符串，但c++中替换后改变了doc_length，所以改用空格\u0020
            std::wstring modified_doc = std::regex_replace(doc, delimiter, L"\u0020");
            int doc_length = modified_doc.size();
            for (int i = 0; i < doc_length; i++) {
                int end = std::min(i + 1 + max_word_len, doc_length + 1);
                for (int j = i + 1; j < end; j++) {
                    std::wstring word = modified_doc.substr(i, j - i);
                    if (word.find(L'\u0020') != std::string::npos) {
                        continue;  // 含有分隔符的不认为是词语
                    }
                    if (word_candidates.find(word) == word_candidates.end()) {
                        // emplace直接调用WordInfo的构造函数，只要写构造参数
                        word_candidates.emplace(word, std::make_shared<WordInfo>(word));
                    }
                    // wordCandidates[word]需要WordInfo的无参构造函数
                    std::shared_ptr<WordInfo> info = word_candidates[word];
                    info->update(i == 0 ? L'\u0020' : modified_doc[i - 1], j < doc_length ? modified_doc[j] : L'\u0020');
                }
            }
            total_length += doc_length;
        }
        assert(word_candidates[L"『"]->text.compare(L"『") == 0);
        assert(word_candidates[L"『三"]->text.compare(L"『三") == 0);
        assert(word_candidates[L"『三国"]->text.compare(L"『三国") == 0);
        assert(word_candidates[L"『三国演"]->text.compare(L"『三国演") == 0);
        assert(word_candidates[L"三"]->text.compare(L"三") == 0);
        assert(word_candidates[L"三国"]->text.compare(L"三国") == 0);
        assert(word_candidates[L"三国演"]->text.compare(L"三国演") == 0);
        assert(word_candidates[L"三国演义"]->text.compare(L"三国演义") == 0);

        // 计算信息熵
        for (auto& pair : word_candidates) {
            pair.second->computeProbobilityEntropy(total_length);
        }
        // 计算互信息
        for (auto& pair : word_candidates) {
            pair.second->computeAggregation(word_candidates);
        }

        // 过滤
        auto condition = [&](const std::shared_ptr<WordInfo>& info) {
            return info->text.size() < 2 || info->p < min_freq || info->entropy < min_entropy || info->aggregation < min_aggregation;
        };
        std::vector<std::shared_ptr<WordInfo>> word_info_list;
        for (const auto& pair : word_candidates) {
            if (condition(pair.second)) {
                word_info_list.push_back(pair.second);
            }
        }

        // 使用优先队列得到前size个元素
        std::priority_queue<std::shared_ptr<WordInfo>, std::vector<std::shared_ptr<WordInfo>>, CompareWordInfo> top_n;
        for (const auto& info : word_info_list) {
            top_n.push(info);
            // 保持堆大小为 size
            if (top_n.size() > size) {
                top_n.pop();
            }
        }

        // 获取结果
        std::vector<WordInfo> result;
        while (!top_n.empty()) {
            result.push_back(*top_n.top());
            top_n.pop();
        }
        std::reverse(result.begin(), result.end());
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
};

#endif  // NEWWORDDISCOVER_H
