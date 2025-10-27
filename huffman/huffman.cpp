#include "huffman.hpp"
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

/////////////////
// HuffmanTree //
/////////////////

// Create tree from content string.
HuffmanTree::HuffmanTree(const std::string& content) {
    generateTree(content);
    encodeTree(treePtr, treeBits, leaves);
    // Initialize traverse pointer.
    ptr = treePtr;
}

// Create tree from encoded file.
HuffmanTree::HuffmanTree(const HuffmanFile& file) {
    treeBits = file.treeBits;
    leaves = file.leaves;
    std::deque tmpTreeBits(treeBits);
    std::deque tmpLeaves(leaves);
    treePtr = decodeTree(tmpTreeBits, tmpLeaves);
    // Initialize traverse pointer.
    ptr = treePtr;
}

// Reset tree state.
void HuffmanTree::reset() {
    ptr = treePtr;
    tStack.clear();
}

bool HuffmanTree::isLeaf() const {
    return ptr->one == nullptr && ptr->zero == nullptr;
}

char HuffmanTree::getChar() const {
    return ptr->ch;
}

/**
 * @brief Travel down encoding tree in the specified direction.
 * @param direction False for zero sub-tree. True for one sub-tree.
 * @return On success, returns true. On failure (nullptr), returns false.
 */
bool HuffmanTree::descend(bool direction) {
    if (direction) {
        // One.
        if (!ptr->one) {
            return false;
        } else {
            tStack.push_back(ptr);
            ptr = ptr->one;
        }
    } else {
        // Zero.
        if (!ptr->zero) {
            return false;
        } else {
            tStack.push_back(ptr);
            ptr = ptr->zero;
        }
    }
    return true;
}

void HuffmanTree::ascend() {
    ptr = tStack.back();
    tStack.pop_back();
}

void HuffmanTree::generateTree(const std::string& content) {
    // Package TreeNode with its priority (frequency of appearance).
    struct PriorityTreeNode {
        std::shared_ptr<TreeNode> node;
        int priority;
    };

    // Compare struct with () operator for std::priority_queue.
    // Use > to make priority queue see lower value as higher priority.
    struct CompareNodes {
        bool operator()(const PriorityTreeNode a, const PriorityTreeNode b) {
            return a.priority > b.priority;
        }
    };

    std::priority_queue<PriorityTreeNode, std::vector<PriorityTreeNode>, CompareNodes> treePQ;

    // Map all characters to their frequencies of appearance.
    std::unordered_map<char, int>
        charFreqMap;

    for (const char c : content) {
        charFreqMap[c] += 1;
    }

    // Check whether there are more than only one character.
    if (charFreqMap.size() < 2) {
        throw std::invalid_argument("Sole character input not allowed!");
    }

    // Enqueue all characters as simple trees (leaves).
    for (const auto& pair : charFreqMap) {
        std::shared_ptr<TreeNode> temp = std::make_shared<TreeNode>();
        temp->ch = pair.first;
        temp->zero = nullptr;
        temp->one = nullptr;
        treePQ.push(PriorityTreeNode{temp, pair.second});
    }

    // Build tree.
    while (treePQ.size() != 1) {
        std::shared_ptr<TreeNode> newTree = std::make_shared<TreeNode>();
        PriorityTreeNode temp;
        int newPriority = 0;
        // Attach zero sub-tree.
        temp = treePQ.top();
        treePQ.pop();
        newTree->zero = temp.node;
        newPriority += temp.priority;

        // Attach one sub-tree.
        temp = treePQ.top();
        treePQ.pop();
        newTree->one = temp.node;
        newPriority += temp.priority;

        // Enqueue new tree.
        treePQ.push(PriorityTreeNode{newTree, newPriority});
    }

    treePtr = treePQ.top().node;
}

