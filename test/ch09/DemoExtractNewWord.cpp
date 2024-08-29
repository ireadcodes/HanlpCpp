#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "hanlp/mining/word/NewWordDiscover.h"

int main() {
    std::vector<std::string> docs;
    std::ifstream file("./data/三国演义.txt");
    if (!file.is_open()) {
        std::cerr << "File cannot find!" << std::endl;
        return -1;
    }
    std::string line;
    while (std::getline(file, line)) {
        docs.push_back(line);
    }
    file.close();

    assert(docs[0] == "『三国演义/作者:罗贯中』\r");
    assert(docs[1] == "\r");
    assert(docs[2] == "正文\r");

    NewWordDiscover discover(4, 0, .5, 100);
    std::vector<WordInfo> wordINfoList = discover.discover(docs, 100);
    for(const auto& info : wordINfoList) {
        std::cout << info.text << ", ";
    }
    std::cout << "\n";
    return 0;
}
