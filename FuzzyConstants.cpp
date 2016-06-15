#include "FuzzyConstants.h"

namespace FuzzyPP {

    FuzzyConstants::FuzzyConstants() {
        std::string b64Chars("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
        Base64 = new unsigned char[b64Chars.length()];
        for (unsigned int i = 0; i < b64Chars.length(); i++)
            Base64[i] = (unsigned char)(b64Chars.c_str()[i]);
    }

    FuzzyConstants &FuzzyConstants::I() {
        static FuzzyConstants instance;
        return instance;
    }
}
