#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string highlight(const std::string& text, std::vector<std::string> keyword_list) {
    std::stringstream result;
    int index = 0;
    while (index < text.size()) {
        bool found = false;
        for (std::string keyword : keyword_list) {
            if (text.compare(index, keyword.size(), keyword) == 0) {
                result << "<span style='color:red;'>" << keyword << "</span>";
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
    std::string text = "天气预报说明天出太阳";
    std::vector<std::string> keyword_list = { "明天", "太阳" };
    std::string result = highlight(text, keyword_list);
    std::cout << result << std::endl;
    return 0;
}
