#pragma once

#include <stdint.h>
#include "structs.h"
#include "asset_utils.h"

// Qusino contract index (adjust if deployed at a different index)
constexpr unsigned int QUSINO_CONTRACT_INDEX = 24;

// Function IDs (match REGISTER_USER_FUNCTION order in contract)
constexpr uint16_t QUSINO_FUNC_GET_USER_ASSET_VOLUME = 1;
constexpr uint16_t QUSINO_FUNC_GET_USER_STAKING_INFO = 2;
constexpr uint16_t QUSINO_FUNC_GET_FAILED_GAME_LIST = 3;
constexpr uint16_t QUSINO_FUNC_GET_SC_INFO = 4;
constexpr uint16_t QUSINO_FUNC_GET_ACTIVE_GAME_LIST = 5;

// Procedure IDs (match REGISTER_USER_PROCEDURE order in contract)
constexpr uint16_t QUSINO_PROC_BUY_QST = 1;
constexpr uint16_t QUSINO_PROC_EARN_STAR = 2;
constexpr uint16_t QUSINO_PROC_EARN_QSC = 3;
constexpr uint16_t QUSINO_PROC_TRANSFER_STAR_OR_QSC = 4;
constexpr uint16_t QUSINO_PROC_STAKE_ASSETS = 5;
constexpr uint16_t QUSINO_PROC_SUBMIT_GAME = 6;
constexpr uint16_t QUSINO_PROC_VOTE_IN_GAME_PROPOSAL = 7;
constexpr uint16_t QUSINO_PROC_DEPOSIT_QST_FOR_SALE = 8;
constexpr uint16_t QUSINO_PROC_TRANSFER_SHARE_MANAGEMENT_RIGHTS = 9;

// Fee constants from contract
constexpr uint64_t QUSINO_GAME_SUBMIT_FEE = 100000000ULL;

// Asset type constants (match contract)
constexpr uint8_t QUSINO_ASSET_TYPE_QUBIC = 0;
constexpr uint8_t QUSINO_ASSET_TYPE_QSC = 1;
constexpr uint8_t QUSINO_ASSET_TYPE_STAR = 2;
constexpr uint8_t QUSINO_ASSET_TYPE_QST = 3;
// Duration constants for staking (match contract)
constexpr uint8_t QUSINO_DURATION_1_MONTH = 1;
constexpr uint8_t QUSINO_DURATION_3_MONTHS = 2;
constexpr uint8_t QUSINO_DURATION_6_MONTHS = 3;
constexpr uint8_t QUSINO_DURATION_12_MONTHS = 4;

// --- Contract I/O structs (layout must match Qusino smart contract) ---

struct Qusino_getUserAssetVolume_input
{
    uint8_t user[32];
};

struct Qusino_getUserAssetVolume_output
{
    uint64_t STARAmount;
    uint64_t QSCAmount;
};

struct Qusino_getUserStakingInfo_input
{
    uint8_t user[32];
    uint32_t offset;
};

struct Qusino_stakeInfo
{
    uint64_t amount;
    uint32_t type;
    uint32_t stakedEpoch;
    uint32_t typeOfAsset;
};

struct Qusino_getUserStakingInfo_output
{
    uint64_t amount[128];
    uint32_t type[128];
    uint32_t stakedEpoch[128];
    uint32_t typeOfAsset[128];
    uint32_t counts;
};

struct Qusino_gameInfo
{
    uint8_t URI[64];
    uint8_t proposer[32];
    uint32_t yesVotes;
    uint32_t noVotes;
    uint32_t proposedEpoch;
};

struct Qusino_getFailedGameList_input
{
    uint32_t offset;
};

struct Qusino_getFailedGameList_output
{
    Qusino_gameInfo games[32];
};

struct Qusino_getSCInfo_output
{
    uint64_t QSCCirclatingSupply;
    uint64_t STARCirclatingSupply;
    uint64_t totalStakedSTAR;
    uint64_t totalStakedQSC;
    uint64_t totalStakedQST;
    uint64_t burntSTAR;
    uint64_t epochRevenue;
    uint64_t maxGameIndex;
    uint64_t numberOfStakers;
    uint64_t QSTAmountForSale;
};

