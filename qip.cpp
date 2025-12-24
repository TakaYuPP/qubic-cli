#include <cstring>
#include <cstdio>

#include "qip.h"
#include "structs.h"
#include "walletUtils.h"
#include "keyUtils.h"
#include "connection.h"
#include "logger.h"
#include "nodeUtils.h"
#include "K12AndKeyUtil.h"
#include "assetUtil.h"

void qipGetICOInfo(const char* nodeIp, int nodePort, uint32_t icoIndex)
{
    auto qc = make_qc(nodeIp, nodePort);
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QIP_getICOInfo_input input;
    } packet;
    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(QIP_getICOInfo_input);
    packet.rcf.inputType = QIP_GET_ICO_INFO_FUNC;
    packet.rcf.contractIndex = QIP_CONTRACT_INDEX;
    packet.input.indexOfICO = icoIndex;
    qc->sendData((uint8_t *) &packet, packet.header.size());

    try
    {
        QIP_getICOInfo_output result = qc->receivePacketWithHeaderAs<QIP_getICOInfo_output>();
        
        char buf[128] = {0};
        LOG("ICO Index: %u\n", icoIndex);
        
        getIdentityFromPublicKey(result.creatorOfICO, buf, false);
        LOG("Creator: %s\n", buf);
        
        getIdentityFromPublicKey(result.issuer, buf, false);
        LOG("Issuer: %s\n", buf);
        
        LOG("Asset Name: ");
        char assetNameBuf[9] = {0};
        memcpy(assetNameBuf, &result.assetName, 8);
        LOG("%s\n", assetNameBuf);
        
        LOG("Distribution Addresses:\n");
        for (int i = 1; i <= 10; i++)
        {
            uint8_t* addr = (i == 1) ? result.address1 : 
                           (i == 2) ? result.address2 :
                           (i == 3) ? result.address3 :
                           (i == 4) ? result.address4 :
                           (i == 5) ? result.address5 :
                           (i == 6) ? result.address6 :
                           (i == 7) ? result.address7 :
                           (i == 8) ? result.address8 :
                           (i == 9) ? result.address9 : result.address10;
            memset(buf, 0, 128);
            getIdentityFromPublicKey(addr, buf, false);
            uint32_t percent = (i == 1) ? result.percent1 :
                               (i == 2) ? result.percent2 :
                               (i == 3) ? result.percent3 :
                               (i == 4) ? result.percent4 :
                               (i == 5) ? result.percent5 :
                               (i == 6) ? result.percent6 :
                               (i == 7) ? result.percent7 :
                               (i == 8) ? result.percent8 :
                               (i == 9) ? result.percent9 : result.percent10;
            LOG("  Address %d: %s (%%%u)\n", i, buf, percent);
        }
        
        LOG("Prices:\n");
        LOG("  Phase 1: %llu\n", result.price1);
        LOG("  Phase 2: %llu\n", result.price2);
        LOG("  Phase 3: %llu\n", result.price3);
        
        LOG("Sale Amounts:\n");
        LOG("  Phase 1: %llu (remaining: %llu)\n", result.saleAmountForPhase1, result.remainingAmountForPhase1);
        LOG("  Phase 2: %llu (remaining: %llu)\n", result.saleAmountForPhase2, result.remainingAmountForPhase2);
        LOG("  Phase 3: %llu (remaining: %llu)\n", result.saleAmountForPhase3, result.remainingAmountForPhase3);
        
        LOG("Start Epoch: %u\n", result.startEpoch);
    }
    catch (std::logic_error)
    {
        LOG("Failed to get ICO info\n");
    }
}

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
                  uint32_t scheduledTickOffset)
{
    uint8_t issuerPubkey[32] = {0};
    uint8_t address1Pubkey[32] = {0};
    uint8_t address2Pubkey[32] = {0};
    uint8_t address3Pubkey[32] = {0};
    uint8_t address4Pubkey[32] = {0};
    uint8_t address5Pubkey[32] = {0};
    uint8_t address6Pubkey[32] = {0};
    uint8_t address7Pubkey[32] = {0};
    uint8_t address8Pubkey[32] = {0};
    uint8_t address9Pubkey[32] = {0};
    uint8_t address10Pubkey[32] = {0};
    
    getPublicKeyFromIdentity(issuer, issuerPubkey);
    getPublicKeyFromIdentity(address1, address1Pubkey);
    getPublicKeyFromIdentity(address2, address2Pubkey);
    getPublicKeyFromIdentity(address3, address3Pubkey);
    getPublicKeyFromIdentity(address4, address4Pubkey);
    getPublicKeyFromIdentity(address5, address5Pubkey);
    getPublicKeyFromIdentity(address6, address6Pubkey);
    getPublicKeyFromIdentity(address7, address7Pubkey);
    getPublicKeyFromIdentity(address8, address8Pubkey);
    getPublicKeyFromIdentity(address9, address9Pubkey);
    getPublicKeyFromIdentity(address10, address10Pubkey);
    
    QIP_createICO_input input;
    memset(&input, 0, sizeof(input));
    memcpy(input.issuer, issuerPubkey, 32);
    memcpy(input.address1, address1Pubkey, 32);
    memcpy(input.address2, address2Pubkey, 32);
    memcpy(input.address3, address3Pubkey, 32);
    memcpy(input.address4, address4Pubkey, 32);
    memcpy(input.address5, address5Pubkey, 32);
    memcpy(input.address6, address6Pubkey, 32);
    memcpy(input.address7, address7Pubkey, 32);
    memcpy(input.address8, address8Pubkey, 32);
    memcpy(input.address9, address9Pubkey, 32);
    memcpy(input.address10, address10Pubkey, 32);
    char assetNameS1[8] = {0};
    memcpy(assetNameS1, assetName, strlen(assetName));
    memcpy(&input.assetName, assetNameS1, 8);
    input.price1 = price1;
    input.price2 = price2;
    input.price3 = price3;
    input.saleAmountForPhase1 = saleAmountForPhase1;
    input.saleAmountForPhase2 = saleAmountForPhase2;
    input.saleAmountForPhase3 = saleAmountForPhase3;
    input.percent1 = percent1;
    input.percent2 = percent2;
    input.percent3 = percent3;
    input.percent4 = percent4;
    input.percent5 = percent5;
    input.percent6 = percent6;
    input.percent7 = percent7;
    input.percent8 = percent8;
    input.percent9 = percent9;
    input.percent10 = percent10;
    input.startEpoch = startEpoch;
    
    LOG("\nSending transaction to create ICO...\n");
    makeContractTransaction(nodeIp, nodePort, seed, QIP_CONTRACT_INDEX, QIP_CREATE_ICO_PROC,
        0, sizeof(input), (uint8_t*)&input, scheduledTickOffset);
}

