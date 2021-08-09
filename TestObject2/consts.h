//
// Created by elizar on 15.07.2021.
//

#ifndef TESTOBJECT2_CONSTS_H
#define TESTOBJECT2_CONSTS_H

//[ OFDM parameters
#define NumOfSubcarriers 64
#define NumOfDataSubcarriers 48
#define CyclicPrefixLength 16
//]

//[ Maximum packet size and constellation parameters for memory allocation
#define MaxCodedPacketSize (2*(MaxPacketSize+6)+288)
#define MaxNumOfSymbols (4 * MaxCodedPacketSize / NumOfDataSubcarriers)
//]

//[ Preamble length
#define  PreambleLength (16 * 10 + 2 * CyclicPrefixLength + 2 * NumOfSubcarriers)

#define MaxSamplingPower 4
#endif //TESTOBJECT2_CONSTS_H