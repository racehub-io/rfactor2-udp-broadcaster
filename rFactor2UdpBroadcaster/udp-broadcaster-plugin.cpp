#include <WinSock2.h>
#include <Windows.h>

#include "udp-broadcaster-plugin.hpp"          // corresponding header file
#include <math.h>                              // for atan2, sqrt
#include <stdio.h>                             // for sample output
#include <assert.h>
#include <io.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>

#include <WS2tcpip.h>

#include <string>
#include <iostream>
#include <sstream>

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

// Saves us from typing std::cout << etc. etc. etc.
using namespace std;

int ContractVersion = 1;

// plugin information

extern "C" __declspec(dllexport)
const char* __cdecl GetPluginName() { return("udp-broadcaster-plugin"); }

extern "C" __declspec(dllexport)
PluginObjectType __cdecl GetPluginType() { return(PO_INTERNALS); }

extern "C" __declspec(dllexport)
int __cdecl GetPluginVersion() { return(7); } // InternalsPluginV01 functionality (if you change this return value, you must derive from the appropriate class!)

extern "C" __declspec(dllexport)
PluginObject * __cdecl CreatePluginObject() { return((PluginObject*) new UdpBroadcasterPlugin); }

extern "C" __declspec(dllexport)
void __cdecl DestroyPluginObject(PluginObject * obj) { delete((UdpBroadcasterPlugin*)obj); }


// UdpBroadcasterPlugin class

void UdpBroadcasterPlugin::WriteToLogFileDebug(const char* const openStr, const char* const msg)
{
#if _DEBUG
    FILE* fo;

    fo = fopen("racehub-udp-log.txt", openStr);
    if (fo != NULL)
    {
        fprintf(fo, "DEBUG: %s\n", msg);
        fclose(fo);
    }
#endif
}

void UdpBroadcasterPlugin::WriteToLogFile(const char* const openStr, const char* const msg)
{
    FILE* fo;

    fo = fopen("racehub-udp-log.txt", openStr);
    if (fo != NULL)
    {
        fprintf(fo, "%s\n", msg);
        fclose(fo);
    }
}

void UdpBroadcasterPlugin::Startup(long version)
{
    // Open ports, read configs etc
    char temp[80];
    sprintf(temp, "-STARTUP- (rFactor2 version %.4f)", (float)version / 10000.0f);
    WriteToLogFile("w", temp);

    // If not broadcasting to network, set localhost IP address
    if (BroadcastToNetwork)
    {
        BroadcastAddress = "255.255.255.255";
    }
    else
    {
        BroadcastAddress = "127.0.0.1";
    }

    // Write settings
    WriteToLogFileDebug("a", ("EnableTelemetry: " + to_string(EnableTelemetry)).c_str());
    WriteToLogFileDebug("a", ("EnableScoring: " + to_string(EnableScoring)).c_str());
    WriteToLogFileDebug("a", ("EnableGraphics: " + to_string(EnableGraphics)).c_str());
    WriteToLogFileDebug("a", ("EnableTelemetry: " + to_string(EnableTelemetry)).c_str());
    WriteToLogFileDebug("a", ("BroadcastPort: " + to_string(BroadcastPort)).c_str());
    WriteToLogFileDebug("a", ("BroadcastAddress: " + BroadcastAddress).c_str());

    // default HW control enabled to true
    mEnabled = true;
}

void UdpBroadcasterPlugin::Shutdown()
{
    WriteToLogFile("a", "-SHUTDOWN-");
}

void UdpBroadcasterPlugin::StartSession()
{
    WriteToLogFile("a", "--STARTSESSION--");
}

void UdpBroadcasterPlugin::EndSession()
{
    WriteToLogFile("a", "--ENDSESSION--");
}

void UdpBroadcasterPlugin::EnterRealtime()
{
    // start up timer every time we enter realtime
    mET = 0.0;
    WriteToLogFile("a", "---ENTERREALTIME---");
}

void UdpBroadcasterPlugin::ExitRealtime()
{
    WriteToLogFile("a", "---EXITREALTIME---");
}

