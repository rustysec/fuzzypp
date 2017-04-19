#include "BlockhashContext.h"

namespace FuzzyPP {
    BlockhashContext::BlockhashContext() {
        Digest = std::vector<unsigned char>(FuzzyConstants::I().SpamSumLength);
    }

    void BlockhashContext::Hash(unsigned char c)
    {
        H = Hash(c, H);
        HalfH = Hash(c, HalfH);
    }

    unsigned int BlockhashContext::Hash(unsigned char c, unsigned int h)
    {
        return (h * HashPrime) ^ c;
    }

    void BlockhashContext::Reset(bool init)
    {
        Digest[init ? DLen : ++DLen] = 0;
        H = HashInit;
        if (DLen < FuzzyConstants::I().SpamSumLength / 2)
        {
            HalfH = HashInit;
            HalfDigest = 0;
        }
    }
}
