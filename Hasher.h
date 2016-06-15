#ifndef HASHER_H_
#define HASHER_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <string.h>

#include "FuzzyHashMode.h"
#include "BlockhashContext.h"
#include "Roll.h"

namespace FuzzyPP {
    class Hasher {
        unsigned int _bhstart = 0;
        unsigned int _bhend = 1;
        std::vector<BlockhashContext> _bh;
        unsigned int _totalSize = 0;
        std::shared_ptr<Roll> _roll;
        
        static int MemcpyEliminateSequences(unsigned char *dst, int pos, unsigned char *src, int n);

        /// <summary>
        /// Obtain the fuzzy hash from the state.
        /// This operation does not change the state at all. It reports the hash for the
        /// concatenation of the data previously fed using fuzzy_update.
        /// </summary>
        std::string Digest(FuzzyHashMode::Mode flags);

        void TryForkBlockhash();

        void TryReduceBlockhash();

        void EngineStep(unsigned char c);

        /// <summary>
        /// Feed the data contained in the given buffer to the state.
        /// </summary>
        void Update(unsigned char *buffer, int len);
        
        Hasher();
        
    public:
        static std::string HashBuffer(unsigned char *buf, int len, FuzzyPP::FuzzyHashMode::Mode flags = FuzzyPP::FuzzyHashMode::Mode::None);
    };
}
#endif /* HASHER_H_ */
