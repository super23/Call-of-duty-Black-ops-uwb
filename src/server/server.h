#pragma once
#include <qcommon/msg_mp.h>
#include <bgame/bg_local.h>
#include <qcommon/net_chan_mp.h>
#include <server_mp/sv_snapshot_mp.h>
#include <server_mp/sv_voice_mp.h>

#ifdef KISAK_DEMON
#define KISAK_STATS
#endif

// Set when every stats upload fragment has been received (see statPacketsReceived).
#define ALL_STATS_PACKETS_RECEIVED 0x7FFFFFFFF

// Stock MP dedicated-server DDL stats blob (CoDMPServer client_t). Not LIVE_STATS_DDL_BUFFER_BYTES (40960).
#define CLIENT_STATS_BUFFER_BYTES 40168
#define CLIENT_MODIFIED_STAT_BYTES (CLIENT_STATS_BUFFER_BYTES / 8) // 5021-byte dirty bitset for stats DDL

struct gentity_s;

// Connection/sign-on state for an sv client slot (clientHeader_t::state).
enum clientSignonState_e
{
    CS_FREE = 0,
    CS_ZOMBIE = 1,         // disconnected; slot kept briefly for reliable acks
    CS_RECONNECTING = 2,
    CS_CONNECTED = 3,      // awaiting gamestate / downloads
    CS_CLIENTLOADING = 4,
    CS_ACTIVE = 5,         // in world
};

// Client-reported vehicle pose for lag-compensated traces (sent in usercmds).
struct PredictedVehicleInfo // sizeof=0x34
{
    bool inVehicle;
    // 3 bytes implicit padding
    float origin[3];
    float angles[3];
    float tVel[3];  // translational velocity
    float aVel[3];  // angular velocity
};

// Per-client network header embedded at the start of client_t.
struct clientHeader_t // sizeof=0x714
{
    clientSignonState_e state;
    int sendAsActive;                 // include in snapshots as if fully active (rate/bandwidth gating)
    int deltaMessage;                 // sequence of last acked snapshot for delta encoding
    int rateDelayed;                  // snapshot send held until rate window opens
    netchan_t netchan;
    float predictedOrigin[3];         // client-predicted origin for archive/delta PS encoding
    int predictedOriginServerTime;
    PredictedVehicleInfo vehicle;
};

// Metadata for one entry in the reliable server-command ring (index & 0x7F).
struct svscmd_info_t // sizeof=0xC
{
    char *cmd;   // pointer into reliableCommandBuffer
    int time;    // svs.time when queued
    int type;    // svscmd_type (SV_CMD_CAN_IGNORE / SV_CMD_RELIABLE)
};

// Connected player slot on the MP dedicated server (CoDMPServer client_t, sizeof=0x84D00).
struct client_t
{
    clientHeader_t header;
    const char *dropReason;
    char userinfo[1024];
    char reliableCommandBuffer[16384];
    int reliableCommandBufferNext;
    svscmd_info_t reliableCommandInfo[128];
    int reliableSequence;
    int reliableAcknowledge;
    int reliableSent;
    int messageAcknowledge;
    int gamestateMessageNum;
    int challenge;                    // connect challenge (anti spoof)
    usercmd_s lastUsercmd;
    int lastClientCommand;
    char lastClientCommandString[1024];
    gentity_s *gentity;
    char name[32];
    char clanAbbrev[5];
    // 3 bytes implicit padding before xuid
    unsigned __int64 xuid;            // platform XUID from userinfo
    int downloading;
    char downloadName[64];
    int download;                     // file handle while downloading pk3
    int downloadSize;
    int downloadCount;
    int downloadClientBlock;
    int downloadCurrentBlock;
    int downloadXmitBlock;
    unsigned __int8 *downloadBlocks[8];
    int downloadBlockSize[8];
    int downloadEOF;
    int downloadSendTime;
    char downloadURL[256];
    int wwwOk;                        // HTTP redirect download available
    int downloadingWWW;
    int clientDownloadingWWW;
    int wwwFallback;                  // fall back from WWW to block download
    int nextReliableTime;
    int nextReliableCount;
    char reservedSlot;                // matched s_reservedSlots[] password slot
    // 3 bytes implicit padding
    int lastPacketTime;
    int lastConnectTime;
    int nextSnapshotTime;
    int lastSnapshotTime;
    int timeoutCount;
    clientSnapshot_t frames[32];      // ring of built snapshots for delta encoding
    int ping;
    int rate;                         // bytes/sec limit from userinfo
    int snapshotMsec;                 // min ms between snapshots (from snaps userinfo)
    int pureAuthentic;                // 0=unchecked, 1=clean, 2=failed pure check
    char netchanOutgoingBuffer[65536];
    char netchanIncomingBuffer[2048];
    int guid;                         // PunkBuster / security guid from userinfo
    unsigned int scriptId;
    bool bIsSplitscreenClient;
    // 3 bytes implicit padding
    int bIsTestClient;
    int bIsDemoClient;
    int serverId;                     // svs.serverId at connect (detect stale gamestates)
    VoicePacket_t voicePackets[40];
    int voicePacketCount;
    bool muteList[32];                // per-listener voice mute (indexed by clientNum)
    bool sendVoice;
    unsigned __int8 stats[CLIENT_STATS_BUFFER_BYTES];
    unsigned __int8 purchasedItems[32]; // unlockable purchase bitfield (8 x uint32)
    unsigned __int8 globalStats[CLIENT_STATS_BUFFER_BYTES];
    unsigned __int8 globalStatsStable[CLIENT_STATS_BUFFER_BYTES];
    unsigned __int8 modifiedStatBytes[CLIENT_MODIFIED_STAT_BYTES];
    // 3 bytes implicit padding before statsSentIndex
    int statsSentIndex;               // scan cursor for modifiedStatBytes in snapshots
    int statsModified;
    __int64 statPacketsReceived;      // upload fragment counter; ALL_STATS_PACKETS_RECEIVED when complete
    int statsValidated;
    char PBguid[33];                  // server PunkBuster guid from challenge
    char clientPBguid[33];
    bool steamAuthorized;
    char steamAuthFailCount;          // kicks after repeated auth failures
    // implicit padding before steamID (8-byte aligned)
    unsigned __int64 steamID;
    bool tempPacketDebugging;         // extra snapshot logging when dvar enabled
    // 3 bytes implicit padding
    int snapshotHistoryTime;          // retail debug: snapshot timing history (unused in port)
    int snapshotHistoryFrames;
    int messageHistorySize;
    unsigned __int64 dw_userID;       // Demonware online user id (bdOnlineUserId)
    int notifyJoin;                   // queue bdMatchMaking::notifyJoin on next frame
    unsigned int dwchallenge;         // Demonware connect challenge for reconnect validation
};

static_assert(sizeof(PredictedVehicleInfo) == 0x34, "PredictedVehicleInfo layout");
static_assert(sizeof(clientHeader_t) == 0x714, "clientHeader_t layout");
static_assert(CLIENT_STATS_BUFFER_BYTES == 40168, "retail client_t stats blob size");
static_assert(CLIENT_MODIFIED_STAT_BYTES == 5021, "modifiedStatBytes bitset size");
static_assert(offsetof(client_t, statsSentIndex) % 4 == 0, "statsSentIndex alignment");
static_assert(offsetof(client_t, xuid) % 8 == 0, "xuid alignment");
static_assert(offsetof(client_t, steamID) % 8 == 0, "steamID alignment");
// Retail CoDMPServer memset uses 0x84D00; runtime size is sizeof(client_t) in sv_client_mp.
