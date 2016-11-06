/*
LivoloTx - Arduino library implementing Livolo switches radio protocol

Copyright (c) 2016 Bitlinker

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef LivoloTx_h
#define LivoloTx_h

#include <inttypes.h>

class LivoloTx
{
  public:
    static const uint8_t KEY_MAX = 10;
    static constexpr uint8_t KEYS[KEY_MAX] = {
      0,
      96,
      120,
      24,
      80,
      48,
      108,
      12,
      72,
      40,
    };
    static const uint8_t KEY_OFF = 106;

  public:
    explicit LivoloTx(int txPin);
    void sendButton(uint16_t remoteId, uint8_t keyId);

  private:
    void sendCommand(uint32_t command, uint8_t numBits);
    inline void sendOne();
    inline void sendZero();
    inline void sendPreamble();
    inline void tx(bool value);

  private:
    bool mIsHigh;
    int mTxPin;
};

#endif // LivoloTx_h
