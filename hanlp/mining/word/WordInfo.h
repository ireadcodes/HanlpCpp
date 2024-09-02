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
    WordInfo(const std::wstring& text) : text(text) {
        aggregation = std::numeric_limits<float>::max();
    }

    void update(wchar_t leftChar, wchar_t rightChar) {
        ++frequency;
        increaseFrequency(leftChar, this->left);
        increaseFrequency(rightChar, this->right);
    }

    void increaseFrequency(wchar_t c, std::unordered_map<wchar_t, std::array<int, 1> >& storage) {
        auto it = storage.find(c);
        if (it == storage.end()) {
            storage[c] = std::array<int, 1>{1};
        } else {
            ++(it->second[0]);
        }
    }

    float computeEntropy(const std::unordered_map<wchar_t, std::array<int, 1> >& storage) {
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
        left_entropy = computeEntropy(left);
        right_entropy = computeEntropy(right);
        entropy = std::min(left_entropy, right_entropy);
        if (text == L"三国") {
            assert(left[L'『'][0] == 1);
            assert(left[L'为'][0] == 1);
            assert(left[L'倾'][0] == 1);
            assert(left[L'分'][0] == 2);
            assert(left[L'此'][0] == 1);
            assert(left[L'\0'][0] == 1);
            assert(right[L'演'][0] == 1);
            assert(right[L'著'][0] == 1);
            assert(right[L'各'][0] == 1);
            assert(right[L'归'][0] == 1);
            assert(right[L'\0'][0] == 3);
            assert(entropy == 0.0);
        }
    }

    void computeAggregation(const std::unordered_map<std::wstring, std::shared_ptr<WordInfo>>& wordCands) {
        // 单个字
        if (text.length() == 1) {
            aggregation = std::sqrt(p);
            return;
        }
        // 多个字
        for (std::size_t i = 1; i < text.length(); i++) {
            std::wstring left_text = text.substr(0, i);
            std::wstring right_text = text.substr(i);
            aggregation = std::min(aggregation, p / wordCands.at(left_text)->p / wordCands.at(right_text)->p);
        }
    }

public:
    // 词语，UTF-8字符串用wstring
    std::wstring text;
    float p;
    // 互信息
    float aggregation;
    // 信息熵
    float entropy;

private:
    // 左邻接字集合，键是相邻字符，值是[频数]，为啥频率要用数组表示而不是整数型？
    std::unordered_map<wchar_t, std::array<int, 1> > left;
    // 右邻接字集合
    std::unordered_map<wchar_t, std::array<int, 1> > right;
    // 词频
    int frequency;

    float left_entropy;
    float right_entropy;
};

#endif  // WORDINFO_H
