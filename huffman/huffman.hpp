#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP
// #define HUFFMAN_DEBUG

#include <cstddef>
#include <deque>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

class HuffmanFile {
public:
    std::deque<bool> treeBits;
    std::deque<char> leaves;
    std::deque<bool> content;

    std::size_t size() const;
};

class HuffmanTree {
public:
    HuffmanTree(const std::string& content);
    HuffmanTree(const HuffmanFile& file);

    std::deque<bool> getTreeBits();
    std::deque<char> getLeaves();

    void reset();
    bool isLeaf();
    char getChar();
    bool descend(bool direction);
    void ascend();

private:
    struct TreeNode {
        char ch;
        std::shared_ptr<TreeNode> zero;
        std::shared_ptr<TreeNode> one;
    };

    std::deque<bool> treeBits;
    std::deque<char> leaves;

    std::shared_ptr<TreeNode> treePtr;  // Root node of encoding tree.
    std::shared_ptr<TreeNode> ptr;      // Current node.

    std::deque<std::shared_ptr<TreeNode>> tStack;  // Stack to record previous nodes during traversal.

    void generateTree(const std::string& content);
    void encodeTree(const std::shared_ptr<TreeNode> treePtr, std::deque<bool>& treeBits, std::deque<char>& leaves) const;
    std::shared_ptr<TreeNode> decodeTree(std::deque<bool>& treeBits, std::deque<char>& leaves) const;
};

class HuffmanEncoder {
public:
    HuffmanFile result() const;
    HuffmanEncoder(const std::string& content);

private:
    HuffmanTree tree;
    HuffmanFile res;

    std::deque<bool> encodeString(const std::string& content);
    void buildCodeMap(std::vector<bool>& code, std::unordered_map<char, std::vector<bool>>& codeMap);
};

class HuffmanDecoder {
public:
    std::string result() const;
    HuffmanDecoder(const HuffmanFile& file);

private:
    HuffmanTree tree;
    std::string res;

    void decodeString(std::deque<bool> content);
};


#endif