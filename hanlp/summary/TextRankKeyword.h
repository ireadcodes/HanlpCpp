#ifndef TEXTRANKKEYWORD_H
#define TEXTRANKKEYWORD_H

#include <cmath>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>


double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

class TextRankKeyword {
public:
    TextRankKeyword() : d(0.85), max_iter(200), min_diff(0.001) {}

    std::vector<std::pair<std::string, double>> getKeywords(const std::vector<std::string>& term_list, int size) {
        std::unordered_map<std::string, double> term_map = getTermAndRank(term_list);
        std::vector<std::pair<std::string, double>> sort_vec(term_map.begin(), term_map.end());
        std::sort(sort_vec.begin(), sort_vec.end(), [](const std::pair<std::string, double>& a, const std::pair<std::string, double>& b){
            return a.second > b.second;
        });
        std::vector<std::pair<std::string, double>> result(sort_vec.begin(), sort_vec.begin() + size);
        return result;
    }

    std::unordered_map<std::string, double> getTermAndRank(const std::vector<std::string>& word_list) {
        // words存储着单词到邻居的映射
        std::unordered_map<std::string, std::unordered_set<std::string>> words;
        std::deque<std::string> que;
        for (const std::string& w : word_list) {
            // 保证窗口中只有5个词
            if (que.size() >= 5) {
                que.pop_front();
            }
            for (const std::string& q_word : que) {
                if (w == q_word) {
                    continue;
                }
                // 既然是邻居,那么关系是相互的
                words[w].insert(q_word);
                words[q_word].insert(w);
            }
            // 将当前单词加入队列
            que.push_back(w);
        }

        // 初始化每个节点的权重
        std::unordered_map<std::string, double> score;
        for (const auto& [key, value] : words) {
            score[key] = sigmoid(value.size());
        }

        for (int i = 0; i < max_iter; ++i) {
            std::unordered_map<std::string, double> m;
            double max_diff = 0;

            for (const auto& [key, value] : words) {
                m[key] = 1 - d;

                // element 是 key 这个词的邻居
                for (const auto& element : value) {
                    std::size_t size = words[element].size();
                    if (key == element || size == 0) {
                        continue;
                    }
                    // 重点：中心词(key)的权重与邻居词的链接总数(size)成反比，与邻居词的权重(score[element])成正比
                    m[key] += d / size * score[element];
                }
                max_diff = std::max(max_diff, std::abs(m[key] - score[key]));
            }

            // 更新权重
            score = m;

            // 算法终止条件
            if (max_diff <= min_diff) {
                break;
            }
        }
        return score;
    }

private:
    // 阻尼系数(DampingFactor)，一般取值为0.85
    double d;
    // 最大迭代次数
    int max_iter;
    double min_diff;
};

#endif  // TEXTRANKKEYWORD_H