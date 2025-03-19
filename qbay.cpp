#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <algorithm>

#include "structs.h"
#include "walletUtils.h"
#include "keyUtils.h"
#include "assetUtil.h"
#include "connection.h"
#include "logger.h"
#include "nodeUtils.h"
#include "K12AndKeyUtil.h"
#include "qbay.h"

#define QBAY_CONTRACT_INDEX 12

// QBAY FUNCTIONS
#define QBAY_INPUT_TYPE_GET_NUMBER_OF_NFT_FOR_USER 1
#define QBAY_INPUT_TYPE_GET_INFO_OF_NFT_USER_POSSESSED 2
#define QBAY_INPUT_TYPE_GET_INFO_OF_MARKETPLACE 3
#define QBAY_INPUT_TYPE_GET_INFO_OF_COLLECTION_BY_CREATOR 4
#define QBAY_INPUT_TYPE_GET_INFO_OF_COLLECTION_BY_ID 5
#define QBAY_INPUT_TYPE_GET_INCOMMING_AUCTIONS 6
#define QBAY_INPUT_TYPE_GET_INFO_OF_NFT_BY_ID 7

// QBAY PROCEDURES
#define QBAY_INPUT_TYPE_SETTING_CFB_AND_QUBIC_PRICE 1
#define QBAY_INPUT_TYPE_CREATE_COLLECTION 2
#define QBAY_INPUT_TYPE_MINT_NFT 3
#define QBAY_INPUT_TYPE_MINT_OF_DROP 4
#define QBAY_INPUT_TYPE_TRANSFER 5
#define QBAY_INPUT_TYPE_LIST_IN_MARKET 6
#define QBAY_INPUT_TYPE_SELL 7
#define QBAY_INPUT_TYPE_CANCEL_SELL 8
#define QBAY_INPUT_TYPE_LIST_IN_EXCHANGE 9
#define QBAY_INPUT_TYPE_CANCEL_EXCHANGE 10
#define QBAY_INPUT_TYPE_MAKE_OFFER 11
#define QBAY_INPUT_TYPE_ACCEPT_OFFER 12
#define QBAY_INPUT_TYPE_CANCEL_OFFER 13
#define QBAY_INPUT_TYPE_CREATE_TRADITIONAL_AUCTION 14
#define QBAY_INPUT_TYPE_BID_ON_TRADITIONAL_AUCTION 15
#define QBAY_INPUT_TYPE_TRANSFER_SHARE_MANAGAMENT_RIGHTS 16
#define QBAY_INPUT_TYPE_CHANGE_STATUS_OF_MARKET 17

struct settingCFBAndQubicPrice_input 
{

    uint64_t CFBPrice;                         //    The amount of $CFB per 1 USD
    uint64_t QubicPrice;						 //    The amount of $Qubic per 1 USD

};

struct settingCFBAndQubicPrice_output 
{

    uint32_t returnCode;

};

struct createCollection_input 
{

    uint64_t priceForDropMint;
    uint32_t volumn;
    uint32_t royalty;
    uint32_t maxSizePerOneId;
    uint8_t uri[64];
    bool typeOfCollection;

};

struct createCollection_output 
{

    uint32_t returnCode;

};

struct mint_input
{

    uint32_t royalty;
    uint32_t collectionId;
    uint8_t URI[64];
    uint8_t typeOfMint;						// 0 means that creator uses the his collection to mint the NFT. 1 means that creator mints the single NFT.

};

struct mint_output 
{

    uint32_t returnCode;

};

struct mintOfDrop_input 
{

    uint32_t collectionId;
    uint8_t URI[64];

};

struct mintOfDrop_output 
{

    uint32_t returnCode;

};

struct transfer_input 
{

    uint8_t receiver[32];
    uint32_t NFTid;

};

struct transfer_output 
{

    uint32_t returnCode;

};

struct listInMarket_input 
{

    uint64_t price;
    uint32_t NFTid;

};

struct listInMarket_output 
{

    uint32_t returnCode;

};

struct sell_input 
{

    uint32_t NFTid;
    bool methodOfPayment;

};

struct sell_output 
{

    uint32_t returnCode;

};

struct cancelSale_input 
{

    uint32_t NFTid;

};

struct cancelSale_output 
{

    uint32_t returnCode;

};

struct listInExchange_input 
{

