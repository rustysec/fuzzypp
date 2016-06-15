#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "FuzzyConstants.h"
#include "FuzzyHashMode.h"
#include "Hasher.h"

using namespace std;
using namespace FuzzyPP;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage:\n\t%s [path to file]\n", argv[0]);
        return -1;
    }

    std::ifstream f(argv[1], std::ios::binary);
    if (!f.is_open()) {
        printf("Cannot open file %s\n", argv[1]);
        return -1;
    }

    f.seekg(0, std::ios::end);
    auto size = f.tellg();
    f.seekg(0, std::ios::beg);

    std::vector<unsigned char> data(size);
    f.read((char*)&data[0], size);
    f.close();

    //auto hasher = new Hasher();
    auto h = Hasher::HashBuffer(data.data(), data.size());
    printf("%s = %s\n", argv[1], h.c_str());
    return 0;
}
