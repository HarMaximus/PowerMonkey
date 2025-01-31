/*******************************************************************************
*  ______                            ______                 _
* (_____ \                          |  ___ \               | |
*  _____) )___   _ _ _   ____   ___ | | _ | |  ___   ____  | |  _  ____  _   _
* |  ____// _ \ | | | | / _  ) / __)| || || | / _ \ |  _ \ | | / )/ _  )| | | |
* | |    | |_| || | | |( (/ / | |   | || || || |_| || | | || |< (( (/ / | |_| |
* |_|     \___/  \____| \____)|_|   |_||_||_| \___/ |_| |_||_| \_)\____) \__  |
*                                                                       (____/
* Copyright (C) 2021 Ivan Dimkovic. All rights reserved.
*
* All trademarks, logos and brand names are the property of their respective
* owners. All company, product and service names used are for identification
* purposes only. Use of these names, trademarks and brands does not imply
* endorsement.
*
* SPDX-License-Identifier: Apache-2.0
* Full text of the license is available in project root directory (LICENSE)
*
* WARNING: This code is a proof of concept for educative purposes. It can
* modify internal computer configuration parameters and cause malfunctions or
* even permanent damage. It has been tested on a limited range of target CPUs
* and has minimal built-in failsafe mechanisms, thus making it unsuitable for
* recommended use by users not skilled in the art. Use it at your own risk.
*
*******************************************************************************/

#include <PiPei.h>
#include <Uefi.h>
#include <Library/UefiLib.h>

//
// Protocols

#include <Protocol/MpService.h>

//
// PowerMonkey

#include "MpDispatcher.h"

//
// Initialized at startup

extern EFI_MP_SERVICES_PROTOCOL* gMpServices;
extern EFI_BOOT_SERVICES* gBS;

/*******************************************************************************
 *
 ******************************************************************************/

///
/// Internal structure for command dispatching 
///

typedef struct _PACKAGE_DISPATCH_DATA
{
  UINTN ApicID;
  VOID* opaqueParam;
} PACKAGE_DISPATCH_DATA;


/*******************************************************************************
 * TBD / TODO: Needs Rewrite
 ******************************************************************************/

EFI_STATUS EFIAPI RunOnPackageOrCore(
  const IN PLATFORM* Platform,
  const IN UINTN ApicID,
  const IN EFI_AP_PROCEDURE proc,
  IN VOID* param OPTIONAL)
{
  EFI_STATUS status = EFI_SUCCESS;

  if (gMpServices) {
    if (ApicID != Platform->BootProcessor) {

      status = gMpServices->StartupThisAP(
        gMpServices,
        proc,
        ApicID,
        NULL,
        1000000,
        param,
        NULL
      );

      if (EFI_ERROR(status)) {
        Print(L"[ERROR] Unable to execute on CPU %u,"
          "status code: 0x%x\n", ApicID, status);
      }

      return status;
    }
  }

  //
  // Platform has no MP services OR we are running on the desired package
  // ... so instead of dispatching, we will just do the work now

  proc(param);

  //
  // ... and that's that

  return status;
}

/*******************************************************************************
 *
 ******************************************************************************/

EFI_STATUS EFIAPI RunOnAllProcessors(
  const IN EFI_AP_PROCEDURE proc,
  IN VOID* param OPTIONAL)
{
  EFI_STATUS status = EFI_SUCCESS;
  EFI_EVENT mpEvent = NULL;
  UINTN eventIdx = 0;

  ///
  /// Start other processors with our workload 
  ///

  if (gMpServices) {

    status = gBS->CreateEvent(
      EVT_NOTIFY_SIGNAL,
      TPL_NOTIFY,
      EfiEventEmptyFunction,
      NULL,
      &mpEvent);

    if (EFI_ERROR(status)) {
      Print(L"[ERROR] Unable to create EFI_EVENT, code: 0x%x\n", status);
      mpEvent = NULL;
    }
    else {

      status = gMpServices->StartupAllAPs(
        gMpServices,
        proc,
        FALSE,
        &mpEvent,
        0,
        param,
        NULL
      );

      if (EFI_ERROR(status)) {
        Print(L"[ERROR] Unable to execute all CPUs, code: 0x%x\n", status);
        gBS->CloseEvent(mpEvent);
        mpEvent = NULL;
      }
    }
  }

  ///
  /// Execute workload on this CPU (BSP)
  ///
  
  proc(param);
  
  ///
  /// Wait until work is done
  ///

  if ((gMpServices) && (mpEvent) && (!EFI_ERROR(status))) {

    //
    // Wait for APs to finish

    gBS->WaitForEvent(1, &mpEvent, &eventIdx);
    gBS->CloseEvent(mpEvent);
  }

  return status;
}