#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "hanlp/mining/word/NewWordDiscover.h"

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No file path provided!" << std::endl;
        return -1;
    }
    std::string file_path = argv[1];
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: File cannot find!" << std::endl;
        return -1;
    }
    std::locale::global(std::locale(""));

    std::vector<std::wstring> docs;
    std::string line;
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convertor;
    while (std::getline(file, line)) {
        std::wstring wide_line = convertor.from_bytes(line);
        docs.push_back(wide_line);
    }
    file.close();

    assert(docs[0] == L"『三国演义/作者:罗贯中』\r");
    assert(docs[2] == L"正文\r");
    assert(docs[4] == L"第001回 宴桃园豪杰三结义 斩黄巾英雄首立功\r");

    NewWordDiscover discover(4, 0, .5, 100);

    std::vector<WordInfo> word_info_list = discover.discover(docs, 100);
    std::wcout << L"discover size: " << word_info_list.size() << std::endl;
    for (auto info : word_info_list) {
        std::wcout << info.text << L", ";
    }
    std::wcout << std::endl;
    return 0;
}