void HuffmanTree::encodeTree(const std::shared_ptr<HuffmanTree::TreeNode> treePtr, std::deque<bool>& treeBits, std::deque<char>& leaves) {
    // Base case: tree is a leaf node.
    if (treePtr->zero == nullptr && treePtr->one == nullptr) {
        // Append current character to leaves.
        leaves.push_back(treePtr->ch);
        // Append 0 to bits.
        treeBits.push_back(0);
    } else {
        // Recursive case: not a leaf node, append encoding of zero and one sub-tree.
        // Append 1 to bits.
        treeBits.push_back(1);
        // Zero sub-tree.
        encodeTree(treePtr->zero, treeBits, leaves);
        // One sub-tree.
        encodeTree(treePtr->one, treeBits, leaves);
    }
}

std::shared_ptr<HuffmanTree::TreeNode> HuffmanTree::decodeTree(std::deque<bool>& treeBits, std::deque<char>& leaves) {
    std::shared_ptr<TreeNode> temp = std::make_shared<TreeNode>();
    // Base case: front bit in bits is zero (leaf node).
    if (treeBits.front() == 0) {
        // Dequeue 0 from bits.
        treeBits.pop_front();
        temp->ch = leaves.front();
        leaves.pop_front();
        temp->zero = nullptr;
        temp->one = nullptr;
    } else {  // Recursive case: not leaf.
        // Dequeue 1 from bits.
        treeBits.pop_front();
        // Append zero sub-tree.
        temp->zero = decodeTree(treeBits, leaves);
        temp->one = decodeTree(treeBits, leaves);
    }

    return temp;
}

std::deque<bool> HuffmanTree::getTreeBits() {
    return treeBits;
}
std::deque<char> HuffmanTree::getLeaves() {
    return leaves;
}

////////////////////
// HuffmanEncoder //
////////////////////

HuffmanEncoder::HuffmanEncoder(const std::string& content) : tree(content) {
    res.content = encodeString(content);
    res.treeBits = tree.getTreeBits();
    res.leaves = tree.getLeaves();
}

HuffmanFile HuffmanEncoder::result() const {
    return res;
}

std::deque<bool> HuffmanEncoder::encodeString(const std::string& content) {
    std::deque<bool> res;
    std::unordered_map<char, std::vector<bool>> codeMap;
    std::vector<bool> code;
    buildCodeMap(code, codeMap);

    for (const char c : content) {
        for (const bool b : codeMap[c]) {
            res.push_back(b);
        }
    }

    return res;
}

void HuffmanEncoder::buildCodeMap(std::vector<bool>& code, std::unordered_map<char, std::vector<bool>>& codeMap) {
    // Base case: is leaf.
    if (tree.isLeaf()) {
        codeMap.insert({tree.getChar(), code});
    } else {
        if (tree.descend(0)) {
            code.push_back(0);
            buildCodeMap(code, codeMap);
            // Backtrack.
            code.pop_back();
            tree.ascend();
        }

        if (tree.descend(1)) {
            code.push_back(1);
            buildCodeMap(code, codeMap);
            // Backtrack.
            code.pop_back();
            tree.ascend();
        }
    }
}

////////////////////
// HuffmanDecoder //
////////////////////

HuffmanDecoder::HuffmanDecoder(const HuffmanFile& file) : tree(file) {
    std::deque<bool> tempContent(file.content);
    decodeString(tempContent);
}

std::string HuffmanDecoder::result() const {
    return res;
}

void HuffmanDecoder::decodeString(std::deque<bool> content) {
    while (!content.empty()) {
        tree.descend(content.front());
        content.pop_front();
        // Check for leaf.
        if (tree.isLeaf()) {
            res += tree.getChar();
            tree.reset();
        }
    }
}

/////////////////
// HuffmanFile //
/////////////////

HuffmanFile::HuffmanFile() {}

