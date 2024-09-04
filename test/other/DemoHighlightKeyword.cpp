#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::wstring highlight(const std::wstring& text, std::vector<std::wstring> keyword_list) {
    std::wstringstream result;
    int index = 0;
    while (index < text.size()) {
        bool found = false;
        for (std::wstring keyword : keyword_list) {
            if (text.compare(index, keyword.size(), keyword) == 0) {
                result << L"<span style='color:red;'>" << keyword << L"</span>";
                index += keyword.size();
                found = true;
                break;
            }
        }
        if (!found) {
            result << text[index];
            index++;
        }
    }
    return result.str();
}

int main() {
    std::locale::global(std::locale(""));
    
    std::wstring text = L"天气预报说明天出太阳";
    std::vector<std::wstring> keyword_list = { L"明天", L"太阳" };
    std::wstring result = highlight(text, keyword_list);
    std::wcout << result << std::endl;
    return 0;
}
