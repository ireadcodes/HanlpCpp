#include <iostream>

#include "mining/word/TfIdfCounter.h"

int main() {
    TfIdfCounter counter;
    // 女排北京奥运会夺冠
    vector<string> v1 = {"女排", "北京", "奥运会", "夺冠"};
    counter.add("《女排夺冠》", v1);
    // 北京奥运会的羽毛球男单决赛
    vector<string> v2 = {"北京", "奥运会", "羽毛球", "男单", "决赛"};
    counter.add("《羽毛球男单》", v2);
    // 中国队女排夺北京奥运会金牌重返巅峰，观众欢呼女排女排女排！
    vector<string> v3 = {"中国队","女排", "夺", "北京", "奥运会", "金牌", "重返", "巅峰", "观众", "欢呼", "女排", "女排", "女排"};
    counter.add("《女排》", v3);
    counter.compute();
    for (const auto & id : counter.documents()) {
        vector<pair<string, double>> keywords = counter.getKeywordOf(id, 3);
        cout << id << ": ";
        for (const auto & keyword : keywords) {
            cout << keyword.first << "=" << keyword.second << " ";
        }
        cout << endl;
    }
    return 0;
}
