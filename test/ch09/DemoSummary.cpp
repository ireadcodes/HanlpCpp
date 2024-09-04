#include <iostream>
#include <string>
#include <vector>
#include "hanlp/summary/TextRankSentence.h"

/*
水利部水资源司司长陈明忠9月29日在国务院新闻办举行的新闻发布会上透露，
根据刚刚完成了水资源管理制度的考核，有部分省接近了红线的指标，有部分省超过红线的指标。
对一些超过红线的地方，陈明忠表示，对一些取用水项目进行区域的限批，严格地进行水资源论证和取水许可的批准。
*/
int main(int argc, char const *argv[]) {
    std::vector<std::string> sentence_list = {
        "水利部水资源司司长陈明忠9月29日在国务院新闻办举行的新闻发布会上透露",
        "根据刚刚完成了水资源管理制度的考核",
        "有部分省接近了红线的指标",
        "有部分省超过红线的指标",
        "对一些超过红线的地方",
        "陈明忠表示",
        "对一些取用水项目进行区域的限批",
        "严格地进行水资源论证和取水许可的批准",
    };
    // 省略了分词过程
    std::vector<std::vector<std::string>> docs = {
        {"水利部", "水资源", "司", "司长", "陈明忠", "9月", "国务院", "新闻办", "举行", "新闻", "发布会", "透露"},
        {"刚刚", "完成", "水资源", "管理", "制度", "考核"},
        {"部分", "省", "接近", "红线", "指标"},
        {"部分", "省", "超过", "红线", "指标"},
        {"超过", "红线", "地方"},
        {"陈明忠", "表示"},
        {"取", "用水", "项目", "进行", "区域", "限"},
        {"严格", "进行", "水资源", "论证", "取水", "许可", "批准"}
    };
    TextRankSentence text_rank(docs);
    std::vector<int> top_sentence = text_rank.getTopSentence(3);
    for (int i : top_sentence) {
        std::cout << sentence_list[i] << std::endl;
    }
    return 0;
}
