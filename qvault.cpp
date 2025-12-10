#include <cstdint>
#include <cstring>

#include "wallet_utils.h"
#include "key_utils.h"
#include "logger.h"
#include "qvault.h"

#define QVAULT_CONTRACT_INDEX 10

// QVAULT FUNCTIONS
#define QVAULT_GETDATA 1

// QVAULT PROCEDURES
#define QVAULT_SUBMITAUTHADDRESS 1
#define QVAULT_CHANGEAUTHADDRESS 2
#define QVAULT_SUBMITFEES 3
#define QVAULT_CHANGEFEES 4
#define QVAULT_SUBMITREINVESTINGADDRESS 5
#define QVAULT_CHANGEREINVESTINGADDRESS 6
#define QVAULT_SUBMITADMINADDRESS 7
#define QVAULT_CHANGEADMINADDRESS 8
#define QVAULT_SUBMITBANNEDADDRESS 9
#define QVAULT_SAVEBANNEDADDRESS 10
#define QVAULT_SUBMITUNBANNEDADDRESS 11
#define QVAULT_SAVEUNBANNEDADDRESS 12

struct submitAuthAddress_input 
{
    uint8_t newAddress[32];
};
struct submitAuthAddress_output
{
};

struct changeAuthAddress_input
{
    uint32_t numberOfChangedAddress;
};
struct changeAuthAddress_output
{
};

struct submitFees_input
{
    uint32_t newQCAPHolder_permille;
    uint32_t newreinvesting_permille;
    uint32_t newdev_permille;
};
struct submitFees_output
{
};

struct changeFees_input
{
    uint32_t newQCAPHolder_permille;
    uint32_t newreinvesting_permille;
    uint32_t newdev_permille;
};
struct changeFees_output
{
};

struct submitReinvestingAddress_input
{
    uint8_t newAddress[32];
};
struct submitReinvestingAddress_output
{
};

struct changeReinvestingAddress_input
{
    uint8_t newAddress[32];
};
struct changeReinvestingAddress_output
{
};

struct submitAdminAddress_input
{
    uint8_t newAddress[32];
};
struct submitAdminAddress_output
{
};

struct changeAdminAddress_input
{
    uint8_t newAddress[32];
};
struct changeAdminAddress_output
{
};

struct submitBannedAddress_input
{
    uint8_t bannedAddress[32];
};
struct submitBannedAddress_output
{
};

struct saveBannedAddress_input
{
    uint8_t bannedAddress[32];
};
struct saveBannedAddress_output
{
};

struct submitUnbannedAddress_input
{
    uint8_t unbannedAddress[32];
};
struct submitUnbannedAddress_output
{
};

struct unblockBannedAddress_input
{
    uint8_t unbannedAddress[32];
};
struct unblockBannedAddress_output
{
};

