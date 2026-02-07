#include <cstring>
#include <cinttypes>

#include "qusino.h"
#include "wallet_utils.h"
#include "key_utils.h"
#include "sanity_check.h"
#include "logger.h"

bool qusinoParseUriHex(const char* hexStr, uint8_t* outUri64)
{
    if (!hexStr || !outUri64 || strlen(hexStr) != 128)
        return false;
    for (int i = 0; i < 64; i++)
    {
        unsigned int b;
        if (sscanf(hexStr + (i * 2), "%2x", &b) != 1)
            return false;
        outUri64[i] = (uint8_t)b;
    }
    return true;
}

void qusinoGetUserAssetVolume(const char* nodeIp, int nodePort, const char* identity)
{
    Qusino_getUserAssetVolume_input input;
    memset(&input, 0, sizeof(input));
    sanityCheckIdentity(identity);
    getPublicKeyFromIdentity(identity, input.user);

    Qusino_getUserAssetVolume_output output;
    memset(&output, 0, sizeof(output));
    if (!runContractFunction(nodeIp, nodePort, QUSINO_CONTRACT_INDEX,
            QUSINO_FUNC_GET_USER_ASSET_VOLUME,
            &input, sizeof(input), &output, sizeof(output), nullptr))
    {
        LOG("ERROR: Didn't receive valid response from getUserAssetVolume\n");
        return;
    }
    LOG("User %s:\n  STAR: %" PRIu64 "\n  QSC:  %" PRIu64 "\n", identity, output.STARAmount, output.QSCAmount);
}

void qusinoGetUserStakingInfo(const char* nodeIp, int nodePort, const char* identity, uint32_t offset)
{
    Qusino_getUserStakingInfo_input input;
    memset(&input, 0, sizeof(input));
    sanityCheckIdentity(identity);
    getPublicKeyFromIdentity(identity, input.user);
    input.offset = offset;

    Qusino_getUserStakingInfo_output output;
    memset(&output, 0, sizeof(output));
    if (!runContractFunction(nodeIp, nodePort, QUSINO_CONTRACT_INDEX,
            QUSINO_FUNC_GET_USER_STAKING_INFO,
            &input, sizeof(input), &output, sizeof(output), nullptr))
    {
        LOG("ERROR: Didn't receive valid response from getUserStakingInfo\n");
        return;
    }
    LOG("Staking entries (count=%" PRIu32 "):\n", output.counts);
    uint32_t n = output.counts > offset ? output.counts - offset : 0;
    if (n > 128) n = 128;
    for (uint32_t i = 0; i < n; i++)
    {
        const char* assetStr = output.typeOfAsset[i] == 1 ? "STAR" : (output.typeOfAsset[i] == 2 ? "QSC" : "QST");
        const char* periodStr = output.type[i] == 1 ? "1m" : (output.type[i] == 2 ? "3m" : (output.type[i] == 3 ? "6m" : "12m"));
        LOG("  [%" PRIu32 "] amount=%" PRIu64 " type=%s asset=%s stakedEpoch=%" PRIu32 "\n",
            i, output.amount[i], periodStr, assetStr, output.stakedEpoch[i]);
    }
}

void qusinoGetFailedGameList(const char* nodeIp, int nodePort, uint32_t offset)
{
    Qusino_getFailedGameList_input input;
    input.offset = offset;

    Qusino_getFailedGameList_output output;
    memset(&output, 0, sizeof(output));
    if (!runContractFunction(nodeIp, nodePort, QUSINO_CONTRACT_INDEX,
            QUSINO_FUNC_GET_FAILED_GAME_LIST,
            &input, sizeof(input), &output, sizeof(output), nullptr))
    {
        LOG("ERROR: Didn't receive valid response from getFailedGameList\n");
        return;
    }
    LOG("Failed games (offset %" PRIu32 "):\n", offset);
    for (int i = 0; i < 32; i++)
    {
        if (output.games[i].proposedEpoch == 0 && output.games[i].yesVotes == 0 && output.games[i].noVotes == 0)
            continue;
        char proposerId[128] = { 0 };
        getIdentityFromPublicKey(output.games[i].proposer, proposerId, false);
        LOG("  [%d] proposer=%s yes=%" PRIu32 " no=%" PRIu32 " proposedEpoch=%" PRIu32 " URI_hex=%.32s...\n",
            i, proposerId, output.games[i].yesVotes, output.games[i].noVotes, output.games[i].proposedEpoch,
            output.games[i].URI);
    }
}

