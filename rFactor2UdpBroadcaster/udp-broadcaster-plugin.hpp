#pragma once

#ifndef _UDP_BROADCASTER_PLUGIN_H
#define _UDP_BROADCASTER_PLUGIN_H

#include "InternalsPlugin.hpp"
#include "telemetry.pb.h"
#include "DataConverter.h"

using namespace rFactor2UdpBroadcaster;


// This is used for the app to use the plugin for its intended purpose
// REMINDER: exported function GetPluginVersion() should return 1 if
// you are deriving from this InternalsPluginV01, 2 for InternalsPluginV02, etc.
class UdpBroadcasterPlugin : public InternalsPluginV07 
{

public:

    // Constructor/destructor
    UdpBroadcasterPlugin() {}
    ~UdpBroadcasterPlugin() {}

    // These are the functions derived from base class InternalsPlugin
    // that can be implemented.
    void Startup(long version);  // game startup
    void Shutdown();               // game shutdown

    void EnterRealtime();          // entering realtime
    void ExitRealtime();           // exiting realtime

    void StartSession();           // session has started
    void EndSession();             // session has ended

    // GAME OUTPUT
    long WantsTelemetryUpdates() { return(2); } // CHANGE TO 1 TO ENABLE TELEMETRY EXAMPLE!
    void UpdateTelemetry(const TelemInfoV01& info);

    bool WantsGraphicsUpdates() { return(true); } // CHANGE TO TRUE TO ENABLE GRAPHICS EXAMPLE!
    void UpdateGraphics(const GraphicsInfoV02& info);

    // GAME INPUT
    bool HasHardwareInputs() { return(false); } // CHANGE TO TRUE TO ENABLE HARDWARE EXAMPLE!
    void UpdateHardware(const double fDT) { mET += fDT; } // update the hardware with the time between frames
    void EnableHardware() { mEnabled = true; }             // message from game to enable hardware
    void DisableHardware() { mEnabled = false; }           // message from game to disable hardware

    // See if the plugin wants to take over a hardware control.  If the plugin takes over the
    // control, this method returns true and sets the value of the double pointed to by the
    // second arg.  Otherwise, it returns false and leaves the double unmodified.
    bool CheckHWControl(const char* const controlName, double& fRetVal);

    bool ForceFeedback(double& forceValue);

    // SCORING OUTPUT
    bool WantsScoringUpdates() { return(true); }
    void UpdateScoring(const ScoringInfoV01& info);

    // PHYSICS OUTPUT
    void SetPhysicsOptions(PhysicsOptionsV01& options);

    // COMMENTARY INPUT
    bool RequestCommentary(CommentaryRequestInfoV01& info);  // SEE FUNCTION BODY TO ENABLE COMMENTARY EXAMPLE

    // At startup, this will be called with increasing index (starting at zero) until false is returned.
    // Feel free to add/remove/rearrange the variables when updating your plugin; the index does not have to be consistent from run to run.
    bool GetCustomVariable(long i, CustomVariableV01& var);

    // This gets the name of each possible setting for a given variable.
    void GetCustomVariableSetting(CustomVariableV01& var, long i, CustomSettingV01& setting);

    // This will be called at startup, shutdown, and any time that the variable is changed (within the UI).
    void AccessCustomVariable(CustomVariableV01& var);

private:
    void BroadcastPacket(const char* buffer, int size);
    void WriteToLogFile(const char* const openStr, const char* const msg);
    void WriteToLogFileDebug(const char* const openStr, const char* const msg);
    double mET;  // needed for the hardware example
    bool mEnabled; // needed for the hardware example
    long BroadcastPort;
    string BroadcastAddress;
    bool BroadcastToNetwork;
    bool EnableTelemetry;
    bool EnableScoring;
    bool EnableGraphics;
    bool EnablePhysics;
};


#endif // _UDP_BROADCASTER_PLUGIN_H