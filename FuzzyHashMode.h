#ifndef FUZZYHASHMODE_H_
#define FUZZYHASHMODE_H_

namespace FuzzyPP {
    class FuzzyHashMode {
    public:
        enum Mode
        {
            None = 0,
            ///<summary>Eliminate sequences of more than three identical characters</summary>
            EliminateSequences = 1,
            ///<summary>Do not truncate the second part to SPAMSUM_LENGTH/2 characters</summary>
            DoNotTruncate = 2
        };
    };
}

#endif /* FUZZYHASHMODE_H_ */
