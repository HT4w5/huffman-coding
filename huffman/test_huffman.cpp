#define HUFFMAN_DEBUG
#include <gtest/gtest.h>
#include <cstddef>
#include <deque>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include "huffman.hpp"

// Helper function to compare two deque<bool>
bool compareDequeBool(const std::deque<bool>& a, const std::deque<bool>& b) {
    if (a.size() != b.size())
        return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

// Helper function to compare two deque<char>
bool compareDequeChar(const std::deque<char>& a, const std::deque<char>& b) {
    if (a.size() != b.size())
        return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

// Test for HuffmanTree constructor with empty string
TEST(HuffmanTreeTest, ConstructorEmptyString) {
    EXPECT_THROW(HuffmanTree tree(""), std::invalid_argument);
}

// Test for HuffmanTree constructor with single character string
TEST(HuffmanTreeTest, ConstructorSingleCharString) {
    EXPECT_THROW(HuffmanTree tree("a"), std::invalid_argument);
}

// Test for HuffmanTree constructor with multiple character string
TEST(HuffmanTreeTest, ConstructorMultipleCharString1) {
    HuffmanTree tree("aaabbbb");
    EXPECT_TRUE(compareDequeChar(tree.getLeaves(), std::deque<char>{'a', 'b'}));
    EXPECT_FALSE(tree.getTreeBits().empty());
}

// Test for HuffmanTree constructor with multiple character string
/* This tree:
 *                 *
 *                / \
 *               *   D
 *              / \
 *             C   *
 *                / \
 *               A   B
 * Ref: https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1224/assignments/a9/
 */
TEST(HuffmanTreeTest, ConstructorMultipleCharString2) {
    HuffmanTree tree("AABBBCCCCDDDDDDDDDD");
    EXPECT_TRUE(compareDequeChar(tree.getLeaves(), std::deque<char>{'C', 'A', 'B', 'D'}));
    EXPECT_FALSE(tree.getTreeBits().empty());
}

// Test for HuffmanTree isLeaf and getChar methods
TEST(HuffmanTreeTest, IsLeafAndGetChar) {
    HuffmanTree tree("AABBBCCCCDDDDDDDDDD");
    EXPECT_FALSE(tree.isLeaf());
    tree.descend(0);
    EXPECT_FALSE(tree.isLeaf());
    tree.descend(0);
    EXPECT_TRUE(tree.isLeaf());
    EXPECT_EQ(tree.getChar(), 'C');
    tree.ascend();
    tree.descend(1);
    EXPECT_FALSE(tree.isLeaf());
    tree.descend(1);
    EXPECT_TRUE(tree.isLeaf());
    EXPECT_EQ(tree.getChar(), 'B');
}

// Test for HuffmanTree reset method
TEST(HuffmanTreeTest, Reset) {
    HuffmanTree tree("AABBBCCCCDDDDDDDDDD");
    tree.descend(0);
    tree.descend(0);
    tree.reset();
    EXPECT_FALSE(tree.isLeaf());
    tree.descend(1);
    EXPECT_TRUE(tree.isLeaf());
    EXPECT_EQ(tree.getChar(), 'D');
}

// Can compress a sample string.
// Ref: https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1224/assignments/a9/
TEST(HuffmanEncoderTest, EncodeSampleString) {
    HuffmanEncoder he("ABANANAABANDANA");
    std::deque<bool> treeBits = {1, 1, 1, 0, 0, 0, 0};
    std::deque<char> leaves = {'D', 'B', 'N', 'A'};
    std::deque<bool> content = {1, 0, 0, 1, 1, 0, 1, 1, 0,
                                1, 1, 1, 0, 0, 1, 1, 0, 1,
                                0, 0, 0, 1, 0, 1, 1};

    EXPECT_EQ(he.result().getTreeBits(), treeBits);
    EXPECT_EQ(he.result().getLeaves(), leaves);
    EXPECT_EQ(he.result().getContent(), content);
}

// Can decompress a small sample file.
// Ref: https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1224/assignments/a9/
TEST(HuffmanDecoderTest, DecodeSampleHuffmanFile) {
    HuffmanFile hf({{1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0},
                    {'u', 'k', 'p', 'n', 'a', 'm', 'h'},
                    {1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1,
                     0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
                     0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0}});

    HuffmanDecoder hd(hf);

    EXPECT_EQ(hd.result(), "humuhumunukunukuapuaa");
}

// Test for HuffmanEncoder and HuffmanDecoder with complex string
TEST(HuffmanEncoderDecoderTest, EncodeDecodeComplexString) {
    std::string content = "this is a test string for huffman encoding and decoding";
    HuffmanEncoder encoder(content);
    HuffmanFile file = encoder.result();
    HuffmanDecoder decoder(file);
    EXPECT_EQ(decoder.result(), content);
}

// Test for HuffmanEncoder and HuffmanDecoder with unique characters
TEST(HuffmanEncoderDecoderTest, EncodeDecodeUniqueChars) {
    std::string content = "abcdef";
    HuffmanEncoder encoder(content);
    HuffmanFile file = encoder.result();
    HuffmanDecoder decoder(file);
    EXPECT_EQ(decoder.result(), content);
}