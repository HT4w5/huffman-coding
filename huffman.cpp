#include "huffman.hpp"
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Constructor for encoding mode.
 */
Huffman::Huffman(std::string content) {
    encodeMode = true;

    // Generate and store encoding tree from content.
    tree = huffmanTreeFor(content);
    this->content = content;
}

/**
 * Constructor for decoding mode.
 */
Huffman::Huffman(HuffmanFile file) {
    encodeMode = false;

    // Decode and store encoding tree from content.
    tree = decodeTree(file.tree, file.leaves);
    this->file = file;
}

bool Huffman::isEncode() {
    return encodeMode;
}

/**
 * Destructor.
 */
Huffman::~Huffman() {
    deleteTree(tree);
}

Huffman::HuffmanFile Huffman::encode() {
    if (!encodeMode) {
        throw std::runtime_error("Huffman in decode mode!");
    }

    HuffmanFile f;

    // Encode content.
    f.content = encodeString(content, tree);

    // Encode tree.
    encodeTree(tree, f.tree, f.leaves);

    return f;
}

std::string Huffman::decode() {
    if (encodeMode) {
        throw std::runtime_error("Huffman in encode mode!");
    }

    std::string c = decodeString(file.content, tree);

    return c;
}

std::queue<bool> Huffman::encodeString(const std::string& content, const EncodingTreeNode* tree) const {
    std::queue<bool> res;
    std::unordered_map<char, std::vector<bool>> codeMap;
    std::vector<bool> code;
    buildCodeMap(tree, code, codeMap);

    for (const char c : content) {
        for (const bool b : codeMap[c]) {
            res.push(b);
        }
    }

    return res;
}

std::string Huffman::decodeString(std::queue<bool> content, const EncodingTreeNode* tree) const {
    std::string res;
    const EncodingTreeNode* head = tree;
    while (!content.empty()) {
        if (content.front() == 0) {
            head = head->zero;
        } else {
            head = head->one;
        }
        content.pop();
        // Check for leaf.
        if (head->zero == nullptr && head->one == nullptr) {
            res += head->ch;
            head = tree;
        }
    }

    return res;
}

void Huffman::buildCodeMap(const EncodingTreeNode* tree, std::vector<bool>& code, std::unordered_map<char, std::vector<bool>>& codeMap) const {
    // Base case: is leaf.
    if (tree->zero == nullptr && tree->one == nullptr) {
        codeMap.insert({tree->ch, code});
    } else {
        if (tree->zero != nullptr) {
            code.push_back(0);
            buildCodeMap(tree->zero, code, codeMap);
            // Backtrack.
            code.pop_back();
        }
        if (tree->one != nullptr) {
            code.push_back(1);
            buildCodeMap(tree->one, code, codeMap);
            // Backtrack.
            code.pop_back();
        }
    }
}

Huffman::EncodingTreeNode* Huffman::huffmanTreeFor(const std::string& content) const {
    // Package EncodingTreeNode with its priority (frequency of appearance).
    struct PriorityTreeNode {
        EncodingTreeNode* node;
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
        EncodingTreeNode* temp = new EncodingTreeNode;
        temp->ch = pair.first;
        temp->zero = nullptr;
        temp->one = nullptr;
        treePQ.push({temp, pair.second});
    }

    // Build tree.
    while (treePQ.size() != 1) {
        EncodingTreeNode* newTree = new EncodingTreeNode;
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
        treePQ.push({newTree, newPriority});
    }

    return treePQ.top().node;
}

void Huffman::encodeTree(const EncodingTreeNode* tree,
                         std::queue<bool>& bits,
                         std::queue<char>& leaves) const {
    // Base case: tree is a leaf node.
    if (tree->zero == nullptr && tree->one == nullptr) {
        // Append current character to leaves.
        leaves.push(tree->ch);
        // Append 0 to bits.
        bits.push(0);
    } else {
        // Recursive case: not a leaf node, append encoding of zero and one sub-tree.
        // Append 1 to bits.
        bits.push(1);
        // Zero sub-tree.
        encodeTree(tree->zero, bits, leaves);
        // One sub-tree.
        encodeTree(tree->one, bits, leaves);
    }
}

/**
 * Decode tree from tree bits and tree leaves queue. Destructive to bits and leaves parameters.
 */
Huffman::EncodingTreeNode* Huffman::decodeTree(std::queue<bool>& bits, std::queue<char>& leaves) const {
    EncodingTreeNode* temp = new EncodingTreeNode;
    // Base case: front bit in bits is zero (leaf node).
    if (bits.front() == 0) {
        // Dequeue 0 from bits.
        bits.pop();
        temp->ch = leaves.front();
        leaves.pop();
        temp->zero = nullptr;
        temp->one = nullptr;
    } else {  // Recursive case: not leaf.
        // Dequeue 1 from bits.
        bits.pop();
        // Append zero sub-tree.
        temp->zero = decodeTree(bits, leaves);
        temp->one = decodeTree(bits, leaves);
    }

    return temp;
}

void Huffman::deleteTree(EncodingTreeNode* tree) const {
    // Base case: tree is nullptr.
    if (tree == nullptr) {
        // Do nothing.
    } else {
        deleteTree(tree->zero);
        deleteTree(tree->one);
        delete tree;
    }
}

HuffmanTree::HuffmanTree(const std::string& content) {
    generateTree(content);
    encodeTree(treePtr, treeBits, leaves);
}

HuffmanTree::HuffmanTree(const HuffmanFile& file) {
    treeBits = file.treeBits;
    leaves = file.leaves;
    std::deque tmpTreeBits(treeBits);
    std::deque tmpLeaves(leaves);
    decodeTree(tmpTreeBits, tmpLeaves);
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
        treePQ.push({temp, pair.second});
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
        treePQ.push({newTree, newPriority});
    }

    treePtr = treePQ.top().node;
}

void HuffmanTree::encodeTree(const std::shared_ptr<HuffmanTree::TreeNode> treePtr, std::deque<bool>& treeBits, std::deque<char>& leaves) const {
    // Clear current encoding.
    treeBits.clear();
    leaves.clear();
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

std::shared_ptr<HuffmanTree::TreeNode> HuffmanTree::decodeTree(std::deque<bool>& treeBits, std::deque<char>& leaves) const {
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