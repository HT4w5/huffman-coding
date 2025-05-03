#include <iostream>
#include <stdexcept>

void assertTrue(std::string failMsg, bool pred) {
    if (!pred) {
        std::cerr << "Assertion failed: " << failMsg << std::endl;
        throw std::runtime_error("Assertion failed");
    }
}

void assertFalse(std::string failMsg, bool pred) {
    assertTrue(failMsg, !pred);
}

void printTitle(std::string str) {
    std::cout << "#############################################" << '\n';
    std::cout << str << '\n';
    std::cout << "#############################################" << std::endl;
}

void printSubTitle(std::string str) {
    std::cout << "*" << str << std::endl;
}
