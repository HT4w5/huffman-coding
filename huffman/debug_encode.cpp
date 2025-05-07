#define HUFFMAN_DEBUG
#include <iostream>
#include "huffman.hpp"

int main(int argc, char* argv[]) {
    std::string input("114514\n");
    HuffmanEncoder he(input);

    // Output encoded info.
    std::cout << "treeBits: ";
    for (const bool b : he.result().getTreeBits()) {
        std::cout << b * 1;
    }
    std::cout << '\n';

    std::cout<<"leaves: ";
    for (const char c : he.result().getLeaves()) {
        std::cout << c;
    }
    std::cout << '\n';

    std::cout << "content: ";
    for (const bool b : he.result().getContent()) {
        std::cout << b * 1;
    }
    std::cout << '\n';
}