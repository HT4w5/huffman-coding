/* huff.cpp - Simple huffman compressor. */

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "huffman/huffman.hpp"

void printHelp();
void compress(const std::string& src, const std::string& dst);
void extract(const std::string& src, const std::string& dst);
std::string readFile(const std::string& path);
void writeFile(const std::string& path, const std::string& content);

int main(int argc, char* argv[]) {
    // Parse arguments.
    if (argc == 1) {
        std::cerr << "No arguments specified" << std::endl;
        return EXIT_FAILURE;
    }
    std::string verb(argv[1]);
    if (verb == "-h" || verb == "--help") {
        printHelp();
    } else if (verb == "-c" || verb == "--compress") {
        if (argc != 4) {
            std::cerr << "Missing / invalid arguments" << std::endl;
            return EXIT_FAILURE;
        }
        try {
            compress(std::string(argv[2]), std::string(argv[3]));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;

    } else if (verb == "-x" || verb == "--extract") {
        if (argc != 4) {
            std::cerr << "Missing / invalid arguments" << std::endl;
            return EXIT_FAILURE;
        }
        try {
            extract(std::string(argv[2]), std::string(argv[3]));
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    } else {
        std::cerr << "Invalid argument" << std::endl;
        return EXIT_FAILURE;
    }
}

void printHelp() {
    std::cout << "huff - Simple huffman compressor\n"
              << "Params:\n"
              << "huff --help     | -h\n"
              << "huff --compress | -c  [source] [target]\n"
              << "huff --extract  | -x  [source] [target]\n";
}

void compress(const std::string& src, const std::string& dst) {
    HuffmanEncoder he(readFile(src));
    he.result().write(dst);
}

void extract(const std::string& src, const std::string& dst) {
    HuffmanFile hf(src);
    HuffmanDecoder hd(hf);
    writeFile(dst, hd.result());
}

std::string readFile(const std::string& path) {
    std::ifstream ifs(path);

    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::ostringstream oss;
    oss << ifs.rdbuf();
    ifs.close();
    return oss.str();
}

void writeFile(const std::string& path, const std::string& content) {
    std::ofstream ofs(path, std::ios::binary);

    if (!ofs.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    ofs.write(content.data(), content.size());
    ofs.close();
}