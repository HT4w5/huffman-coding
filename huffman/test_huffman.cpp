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

// Test for HuffmanEncoder with multiple character string
TEST(HuffmanEncoderTest, EncodeMultipleCharString) {
    HuffmanEncoder encoder("ab");
    HuffmanFile result = encoder.result();
    std::deque<bool> expected{0, 1, 1};
    EXPECT_EQ(result.getContent(), expected);
}

// Test for HuffmanDecoder with empty HuffmanFile
TEST(HuffmanDecoderTest, DecodeEmptyHuffmanFile) {
    HuffmanFile file;
    HuffmanDecoder decoder(file);
    EXPECT_TRUE(decoder.result().empty());
}

// Test for HuffmanDecoder with single character HuffmanFile
TEST(HuffmanDecoderTest, DecodeSingleCharHuffmanFile) {
    HuffmanEncoder encoder("a");
    HuffmanFile file = encoder.result();
    HuffmanDecoder decoder(file);
    EXPECT_EQ(decoder.result(), "a");
}

// Test for HuffmanDecoder with multiple character HuffmanFile
TEST(HuffmanDecoderTest, DecodeMultipleCharHuffmanFile) {
    HuffmanEncoder encoder("abracadabra");
    HuffmanFile file = encoder.result();
    HuffmanDecoder decoder(file);
    EXPECT_EQ(decoder.result(), "abracadabra");
}

// Test for HuffmanEncoder and HuffmanDecoder with complex string
TEST(HuffmanEncoderDecoderTest, EncodeDecodeComplexString) {
    std::string content = "this is a test string for huffman encoding and decoding";
    HuffmanEncoder encoder(content);
    HuffmanFile file = encoder.result();
    HuffmanDecoder decoder(file);
    EXPECT_EQ(decoder.result(), content);
}

// Test for HuffmanEncoder and HuffmanDecoder with all identical characters
TEST(HuffmanEncoderDecoderTest, EncodeDecodeIdenticalChars) {
    std::string content = "aaaaaaaaaa";
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

// Test for HuffmanEncoder and HuffmanDecoder with edge case string
TEST(HuffmanEncoderDecoderTest, EncodeDecodeEdgeCase) {
    std::string content = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f";
    HuffmanEncoder encoder(content);
    HuffmanFile file = encoder.result();
    HuffmanDecoder decoder(file);
    EXPECT_EQ(decoder.result(), content);
}