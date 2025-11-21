#pragma once

#include "qipStruct.h"

// Get ICO information
void qipGetICOInfo(const char* nodeIp, int nodePort, uint32_t icoIndex);

// Create a new ICO
void qipCreateICO(const char* nodeIp, int nodePort, const char* seed,
                  const char* issuer, const char* address1, const char* address2, const char* address3,
                  const char* address4, const char* address5, const char* address6, const char* address7,
                  const char* address8, const char* address9, const char* address10,
                  const char* assetName,
                  uint64_t price1, uint64_t price2, uint64_t price3,
                  uint64_t saleAmountForPhase1, uint64_t saleAmountForPhase2, uint64_t saleAmountForPhase3,
                  uint32_t percent1, uint32_t percent2, uint32_t percent3, uint32_t percent4, uint32_t percent5,
                  uint32_t percent6, uint32_t percent7, uint32_t percent8, uint32_t percent9, uint32_t percent10,
                  uint32_t startEpoch,
                  uint32_t scheduledTickOffset);

// Buy tokens from an ICO
void qipBuyToken(const char* nodeIp, int nodePort, const char* seed,
                 uint32_t icoIndex, uint64_t amount, uint32_t phase,
                 uint32_t scheduledTickOffset);

// Transfer share management rights
void qipTransferShareManagementRights(const char* nodeIp, int nodePort, const char* seed,
                                      const char* assetName, const char* issuer,
                                      uint32_t newManagingContractIndex, uint64_t numberOfShares,
                                      uint32_t scheduledTickOffset);

