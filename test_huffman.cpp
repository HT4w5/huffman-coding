#include <gtest/gtest.h>
#include "huffman.hpp"

// Test Fixture for HuffmanDecoder
class HuffmanDecoderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup test content and tree bits for a simple HuffmanTree
        testContent = "abbccc";
        treeBits = {1, 0, 0, 1, 0, 1, 0};
        leaves = {'a', 'b', 'c'};
        huffmanFile = {testContentBits, treeBits, leaves};
        decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    }

    void encodeStringToBits(const std::string& str, std::deque<bool>& bits) {
        HuffmanTree tree(str);
        HuffmanEncoder encoder(str);
        bits = encoder.result().content;
    }

    std::string testContent;
    std::deque<bool> testContentBits;
    std::deque<bool> treeBits;
    std::deque<char> leaves;
    HuffmanFile huffmanFile;
    std::unique_ptr<HuffmanDecoder> decoder;
};

// Test Happy Path
TEST_F(HuffmanDecoderTest, DecodeSimpleString) {
    encodeStringToBits(testContent, testContentBits);
    huffmanFile.content = testContentBits;
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_EQ(decoder->result(), testContent);
}

// Test Edge Case: Empty Content
TEST_F(HuffmanDecoderTest, DecodeEmptyContent) {
    huffmanFile.content = {};
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_EQ(decoder->result(), "");
}

// Test Edge Case: Single Character Content
TEST_F(HuffmanDecoderTest, DecodeSingleCharacterContent) {
    testContent = "a";
    encodeStringToBits(testContent, testContentBits);
    treeBits = {0};
    leaves = {'a'};
    huffmanFile = {testContentBits, treeBits, leaves};
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_EQ(decoder->result(), testContent);
}

// Test Edge Case: All Characters Same
TEST_F(HuffmanDecoderTest, DecodeAllCharactersSame) {
    testContent = "aaaa";
    encodeStringToBits(testContent, testContentBits);
    treeBits = {0};
    leaves = {'a'};
    huffmanFile = {testContentBits, treeBits, leaves};
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_EQ(decoder->result(), testContent);
}

// Test Edge Case: Invalid Tree Bits
TEST_F(HuffmanDecoderTest, DecodeInvalidTreeBits) {
    huffmanFile.treeBits = {1, 0, 0, 1, 0, 1}; // Invalid tree bits
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_THROW(decoder->decodeString(testContentBits), std::runtime_error);
}

// Test Edge Case: Invalid Content Bits
TEST_F(HuffmanDecoderTest, DecodeInvalidContentBits) {
    huffmanFile.content = {1, 1, 1, 1}; // Invalid content bits
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_THROW(decoder->decodeString(huffmanFile.content), std::runtime_error);
}

// Test Edge Case: Mixed Case Content
TEST_F(HuffmanDecoderTest, DecodeMixedCaseContent) {
    testContent = "AbC";
    encodeStringToBits(testContent, testContentBits);
    treeBits = {1, 0, 0, 1, 0, 1, 0, 0};
    leaves = {'A', 'b', 'C'};
    huffmanFile = {testContentBits, treeBits, leaves};
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_EQ(decoder->result(), testContent);
}

// Test Edge Case: Content Bits Leading To Non-Leaf Node
TEST_F(HuffmanDecoderTest, DecodeNonLeafTraversal) {
    huffmanFile.content = {1, 0}; // Leads to a non-leaf node
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_THROW(decoder->decodeString(huffmanFile.content), std::runtime_error);
}

// Test Edge Case: Large Content
TEST_F(HuffmanDecoderTest, DecodeLargeContent) {
    testContent = std::string(1000, 'a') + std::string(500, 'b') + std::string(250, 'c');
    encodeStringToBits(testContent, testContentBits);
    treeBits = {1, 0, 0, 1, 0, 1, 0};
    leaves = {'a', 'b', 'c'};
    huffmanFile = {testContentBits, treeBits, leaves};
    decoder = std::make_unique<HuffmanDecoder>(huffmanFile);
    EXPECT_EQ(decoder->result(), testContent);
}