void UdpBroadcasterPlugin::BroadcastPacket(const char* buffer, int size)
{

    // Structure to store the WinSock version. This is filled in
    // on the call to WSAStartup()
    WSADATA data;

    // To start WinSock, the required version must be passed to
    // WSAStartup(). This server is going to use WinSock version
    // 2 so I create a word that will store 2 and 2 in hex i.e.
    // 0x0202
    WORD version = MAKEWORD(2, 2);

    // Start WinSock
    int wsOk = WSAStartup(version, &data);
    if (wsOk != 0)
    {
        // Not ok! Get out quickly
        std::ostringstream oss;
        oss << "Can't start Winsock! ";
        cout << oss.str();
        WriteToLogFile("a", oss.str().c_str());
        return;
    }

    // Create a hint structure for the server
    sockaddr_in server;
    server.sin_family = AF_INET; // AF_INET = IPv4 addresses
    server.sin_port = htons(BroadcastPort); // Little to big endian conversion
    inet_pton(AF_INET, BroadcastAddress.c_str(), &server.sin_addr); // Convert from string to byte array

    // Socket creation, note that the socket type is datagram
    SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

    // Write out to that socket
    int sendOk = sendto(out, buffer, size, 0, (sockaddr*)&server, sizeof(server));
    if (sendOk == SOCKET_ERROR)
    {
        std::ostringstream oss;
        oss << "Socket send packet failed, WSAGetLastError Returned: " << WSAGetLastError();
        cout << oss.str() << endl;
        WriteToLogFile("a", oss.str().c_str());
    }
    else
    {
        std::ostringstream oss;
        oss << "Bytes sent to server: " << sendOk;
        cout << oss.str() << endl;
        WriteToLogFileDebug("a", oss.str().c_str());
    }

    // Close the socket
    closesocket(out);

    // Close down Winsock
    WSACleanup();
}

void UdpBroadcasterPlugin::UpdateTelemetry(const TelemInfoV01& info)
{
    if (!EnableTelemetry)
    {
        return;
    }

    WriteToLogFileDebug("a", "Serializing telemetry data");
    DataPacket data;

    data.mutable_packetheader()->set_packettype(PacketType::TELEMETRY);
    data.mutable_packetheader()->set_contractversion(ContractVersion);
    data.mutable_packetheader()->set_epochtime(time(nullptr));

    try
    {
        TelemInfo* telem = ToProtoType(info);
        data.set_allocated_telemetry(telem);
    }
    catch (exception e)
    {
        WriteToLogFile("a", e.what());
        throw;
    }

    int size = data.ByteSize();
    void* buffer = malloc(size);
    data.SerializeToArray(buffer, size);

    WriteToLogFileDebug("a", ("Broadcasting telemetry data of size: " + to_string(size)).c_str());
    BroadcastPacket((char*)buffer, size);
}

void UdpBroadcasterPlugin::UpdateGraphics(const GraphicsInfoV02& info)
{
    if (!EnableGraphics)
    {
        return;
    }

    WriteToLogFileDebug("a", "Serializing graphics data");

    DataPacket data;
    data.mutable_packetheader()->set_packettype(PacketType::GRAPHICS);
    data.mutable_packetheader()->set_contractversion(ContractVersion);
    data.mutable_packetheader()->set_epochtime(time(nullptr));

    try
    {
        GraphicsInfo* graphics = ToProtoType(info);
        data.set_allocated_graphics(graphics);
    }
    catch (exception e)
    {
        WriteToLogFile("a", e.what());
        throw;
    }

    int size = data.ByteSize();
    void* buffer = malloc(size);
    data.SerializeToArray(buffer, size);

    WriteToLogFileDebug("a", ("Broadcasting graphics data of size: " + to_string(size)).c_str());
    BroadcastPacket((char*)buffer, size);
}