    uint32_t possessedNFT;
    uint32_t anotherNFT;

};

struct listInExchange_output 
{

    uint32_t returnCode;

};

struct cancelExchange_input 
{

    uint32_t possessedNFT;
    
};

struct cancelExchange_output 
{

    uint32_t returnCode;

};

struct makeOffer_input 
{

    int64_t askPrice;
    uint32_t NFTid;
    bool paymentMethod;
    
};

struct makeOffer_output 
{

    uint32_t returnCode;

};

struct acceptOffer_input 
{

    uint32_t NFTid;

};

struct acceptOffer_output 
{

    uint32_t returnCode;

};

struct cancelOffer_input 
{

    uint32_t NFTid;

};

struct cancelOffer_output 
{

    uint32_t returnCode;

};

struct createTraditionalAuction_input
{

    uint64_t minPrice;
    uint32_t NFTId;
    uint32_t startYear;
    uint32_t startMonth;
    uint32_t startDay;
    uint32_t startHour;
    uint32_t endYear;
    uint32_t endMonth;
    uint32_t endDay;
    uint32_t endHour;
    bool paymentMethodOfAuction;
    
};

struct createTraditionalAuction_output
{

    uint32_t returnCode;

};

struct bidOnTraditionalAuction_input
{

    uint64_t price;
    uint32_t NFTId;
    bool paymentMethod;

};

struct bidOnTraditionalAuction_output
{

    uint32_t returnCode;

};

