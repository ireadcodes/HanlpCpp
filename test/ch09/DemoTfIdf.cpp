#include "hanlp/mining/word/TfIdfCounter.h"

int main()
{
    TfIdfCounter counter;
    // 女排北京奥运会夺冠
    counter.add("《女排夺冠》", {"女排", "北京", "奥运会", "夺冠"});
    // 北京奥运会的羽毛球男单决赛
    counter.add("《羽毛球男单》", {"北京", "奥运会", "羽毛球", "男单", "决赛"});
    // 中国队女排夺北京奥运会金牌重返巅峰，观众欢呼女排女排女排！
    counter.add("《女排》", {"中国队","女排", "夺", "北京", "奥运会", "金牌", "重返", "巅峰", "观众", "欢呼", "女排", "女排", "女排"});
    counter.compute();
    for (const string& id : counter.documents()) {
        vector<pair<string, double>> result = counter.getKeywordsOf(id, 3);
        cout << id << " : ";
        for (const pair<string, double>& entry : result) {
            cout << "(" << entry.first << "=" << entry.second << ") ";
        }
        cout << endl;
    }

    // 测试语料库之外的文档：奥运会反兴奋剂
    vector<pair<string, double>> res = counter.getKeywordsWithTfidf({"奥运会", "反", "兴奋剂"}, 3);
    for (const pair<string, double>& entry : res) {
        cout << "(" << entry.first << "=" << entry.second << ") ";
    }
    cout << endl;
    return 0;
}
