#pragma once

// SC structs

struct OrderResponse
{
    uint64_t orderId;        // Order ID as uint64
    uint8_t originAccount[32];      // Origin account
    uint8_t destinationAccount[32]; // Destination account
    uint64_t amount;         // Amount as uint64
    uint8_t memo[64]; // Notes or metadata
    uint32_t sourceChain;    // Source chain identifier
};

struct vottunBridgeGetOrder_input
{
    uint64_t orderId;
};

struct vottunBridgeGetOrder_output
{
    uint8_t status;
    OrderResponse order; // Updated response format
    uint8_t message[32];

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct vottunBridgeGetTotalReceivedTokens_input
{
    uint64_t amount;
};

struct vottunBridgeGetTotalReceivedTokens_output
{
    uint64_t totalTokens;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct vottunBridgeGetAdminID_input
{
    uint8_t idInput;
};

struct vottunBridgeGetAdminID_output
{
    uint8_t adminId[32];

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

struct vottunBridgeGetTotalLockedTokens_input
{
    // No input parameters
};

struct vottunBridgeGetTotalLockedTokens_output
{
    uint64_t totalLockedTokens;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

// Estructura para la entrada de la función getOrderByDetails
struct vottunBridgeGetOrderByDetails_input
{
    uint8_t ethAddress[32];        // Dirección Ethereum
    uint64_t amount;        // Monto de la transacción
    uint8_t status;         // Estado de la orden (0 = creada, 1 = completada, 2 = reembolsada)
};

// Estructura para la salida de la función getOrderByDetails
struct vottunBridgeGetOrderByDetails_output
{
    uint8_t status;   // Operation status (0 = success, other = error)
    uint64_t orderId; // ID of the found order
    uint8_t qubicDestination[32]; // Destination address on Qubic (for EVM to Qubic orders)

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

// Bridge Order Structure
struct BridgeOrder
{
    uint8_t qubicSender[32];              // Sender address on Qubic
    uint8_t qubicDestination[32];         // Destination address on Qubic
    uint8_t ethAddress[64]; // Destination Ethereum address
    uint64_t orderId;              // Unique ID for the order
    uint64_t amount;               // Amount to transfer
    uint8_t orderType;             // Type of order (e.g., mint, transfer)
    uint8_t status;                // Order status (e.g., Created, Pending, Refunded)
    bool fromQubicToEthereum;     // Direction of transfer
    bool tokensReceived;          // Flag to indicate if tokens have been received
    bool tokensLocked;            // Flag to indicate if tokens are in locked state
};

struct vottunBridgeGetContractInfo_input
{
    // No parameters
};

struct vottunBridgeGetContractInfo_output
{
    uint8_t admin[32];
    uint8_t managers[16][32];
    uint64_t nextOrderId;
    uint64_t lockedTokens;
    uint64_t totalReceivedTokens;
    uint64_t earnedFees;
    uint32_t tradeFeeBillionths;
    uint32_t sourceChain;
    // Debug info
    BridgeOrder firstOrders[16]; // First 16 orders
    uint64_t totalOrdersFound;            // How many non-empty orders exist
    uint64_t emptySlots;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

// Get Available Fees structures
struct vottunBridgeGetAvailableFees_input
{
    // No parameters
};

struct vottunBridgeGetAvailableFees_output
{
    uint64_t availableFees;
    uint64_t totalEarnedFees;
    uint64_t totalDistributedFees;

    static constexpr unsigned char type()
    {
        return RespondContractFunction::type();
    }
};

void createOrder(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, const char* qubicDestination, const char* ethAddress, uint64_t amount, bool fromQubicToEthereum);
void setAdmin(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, const char* identity);
void addManager(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, const char* identity);
void removeManager(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, const char* identity);
void completeOrder(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t orderId);
void refundOrder(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t orderId);
void transferToContract(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t amount, uint64_t orderId);
void addLiquidity(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t amount);
void withdrawFees(const char* nodeIp, int nodePort, const char* seed, uint32_t scheduledTickOffset, uint64_t amount);

void getOrder(const char* nodeIp, int nodePort, uint64_t orderId);
void getTotalReceivedTokens(const char* nodeIp, int nodePort, uint64_t amount);
void getAdminID(const char* nodeIp, int nodePort, uint8_t idInput);
void getTotalLockedTokens(const char* nodeIp, int nodePort);
void getOrderByDetails(const char* nodeIp, int nodePort, const char* ethAddress, uint64_t amount, uint8_t status);
void getContractInfo(const char* nodeIp, int nodePort);
void getAvailableFees(const char* nodeIp, int nodePort);