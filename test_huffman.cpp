#define HUFFMAN_DEBUG
#include <iostream>
#include <queue>
#include "assert.hpp"
#include "huffman.hpp"

template <typename T>
bool isEqualQueue(std::queue<T> a, std::queue<T> b) {
    std::queue<T> x(a);
    std::queue<T> y(b);

    if (x.size() != y.size()) {
        return false;
    }

    while (!x.empty()) {
        if (x.front() != y.front()) {
            return false;
        }
        x.pop();
        y.pop();
    }

    return true;
}

bool isEqualTrees(Huffman::EncodingTreeNode* t1, Huffman::EncodingTreeNode* t2) {
    // Empty.
    if (t1 == nullptr && t2 == nullptr) {
        return true;
    } else if (t1 == nullptr || t2 == nullptr) {
        return false;
    }
    // Leaf.
    if (t1->one == nullptr && t1->zero == nullptr &&
        t2->one == nullptr && t2->zero == nullptr) {
        if (t1->ch == t2->ch) {
            return true;
        } else {
            return false;
        }
    } else {  // Not leaf.
        return isEqualTrees(t1->zero, t2->zero) && isEqualTrees(t1->one, t2->one);
    }
}

void deleteTree(Huffman::EncodingTreeNode* tree) {
    // Base case: tree is nullptr.
    if (tree == nullptr) {
        // Do nothing.
    } else {
        deleteTree(tree->zero);
        deleteTree(tree->one);
        delete tree;
    }
}

