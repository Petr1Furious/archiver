#include "binary_trie.h"

BinaryTrie::TrieNode::TrieNode() {
    nodes[0] = nodes[1] = nullptr;
}

BinaryTrie::TrieNode::TrieNode(uint16_t character, size_t count) : character(character), count(count) {
    nodes[0] = nodes[1] = nullptr;
}

BinaryTrie::TrieNode::~TrieNode() {
    delete nodes[0];
    delete nodes[1];
}

bool BinaryTrie::TrieNodeComp::operator()(TrieNode* a, TrieNode* b) const {
    return a->count < b->count;
}

void BinaryTrie::AddChar(uint16_t character, size_t count) {
}

void BinaryTrie::BuildByCharCounts(size_t* char_count, uint16_t char_count_size) {
    std::multiset<TrieNode*, TrieNodeComp> nodes_queue;

    for (uint16_t i = 0; i < char_count_size; i++) {
        if (char_count[i] > 0) {
            nodes_queue.insert(new TrieNode(i, char_count[i]));
        }
    }

    while (nodes_queue.size() > 1) {
        TrieNode* a = *nodes_queue.begin();
        nodes_queue.erase(nodes_queue.begin());
        TrieNode* b = *nodes_queue.begin();
        nodes_queue.erase(nodes_queue.begin());

        auto* new_node = new TrieNode(0, a->count + b->count);
        new_node->nodes[0] = a;
        new_node->nodes[1] = b;
        nodes_queue.insert(new_node);
    }
    root_node_ = *nodes_queue.begin();
    nodes_queue.erase(nodes_queue.begin());
}

const BinaryTrie::TrieNode* BinaryTrie::GetRoot() const {
    return root_node_;
}

void BinaryTrie::GetCodesLen(std::vector<CharCode>& codes) {
    Dfs(codes, root_node_, 0);
}

void BinaryTrie::BuildByHuffmanCodes(const std::vector<CharCode>& codes) {
    root_node_ = new BinaryTrie::TrieNode();
    for (const CharCode& code: codes) {
        TrieNode* cur_node = root_node_;
        for (bool bit: code.code) {
            if (cur_node->nodes[bit] == nullptr) {
                cur_node->nodes[bit] = new BinaryTrie::TrieNode();
            }
            cur_node = cur_node->nodes[bit];
        }
        cur_node->character = code.character;
    }
}

uint16_t BinaryTrie::GetChar(const std::vector<bool>& code) const {
    const TrieNode* cur_node = root_node_;
    for (bool bit: code) {
        cur_node = cur_node->nodes[bit];
    }

    return cur_node->character;
}

void BinaryTrie::MoveNodeByBit(const BinaryTrie::TrieNode*& cur_node, bool bit) {
    cur_node = cur_node->nodes[bit];
}

BinaryTrie::~BinaryTrie() {
    delete root_node_;
}

void BinaryTrie::Dfs(std::vector<CharCode>& codes, const BinaryTrie::TrieNode* cur_node, uint16_t cur_code_len) {
    if (cur_node->nodes[0] == nullptr) {
        codes.push_back(CharCode{cur_node->character, std::vector<bool>(cur_code_len, false)});
        return;
    }

    for (const TrieNode* node: cur_node->nodes) {
        Dfs(codes, node, cur_code_len + 1);
    }
}
