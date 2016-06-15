/*
 * Hasher.cpp
 *
 *  Created on: Jun 14, 2016
 *      Author: russ
 */

#include "Hasher.h"

namespace FuzzyPP {
    Hasher::Hasher()
    {
        _roll = new Roll();
        
        //_bh = new BlockhashContext[FuzzyConstants::I().NumBlockhashes];
        _bh = std::vector<BlockhashContext>(FuzzyConstants::I().NumBlockhashes);
        
        for (int i = 0; i < FuzzyConstants::I().NumBlockhashes; i++)
            _bh[i].Reset(true);
        //    _bh[i] = new BlockhashContext();
        _bh[0].Reset(true);
    }

    int Hasher::MemcpyEliminateSequences(unsigned char *dst, int pos, unsigned char *src, int n)
    {
        auto i = 0;

        while (i < 3 && i < n)
            dst[pos++] = src[i++];

        while (i < n)
        {
            auto current = src[i++];
            if (current == dst[pos - 1] && current == dst[pos - 2] && current == dst[pos - 3])
                n--;
            else
                dst[pos++] = current;
        }

        return n;
    }
    
    std::string Hasher::Digest(FuzzyHashMode::Mode flags)
    {
        auto result = new unsigned char[FuzzyConstants::I().MaxResultLength];
        auto pos = 0;

        unsigned int bi = _bhstart;
        unsigned int h = _roll->Sum();
        int i; // Exclude terminating '\0'.

        /* Initial blocksize guess. */
        while ((unsigned int)(((unsigned int)FuzzyConstants::I().MinBlocksize) << (int)(bi)) * FuzzyConstants::I().SpamSumLength < _totalSize)
        {
            ++bi;
            if (bi >= FuzzyConstants::I().NumBlockhashes)
            {
                printf("EOVERFLOW\n");
            }
        }
        /* Adapt blocksize guess to actual digest length. */
        while (bi >= _bhend)
        {
            --bi;
        }
        while (bi > _bhstart && _bh[bi].DLen < FuzzyConstants::I().SpamSumLength / 2)
        {
            --bi;
        }

        auto actualBlockSize = (((unsigned int)FuzzyConstants::I().MinBlocksize) << (int)(bi));
        auto blockSizeString = std::to_string(actualBlockSize);
        auto blockSizeChars = blockSizeString.c_str(); //actualBlockSize.ToString().ToCharArray();
        i = blockSizeString.length();
        for (int j = 0; j < i; j++)
            result[j + pos] = (unsigned char)blockSizeChars[j];
        result[i++] = (unsigned char)':';

        pos += i;
        i = (int)_bh[bi].DLen;

        if (flags & FuzzyHashMode::Mode::EliminateSequences)
            i = MemcpyEliminateSequences(result, pos, _bh[bi].Digest, i);
        else
            memcpy(&result[pos], &_bh[bi].Digest[0], i);

        pos += i;
        if (h != 0)
        {
            auto base64Val = FuzzyConstants::I().Base64[_bh[bi].H % 64];
            result[pos] = base64Val;
            if (!(flags & FuzzyHashMode::Mode::EliminateSequences) || i < 3 || base64Val != result[pos - 1] || base64Val != result[pos - 2] || base64Val != result[pos - 3])
                ++pos;
        }
        else if (_bh[bi].Digest[i] != '\0')
        {
            auto digestVal = _bh[bi].Digest[i];
            result[pos] = digestVal;
            if (!(flags & FuzzyHashMode::Mode::EliminateSequences) || i < 3 || digestVal != result[pos - 1] || digestVal != result[pos - 2] || digestVal != result[pos - 3])
                ++pos;
        }
        result[pos++] = (unsigned char)':';

        if (bi < _bhend - 1)
        {
            ++bi;
            i = (int)_bh[bi].DLen;
            if (!(flags & FuzzyHashMode::Mode::DoNotTruncate) && i > FuzzyConstants::I().SpamSumLength / 2 - 1)
            {
                i = FuzzyConstants::I().SpamSumLength / 2 - 1;
            }

            if (flags & FuzzyHashMode::Mode::EliminateSequences)
                i = MemcpyEliminateSequences(result, pos, _bh[bi].Digest, i);
            else {
                memcpy(&result[pos], &_bh[bi].Digest[0], i);
            }
            pos += i;
            if (h != 0)
            {
                h = (flags & FuzzyHashMode::Mode::DoNotTruncate) ? _bh[bi].H : _bh[bi].HalfH;
                auto base64Val = FuzzyConstants::I().Base64[h % 64];
                result[pos] = base64Val;
                if (!(flags & FuzzyHashMode::Mode::EliminateSequences) || i < 3 || base64Val != result[pos - 1] || base64Val != result[pos - 2] || base64Val != result[pos - 3])
                    ++pos;
            }
            else
            {
                i = (flags & FuzzyHashMode::Mode::DoNotTruncate) ? _bh[bi].Digest[_bh[bi].DLen] : _bh[bi].HalfDigest;
                if (i != '\0')
                {
                    result[pos] = (unsigned char)i;
                    if (!(flags & FuzzyHashMode::Mode::EliminateSequences) || i < 3 || i != result[pos - 1] || i != result[pos - 2] || i != result[pos - 3])
                        ++pos;
                }
            }
        }
        else if (h != 0)
        {
            result[pos++] = FuzzyConstants::I().Base64[_bh[bi].H % 64];
        }
        
        return std::string((char*)result);
    }
    
