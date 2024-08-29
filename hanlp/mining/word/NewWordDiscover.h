#ifndef NEWWORDDISCOVER_H
#define NEWWORDDISCOVER_H

#include <algorithm>
#include <queue>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "hanlp/mining/word/WordInfo.h"

struct CompareWordInfo {
    bool operator()(const WordInfo& a, const WordInfo& b) {
        // 按照p值从大到小排序
        return a.p < b.p;
    }
};

class NewWordDiscover {
public:
    NewWordDiscover(int maxWordLen, int minFreq, double minEntropy, double minAggregation)
        : maxWordLen(maxWordLen),
          minFreq(minFreq),
          minEntropy(minEntropy),
          minAggregation(minAggregation) {}

    std::vector<WordInfo> discover(const std::vector<std::string>& docs, unsigned int size) {
        std::unordered_map<std::string, WordInfo> wordCandidates;
        int totalLength = 0;
        std::regex delimiter("[\\s\\d,.<>/?:;'\"\\[\\]{}()|~!@#$%^&*\\-_=+，。《》、？：；“”‘’｛｝【】（）…￥！—┄－]+");
        for (const auto& doc : docs) {
            std::string modifiedDoc = std::regex_replace(doc, delimiter, "\0");
            int docLength = modifiedDoc.length();
            for (int i = 0; i < docLength; i++) {
                int end = std::min(i + 1 + maxWordLen, docLength + 1);
                for (int j = i + 1; j < end; j++) {
                    std::string word = modifiedDoc.substr(i, j - 1);
                    if (word.find('\0') != std::string::npos) {
                        continue;
                    }
                    if (wordCandidates.find(word) == wordCandidates.end()) {
                        // emplace直接调用WordInfo的构造函数，只要写构造参数
                        wordCandidates.emplace(word, word);
                    }
                    // wordCandidates[word]需要WordInfo的无参构造函数
                    WordInfo& info = wordCandidates[word];
                    info.update(i == 0 ? '\0' : doc[i - 1], j < docLength ? doc[j] : '\0');
                }
            }
            totalLength += docLength;
        }

        for (auto& pair : wordCandidates) {
            pair.second.computeProbobilityEntropy(totalLength);
        }
        for (auto& pair : wordCandidates) {
            pair.second.computeAggregation(wordCandidates);
        }

        // 过滤
        auto condition = [&](const WordInfo& info) {
            return info.text.size() < 2 || info.p < minFreq || info.entropy < minEntropy || info.aggregation < minAggregation;
        };
        std::vector<WordInfo> wordInfoList;
        for (const auto& pair : wordCandidates) {
            if (condition(pair.second)) {
                wordInfoList.push_back(pair.second);
            }
        }

        // 使用优先队列得到前size个元素
        std::priority_queue<WordInfo, std::vector<WordInfo>, CompareWordInfo> topN;
        for (const auto& info : wordInfoList) {
            topN.push(info);
            // 保持堆大小为 size
            if (topN.size() > size) {
                topN.pop();
            }
        }

        // 获取结果
        std::vector<WordInfo> result;
        while (!topN.empty()) {
            result.push_back(topN.top());
            topN.pop();
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

private:
    // 词语最长长度
    int maxWordLen;
    // 词语最低频率
    int minFreq;
    // 词语最低信息熵
    double minEntropy;
    // 词语最低互信息
    double minAggregation;
};

#endif  // NEWWORDDISCOVER_H
