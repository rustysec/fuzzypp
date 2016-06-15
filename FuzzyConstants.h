/*
 * FuzzyConstants.h
 *
 *  Created on: Jun 14, 2016
 *      Author: russ
 */

#ifndef FUZZYCONSTANTS_H_
#define FUZZYCONSTANTS_H_

#include <string>
#include <stdio.h>

namespace FuzzyPP {
    class FuzzyConstants {
    private:
        FuzzyConstants();

    public:
        const int RollingWindow = 7;
        const int MinBlocksize = 3;
        const int NumBlockhashes = 31;
        const int SpamSumLength = 64;
        const int MaxResultLength = 2 * SpamSumLength + 20;
        unsigned char *Base64;

        // Implement this as a singleton
        static FuzzyConstants& I();

        FuzzyConstants(FuzzyConstants const&) = delete;
        void operator=(FuzzyConstants const&) = delete;
    };
}
#endif /* FUZZYCONSTANTS_H_ */
