#ifndef ARCHIVER_BINARY_TRIE_H
#define ARCHIVER_BINARY_TRIE_H

#include "char_code.h"

#include <cstdint>
#include <cstddef>
#include <vector>
#include <queue>
#include <set>

class BinaryTrie;

class BinaryTrie {
public:
    class TrieNode {
    public:
        uint16_t character{};
        size_t count{};
        TrieNode* nodes[2]{};

        TrieNode();

        TrieNode(uint16_t character, size_t count);

        ~TrieNode();
    };

    class TrieNodeComp {
    public:
        bool operator()(TrieNode* a, TrieNode* b) const;
    };

public:
    void AddChar(uint16_t character, size_t count);

    void BuildByCharCounts(size_t* char_count, uint16_t char_count_size);

    const TrieNode* GetRoot() const;

    void GetCodesLen(std::vector<CharCode>& codes);

    void BuildByHuffmanCodes(const std::vector<CharCode>& codes);

    uint16_t GetChar(const std::vector<bool>& code) const;

    static void MoveNodeByBit(const TrieNode*& cur_node, bool bit);

    ~BinaryTrie();

private:
    void Dfs(std::vector<CharCode>& codes, const TrieNode* cur_node, uint16_t cur_code_len);

private:
    TrieNode* root_node_;
};

#endif  // ARCHIVER_BINARY_TRIE_H
