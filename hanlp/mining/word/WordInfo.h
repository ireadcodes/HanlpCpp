#ifndef WORDINFO_H
#define WORDINFO_H

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>

class WordInfo {
public:
    explicit WordInfo(const std::wstring& text) : text(text) {
        p = 0;
        frequency = 0;
        entropy = 0;
        left_entropy = 0;
        right_entropy = 0;
        aggregation = std::numeric_limits<double>::max();
    }

    void update(const wchar_t left_char, const wchar_t right_char) {
        ++frequency;
        increaseFrequency(left_char, this->left);
        increaseFrequency(right_char, this->right);
    }

    void increaseFrequency(const wchar_t c, std::unordered_map<wchar_t, std::array<int, 1> >& storage) {
        auto it = storage.find(c);
        if (it == storage.end()) {
            storage[c] = std::array<int, 1>{1};
        } else {
            ++(it->second[0]);
        }
    }

    double computeEntropy(const std::unordered_map<wchar_t, std::array<int, 1> >& storage) {
        double sum = 0.0;
        for (const auto& entry : storage) {
            const std::array<int, 1>& values = entry.second;
            // 相邻词的频数除以本词频数
            double pp = values[0] / static_cast<double>(frequency);
            // 计算信息熵
            sum -= pp * log(pp);
        }
        return sum;
    }

    void computeProbobilityEntropy(const int length) {
        if (text.compare(L"三国") == 0) {
            assert(left[L'为'][0] == 1);
            assert(left[L' '][0] == 1);
            assert(right[L'演'][0] == 1);
            assert(right[L' '][0] == 1);
        }
        p = static_cast<double>(frequency) / static_cast<double>(length);
        left_entropy = computeEntropy(left);
        right_entropy = computeEntropy(right);
        entropy = std::min(left_entropy, right_entropy);
    }

    void computeAggregation(const std::unordered_map<std::wstring, std::shared_ptr<WordInfo>>& word_cands) {
        // 单个字
        if (text.size() == 1) {
            aggregation = std::sqrt(p);
            return;
        }
        // 多个字
        for (std::size_t i = 1; i < text.size(); i++) {
            std::wstring left_text = text.substr(0, i);
            std::wstring right_text = text.substr(i);
            aggregation = std::min(aggregation, p / word_cands.at(left_text)->p / word_cands.at(right_text)->p);
        }
    }

public:
    // 词语，UTF-8字符串用wstring
    std::wstring text;
    // 词出现的频率
    double p;
    // 互信息
    double aggregation;
    // 信息熵
    double entropy;

private:
    // 左邻接字集合，键是相邻字符，值是[频数]，为啥频率要用数组表示而不是整数型？
    std::unordered_map<wchar_t, std::array<int, 1> > left;
    // 右邻接字集合
    std::unordered_map<wchar_t, std::array<int, 1> > right;
    // 词频
    int frequency;

    double left_entropy;
    double right_entropy;
};

#endif  // WORDINFO_H
