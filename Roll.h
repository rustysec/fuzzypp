#ifndef ROLL_H_
#define ROLL_H_

#include "FuzzyConstants.h"
#include <vector>

namespace FuzzyPP {
    class Roll {
        std::vector<unsigned char> _window = std::vector<unsigned char>(FuzzyConstants::I().RollingWindow);
        unsigned int _h1;
        unsigned int _h2;
        unsigned int _h3;
        unsigned int _n;

    public:
        unsigned int Sum();

        /*
         * a rolling hash, based on the Adler checksum. By using a rolling hash
         * we can perform auto resynchronisation after inserts/deletes

         * internally, h1 is the sum of the bytes in the window and h2
         * is the sum of the bytes times the index

         * h3 is a shift/xor based rolling hash, and is mostly needed to ensure that
         * we can cope with large blocksize values
         */
        void Hash(unsigned char c);

        Roll();
    };
}
#endif /* ROLL_H_ */