void settingCFBAndQubicPrice(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t cfbPrice, uint64_t qubicPrice)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        settingCFBAndQubicPrice_input input;
        unsigned char signature[64];
    } packet;

    packet.input.CFBPrice = cfbPrice;
    packet.input.QubicPrice = qubicPrice;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_SETTING_CFB_AND_QUBIC_PRICE;
    packet.transaction.inputSize = sizeof(settingCFBAndQubicPrice_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(settingCFBAndQubicPrice_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(settingCFBAndQubicPrice_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("settingCFBAndQubicPrice tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void createCollection(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t priceForDropMint, uint32_t volumn, uint32_t royalty, uint32_t maxsizePerOneid, bool typeOfCollection, const char* uri)
{
    auto qc = make_qc(nodeIp, nodePort);
    
    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        createCollection_input input;
        unsigned char signature[64];
    } packet;

    packet.input.maxSizePerOneId = maxsizePerOneid;
    packet.input.priceForDropMint = priceForDropMint;
    packet.input.royalty = royalty;
    packet.input.typeOfCollection = typeOfCollection;
    packet.input.volumn = volumn;
    memcpy(packet.input.uri, uri, 64);
    packet.input.uri[63] = 0;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    printf("volumn: %u royalty: %u\n%s", volumn, royalty, packet.input.uri);
    packet.transaction.inputType = QBAY_INPUT_TYPE_CREATE_COLLECTION;
    packet.transaction.inputSize = sizeof(createCollection_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(createCollection_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(createCollection_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("createCollection tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void mintNFT(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t royalty, uint32_t collectionId, const char* uri, bool typeOfMint)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        mint_input input;
        unsigned char signature[64];
    } packet;

    packet.input.collectionId = collectionId;
    packet.input.royalty = royalty;
    packet.input.typeOfMint = typeOfMint;
    memcpy(packet.input.URI, uri, 64);
    packet.input.URI[63] = 0;

    printf("%u %u %d %s", packet.input.collectionId, packet.input.royalty, packet.input.typeOfMint, packet.input.URI);

    packet.transaction.amount = 5000000;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_MINT_NFT;
    packet.transaction.inputSize = sizeof(mint_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(mint_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(mint_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("mint tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void mintNFTOfDrop(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t collectionId, uint64_t price, const char* uri)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        mintOfDrop_input input;
        unsigned char signature[64];
    } packet;

    packet.input.collectionId = collectionId;
    memcpy(packet.input.URI, uri, 64);

    packet.transaction.amount = price;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_MINT_OF_DROP;
    packet.transaction.inputSize = sizeof(mintOfDrop_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(mintOfDrop_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(mintOfDrop_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("mintOfDrop tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void transfer(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t NFTId, const char* identity)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        transfer_input input;
        unsigned char signature[64];
    } packet;

    packet.input.NFTid = NFTId;
    memcpy(packet.input.receiver, identity, 32);

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_TRANSFER;
    packet.transaction.inputSize = sizeof(transfer_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(transfer_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(transfer_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("transfer tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void listInMarket(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t price, uint32_t NFTId)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        listInMarket_input input;
        unsigned char signature[64];
    } packet;

    packet.input.NFTid = NFTId;
    packet.input.price = price;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_LIST_IN_MARKET;
    packet.transaction.inputSize = sizeof(listInMarket_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(listInMarket_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(listInMarket_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("listInMarket tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void sell(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t NFTId, bool methodOfPayment)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        sell_input input;
        unsigned char signature[64];
    } packet;

    packet.input.methodOfPayment = methodOfPayment;
    packet.input.NFTid = NFTId;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_SELL;
    packet.transaction.inputSize = sizeof(sell_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(sell_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(sell_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("sell tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void cancelSell(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t NFTId)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        cancelSale_input input;
        unsigned char signature[64];
    } packet;

    packet.input.NFTid = NFTId;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_CANCEL_SELL;
    packet.transaction.inputSize = sizeof(cancelSale_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(cancelSale_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(cancelSale_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("cancelSale tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void listInExchange(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t NFTId1, uint32_t NFTId2)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        listInExchange_input input;
        unsigned char signature[64];
    } packet;

    packet.input.anotherNFT = NFTId1;
    packet.input.possessedNFT = NFTId2;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_LIST_IN_EXCHANGE;
    packet.transaction.inputSize = sizeof(listInExchange_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(listInExchange_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(listInExchange_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("listInExchange tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void cancelExchange(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t NFTId)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        cancelExchange_input input;
        unsigned char signature[64];
    } packet;

    packet.input.possessedNFT = NFTId;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_CANCEL_EXCHANGE;
    packet.transaction.inputSize = sizeof(cancelExchange_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(cancelExchange_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(cancelExchange_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("cancelExchange tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void makeOffer(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, int64_t price, uint32_t NFTId, bool paymentMethod)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        makeOffer_input input;
        unsigned char signature[64];
    } packet;

    packet.input.askPrice = price;
    packet.input.NFTid = NFTId;
    packet.input.paymentMethod = paymentMethod;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_MAKE_OFFER;
    packet.transaction.inputSize = sizeof(makeOffer_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(makeOffer_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(makeOffer_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("makeOffer tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void acceptOffer(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t NFTId)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        acceptOffer_input input;
        unsigned char signature[64];
    } packet;

    packet.input.NFTid = NFTId;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_ACCEPT_OFFER;
    packet.transaction.inputSize = sizeof(acceptOffer_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(acceptOffer_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(acceptOffer_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("acceptOffer tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void cancelOffer(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t NFTId)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        cancelOffer_input input;
        unsigned char signature[64];
    } packet;

    packet.input.NFTid = NFTId;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_CANCEL_OFFER;
    packet.transaction.inputSize = sizeof(cancelOffer_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(cancelOffer_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(cancelOffer_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("cancelOffer tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void createTraditoinalAuction(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t minPrice, uint32_t NFTId, bool paymentMethodOfAuction, uint32_t startYear, uint32_t startMonth, uint32_t startDay, uint32_t startHour, uint32_t endYear, uint32_t endMonth, uint32_t endDay, uint32_t endHour)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        createTraditionalAuction_input input;
        unsigned char signature[64];
    } packet;

    packet.input.minPrice = minPrice;
    packet.input.NFTId = NFTId;
    packet.input.paymentMethodOfAuction = paymentMethodOfAuction;
    packet.input.startYear = startYear;
    packet.input.startMonth = startMonth;
    packet.input.startDay = startDay;
    packet.input.startHour = startHour;
    packet.input.endYear = endYear;
    packet.input.endMonth = endMonth;
    packet.input.endDay = endDay;
    packet.input.endHour = endHour;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_CREATE_TRADITIONAL_AUCTION;
    packet.transaction.inputSize = sizeof(createTraditionalAuction_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(createTraditionalAuction_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(createTraditionalAuction_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("createTraditionalAuction tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

struct changeStatusOfMarketPlace_input
{
    bool status;
};

struct changeStatusOfMarketPlace_output
{
    uint32_t returnCode;
};

void bidOnTraditoinalAuction(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t price, uint32_t NFTId, bool paymentMethod)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        bidOnTraditionalAuction_input input;
        unsigned char signature[64];
    } packet;

    packet.input.NFTId = NFTId;
    packet.input.paymentMethod = paymentMethod;
    packet.input.price = price;

    if(paymentMethod == 0)
    {
        packet.transaction.amount = price;
    }
    else 
    {
        packet.transaction.amount = 0;
    }
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_BID_ON_TRADITIONAL_AUCTION;
    packet.transaction.inputSize = sizeof(bidOnTraditionalAuction_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(bidOnTraditionalAuction_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(bidOnTraditionalAuction_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("bidOnTraditionalAuction tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void changeStatusOfMarket(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, bool statusOfMarket)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t privateKey[32] = {0};
    uint8_t sourcePublicKey[32] = {0};  
    uint8_t destPublicKey[32] = {0};
    uint8_t subseed[32] = {0};
    uint8_t digest[32] = {0};
    uint8_t signature[64] = {0};
    char publicIdentity[128] = {0};
    char txHash[128] = {0};
    getSubseedFromSeed((uint8_t*)seed, subseed);
    getPrivateKeyFromSubSeed(subseed, privateKey);
    getPublicKeyFromPrivateKey(privateKey, sourcePublicKey);
    const bool isLowerCase = false;
    getIdentityFromPublicKey(sourcePublicKey, publicIdentity, isLowerCase);
    ((uint64_t*)destPublicKey)[0] = QBAY_CONTRACT_INDEX;
    ((uint64_t*)destPublicKey)[1] = 0;
    ((uint64_t*)destPublicKey)[2] = 0;
    ((uint64_t*)destPublicKey)[3] = 0;

    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        Transaction transaction;
        changeStatusOfMarketPlace_input input;
        unsigned char signature[64];
    } packet;

    packet.input.status = statusOfMarket;

    packet.transaction.amount = 0;
    memcpy(packet.transaction.sourcePublicKey, sourcePublicKey, 32);
    memcpy(packet.transaction.destinationPublicKey, destPublicKey, 32);
    uint32_t currentTick = getTickNumberFromNode(qc);
    packet.transaction.tick = currentTick + scheduledTickOffset;
    packet.transaction.inputType = QBAY_INPUT_TYPE_CHANGE_STATUS_OF_MARKET;
    packet.transaction.inputSize = sizeof(changeStatusOfMarketPlace_input);
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(changeStatusOfMarketPlace_input),
                   digest,
                   32);
    sign(subseed, sourcePublicKey, digest, signature);
    memcpy(packet.signature, signature, 64);
    packet.header.setSize(sizeof(packet));
    packet.header.zeroDejavu();
    packet.header.setType(BROADCAST_TRANSACTION);
    qc->sendData((uint8_t *) &packet, packet.header.size());
    KangarooTwelve((unsigned char*)&packet.transaction,
                   sizeof(packet.transaction) + sizeof(changeStatusOfMarketPlace_input) + SIGNATURE_SIZE,
                   digest,
                   32); // recompute digest for txhash
    getTxHashFromDigest(digest, txHash);
    LOG("changeStatusOfMarket tx has been sent!\n");
    printReceipt(packet.transaction, txHash, reinterpret_cast<const uint8_t *>(&packet.input));
    LOG("run ./qubic-cli [...] -checktxontick %u %s\n", currentTick + scheduledTickOffset, txHash);
    LOG("to check your tx confirmation status\n");
}

void qbayTransferAssetManagementRights(const char* nodeIp, int nodePort,
    const char* seed,
    const char* pAssetName,
    const char* pIssuerInQubicFormat,
    uint32_t newManagingContractIndex,
    int64_t numberOfShares,
    uint32_t scheduledTickOffset)
{
    qbayTransferShareManagementRights_input v;
    memset(&v, 0, sizeof(v));
    getPublicKeyFromIdentity(pIssuerInQubicFormat, v.asset.issuer);
    v.asset.assetName = 0;
    memcpy(&v.asset.assetName, pAssetName, strlen(pAssetName));
    v.newManagingContractIndex = newManagingContractIndex;
    v.numberOfShares = numberOfShares;

    LOG("\nSending tx for transferring management rights ...\n");
    makeContractTransaction(nodeIp, nodePort, seed, QBAY_CONTRACT_INDEX, QBAY_INPUT_TYPE_TRANSFER_SHARE_MANAGAMENT_RIGHTS,
        1000000, sizeof(v), (uint8_t*)&v, scheduledTickOffset);
}

void getNumberOfNFTForUser(const char* nodeIp, int nodePort, const char* identity)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    
    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QBAYGetNumberOfNFTForUser_input input;
    } packet;
    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(QBAYGetNumberOfNFTForUser_input);
    packet.rcf.inputType = QBAY_INPUT_TYPE_GET_NUMBER_OF_NFT_FOR_USER;
    packet.rcf.contractIndex = QBAY_CONTRACT_INDEX;
    memcpy(packet.input.address, publicKey, 32);
    
    qc->sendData((uint8_t *) &packet, packet.header.size());

    QBAYGetNumberOfNFTForUser_output result;
    try
    {
        result = qc->receivePacketWithHeaderAs<QBAYGetNumberOfNFTForUser_output>();
    }
    catch (std::logic_error& e)
    {
        LOG("Failed to receive data\n");
        return;
    }

    if(result.numberOfNFT == 0) printf("%s don't have NFT.\n", identity);
    if(result.numberOfNFT == 1) printf("%s possessed %u NFT.\n", identity, result.numberOfNFT);
    if(result.numberOfNFT >= 2) printf("%s possessed %u NFTs.\n", identity, result.numberOfNFT);
}

void getInfoOfNFTUserPossessed(const char* nodeIp, int nodePort, uint32_t NFTNumber, const char* identity)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    
    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QBAYGetInfoOfNFTUserPossessed_input input;
    } packet;
    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(QBAYGetInfoOfNFTUserPossessed_input);
    packet.rcf.inputType = QBAY_INPUT_TYPE_GET_INFO_OF_NFT_USER_POSSESSED;
    packet.rcf.contractIndex = QBAY_CONTRACT_INDEX;
    memcpy(packet.input.address, publicKey, 32);
    packet.input.NFTNumber = NFTNumber;
    
    qc->sendData((uint8_t *) &packet, packet.header.size());

    QBAYGetInfoOfNFTUserPossessed_output result;
    try
    {
        result = qc->receivePacketWithHeaderAs<QBAYGetInfoOfNFTUserPossessed_output>();
    }
    catch (std::logic_error& e)
    {
        LOG("Failed to receive data\n");
        return;
    }

    char creator[128] = {0};
    char possesor[128] = {0};
    char askUser[128] = {0};
    char creatorOfAuction[128] = {0};

    getIdentityFromPublicKey(result.creator, creator, false);
    getIdentityFromPublicKey(result.possesor, possesor, false);
    getIdentityFromPublicKey(result.askUser, askUser, false);
    getIdentityFromPublicKey(result.creatorOfAuction, creatorOfAuction, false);

    printf("salePrice: %lld\n", result.salePrice);
    printf("askMaxPrice: %lld\n", result.askMaxPrice);
    printf("currentPriceOfAuction: %llu\n", result.currentPriceOfAuction);
    printf("royalty: %u\n", result.royalty);
    printf("NFTidForExchange: %u\n", result.NFTidForExchange);
    printf("creator: %s\n", creator);
    printf("possesor: %s\n", possesor);
    printf("askUser: %s\n", askUser);
    printf("creatorOfAuction: %s\n", creatorOfAuction);
    printf("URI: %s\n", result.URI);
    printf("statusOfAuction: %u\n", result.statusOfAuction);
    printf("AuctionStarted: year:%u month:%u day:%u hour:%u minute:%u second:%u\n", result.yearAuctionStarted, result.monthAuctionStarted, result.dayAuctionStarted, result.hourAuctionStarted, result.minuteAuctionStarted, result.secondAuctionStarted);
    printf("AuctionEnded: year:%u month:%u day:%u hour:%u minute:%u second:%u\n", result.yearAuctionEnded, result.monthAuctionEnded, result.dayAuctionEnded, result.hourAuctionEnded, result.minuteAuctionEnded, result.secondAuctionEnded);
    printf("statusOfSale: %d\n", result.statusOfSale);
    printf("statusOfAsk: %d\n", result.statusOfAsk);
    printf("paymentMethodOfAsk: %d\n", result.paymentMethodOfAsk);
    printf("statusOfExchange: %d\n", result.statusOfExchange);
    printf("paymentMethodOfAuction: %d\n", result.paymentMethodOfAuction);
    
}

void getInfoOfMarketplace(const char* nodeIp, int nodePort)
{
    auto qc = make_qc(nodeIp, nodePort);
    
    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QBAYGetInfoOfMarketplace_input input;
    } packet;

    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(QBAYGetInfoOfMarketplace_input);
    packet.rcf.inputType = QBAY_INPUT_TYPE_GET_INFO_OF_MARKETPLACE;
    packet.rcf.contractIndex = QBAY_CONTRACT_INDEX;
    packet.input.t = 10;
    
    qc->sendData((uint8_t *) &packet, packet.header.size());

    QBAYGetInfoOfMarketplace_output result;
    try
    {
        result = qc->receivePacketWithHeaderAs<QBAYGetInfoOfMarketplace_output>();
    }
    catch (std::logic_error& e)
    {
        LOG("Failed to receive data\n");
        return;
    }

    printf("priceOfCFB: %llu\npriceOfQubic: %llu\nnumberOfNFTIncoming: %llu\nnumberOfCollection: %u\nearnedQubic: %llu\nearnedCFB: %llu\nnumberOfNFT: %u\nMarketStatus: %d", result.priceOfCFB, result.priceOfQubic, result.numberOfNFTIncoming, result.numberOfCollection,result.earnedQubic, result.earnedCFB, result.numberOfNFT, result.statusOfMarketPlace);
}

void getInfoOfCollectionByCreator(const char* nodeIp, int nodePort, const char* identity, uint32_t collectionId)
{
    auto qc = make_qc(nodeIp, nodePort);

    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    
    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QBAYGetInfoOfCollectionByCreator_input input;
    } packet;

    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(QBAYGetInfoOfCollectionByCreator_input);
    packet.rcf.inputType = QBAY_INPUT_TYPE_GET_INFO_OF_COLLECTION_BY_CREATOR;
    packet.rcf.contractIndex = QBAY_CONTRACT_INDEX;
    memcpy(packet.input.creator, publicKey, 32);
    packet.input.orderOfCollection = collectionId;
    
    qc->sendData((uint8_t *) &packet, packet.header.size());

    QBAYGetInfoOfCollectionByCreator_output result;
    try
    {
        result = qc->receivePacketWithHeaderAs<QBAYGetInfoOfCollectionByCreator_output>();
    }
    catch (std::logic_error& e)
    {
        LOG("Failed to receive data\n");
        return;
    }

    printf("priceForDropMint: %llu\nidOfCollection: %u\nroyalty: %u\ncurrentSize: %u\nmaxSizeHoldingPerOneId: %u\ntypeOfCollection: %d\nURI: %s", result.priceForDropMint, result.idOfCollection, result.royalty, result.currentSize, result.maxSizeHoldingPerOneId, result.typeOfCollection, result.URI);
}

void getInfoOfCollectionById(const char* nodeIp, int nodePort, uint32_t collectionId)
{
    auto qc = make_qc(nodeIp, nodePort);
    
    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QBAYGetInfoOfCollectionById_input input;
    } packet;

    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(QBAYGetInfoOfCollectionById_input);
    packet.rcf.inputType = QBAY_INPUT_TYPE_GET_INFO_OF_COLLECTION_BY_ID;
    packet.rcf.contractIndex = QBAY_CONTRACT_INDEX;
    packet.input.idOfColletion = collectionId;
    
    qc->sendData((uint8_t *) &packet, packet.header.size());

    QBAYGetInfoOfCollectionById_output result;
    try
    {
        result = qc->receivePacketWithHeaderAs<QBAYGetInfoOfCollectionById_output>();
    }
    catch (std::logic_error& e)
    {
        LOG("Failed to receive data\n");
        return;
    }

    char creator[128] = {0};
    getIdentityFromPublicKey(result.creator, creator, false);

    printf("creator: %s\npriceForDropMint: %llu\nroyalty: %u\ncurrentSize: %u\nmaxSizeHoldingPerOneId: %u\ntypeOfCollection: %d\nURI: %s", creator, result.priceForDropMint, result.royalty, result.currentSize,result.maxSizeHoldingPerOneId, result.typeOfCollection, result.URI);
}

void getIncommingAuctions(const char* nodeIp, int nodePort, uint32_t offset, uint32_t count)
{
    auto qc = make_qc(nodeIp, nodePort);
    
    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QBAYGetIncomingAuctions_input input;
    } packet;

    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(QBAYGetIncomingAuctions_input);
    packet.rcf.inputType = QBAY_INPUT_TYPE_GET_INCOMMING_AUCTIONS;
    packet.rcf.contractIndex = QBAY_CONTRACT_INDEX;
    packet.input.offset = offset;
    packet.input.count = count;
    
    qc->sendData((uint8_t *) &packet, packet.header.size());

    QBAYGetIncomingAuctions_output result;
    try
    {
        result = qc->receivePacketWithHeaderAs<QBAYGetIncomingAuctions_output>();
    }
    catch (std::logic_error& e)
    {
        LOG("Failed to receive data\n");
        return;
    }

    for(uint32_t i = 0 ; i < count; i++)
    {
        printf("%d ", result.NFTId[i]);
    }

}

void getInfoOfNFTById(const char* nodeIp, int nodePort, uint32_t NFTId)
{
    auto qc = make_qc(nodeIp, nodePort);
    
    #pragma pack(1)
    struct {
        RequestResponseHeader header;
        RequestContractFunction rcf;
        QBAYGetInfoOfNFTById_input input;
    } packet;
    packet.header.setSize(sizeof(packet));
    packet.header.randomizeDejavu();
    packet.header.setType(RequestContractFunction::type());
    packet.rcf.inputSize = sizeof(QBAYGetInfoOfNFTById_input);
    packet.rcf.inputType = QBAY_INPUT_TYPE_GET_INFO_OF_NFT_BY_ID;
    packet.rcf.contractIndex = QBAY_CONTRACT_INDEX;
    packet.input.NFTId = NFTId;
    
    qc->sendData((uint8_t *) &packet, packet.header.size());

    QBAYGetInfoOfNFTById_output result;
    try
    {
        result = qc->receivePacketWithHeaderAs<QBAYGetInfoOfNFTById_output>();
    }
    catch (std::logic_error& e)
    {
        LOG("Failed to receive data\n");
        return;
    }

    char creator[128] = {0};
    char possesor[128] = {0};
    char askUser[128] = {0};
    char creatorOfAuction[128] = {0};

    getIdentityFromPublicKey(result.creator, creator, false);
    getIdentityFromPublicKey(result.possesor, possesor, false);
    getIdentityFromPublicKey(result.askUser, askUser, false);
    getIdentityFromPublicKey(result.creatorOfAuction, creatorOfAuction, false);

    printf("salePrice: %lld\n", result.salePrice);
    printf("askMaxPrice: %lld\n", result.askMaxPrice);
    printf("currentPriceOfAuction: %llu\n", result.currentPriceOfAuction);
    printf("royalty: %u\n", result.royalty);
    printf("NFTidForExchange: %u\n", result.NFTidForExchange);
    printf("creator: %s\n", creator);
    printf("possesor: %s\n", possesor);
    printf("askUser: %s\n", askUser);
    printf("creatorOfAuction: %s\n", creatorOfAuction);
    printf("URI: %s\n", result.URI);
    printf("statusOfAuction: %u\n", result.statusOfAuction);
    printf("AuctionStarted: year:%u month:%u day:%u hour:%u minute:%u second:%u\n", result.yearAuctionStarted, result.monthAuctionStarted, result.dayAuctionStarted, result.hourAuctionStarted, result.minuteAuctionStarted, result.secondAuctionStarted);
    printf("AuctionEnded: year:%u month:%u day:%u hour:%u minute:%u second:%u\n", result.yearAuctionEnded, result.monthAuctionEnded, result.dayAuctionEnded, result.hourAuctionEnded, result.minuteAuctionEnded, result.secondAuctionEnded);
    printf("statusOfSale: %d\n", result.statusOfSale);
    printf("statusOfAsk: %d\n", result.statusOfAsk);
    printf("paymentMethodOfAsk: %d\n", result.paymentMethodOfAsk);
    printf("statusOfExchange: %d\n", result.statusOfExchange);
    printf("paymentMethodOfAuction: %d\n", result.paymentMethodOfAuction);
    
}