void submitAuthAddress(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, const char* identity)
{
    submitAuthAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.newAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_SUBMITAUTHADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void changeAuthAddress(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t numberOfChangedAddress)
{
    changeAuthAddress_input input{};
    input.numberOfChangedAddress = numberOfChangedAddress;
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_CHANGEAUTHADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void submitFees(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t newQCAPHolder_permille, uint32_t newreinvesting_permille, uint32_t newdev_permille)
{
    submitFees_input input{};
    input.newQCAPHolder_permille = newQCAPHolder_permille;
    input.newreinvesting_permille = newreinvesting_permille;
    input.newdev_permille = newdev_permille;
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_SUBMITFEES, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void changeFees(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint32_t newQCAPHolder_permille, uint32_t newreinvesting_permille, uint32_t newdev_permille)
{
    changeFees_input input{};
    input.newQCAPHolder_permille = newQCAPHolder_permille;
    input.newreinvesting_permille = newreinvesting_permille;
    input.newdev_permille = newdev_permille;
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_CHANGEFEES, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void submitReinvestingAddress(const char* nodeIp, int nodePort, const char* seed,  uint32_t scheduledTickOffset, const char* identity)
{
    submitReinvestingAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.newAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_SUBMITREINVESTINGADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void changeReinvestingAddress(const char* nodeIp, int nodePort, const char* seed,  uint32_t scheduledTickOffset, const char* identity)
{
    changeReinvestingAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.newAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_CHANGEREINVESTINGADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void submitAdminAddress(const char* nodeIp, int nodePort, const char* seed,  uint32_t scheduledTickOffset, const char* identity)
{
    submitAdminAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.newAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_SUBMITADMINADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void changeAdminAddress(const char* nodeIp, int nodePort, const char* seed,  uint32_t scheduledTickOffset, const char* identity)
{
    changeAdminAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.newAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_CHANGEADMINADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void getData(const char* nodeIp, int nodePort)
{
    QVaultGetData_input input{};
    input.t = 10;
    QVaultGetData_output result{};
    
    if (!runContractFunction(nodeIp, nodePort, QVAULT_CONTRACT_INDEX,
                            QVAULT_GETDATA, &input, sizeof(input),
                            &result, sizeof(result)))
    {
        LOG("Failed to receive data\n");
        return;
    }

    char auth1[128] = {0};
    char auth2[128] = {0};
    char auth3[128] = {0};
    char reinvesting_address[128] = {0};
    char admin_address[128] = {0};
    char newAuth1[128] = {0};
    char newAuth2[128] = {0};
    char newAuth3[128] = {0};
    char newReinvestingAddress1[128] = {0};
    char newReinvestingAddress2[128] = {0};
    char newReinvestingAddress3[128] = {0};
    char newAdminAddress1[128] = {0};
    char newAdminAddress2[128] = {0};
    char newAdminAddress3[128] = {0};
    char newBannedAddress1[128] = {0};
    char newBannedAddress2[128] = {0};
    char newBannedAddress3[128] = {0};
    char newUnbannedAddress1[128] = {0};
    char newUnbannedAddress2[128] = {0};
    char newUnbannedAddress3[128] = {0};
    getIdentityFromPublicKey(result.AUTH_ADDRESS1, auth1, false);
    getIdentityFromPublicKey(result.AUTH_ADDRESS2, auth2, false);
    getIdentityFromPublicKey(result.AUTH_ADDRESS3, auth3, false);
    getIdentityFromPublicKey(result.Reinvesting_address, reinvesting_address, false);
    getIdentityFromPublicKey(result.admin_address, admin_address, false);
    getIdentityFromPublicKey(result.newAuthAddress1, newAuth1, false);
    getIdentityFromPublicKey(result.newAuthAddress2, newAuth2, false);
    getIdentityFromPublicKey(result.newAuthAddress3, newAuth3, false);
    getIdentityFromPublicKey(result.newReinvesting_address1, newReinvestingAddress1, false);
    getIdentityFromPublicKey(result.newReinvesting_address2, newReinvestingAddress2, false);
    getIdentityFromPublicKey(result.newReinvesting_address3, newReinvestingAddress3, false);
    getIdentityFromPublicKey(result.newAdmin_address1, newAdminAddress1, false);
    getIdentityFromPublicKey(result.newAdmin_address2, newAdminAddress2, false);
    getIdentityFromPublicKey(result.newAdmin_address3, newAdminAddress3, false);
    getIdentityFromPublicKey(result.bannedAddress1, newBannedAddress1, false);
    getIdentityFromPublicKey(result.bannedAddress2, newBannedAddress2, false);
    getIdentityFromPublicKey(result.bannedAddress3, newBannedAddress3, false);
    getIdentityFromPublicKey(result.unbannedAddress1, newUnbannedAddress1, false);
    getIdentityFromPublicKey(result.unbannedAddress2, newUnbannedAddress2, false);
    getIdentityFromPublicKey(result.unbannedAddress3, newUnbannedAddress3, false);

    printf("AUTH_ADDRESS1: %s", auth1);
    printf("\nAUTH_ADDRESS2: %s", auth2);
    printf("\nAUTH_ADDRESS3: %s", auth3);
    printf("\nReinvesting_address: %s", reinvesting_address);
    printf("\nadmin_address: %s", admin_address);
    printf("\nnewAuthAddress1: %s", newAuth1);
    printf("\nnewAuthAddress2: %s", newAuth2);
    printf("\nnewAuthAddress3: %s", newAuth3);
    printf("\nnewReinvesting_address1: %s", newReinvestingAddress1);
    printf("\nnewReinvesting_address2: %s", newReinvestingAddress2);
    printf("\nnewReinvesting_address3: %s", newReinvestingAddress3);
    printf("\nnewAdmin_address1: %s", newAdminAddress1);
    printf("\nnewAdmin_address2: %s", newAdminAddress2);
    printf("\nnewAdmin_address3: %s", newAdminAddress3);
    printf("\nPermille for development:%u Permille for Reinvesting:%u Permille for Computors:%u Permille for Holders:%u\n", result.dev_permille, result.reinvesting_permille, result.computor_permille, result.QCAPHolder_permille);

    printf("\nbanned_address1: %s", newBannedAddress1);
    printf("\nbanned_address2: %s", newBannedAddress2);
    printf("\nbanned_address3: %s", newBannedAddress3);
    printf("\nunbanned_address1: %s", newUnbannedAddress1);
    printf("\nunbanned_address2: %s", newUnbannedAddress2);
    printf("\nunbanned_address3: %s", newUnbannedAddress3);
}

void submitBannedAddress(const char* nodeIp, int nodePort, const char* seed,  uint32_t scheduledTickOffset, const char* identity)
{
    submitBannedAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.bannedAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_SUBMITBANNEDADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void saveBannedAddress(const char* nodeIp, int nodePort, const char* seed,  uint32_t scheduledTickOffset, const char* identity)
{
    saveBannedAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.bannedAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_SAVEBANNEDADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void submitUnbannedannedAddress(const char* nodeIp, int nodePort, const char* seed,  uint32_t scheduledTickOffset, const char* identity)
{
    submitUnbannedAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.unbannedAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_SUBMITUNBANNEDADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}

void saveUnbannedAddress(const char* nodeIp, int nodePort, const char* seed,  uint32_t scheduledTickOffset, const char* identity)
{
    unblockBannedAddress_input input{};
    uint8_t publicKey[32] = {0};
    getPublicKeyFromIdentity(identity, publicKey);
    memcpy(input.unbannedAddress, publicKey, 32);
    
    makeContractTransaction(nodeIp, nodePort, seed, QVAULT_CONTRACT_INDEX,
                          QVAULT_SAVEUNBANNEDADDRESS, 0,
                          sizeof(input), &input, scheduledTickOffset);
}