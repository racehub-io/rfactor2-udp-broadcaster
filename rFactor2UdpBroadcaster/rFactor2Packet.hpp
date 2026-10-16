
#include "InternalsPlugin.hpp"

#pragma once

// may need a pragma pack(push, 4) here to ensure properties are packed with no padding

enum PacketType : UINT8
{
    Telemetry = 0,
    Scoring = 1,
    Graphics = 2
};

struct PacketInfo
{
    PacketType PacketType;
    UINT8 ContractVersion = 1;
};

struct rFactor2Telem
{
    PacketInfo Info;
    TelemInfoV01 Data;
};

struct rFactor2Scoring
{
    PacketInfo Info;
    ScoringInfoV01 Data;
    VehicleScoringInfoV01 ScoringData;
};

struct rFactor2Graphics
{
    PacketInfo Info;
    GraphicsInfoV01 Data;
};