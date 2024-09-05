#include "hanlp/global.h"

class Node {
public:
    Node(const string& value) : _value(value) {}

    // 添加子节点到父节点
    Node* addChild(wchar_t char_key, const std::string& value, bool overwrite = false) {
        Node* child = _children[char_key];
        if (child == nullptr) {
            child = new Node(value);
            _children[char_key] = child;
        } else if (overwrite) {
            child->_value = value;
        }
        return child;
    }

    optional<Node*> getChild(wchar_t char_key) {
        if (_children.count(char_key)) {
            return _children[char_key];
        }
        return nullopt;
    }

    string getValue() const {
        return _value;
    }

    void setValue(const string& value) {
        _value = value;
    }

private:
    unordered_map<wchar_t, Node*> _children;
    string _value;
};

class Trie : public Node {
public:
    Trie() : Node("") {}

    void set(const string& key, const string& value) {
        Node* state = this;
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        wstring wide_key = converter.from_bytes(key);
        for (size_t i = 0; i < wide_key.size(); i++) {
            wchar_t char_key = wide_key[i];
            // 判断是否是最后一个字
            if (i < wide_key.size() - 1) {
                state = state->addChild(char_key, "", false);
            } else {
                state = state->addChild(char_key, value, true);
            }
        }
    }

    optional<string> get(const string& key) {
        Node* state = this;
        wstring_convert<codecvt_utf8<wchar_t>> converter;
        wstring wide_key = converter.from_bytes(key);
        for (wchar_t char_key : wide_key) {
            auto child = state->getChild(char_key);
            if (!child.has_value()) {
                return nullopt;
            }
            state = child.value();
        }
        return state->getValue();
    }

    bool contains(const std::string& key) {
        optional<string> res = get(key);
        if ("" == res) {
            return false;
        }
        return res.has_value();
    }
};

int main() {
    Trie trie;

    // 增
    trie.set("自然", "nature");
    trie.set("自然人", "human");
    trie.set("自然语言", "language");
    trie.set("自语", "talk to oneself");
    trie.set("入门", "introduction");

    // 查
    assert(trie.contains("自然"));

    // 删
    trie.set("自然", "");
    assert(!trie.contains("自然"));

    // 改
    trie.set("自然语言", "human language");
    assert(trie.get("自然语言").value() == "human language");

    // 查
    assert(trie.get("入门").value() == "introduction");

    std::cout << "All assertions passed!" << std::endl;
    return 0;
}