void unitTest() {
    /**
     * Test 0: isEqualTrees.
     */
    printTitle("Running Test 0: isEqualTrees()");
    // 0.1 Equal trees.
    printSubTitle("0.1 Equal trees");
    {
        Huffman::EncodingTreeNode* tree = new Huffman::EncodingTreeNode{
            ' ', new Huffman::EncodingTreeNode{'a', nullptr, nullptr}, new Huffman::EncodingTreeNode{'b', nullptr, nullptr}};

        assertTrue("isEqualTrees returned false for equal trees!", isEqualTrees(tree, tree));

        deleteTree(tree);

        tree = new Huffman::EncodingTreeNode{
            ' ', nullptr, nullptr};

        assertTrue("isEqualTrees returned false for equal trees!", isEqualTrees(tree, tree));

        deleteTree(tree);

        tree = new Huffman::EncodingTreeNode{
            ' ', new Huffman::EncodingTreeNode{'a', nullptr, nullptr}, nullptr};

        assertTrue("isEqualTrees returned false for equal leaf-only trees!", isEqualTrees(tree, tree));

        deleteTree(tree);

        tree = nullptr;

        assertTrue("isEqualTrees returned false for equal nullptrs!", isEqualTrees(tree, tree));

        deleteTree(tree);
    }
    printSubTitle("0.1 Success!");

    printSubTitle("0.2 Inequal trees");
    {
        Huffman::EncodingTreeNode* abTree = new Huffman::EncodingTreeNode{
            ' ', new Huffman::EncodingTreeNode{'a', nullptr, nullptr}, new Huffman::EncodingTreeNode{'b', nullptr, nullptr}};

        Huffman::EncodingTreeNode* nullTree = nullptr;

        Huffman::EncodingTreeNode* leafTree = new Huffman::EncodingTreeNode{'a', nullptr, nullptr};

        assertFalse("abTree, nullTree", isEqualTrees(abTree, nullTree));
        assertFalse("abTree, leafTree", isEqualTrees(abTree, leafTree));
        assertFalse("nullTree, leafTree", isEqualTrees(nullTree, leafTree));

        deleteTree(abTree);
        deleteTree(leafTree);
    }
    printSubTitle("0.2 Success!");
    printTitle("Test 0 Success!");

    /**
     * Test 1: Can generate correct huffman tree.
     */

    printTitle("Running Test 1: huffmanTreeFor()");

    // 1.1 Error report.
    printSubTitle("1.1 Error report");
    {
        bool thrown = false;
        try {
            Huffman emptyHuff("");
        } catch (const std::invalid_argument& e) {
            thrown = true;
        }

        assertTrue("Didn't report error on empty string", thrown);
    }
    {
        bool thrown = false;
        try {
            Huffman oneCharHuff("e");
        } catch (const std::invalid_argument& e) {
            thrown = true;
        }
        try {
            Huffman oneCharHuff("eeee");
        } catch (const std::invalid_argument& e) {
            thrown &= true;
        }
        try {
            Huffman oneCharHuff("eeeeeeeeeeeeeeeeeeeeeee");
        } catch (const std::invalid_argument& e) {
            thrown &= true;
        }

        assertTrue("Didn't report error on single-character string", thrown);
    }
    printSubTitle("1.1 Success!");

    // 1.2 Builds tree for two characters.
    printSubTitle("1.2 Builds tree for two characters.");
    {
        Huffman::EncodingTreeNode* expected = new Huffman::EncodingTreeNode{
            ' ',
            new Huffman::EncodingTreeNode{'a', nullptr, nullptr},
            new Huffman::EncodingTreeNode{'b', nullptr, nullptr}};

        Huffman huff("aaabbbb");
        assertTrue("Couldn't build correct tree for ab", isEqualTrees(expected, huff.tree));
        deleteTree(expected);
    }
    printSubTitle("1.2 Success!");

    printSubTitle("1.3 Uses cumulative weights.");
    {
        Huffman::EncodingTreeNode* expected = new Huffman::EncodingTreeNode{
            '*',
            new Huffman::EncodingTreeNode{'!',
                                          new Huffman::EncodingTreeNode{'C', nullptr, nullptr},
                                          new Huffman::EncodingTreeNode{'?',
                                                                        new Huffman::EncodingTreeNode{'A', nullptr, nullptr},
                                                                        new Huffman::EncodingTreeNode{'B', nullptr, nullptr}}},
            new Huffman::EncodingTreeNode{'D', nullptr, nullptr}};

        Huffman huff("AABBBCCCCDDDDDDDDDD");
        assertTrue("Couldn't build correct tree for 1.3", isEqualTrees(expected, huff.tree));
        deleteTree(expected);
    }
    printSubTitle("1.3 Success!");

    printSubTitle("1.4 Uses cumulative weights.");
    {
        Huffman::EncodingTreeNode* expected = new Huffman::EncodingTreeNode{
            ' ',
            new Huffman::EncodingTreeNode{
                ' ',
                new Huffman::EncodingTreeNode{'D', nullptr, nullptr},
                new Huffman::EncodingTreeNode{'E', nullptr, nullptr}},
            new Huffman::EncodingTreeNode{
                ' ',
                new Huffman::EncodingTreeNode{'F', nullptr, nullptr},
                new Huffman::EncodingTreeNode{
                    ' ',
                    new Huffman::EncodingTreeNode{'C', nullptr, nullptr},
                    new Huffman::EncodingTreeNode{
                        ' ',
                        new Huffman::EncodingTreeNode{'A', nullptr, nullptr},
                        new Huffman::EncodingTreeNode{'B', nullptr, nullptr},
                    }}}};

        Huffman huff("AABBBCCCCDDDDDDEEEEEEEFFFFFFFF");
        assertTrue("Couldn't build correct tree for 1.4", isEqualTrees(expected, huff.tree));
        deleteTree(expected);
    }
    printSubTitle("1.4 Success!");

    printTitle("Running Test 2: encodeTree()");
    printSubTitle("2.1 Can encode an example tree");
    {
        Huffman huff("ABBCCCC");

        std::deque<bool> tmp1{1, 1, 0, 0, 0};
        std::deque<char> tmp2{'A', 'B', 'C'};

        std::queue<bool> expectedBits(tmp1);
        std::queue<char> expectedLeaves(tmp2);

        std::queue<bool> bits;
        std::queue<char> leaves;

        huff.encodeTree(huff.tree, bits, leaves);
        assertTrue("Couldn't encode an example tree!", isEqualQueue(bits, expectedBits) && isEqualQueue(leaves, expectedLeaves));
    }
    printSubTitle("2.1 Success!");

    printTitle("Running Test 3: encodeTree() and decodeTree()");
    printSubTitle("3.1 decodeTree undoes encodeTree");
    {
        Huffman huff("ABBCCCC");

        std::queue<bool> bits;
        std::queue<char> leaves;

        huff.encodeTree(huff.tree, bits, leaves);

        Huffman::EncodingTreeNode* resultTree = huff.decodeTree(bits, leaves);

        assertTrue("decodeTree couldnt undo encodeTree!", isEqualTrees(huff.tree, resultTree));

        deleteTree(resultTree);

        Huffman huff2("1145141919810");

        std::queue<bool> bits2;
        std::queue<char> leaves2;

        huff2.encodeTree(huff2.tree, bits2, leaves2);

        Huffman::EncodingTreeNode* resultTree2 = huff2.decodeTree(bits2, leaves2);

        assertTrue("decodeTree couldnt undo encodeTree!", isEqualTrees(huff2.tree, resultTree2));

        deleteTree(resultTree2);
    }
    printSubTitle("3.1 Success!");

    printTitle("Running Test 4: encode() and decode()");
    printSubTitle("4.1 decode() undoes encode()");
    {
        std::string expected = "1145141919810";

        Huffman en(expected);

        Huffman::HuffmanFile f = en.encode();

        Huffman de(f);

        assertTrue("decode() couldn't undo encode()", expected == de.decode());
    }
    {
        std::string expected = "12";

        Huffman en(expected);

        Huffman::HuffmanFile f = en.encode();

        Huffman de(f);

        assertTrue("decode() couldn't undo encode()", expected == de.decode());
    }
    {
        std::string expected = "CS2024Forever";

        Huffman en(expected);

        Huffman::HuffmanFile f = en.encode();

        Huffman de(f);

        assertTrue("decode() couldn't undo encode()", expected == de.decode());
    }
    {
        std::string expected = "bruh";

        Huffman en(expected);

        Huffman::HuffmanFile f = en.encode();

        Huffman de(f);

        assertTrue("decode() couldn't undo encode()", expected == de.decode());
    }
    {
        std::string expected = "Don't do it!!";

        Huffman en(expected);

        Huffman::HuffmanFile f = en.encode();

        Huffman de(f);

        assertTrue("decode() couldn't undo encode()", expected == de.decode());
    }
    printSubTitle("4.1 Success!");

    printSubTitle("4.2 Handles special characters");
    {
        std::string special = "!@#$%^&*()\n\t\\\"'";
        Huffman huff(special);
        Huffman dehuff(huff.encode());
        assertTrue("Special characters", special == dehuff.decode());
    }
    printSubTitle("4.2 Success!");

    printSubTitle("4.3 Handles repeated patterns");
    {
        std::string repeated = "ABABABABABABABABABAB";
        Huffman huff(repeated);
        Huffman dehuff(huff.encode());
        assertTrue("Repeated patterns", repeated == dehuff.decode());
    }
    printSubTitle("4.3 Success!");

    printTitle("Running Test 5: HuffmanFile constructor");
    printSubTitle("5.1 Constructs from encoded file");
    {
        std::string content = "Test content for HuffmanFile";
        Huffman encoder(content);
        Huffman::HuffmanFile file = encoder.encode();

        Huffman decoder(file);
        assertTrue("HuffmanFile constructor", content == decoder.decode());
    }
    printSubTitle("5.1 Success!");

    printTitle("Running Test 6: isEncode()");
    printSubTitle("6.1 Returns correct mode");
    {
        std::string content = "Test content";
        Huffman encoder(content);
        assertTrue("Encoder should be in encode mode", encoder.isEncode());

        Huffman::HuffmanFile file = encoder.encode();
        Huffman decoder(file);
        assertFalse("Decoder should be in encode mode", decoder.isEncode());
    }
    printSubTitle("6.1 Success!");
}

int main(void) {
    unitTest();
}