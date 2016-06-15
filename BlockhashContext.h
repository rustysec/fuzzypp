#ifndef BLOCKHASHCONTEXT_H_
#define BLOCKHASHCONTEXT_H_

#include "FuzzyConstants.h"
#include <vector>

namespace FuzzyPP {
class BlockhashContext {
    private:
        static const int HashPrime = 0x01000193;
        static const int HashInit = 0x28021967;
    public:
        unsigned int H;
        unsigned int HalfH;
        std::vector<unsigned char> Digest;
        unsigned char HalfDigest;
        unsigned int DLen;

        void Hash(unsigned char c);
        static unsigned int Hash(unsigned char c, unsigned int h);
        void Reset(bool init = false);
        BlockhashContext();
    };
}
#endif /* BLOCKHASHCONTEXT_H_ */