    void Hasher::TryForkBlockhash()
    {
        BlockhashContext *obh;
        BlockhashContext *nbh;
        if (_bhend >= FuzzyConstants::I().NumBlockhashes)
            return;
        obh = &_bh[_bhend - 1];
        nbh = &_bh[_bhend];
        nbh->H = obh->H;
        nbh->HalfH = obh->HalfH;
        nbh->Digest[0] = 0;
        nbh->HalfDigest = 0;
        nbh->DLen = 0;
        ++_bhend;
    }

    void Hasher::TryReduceBlockhash()
    {
        if (_bhend - _bhstart < 2)
            /* Need at least two working hashes. */
            return;
        if ((unsigned int)(((unsigned int)FuzzyConstants::I().MinBlocksize) << (int)(_bhstart)) * FuzzyConstants::I().SpamSumLength >= _totalSize)
            /* Initial blocksize estimate would select this or a smaller
             * blocksize. */
            return;
        if (_bh[_bhstart + 1].DLen < FuzzyConstants::I().SpamSumLength / 2)
            /* Estimate adjustment would select this blocksize. */
            return;
        /* At this point we are clearly no longer interested in the
         * start_blocksize. Get rid of it. */
        ++_bhstart;
    }
        
    void Hasher::EngineStep(unsigned char c)
    {
        unsigned int h;
        unsigned int i;
        /* At each character we update the rolling hash and the normal hashes.
         * When the rolling hash hits a reset value then we emit a normal hash
         * as a element of the signature and reset the normal hash. */
        _roll->Hash(c);
        h = _roll->Sum();

        for (i = _bhstart; i < _bhend; ++i)
            _bh[i].Hash(c);

        for (i = _bhstart; i < _bhend; ++i)
        {
            /* With growing blocksize almost no runs fail the next test. */
            if (h % (((unsigned int)FuzzyConstants::I().MinBlocksize) << (int)(i)) != (((unsigned int)FuzzyConstants::I().MinBlocksize) << (int)(i)) - 1)
                /* Once this condition is false for one bs, it is
                 * automatically false for all further bs. I.e. if
                 * h === -1 (mod 2*bs) then h === -1 (mod bs). */
                break;
            /* We have hit a reset point. We now emit hashes which are
             * based on all characters in the piece of the message between
             * the last reset point and this one */
            if (0 == _bh[i].DLen)
            {
                /* Can only happen 30 times. */
                /* First step for this blocksize. Clone next. */
                this->TryForkBlockhash();
            }
            _bh[i].Digest[_bh[i].DLen] = FuzzyConstants::I().Base64[_bh[i].H % 64];
            _bh[i].HalfDigest = FuzzyConstants::I().Base64[_bh[i].HalfH % 64];
            if (_bh[i].DLen < FuzzyConstants::I().SpamSumLength - 1)
            {
                /* We can have a problem with the tail overflowing. The
                 * easiest way to cope with this is to only reset the
                 * normal hash if we have room for more characters in
                 * our signature. This has the effect of combining the
                 * last few pieces of the message into a single piece
                 * */
                _bh[i].Reset();
            }
            else
            {
                this->TryReduceBlockhash();
            }
        }
    }
    
    void Hasher::Update(unsigned char *buffer, int len)
    {
        _totalSize += len; //(unsigned int)buffer.Length;

        for (int i = 0; i < len; i++)
            this->EngineStep(buffer[i]);
    }
    
    /// <summary>
    /// Computes the fuzzy hash of the first len bytes of the buffer.
    /// </summary>
    std::string Hasher::HashBuffer(unsigned char *buf, int len, FuzzyPP::FuzzyHashMode::Mode flags)
    {
        auto ctx = new Hasher();
        ctx->Update(buf, len);
        return ctx->Digest(flags);
    }
}