void UdpBroadcasterPlugin::UpdateScoring(const ScoringInfoV01& info)
{
    if (!EnableScoring)
    {
        return;
    }

    WriteToLogFileDebug("a", "Serializing scoring data");

    DataPacket data;
    data.mutable_packetheader()->set_packettype(PacketType::SCORING);
    data.mutable_packetheader()->set_contractversion(ContractVersion);
    data.mutable_packetheader()->set_epochtime(time(nullptr));

    try
    {
        ScoringInfo* scoring = ToProtoType(info);
        data.set_allocated_scoring(scoring);
    }
    catch (exception e)
    {
        WriteToLogFile("a", e.what());
        throw;
    }

    int size = data.ByteSize();
    void* buffer = malloc(size);
    data.SerializeToArray(buffer, size);

    WriteToLogFileDebug("a", ("Broadcasting scoring data of size: " + to_string(size)).c_str());
    BroadcastPacket((char*)buffer, size);
}

void UdpBroadcasterPlugin::SetPhysicsOptions(PhysicsOptionsV01& options)
{
    if (!EnablePhysics)
    {
        return;
    }

    WriteToLogFileDebug("a", "Serializing physics data");

    DataPacket data;
    data.mutable_packetheader()->set_packettype(PacketType::PHYSICS);
    data.mutable_packetheader()->set_contractversion(ContractVersion);
    data.mutable_packetheader()->set_epochtime(time(nullptr));

    try
    {
        PhysicsInfo* physics = ToProtoType(options);
        data.set_allocated_physics(physics);
    }
    catch (exception e)
    {
        WriteToLogFile("a", e.what());
        throw;
    }

    int size = data.ByteSize();
    void* buffer = malloc(size);
    data.SerializeToArray(buffer, size);

    WriteToLogFileDebug("a", ("Broadcasting physics data of size: " + to_string(size)).c_str());
    BroadcastPacket((char*)buffer, size);
}

bool UdpBroadcasterPlugin::CheckHWControl(const char* const controlName, double& fRetVal)
{
    // only if enabled, of course
    if (!mEnabled)
        return(false);

    // Note that incoming value is the game's computation, in case you're interested.

    // Sorry, no control allowed over actual vehicle inputs ... would be too easy to cheat!
    // However, you can still look at the values.

    // Note: since the game calls this function every frame for every available control, you might consider
    // doing a binary search if you are checking more than 7 or 8 strings, just to keep the speed up.
    if (_stricmp(controlName, "LookLeft") == 0)
    {
        const double headSwitcheroo = fmod(mET, 2.0);
        if (headSwitcheroo < 0.5)
            fRetVal = 1.0;
        else
            fRetVal = 0.0;
        return(true);
    }
    else if (_stricmp(controlName, "LookRight") == 0)
    {
        const double headSwitcheroo = fmod(mET, 2.0);
        if ((headSwitcheroo > 1.0) && (headSwitcheroo < 1.5))
            fRetVal = 1.0;
        else
            fRetVal = 0.0;
        return(true);
    }

    return(false);
}


bool UdpBroadcasterPlugin::ForceFeedback(double& forceValue)
{
    // Note that incoming value is the game's computation, in case you're interested.
#if 0 // enable to log it out (note that this is a very very slow implementation)
    FILE* fo = fopen("FFB.txt", "a");
    if (fo != NULL)
    {
        fprintf(fo, "\nFFB=%.4f", forceValue);
        fclose(fo);
    }
#endif

    // CHANGE COMMENTS TO ENABLE FORCE EXAMPLE
    return(false);

    // I think the bounds are -11500 to 11500 ...
  //  forceValue = 11500.0 * sinf( mET );
  //  return( true );
}

bool UdpBroadcasterPlugin::RequestCommentary(CommentaryRequestInfoV01& info)
{
    // COMMENT OUT TO ENABLE EXAMPLE
    return(false);

    // only if enabled, of course
    if (!mEnabled)
        return(false);

    // Note: function is called twice per second

    // Say green flag event for no particular reason every 20 seconds ...
    const double timeMod20 = fmod(mET, 20.0);
    if (timeMod20 > 19.0)
    {
        strcpy(info.mName, "GreenFlag");
        info.mInput1 = 0.0;
        info.mInput2 = 0.0;
        info.mInput3 = 0.0;
        info.mSkipChecks = true;
        return(true);
    }

    return(false);
}