HuffmanFile::HuffmanFile(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);

    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    // Check magic bytes.
    char magic[4];
    ifs.read(magic, 4);
    if (std::string(magic, 4) != "HUFF") {
        throw std::runtime_error("Invalid file format: missing HUFF magic header");
    }

    // Read metadata.
    uint32_t treeBitsSize, leafCount, contentSize;
    ifs.read(reinterpret_cast<char*>(&treeBitsSize), sizeof(treeBitsSize));
    ifs.read(reinterpret_cast<char*>(&leafCount), sizeof(leafCount));
    ifs.read(reinterpret_cast<char*>(&contentSize), sizeof(contentSize));

    // Read treeBits.
    std::size_t treeBytes = (treeBitsSize + 7) / 8;
    std::vector<uint8_t> treeBytesBuffer(treeBytes);
    ifs.read(reinterpret_cast<char*>(treeBytesBuffer.data()), treeBytes);
    treeBits = unpackBits(treeBytesBuffer, treeBitsSize);

    // Read leaves.
    for (uint32_t i = 0; i < leafCount; ++i) {
        char c;
        ifs.read(&c, 1);
        leaves.push_back(c);
    }

    // Read content bits.
    std::size_t contentBytes = (contentSize + 7) / 8;
    std::vector<uint8_t> contentBuffer(contentBytes);
    ifs.read(reinterpret_cast<char*>(contentBuffer.data()), contentBytes);
    content = unpackBits(contentBuffer, contentSize);

    ifs.close();
}

std::deque<bool> HuffmanFile::unpackBits(const std::vector<uint8_t>& bytes, std::size_t bitCount) {
    std::deque<bool> bits;
    for (std::size_t i = 0; i < bitCount; ++i) {
        std::size_t byteIndex = i / 8;
        std::size_t bitOffset = 7 - (i % 8);
        bool bit = (bytes[byteIndex] >> bitOffset) & 1;
        bits.push_back(bit);
    }
    return bits;
}

void HuffmanFile::write(const std::string& path) {
    std::ofstream ofs(path, std::ios::out | std::ios::binary);

    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    // Write magic bytes "HUFF".
    ofs.write("HUFF", 4);
    // Write size info.
    uint32_t treeBitsSize = treeBits.size();  // In bits.
    uint32_t leafCount = leaves.size();       // In bytes.
    uint32_t contentSize = content.size();    // In bits.

    ofs.write(reinterpret_cast<const char*>(&treeBitsSize), sizeof(treeBitsSize));
    ofs.write(reinterpret_cast<const char*>(&leafCount), sizeof(leafCount));
    ofs.write(reinterpret_cast<const char*>(&contentSize), sizeof(contentSize));

    // Write tree data and content data.
    // Pack treeBits into bytes.

    for (std::size_t i = 0; i < treeBits.size(); i += 8) {
        uint8_t byte = 0;
        for (int b = 0; b < 8 && i + b < treeBits.size(); ++b) {
            if (treeBits[i + b]) {
                byte |= (1 << (7 - b));
            }
        }
        ofs.put(byte);
    }

    // Write leaves.
    for (const char c : leaves) {
        ofs.put(c);
    }

    // Pack content bits into bytes
    for (size_t i = 0; i < content.size(); i += 8) {
        uint8_t byte = 0;
        for (int b = 0; b < 8 && i + b < content.size(); ++b) {
            if (content[i + b])
                byte |= (1 << (7 - b));
        }
        ofs.put(byte);
    }

    ofs.close();
}

// Return size of actual file in bytes.
std::size_t HuffmanFile::size() const {
    std::size_t size = 4;

    // treeBits.
    size += (treeBits.size() + 7) / 8;
    // leaves.
    size += leaves.size();
    // content.
    size += (content.size() + 7) / 8;

    return size;
}

#ifdef HUFFMAN_DEBUG
std::deque<bool> HuffmanFile::getTreeBits() {
    return treeBits;
}
std::deque<char> HuffmanFile::getLeaves() {
    return leaves;
}
std::deque<bool> HuffmanFile::getContent() {
    return content;
}

HuffmanFile::HuffmanFile(std::deque<bool> treeBits,
                         std::deque<char> leaves,
                         std::deque<bool> content) {
    this->treeBits = treeBits;
    this->leaves = leaves;
    this->content = content;
}
#endif