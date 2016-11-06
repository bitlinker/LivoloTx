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


#include "LivoloTx.h"
#include <Arduino.h>
#include <stdint.h>

#define LIVOLO_PREAMBLE_DURATION 525
#define LIVOLO_ZERO_DURATION 120
#define LIVOLO_ONE_DURATION 315
#define LIVOLO_NUM_REPEATS 150

LivoloTx::LivoloTx(int txPin)
  : mIsHigh(false)
  , mTxPin(txPin)
{
}

void LivoloTx::sendButton(uint16_t remoteId, uint8_t keyId)
{
  // 7 bit Key Id and 16 bit Remote Id
  uint32_t command = ((uint32_t)keyId & 0x7F) | (remoteId << 7);
  sendCommand(command, 23);
}


void LivoloTx::sendCommand(uint32_t command, uint8_t numBits)
{
  for (uint8_t repeat = 0; repeat < LIVOLO_NUM_REPEATS; ++repeat)
  {
    uint32_t mask = (1 << (numBits - 1));
    sendPreamble();
    for (uint8_t i = numBits; i > 0; --i)
    {
      if ((command & mask) > 0)
      {
        sendOne();
      }
      else
      {
        sendZero();
      }
      mask >>= 1;
    }
  }
  tx(false);
}

void LivoloTx::sendOne()
{
  delayMicroseconds(LIVOLO_ONE_DURATION);
  mIsHigh = !mIsHigh;
  tx(mIsHigh);
}

void LivoloTx::sendZero()
{
  delayMicroseconds(LIVOLO_ZERO_DURATION);
  tx(!mIsHigh);
  delayMicroseconds(LIVOLO_ZERO_DURATION);
  tx(mIsHigh);
}

void LivoloTx::sendPreamble()
{
  tx(true);
  delayMicroseconds(LIVOLO_PREAMBLE_DURATION);
  tx(false);
  mIsHigh = false;
}

void LivoloTx::tx(bool value)
{
  digitalWrite(mTxPin, value ? HIGH : LOW);
}
