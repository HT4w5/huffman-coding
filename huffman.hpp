#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP
// #define HUFFMAN_DEBUG

#include <queue>
#include <string>
#include <unordered_map>

/**
 * @brief Huffman encoder and decoder.
 */
class Huffman {
public:
    // Type declarations.

    /**
     * @brief Type representing a huffman encoded file.
     */
    struct HuffmanFile {
        std::queue<bool> tree;
        std::queue<char> leaves;
        std::queue<bool> content;
    };

#ifdef HUFFMAN_DEBUG
    /**
     * @brief Type representing a node in the encoding tree.
     */
    struct EncodingTreeNode {
        char ch;                 // Character stored here.
        EncodingTreeNode* zero;  // Child tree 0.
        EncodingTreeNode* one;   // Child tree 1.
    };
#endif

    // Constructors.

    Huffman(std::string content);
    Huffman(HuffmanFile file);

    // Destructor.
    ~Huffman();

    // Member functions.

    HuffmanFile encode();
    std::string decode();
    bool isEncode();
#ifdef HUFFMAN_DEBUG
    // Debug testing functions.
    friend void unitTest();
#endif

private:
#ifndef HUFFMAN_DEBUG
    /**
     * @brief Type representing a node in the encoding tree.
     */
    struct EncodingTreeNode {
        char ch;                 // Character stored here.
        EncodingTreeNode* zero;  // Child tree 0.
        EncodingTreeNode* one;   // Child tree 1.
    };
#endif

    EncodingTreeNode* tree;  // Encoding tree for current content.
    bool encodeMode;
    HuffmanFile file;
    std::string content;

    EncodingTreeNode* huffmanTreeFor(const std::string& content) const;

    std::queue<bool> encodeString(const std::string& content, const EncodingTreeNode* tree) const;

    std::string decodeString(const std::queue<bool> content, const EncodingTreeNode* tree) const;

    void encodeTree(const EncodingTreeNode* tree, std::queue<bool>& bits, std::queue<char>& leaves) const;

    EncodingTreeNode* decodeTree(std::queue<bool>& bits, std::queue<char>& leaves) const;
    void deleteTree(EncodingTreeNode* tree) const;

    void buildCodeMap(const EncodingTreeNode* tree, std::vector<bool>& code, std::unordered_map<char, std::vector<bool>>& codeMap) const;
};

#endif