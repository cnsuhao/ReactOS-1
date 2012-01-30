/*
 * PROJECT:         ReactOS Windows-Compatible Session Manager
 * LICENSE:         BSD 2-Clause License
 * FILE:            lib/smlib/smclient.c
 * PURPOSE:         SMSS Client Library Stubs for calling SM APIs from a client
 * PROGRAMMERS:     Alex Ionescu
 */

/* INCLUDES *******************************************************************/

#include "precomp.h"
#include "sm/smmsg.h" // To go in precomp.h after
#define NDEBUG
#include "debug.h"

/* FUNCTIONS ******************************************************************/

NTSTATUS
NTAPI
SmExecPgm(IN HANDLE SmApiPort,
          IN PRTL_USER_PROCESS_INFORMATION ProcessInformation,
          IN BOOLEAN DebugFlag)
{
    NTSTATUS Status;
    SM_API_MSG SmApiMsg;

#ifdef _WIN64 // You can take care of this Timo
    /* 64-bit SMSS needs to talk to 32-bit processes so do the LPC conversion */
    if (SmpIsWow64Process())
    {
        return SmpWow64ExecPgm(SmApiPort, ProcessInformation, DebugFlag);
    }
#endif

    /* Initialize the generic LPC header */
    SmApiMsg.h.u2.ZeroInit = 0;
    SmApiMsg.h.u1.s1.DataLength = sizeof(SM_EXEC_PGM_MSG) + 8;
    SmApiMsg.h.u1.s1.TotalLength = sizeof(SmApiMsg);

    /* Initalize this specific API's parameters */
    SmApiMsg.ApiNumber = SmpExecPgm;
    RtlCopyMemory(&SmApiMsg.u.ExecPgm.ProcessInformation,
                  ProcessInformation,
                  sizeof(SmApiMsg.u.ExecPgm.ProcessInformation));
    SmApiMsg.u.ExecPgm.DebugFlag = DebugFlag;

    /* Send the message to SMSS */
    Status = NtRequestWaitReplyPort(SmApiPort, &SmApiMsg.h, &SmApiMsg.h);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("SmExecPgm: NtRequestWaitReply Failed %lx\n", Status);
    }
    else
    {
        /* Upon success, we use the API's return value */
        Status = SmApiMsg.ReturnValue;
    }

    /* Close the handles that the parent passed in and return status */
    NtClose(ProcessInformation->ProcessHandle);
    NtClose(ProcessInformation->ThreadHandle);
    return Status;
}

NTSTATUS
NTAPI
SmConnectToSm(IN PUNICODE_STRING SbApiPortName,
              IN HANDLE SbApiPort,
              IN ULONG ImageType,
              IN HANDLE SmApiPort)
{
    NTSTATUS Status;
    SB_CONNECTION_INFO ConnectInfo;
    UNICODE_STRING DestinationString;
    SECURITY_QUALITY_OF_SERVICE SecurityQos;
    ULONG ConnectInfoLength = sizeof(ConnectInfo);

    /* Setup the QoS structure */
    SecurityQos.ImpersonationLevel = SecurityIdentification;
    SecurityQos.ContextTrackingMode = SECURITY_DYNAMIC_TRACKING;
    SecurityQos.EffectiveOnly = TRUE;

    /* Set the SM API port name */
    RtlInitUnicodeString(&DestinationString, L"\\SmApiPort");

    /* Check if this is a client connecting to SMSS, or SMSS to itself */
    if (SbApiPortName)
    {
        /* A client SB port as well as an image type must be present */
        if (!(SbApiPort) || !(ImageType)) return STATUS_INVALID_PARAMETER_MIX;

        /* Copy the client port name, and NULL-terminate it */
        RtlCopyMemory(ConnectInfo.SbApiPortName,
                      SbApiPortName->Buffer,
                      SbApiPortName->Length);
        ConnectInfo.SbApiPortName[SbApiPortName->Length /
                                  sizeof(WCHAR)] = UNICODE_NULL;

        /* Save the subsystem type */
        ConnectInfo.SubsystemType = ImageType;
    }
    else
    {
        /* No client port, and the subsystem type is not set */
        ConnectInfo.SbApiPortName[0] = UNICODE_NULL;
        ConnectInfo.SubsystemType = IMAGE_SUBSYSTEM_UNKNOWN;
    }

    /* Connect to SMSS and exchange connection information */
    Status = NtConnectPort(SmApiPort,
                           &DestinationString,
                           &SecurityQos,
                           NULL,
                           NULL,
                           NULL,
                           &ConnectInfo,
                           &ConnectInfoLength);
    if (!NT_SUCCESS(Status))
    {
        DPRINT1("SmConnectToSm: Connect to Sm failed %lx\n", Status);
    }
    else
    {
        /* Treat a warning or informational status as success */
        Status = STATUS_SUCCESS;
    }

    /* Return if the connection was successful or not */
    return Status;
}