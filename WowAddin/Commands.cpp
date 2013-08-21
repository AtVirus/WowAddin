#include "stdafx.h"
#include <sstream>

ClientServices s_client;
ObjectMgr s_objMgr;

BOOL CCommand_TestCommand(char const* cmd, char const* args)
{
    Console::Write("Hello from TestCommand: cmd %s, args %s", INPUT_COLOR, cmd, args);

    uint64 guid = s_objMgr.GetActivePlayerGuid();
    CGObject_C *pPlayer = s_objMgr.GetObjectPtr(guid, TYPEMASK_PLAYER);

    if (!pPlayer)
    {
        Console::Write("TestCommand: Not in world!", ERROR_COLOR);
        return TRUE;
    }

    C3Vector pos;
    pPlayer->GetPosition(pos);

    const char *model;
    BOOL result = pPlayer->GetModelFileName(&model);

    Console::Write("Local player: position %f %f %f, model %s", ECHO_COLOR, pos.X, pos.Y, pos.Z, result ? model : "Unknown");

    CGObject_C *pTarget = s_objMgr.GetObjectPtr(pPlayer->GetValue<uint64>(UNIT_FIELD_TARGET), TYPEMASK_UNIT);

    if (pTarget)
        Console::Write("Target %s, guid 0x%016llX", ECHO_COLOR, pTarget->GetObjectName(), pTarget->GetValue<uint64>(OBJECT_FIELD_GUID));
    else
        Console::Write("No target!", ECHO_COLOR);

    return TRUE;
}

BOOL CCommand_Beastmaster(char const* cmd, char const* args)
{
    CDataStore data;
    data.PutInt32(CMSG_BEASTMASTER);
    int state = _strnicmp(args, "off", INT_MAX) != 0;
    data.PutInt8(state);
    data.Finalize();
    s_client.SendPacket(&data);

    Console::Write("Beastmaster mode is %s", ECHO_COLOR, state ? "on" : "off");

    return TRUE;
}

BOOL CCommand_Invis(char const* cmd, char const* args)
{
    CDataStore data;
    data.PutInt32(CMSG_GM_INVIS);
    int state = _strnicmp(args, "off", INT_MAX) != 0;
    data.PutInt32(state);
    data.Finalize();
    s_client.SendPacket(&data);

    Console::Write("GM invis mode is %s", ECHO_COLOR, state ? "on" : "off");

    return TRUE;
}

BOOL CCommand_DBLookup(char const* cmd, char const* args)
{
    CDataStore data;
    data.PutInt32(CMSG_DBLOOKUP);
    data.PutString(args);
    data.Finalize();
    s_client.SendPacket(&data);

    return TRUE;
}

BOOL CCommand_KillServer(char const* cmd, char const* args)
{
    long killmuch = atoi(args);

    for (long i = 0; i < killmuch; ++i)
    {
        CDataStore data;
        data.PutInt32(CMSG_CHAR_ENUM);
        data.Finalize();
        s_client.SendPacket(&data);
        Console::Write("", ECHO_COLOR);
    }

    std::ostringstream ss;
    ss << "Sent CMSG_CHAR_ENUM " << killmuch << " times";

    Console::Write(ss.str().c_str(), ECHO_COLOR);

    return TRUE;
}