#pragma once

#include "structs.h"

struct QBAYGetNumberOfNFTForUser_input
{
    uint8_t address[32];
};

struct QBAYGetNumberOfNFTForUser_output
{
    uint32_t numberOfNFT;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct QBAYGetInfoOfNFTUserPossessed_input
{
    uint8_t address[32];
    uint32_t NFTNumber;
};

#pragma pack(1)

struct QBAYGetInfoOfNFTUserPossessed_output
{
    uint8_t creator[32];                        // Identity of NFT creator
    uint8_t possesor[32];						// Identity of NFT possesor
    uint8_t askUser[32];					    // Identity of Asked user
    uint8_t creatorOfAuction[32];				// Creator of Auction
    int64_t salePrice;							// This price should be set by possesor
    int64_t askMaxPrice;						// This price is the max of asked prices
    uint64_t currentPriceOfAuction;				// This price is the start price of auctions
    uint32_t royalty;							// Percent from 0 ~ 100
    uint32_t NFTidForExchange;					// NFT Id that want to exchange
    uint8_t URI[64];			                // URI for this NFT
    uint8_t statusOfAuction;					// Status of Auction(0 means that there is no auction, 1 means that tranditional Auction is started, 2 means that one user buyed the NFT in traditinoal auction, 3 means that dutch auction is started)
    uint8_t yearAuctionStarted;
    uint8_t monthAuctionStarted;
    uint8_t dayAuctionStarted;
    uint8_t hourAuctionStarted;
    uint8_t minuteAuctionStarted;
    uint8_t secondAuctionStarted;
    uint8_t yearAuctionEnded;
    uint8_t monthAuctionEnded;
    uint8_t dayAuctionEnded;
    uint8_t hourAuctionEnded;
    uint8_t minuteAuctionEnded;
    uint8_t secondAuctionEnded;
    bool statusOfSale;							// Status of Sale, 0 means possesor don't want to sell
    bool statusOfAsk;							// Status of Ask
    bool paymentMethodOfAsk;					// 0 means the asked user want to buy using $Qubic, 1 means that want to buy using $CFB
    bool statusOfExchange;						// Status of Exchange
    bool paymentMethodOfAuction;				// 0 means the user can buy using only $Qubic, 1 means that can buy using only $CFB

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct QBAYGetInfoOfMarketplace_input
{
    uint32_t t;
};

struct QBAYGetInfoOfMarketplace_output
{
    uint64_t priceOfCFB;							// The amount of $CFB per 1 USD
    uint64_t priceOfQubic;						// The amount of $Qubic per 1 USD
    uint64_t numberOfNFTIncoming;					// The number of NFT after minting of all NFTs for collection
    uint64_t earnedQubic;
    uint64_t earnedCFB;
    uint32_t numberOfCollection;
    uint32_t numberOfNFT;
    bool statusOfMarketPlace;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct QBAYGetInfoOfCollectionByCreator_input
{
    uint8_t creator[32];
    uint32_t orderOfCollection;
};

struct QBAYGetInfoOfCollectionByCreator_output
{
    uint64_t priceForDropMint;
    uint32_t idOfCollection;
    uint32_t royalty;
    int32_t currentSize;	
    uint32_t maxSizeHoldingPerOneId;   
    uint8_t URI[64];
    bool typeOfCollection;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct QBAYGetInfoOfCollectionById_input
{
    uint32_t idOfColletion;
};

struct QBAYGetInfoOfCollectionById_output
{
    uint8_t creator[32];	
    uint64_t priceForDropMint;
    uint32_t royalty;
    int32_t currentSize;	
    uint32_t maxSizeHoldingPerOneId;   
    uint8_t URI[64];
    bool typeOfCollection;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct QBAYGetIncomingAuctions_input
{
    uint32_t offset;
    uint32_t count;
};

struct QBAYGetIncomingAuctions_output
{
    uint32_t NFTId[1024];

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct QBAYGetInfoOfNFTById_input
{
    uint32_t NFTId;
};

#pragma pack(1)

struct QBAYGetInfoOfNFTById_output
{
    uint8_t creator[32];                        // Identity of NFT creator
    uint8_t possesor[32];						// Identity of NFT possesor
    uint8_t askUser[32];					    // Identity of Asked user
    uint8_t creatorOfAuction[32];				// Creator of Auction
    int64_t salePrice;							// This price should be set by possesor
    int64_t askMaxPrice;						// This price is the max of asked prices
    uint64_t currentPriceOfAuction;				// This price is the start price of auctions
    uint32_t royalty;							// Percent from 0 ~ 100
    uint32_t NFTidForExchange;					// NFT Id that want to exchange
    uint8_t URI[64];			                // URI for this NFT
    uint8_t statusOfAuction;					// Status of Auction(0 means that there is no auction, 1 means that tranditional Auction is started, 2 means that one user buyed the NFT in traditinoal auction, 3 means that dutch auction is started)
    uint8_t yearAuctionStarted;
    uint8_t monthAuctionStarted;
    uint8_t dayAuctionStarted;
    uint8_t hourAuctionStarted;
    uint8_t minuteAuctionStarted;
    uint8_t secondAuctionStarted;
    uint8_t yearAuctionEnded;
    uint8_t monthAuctionEnded;
    uint8_t dayAuctionEnded;
    uint8_t hourAuctionEnded;
    uint8_t minuteAuctionEnded;
    uint8_t secondAuctionEnded;
    bool statusOfSale;							// Status of Sale, 0 means possesor don't want to sell
    bool statusOfAsk;							// Status of Ask
    bool paymentMethodOfAsk;					// 0 means the asked user want to buy using $Qubic, 1 means that want to buy using $CFB
    bool statusOfExchange;						// Status of Exchange
    bool paymentMethodOfAuction;				// 0 means the user can buy using only $Qubic, 1 means that can buy using only $CFB

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

#pragma pack (push, 1)
struct qbayTransferShareManagementRights_input
{
    struct
    {
        uint8_t issuer[32];
        uint64_t assetName;
    } asset;
    int64_t numberOfShares;
    uint32_t newManagingContractIndex;
};
#pragma pack (pop)
static_assert(sizeof(qbayTransferShareManagementRights_input) == 52, "wrong implementation");


void settingCFBAndQubicPrice(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset,
    uint64_t cfbPrice, 
    uint64_t qubicPrice);
    
void createCollection(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t priceForDropMint, 
    uint32_t volumn, 
    uint32_t royalty, 
    uint32_t maxsizePerOneid, 
    bool typeOfCollection, 
    const char* uri);

void mintNFT(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t royalty, 
    uint32_t collectionId, 
    const char* uri, 
    bool typeOfMint);

void mintNFTOfDrop(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t collectionId, 
    uint64_t price, 
    const char* uri);

void transfer(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t NFTId, 
    const char* identity);

void listInMarket(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint64_t price, 
    uint32_t NFTId);

void sell(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t NFTId, 
    bool methodOfPayment);

void cancelSell(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t NFTId);

void listInExchange(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t NFTId1, 
    uint32_t NFTId2);

void cancelExchange(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t NFTId);

void makeOffer(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    int64_t price, 
    uint32_t NFTId, 
    bool paymentMethod);

void acceptOffer(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t NFTId);

void cancelOffer(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint32_t NFTId);

void createTraditoinalAuction(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint64_t minPrice, 
    uint32_t NFTId, 
    bool paymentMethodOfAuction, 
    uint32_t startYear, 
    uint32_t startMonth, 
    uint32_t startDay, 
    uint32_t startHour, 
    uint32_t endYear, 
    uint32_t endMonth, 
    uint32_t endDay, 
    uint32_t endHour);

void bidOnTraditoinalAuction(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    uint64_t price, 
    uint32_t NFTId, 
    bool paymentMethod);

void getNumberOfNFTForUser(const char* nodeIp, int nodePort, 
    const char* identity);

void getInfoOfNFTUserPossessed(const char* nodeIp, int nodePort, 
    uint32_t NFTNumber, 
    const char* identity);

void getInfoOfMarketplace(const char* nodeIp, int nodePort);

void getInfoOfCollectionByCreator(const char* nodeIp, int nodePort, 
    const char* identity, 
    uint32_t collectionId);

void getInfoOfCollectionById(const char* nodeIp, int nodePort, 
    uint32_t collectionId);

void getIncommingAuctions(const char* nodeIp, int nodePort, 
    uint32_t offset, 
    uint32_t count);

void getInfoOfNFTById(const char* nodeIp, int nodePort, 
    uint32_t nftId);

void changeStatusOfMarket(const char* nodeIp, int nodePort, 
    const char* seed, 
    uint32_t scheduledTickOffset, 
    bool statusOfMarket);

void qbayTransferAssetManagementRights(const char* nodeIp, int nodePort,
    const char* seed,
    const char* pAssetName,
    const char* pIssuerInQubicFormat,
    uint32_t newManagingContractIndex,
    int64_t numberOfShares,
    uint32_t scheduledTickOffset);