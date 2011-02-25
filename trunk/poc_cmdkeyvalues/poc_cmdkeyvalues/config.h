#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_VERSION "1.0.10.1" // Engine build

#define MAGIC_VERSION 0x5A4F4933 // Some version constant

#define PROTOCOL_VERSION 15 // Net protocol version

#define PROTOCOL_STEAM 0x03 // Protocol type (Steam authentication)

#define STEAM_KEYSIZE 2048 // Max authentication key size

#define TIMEOUT_SECONDS 5 // Seconds before timeout

// net messages

#define net_NOP 0
#define net_Disconnect 1
#define net_StringCmd 4

#define clc_CmdKeyValues 16

#endif // CONFIG_H