bool UdpBroadcasterPlugin::GetCustomVariable(long i, CustomVariableV01& var)
{
    // These are the default values inserted into CustomPluginVariables.json
    // when they don't exist.

    string text = "GetCustomVariable - Invoked for i = " + to_string(i);
    WriteToLogFileDebug("a", text.c_str());

    if (i == 0)
    {
        // rF2 will automatically create this variable and default it to 1
        // (true) unless we create it first, in which case we can choose the default.
        strcpy_s(var.mCaption, " Enabled");
        var.mNumSettings = 2;
        var.mCurrentSetting = 1;
        return(true);
    }
    else if (i == 1)
    {
        strcpy_s(var.mCaption, "EnableTelemetry");
        var.mNumSettings = 2;
        var.mCurrentSetting = 1;
        return(true);
    }
    else if (i == 2)
    {
        strcpy_s(var.mCaption, "EnableScoring");
        var.mNumSettings = 2;
        var.mCurrentSetting = 1;
        return(true);
    }
    else if (i == 3)
    {
        strcpy_s(var.mCaption, "EnableGraphics");
        var.mNumSettings = 2;
        var.mCurrentSetting = 1;
        return(true);
    }
    else if (i == 4)
    {
        strcpy_s(var.mCaption, "BroadcastToNetwork");
        var.mNumSettings = 2;
        var.mCurrentSetting = 0;
        return(true);
    }
    else if (i == 5)
    {
        strcpy_s(var.mCaption, "BroadcastPort");
        var.mNumSettings = 65535;
        var.mCurrentSetting = 20888;
        return(true);
    }
    else
    {
        return(false);
    }
}

void UdpBroadcasterPlugin::GetCustomVariableSetting(CustomVariableV01& var, long i, CustomSettingV01& setting)
{
    string text = "GetCustomVariableSetting - Invoked for " + string(var.mCaption);
    WriteToLogFileDebug("a", text.c_str());

    // Default in case we forget something
    strcpy_s(setting.mName, "Error");

    if (0 == strcmp(var.mCaption, "EnableTelemetry"))
    {
        if (0 == i) strcpy_s(setting.mName, "No");
        else         strcpy_s(setting.mName, "Yes");
    }
    else if (0 == strcmp(var.mCaption, "EnableScoring"))
    {
        if (0 == i) strcpy_s(setting.mName, "No");
        else         strcpy_s(setting.mName, "Yes");
    }
    else if (0 == strcmp(var.mCaption, "EnableGraphics"))
    {
        if (0 == i) strcpy_s(setting.mName, "No");
        else         strcpy_s(setting.mName, "Yes");
    }
    else if (0 == strcmp(var.mCaption, "BroadcastToNetwork"))
    {
        if (0 == i) strcpy_s(setting.mName, "No");
        else         strcpy_s(setting.mName, "Yes");
    }
}

void UdpBroadcasterPlugin::AccessCustomVariable(CustomVariableV01& var)
{
    string text = "AccessCustomVariable - Invoked for " + string(var.mCaption)
                    + "setting: " + to_string(var.mCurrentSetting);
    WriteToLogFileDebug("a", text.c_str());

    if (_stricmp(var.mCaption, " Enabled") == 0)
        ; // Do nothing; this variable is just for rF2 to know whether to keep the plugin loaded.
    else if (_stricmp(var.mCaption, "EnableTelemetry") == 0)
    {
        UdpBroadcasterPlugin::EnableTelemetry = var.mCurrentSetting;
    }
    else if (_stricmp(var.mCaption, "EnableScoring") == 0)
    {
        UdpBroadcasterPlugin::EnableScoring = var.mCurrentSetting;
    }
    else if (_stricmp(var.mCaption, "EnableGraphics") == 0)
    {
        UdpBroadcasterPlugin::EnableGraphics = var.mCurrentSetting;
    }
    else if (_stricmp(var.mCaption, "BroadcastToNetwork") == 0)
    {
        UdpBroadcasterPlugin::BroadcastToNetwork = var.mCurrentSetting;
    }
    else if (_stricmp(var.mCaption, "BroadcastPort") == 0)
    {
        UdpBroadcasterPlugin::BroadcastPort = var.mCurrentSetting;
    }

}