struct Qusino_getActiveGameList_input
{
    uint32_t offset;
};

struct Qusino_getActiveGameList_output
{
    Qusino_gameInfo games[32];
    uint64_t gameIndexes[32];
};

// Procedure input structs (match contract: uint8 type fields)
struct Qusino_buyQST_input
{
    uint64_t amount;
    uint8_t type;  // QUSINO_ASSET_TYPE_QUBIC (0) or QUSINO_ASSET_TYPE_QSC (1)
    uint8_t _padding[7];
};

struct Qusino_earnSTAR_input
{
    uint64_t amount;
};

struct Qusino_earnQSC_input
{
    uint64_t amount;
};

struct Qusino_transferSTAROrQSC_input
{
    uint8_t dest[32];
    uint64_t amount;
    uint8_t type;  // QUSINO_ASSET_TYPE_STAR (2) or QUSINO_ASSET_TYPE_QSC (1)
    uint8_t _padding[7];
};

struct Qusino_stakeAssets_input
{
    uint64_t amount;
    uint8_t typeOfAsset;  // QUSINO_ASSET_TYPE_QSC (1), STAR (2), QST (3)
    uint8_t type;         // QUSINO_DURATION_1_MONTH (1), 3_MONTHS (2), 6_MONTHS (3), 12_MONTHS (4)
};

struct Qusino_submitGame_input
{
    uint8_t URI[64];
};

struct Qusino_voteInGameProposal_input
{
    uint8_t URI[64];
    uint64_t gameIndex;
    uint8_t yesNo;  // 1 - yes, 0 - no
    uint8_t _padding[7];
};

struct Qusino_depositQSTForSale_input
{
    uint64_t amount;
};

struct Qusino_TransferShareManagementRights_input
{
    qpi::Asset asset;
    int64_t numberOfShares;
    uint32_t newManagingContractIndex;
    uint32_t _padding;
};

// --- API declarations ---

void qusinoGetUserAssetVolume(const char* nodeIp, int nodePort, const char* identity);
void qusinoGetUserStakingInfo(const char* nodeIp, int nodePort, const char* identity, uint32_t offset);
void qusinoGetFailedGameList(const char* nodeIp, int nodePort, uint32_t offset);
void qusinoGetSCInfo(const char* nodeIp, int nodePort);
void qusinoGetActiveGameList(const char* nodeIp, int nodePort, uint32_t offset);

void qusinoBuyQST(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, bool useQSC, uint32_t scheduledTickOffset);
void qusinoEarnSTAR(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, uint32_t scheduledTickOffset);
void qusinoEarnQSC(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, uint32_t scheduledTickOffset);
void qusinoTransferSTAROrQSC(const char* nodeIp, int nodePort, const char* seed, const char* destIdentity, uint64_t amount, bool transferSTAR, uint32_t scheduledTickOffset);
void qusinoStakeAssets(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, uint32_t stakingType, uint32_t typeOfAsset, uint32_t scheduledTickOffset);
void qusinoSubmitGame(const char* nodeIp, int nodePort, const char* seed, const uint8_t* URI64, uint32_t scheduledTickOffset);
void qusinoVoteInGameProposal(const char* nodeIp, int nodePort, const char* seed, const uint8_t* URI64, uint64_t gameIndex, bool yesNo, uint32_t scheduledTickOffset);
void qusinoDepositQSTForSale(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, uint32_t scheduledTickOffset);
void qusinoTransferShareManagementRights(const char* nodeIp, int nodePort, const char* seed, const char* issuerIdentity, const char* assetName, int64_t numberOfShares, uint32_t newManagingContractIndex, uint32_t scheduledTickOffset);

// Parse 128 hex chars into 64-byte URI. Returns true on success.
bool qusinoParseUriHex(const char* hexStr, uint8_t* outUri64);