void qipBuyToken(const char* nodeIp, int nodePort, const char* seed,
                 uint32_t icoIndex, uint64_t amount, uint32_t phase,
                 uint32_t scheduledTickOffset)
{
    // First, get ICO info to determine the current price
    auto qc = make_qc(nodeIp, nodePort);
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QIP_getICOInfo_input input;
    } queryPacket;
    queryPacket.header.setSize(sizeof(queryPacket));
    queryPacket.header.randomizeDejavu();
    queryPacket.header.setType(RequestContractFunction::type());
    queryPacket.rcf.inputSize = sizeof(QIP_getICOInfo_input);
    queryPacket.rcf.inputType = QIP_GET_ICO_INFO_FUNC;
    queryPacket.rcf.contractIndex = QIP_CONTRACT_INDEX;
    queryPacket.input.indexOfICO = icoIndex;
    qc->sendData((uint8_t *) &queryPacket, queryPacket.header.size());

    uint64_t estimatedPayment = 0;
    try
    {
        QIP_getICOInfo_output icoInfo = qc->receivePacketWithHeaderAs<QIP_getICOInfo_output>();
        
        // Get the price based on the specified phase (1, 2, or 3)
        uint64_t price = 0;
        
        if (phase == 1)
        {
            price = icoInfo.price1;
        }
        else if (phase == 2)
        {
            price = icoInfo.price2;
        }
        else if (phase == 3)
        {
            price = icoInfo.price3;
        }
        // phase is already validated in argparser.h, so this else branch is unnecessary..
        
        // Calculate estimated payment (amount * price)
        estimatedPayment = amount * price;
        LOG("Estimated payment: %llu (based on Phase %u price: %llu)\n", estimatedPayment, phase, price);
        LOG("Note: Actual price depends on current epoch. Contract will use correct phase price.\n");
    }
    catch (std::logic_error)
    {
        LOG("Warning: Could not fetch ICO info. Using amount as payment estimate.\n");
        // Fallback: use a high estimate
        estimatedPayment = amount * 1000000; // Conservative high estimate
    }

    QIP_buyToken_input input;
    memset(&input, 0, sizeof(input));
    input.indexOfICO = icoIndex;
    input.amount = amount;
    
    LOG("\nSending transaction to buy tokens...\n");
    LOG("Buying %llu tokens from ICO #%u\n", amount, icoIndex);
    LOG("Transaction amount (invocation reward): %llu\n", estimatedPayment);
    makeContractTransaction(nodeIp, nodePort, seed, QIP_CONTRACT_INDEX, QIP_BUY_TOKEN_PROC,
        estimatedPayment, sizeof(input), (uint8_t*)&input, scheduledTickOffset);
}

void qipTransferShareManagementRights(const char* nodeIp, int nodePort, const char* seed,
                                      const char* assetName, const char* issuer,
                                      uint32_t newManagingContractIndex, uint64_t numberOfShares,
                                      uint32_t scheduledTickOffset)
{
    QIP_TransferShareManagementRights_input input;
    memset(&input, 0, sizeof(input));
    getPublicKeyFromIdentity(issuer, input.asset.issuer);
    input.asset.assetName = 0;
    memcpy(&input.asset.assetName, assetName, strlen(assetName));
    input.newManagingContractIndex = newManagingContractIndex;
    input.numberOfShares = (int64_t)numberOfShares;
    
    LOG("\nSending transaction to transfer share management rights...\n");
    makeContractTransaction(nodeIp, nodePort, seed, QIP_CONTRACT_INDEX, QIP_TRANSFER_SHARE_MANAGEMENT_RIGHTS_PROC,
        100, sizeof(input), (uint8_t*)&input, scheduledTickOffset);
}

