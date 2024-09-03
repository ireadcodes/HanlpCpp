#include <cassert>
#include <codecvt>
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
        // 删除换行符
        const std::size_t last = line.find_last_not_of('\r');
        std::wstring wide_line = convertor.from_bytes(line.substr(0, last + 1));
        docs.push_back(wide_line);
    }
    file.close();

    assert(docs[0] == L"『三国演义/作者:罗贯中』");
    assert(docs[2] == L"正文");
    assert(docs[4] == L"第001回 宴桃园豪杰三结义 斩黄巾英雄首立功");

    NewWordDiscover new_word_discover(4, 0, .5, 100);

    std::vector<WordInfo> word_info_list = new_word_discover.discover(docs, 100);
    for (const auto& info : word_info_list) {
        std::wcout << info.text << L": " << info.p << std::endl;
    }
    return 0;
}
