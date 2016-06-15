#include "Roll.h"

namespace FuzzyPP {
    Roll::Roll() {
    }

    unsigned int Roll::Sum()
    {
        return _h1 + _h2 + _h3;
    }

    void Roll::Hash(unsigned char c)
    {
        _h2 -= _h1;
        _h2 += FuzzyConstants::I().RollingWindow * (unsigned int)c;

        _h1 += (unsigned int)c;
        _h1 -= (unsigned int)_window[_n % FuzzyConstants::I().RollingWindow];

        _window[_n % FuzzyConstants::I().RollingWindow] = c;
        _n++;

        /* The original spamsum AND'ed this value with 0xFFFFFFFF which
         * in theory should have no effect. This AND has been removed
         * for performance (jk) */
        _h3 <<= 5;
        _h3 ^= c;
    }
}