void qusinoGetSCInfo(const char* nodeIp, int nodePort)
{
    Qusino_getSCInfo_output output;
    memset(&output, 0, sizeof(output));
    if (!runContractFunction(nodeIp, nodePort, QUSINO_CONTRACT_INDEX,
            QUSINO_FUNC_GET_SC_INFO, nullptr, 0, &output, sizeof(output), nullptr))
    {
        LOG("ERROR: Didn't receive valid response from getSCInfo\n");
        return;
    }
    LOG("Qusino SC Info:\n");
    LOG("  QSCCirculatingSupply:  %" PRIu64 "\n", output.QSCCirclatingSupply);
    LOG("  STARCirculatingSupply: %" PRIu64 "\n", output.STARCirclatingSupply);
    LOG("  totalStakedSTAR:       %" PRIu64 "\n", output.totalStakedSTAR);
    LOG("  totalStakedQSC:       %" PRIu64 "\n", output.totalStakedQSC);
    LOG("  totalStakedQST:       %" PRIu64 "\n", output.totalStakedQST);
    LOG("  burntSTAR:            %" PRIu64 "\n", output.burntSTAR);
    LOG("  epochRevenue:         %" PRIu64 "\n", output.epochRevenue);
    LOG("  maxGameIndex:         %" PRIu64 "\n", output.maxGameIndex);
    LOG("  numberOfStakers:      %" PRIu64 "\n", output.numberOfStakers);
    LOG("  QSTAmountForSale:     %" PRIu64 "\n", output.QSTAmountForSale);
}

void qusinoGetActiveGameList(const char* nodeIp, int nodePort, uint32_t offset)
{
    Qusino_getActiveGameList_input input;
    input.offset = offset;

    Qusino_getActiveGameList_output output;
    memset(&output, 0, sizeof(output));
    if (!runContractFunction(nodeIp, nodePort, QUSINO_CONTRACT_INDEX,
            QUSINO_FUNC_GET_ACTIVE_GAME_LIST,
            &input, sizeof(input), &output, sizeof(output), nullptr))
    {
        LOG("ERROR: Didn't receive valid response from getActiveGameList\n");
        return;
    }
    LOG("Active games (offset %" PRIu32 "):\n", offset);
    for (int i = 0; i < 32; i++)
    {
        if (output.games[i].proposedEpoch == 0 && output.games[i].yesVotes == 0 && output.games[i].noVotes == 0)
            continue;
        char proposerId[128] = { 0 };
        getIdentityFromPublicKey(output.games[i].proposer, proposerId, false);
        LOG("  [%d] gameIndex=%" PRIu64 " proposer=%s yes=%" PRIu32 " no=%" PRIu32 " proposedEpoch=%" PRIu32 "\n",
            i, output.gameIndexes[i], proposerId, output.games[i].yesVotes, output.games[i].noVotes, output.games[i].proposedEpoch);
    }
}

void qusinoBuyQST(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, bool useQSC, uint32_t scheduledTickOffset)
{
    Qusino_buyQST_input input;
    memset(&input, 0, sizeof(input));
    input.amount = amount;
    input.type = useQSC ? 1 : 0;
    // For Qubic (type 0), contract uses invocationReward(); send at least amount*minPrice (use 1000 as safe estimate; contract refunds excess).
    uint64_t fee = useQSC ? 0 : (amount * 1000ULL);
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_BUY_QST, fee, sizeof(input), &input, scheduledTickOffset);
    LOG("BuyQST transaction sent (type=%s).\n", useQSC ? "QSC" : "Qubic");
}

void qusinoEarnSTAR(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, uint32_t scheduledTickOffset)
{
    Qusino_earnSTAR_input input;
    input.amount = amount;
    uint64_t fee = amount * 1000ULL;  // QUSINO_STAR_PRICE = 1000
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_EARN_STAR, fee, sizeof(input), &input, scheduledTickOffset);
    LOG("EarnSTAR transaction sent.\n");
}

void qusinoEarnQSC(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, uint32_t scheduledTickOffset)
{
    Qusino_earnQSC_input input;
    input.amount = amount;
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_EARN_QSC, 0, sizeof(input), &input, scheduledTickOffset);
    LOG("EarnQSC transaction sent (transfers QST from contract to you, adds QSC+STAR bonus).\n");
}

