#ifndef WORDINFO_H
#define WORDINFO_H

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <string>
#include <unordered_map>

class WordInfo {
public:
    WordInfo() : WordInfo("") {}

    WordInfo(const std::string& text) : text(text) {
        aggregation = std::numeric_limits<float>::max();
    }

    void update(char leftChar, char rightChar) {
        ++frequency;
        increaseFrequency(leftChar, this->left);
        increaseFrequency(rightChar, this->right);
    }

    void increaseFrequency(char c, std::unordered_map<char, std::array<int, 1> >& storage) {
        auto it = storage.find(c);
        if (it == storage.end()) {
            storage[c] = std::array<int, 1>{1};
        } else {
            ++(it->second[0]);
        }
    }

    float computeEntropy(const std::unordered_map<char, std::array<int, 1> >& storage) {
        float sum = 0.0F;
        for (const auto& entry : storage) {
            const std::array<int, 1>& values = entry.second;
            // 相邻词的频数除以本词频数
            float pp = values[0] / (float)frequency;
            // 计算信息熵
            sum -= pp * log(pp);
        }
        return sum;
    }

    void computeProbobilityEntropy(int length) {
        p = frequency / length;
        leftEntropy = computeEntropy(left);
        rightEntropy = computeEntropy(right);
        entropy = std::min(leftEntropy, rightEntropy);
    }

    void computeAggregation(const std::unordered_map<std::string, WordInfo>& wordCands) {
        // 单个字
        if (text.length() == 1) {
            aggregation = std::sqrt(p);
            return;
        }
        // 多个字
        for (size_t i = 0; i < text.length(); i++) {
            std::string leftText = text.substr(0, i);
            std::string rightText = text.substr(i);

            if (wordCands.find(leftText) != wordCands.end() && wordCands.find(rightText) != wordCands.end()) {
                aggregation = std::min(aggregation, p / wordCands.at(leftText).p / wordCands.at(rightText).p);
            }
        }
    }

public:
    // 词语
   std::string text;
    float p;
    // 互信息
    float aggregation;
    // 信息熵
    float entropy;

private:
    // 左邻接字集合，键是相邻字符，值是[频数]，为啥频率要用数组表示而不是整数型？
    std::unordered_map<char, std::array<int, 1> > left;
    // 右邻接字集合
    std::unordered_map<char, std::array<int, 1> > right;
    // 词频
    int frequency;

    float leftEntropy;
    float rightEntropy;
};

#endif  // WORDINFO_H
