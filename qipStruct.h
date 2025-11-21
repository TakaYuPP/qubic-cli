#pragma once

#include "structs.h"

// QIP Contract Index (to be determined based on deployment)
// For now, we'll use a placeholder that can be configured
#define QIP_CONTRACT_INDEX 11  // Update this based on actual contract deployment

// QIP Function IDs (matching contract registration)
#define QIP_GET_ICO_INFO_FUNC 1

// QIP Procedure IDs (matching contract registration)
#define QIP_CREATE_ICO_PROC 1
#define QIP_BUY_TOKEN_PROC 2
#define QIP_TRANSFER_SHARE_MANAGEMENT_RIGHTS_PROC 3

// Input structure for getICOInfo function
struct QIP_getICOInfo_input
{
    uint32_t indexOfICO;
};

// Output structure for getICOInfo function
struct QIP_getICOInfo_output
{
    uint8_t creatorOfICO[32];
    uint8_t issuer[32];
    uint8_t address1[32], address2[32], address3[32], address4[32], address5[32];
    uint8_t address6[32], address7[32], address8[32], address9[32], address10[32];
    uint64_t assetName;
    uint64_t price1;
    uint64_t price2;
    uint64_t price3;
    uint64_t saleAmountForPhase1;
    uint64_t saleAmountForPhase2;
    uint64_t saleAmountForPhase3;
    uint64_t remainingAmountForPhase1;
    uint64_t remainingAmountForPhase2;
    uint64_t remainingAmountForPhase3;
    uint32_t percent1, percent2, percent3, percent4, percent5;
    uint32_t percent6, percent7, percent8, percent9, percent10;
    uint32_t startEpoch;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

// Input structure for createICO procedure
struct QIP_createICO_input
{
    uint8_t issuer[32];
    uint8_t address1[32], address2[32], address3[32], address4[32], address5[32];
    uint8_t address6[32], address7[32], address8[32], address9[32], address10[32];
    uint64_t assetName;
    uint64_t price1;
    uint64_t price2;
    uint64_t price3;
    uint64_t saleAmountForPhase1;
    uint64_t saleAmountForPhase2;
    uint64_t saleAmountForPhase3;
    uint32_t percent1, percent2, percent3, percent4, percent5;
    uint32_t percent6, percent7, percent8, percent9, percent10;
    uint32_t startEpoch;
};

// Output structure for createICO procedure
struct QIP_createICO_output
{
    int32_t returnCode;
};

// Input structure for buyToken procedure
struct QIP_buyToken_input
{
    uint32_t indexOfICO;
    uint64_t amount;
};

// Output structure for buyToken procedure
struct QIP_buyToken_output
{
    int32_t returnCode;
};

// Input structure for TransferShareManagementRights procedure
struct QIP_TransferShareManagementRights_input
{
    struct
    {
        uint8_t issuer[32];
        uint64_t assetName;
    } asset;
    int64_t numberOfShares;
    uint32_t newManagingContractIndex;
};

// Output structure for TransferShareManagementRights procedure
struct QIP_TransferShareManagementRights_output
{
    int64_t transferredNumberOfShares;
};