void qusinoTransferSTAROrQSC(const char* nodeIp, int nodePort, const char* seed, const char* destIdentity, uint64_t amount, bool transferSTAR, uint32_t scheduledTickOffset)
{
    Qusino_transferSTAROrQSC_input input;
    memset(&input, 0, sizeof(input));
    sanityCheckIdentity(destIdentity);
    getPublicKeyFromIdentity(destIdentity, input.dest);
    input.amount = amount;
    input.type = transferSTAR ? 1 : 0;
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_TRANSFER_STAR_OR_QSC, 0, sizeof(input), &input, scheduledTickOffset);
    LOG("Transfer %s transaction sent.\n", transferSTAR ? "STAR" : "QSC");
}

void qusinoStakeAssets(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, uint32_t stakingType, uint32_t typeOfAsset, uint32_t scheduledTickOffset)
{
    if (stakingType < 1 || stakingType > 4)
    {
        LOG("ERROR: stakingType must be 1 (1m), 2 (3m), 3 (6m), or 4 (12m).\n");
        return;
    }
    if (typeOfAsset < 1 || typeOfAsset > 3)
    {
        LOG("ERROR: typeOfAsset must be 1 (STAR), 2 (QSC), or 3 (QST).\n");
        return;
    }
    Qusino_stakeAssets_input input;
    input.amount = amount;
    input.type = stakingType;
    input.typeOfAsset = typeOfAsset;
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_STAKE_ASSETS, 0, sizeof(input), &input, scheduledTickOffset);
    LOG("StakeAssets transaction sent.\n");
}

void qusinoSubmitGame(const char* nodeIp, int nodePort, const char* seed, const uint8_t* URI64, uint32_t scheduledTickOffset)
{
    Qusino_submitGame_input input;
    memset(&input, 0, sizeof(input));
    if (URI64)
        memcpy(input.URI, URI64, 64);
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_SUBMIT_GAME, QUSINO_GAME_SUBMIT_FEE, sizeof(input), &input, scheduledTickOffset);
    LOG("SubmitGame transaction sent (fee=%" PRIu64 ").\n", (uint64_t)QUSINO_GAME_SUBMIT_FEE);
}

void qusinoVoteInGameProposal(const char* nodeIp, int nodePort, const char* seed, const uint8_t* URI64, uint64_t gameIndex, bool yesNo, uint32_t scheduledTickOffset)
{
    Qusino_voteInGameProposal_input input;
    memset(&input, 0, sizeof(input));
    if (URI64)
        memcpy(input.URI, URI64, 64);
    input.gameIndex = gameIndex;
    input.yesNo = yesNo ? 1 : 0;
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_VOTE_IN_GAME_PROPOSAL, 0, sizeof(input), &input, scheduledTickOffset);
    LOG("VoteInGameProposal transaction sent.\n");
}

void qusinoDepositQSTForSale(const char* nodeIp, int nodePort, const char* seed, uint64_t amount, uint32_t scheduledTickOffset)
{
    Qusino_depositQSTForSale_input input;
    input.amount = amount;
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_DEPOSIT_QST_FOR_SALE, 0, sizeof(input), &input, scheduledTickOffset);
    LOG("DepositQSTForSale transaction sent.\n");
}

void qusinoTransferShareManagementRights(const char* nodeIp, int nodePort, const char* seed, const char* issuerIdentity, const char* assetName, int64_t numberOfShares, uint32_t newManagingContractIndex, uint32_t scheduledTickOffset)
{
    Qusino_TransferShareManagementRights_input input;
    memset(&input, 0, sizeof(input));
    sanityCheckIdentity(issuerIdentity);
    sanityCheckValidAssetName(assetName);
    getPublicKeyFromIdentity(issuerIdentity, input.asset.issuer);
    input.asset.assetName = assetNameFromString(assetName);
    input.numberOfShares = numberOfShares;
    input.newManagingContractIndex = newManagingContractIndex;
    uint64_t fee = 100;  // contract's transferRightsFee
    makeContractTransaction(nodeIp, nodePort, seed, QUSINO_CONTRACT_INDEX,
        QUSINO_PROC_TRANSFER_SHARE_MANAGEMENT_RIGHTS, fee, sizeof(input), &input, scheduledTickOffset);
    LOG("TransferShareManagementRights transaction sent.\n");
}
