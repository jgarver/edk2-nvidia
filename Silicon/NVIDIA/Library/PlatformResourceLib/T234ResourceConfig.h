/** @file
*
*  Copyright (c) 2020-2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#ifndef __T234_RESOURCE_CONFIG_H__
#define __T234_RESOURCE_CONFIG_H__

#include <Library/PlatformResourceLib.h>

BOOLEAN
T234UARTInstanceInfo(
  IN  UINT32                SharedUARTInstanceId,
  OUT UINT32                *UARTInstanceType,
  OUT EFI_PHYSICAL_ADDRESS  *UARTInstanceAddress
);

UINT64
T234GetDTBBaseAddress (
  IN UINTN CpuBootloaderAddress
);

EFI_STATUS
EFIAPI
T234GetCarveoutInfo (
  IN UINTN               CpuBootloaderAddress,
  IN TEGRA_CARVEOUT_TYPE Type,
  IN UINTN               *Base,
  IN UINT32              *Size
);

TEGRA_BOOT_TYPE
T234GetBootType (
  IN UINTN CpuBootloaderAddress
);

UINT64
T234GetGRBlobBaseAddress (
  IN UINTN CpuBootloaderAddress
);

BOOLEAN
T234GetGROutputBaseAndSize (
  IN  UINTN CpuBootloaderAddress,
  OUT UINTN *Base,
  OUT UINTN *Size
);

BOOLEAN
T234GetFsiNsBaseAndSize (
  IN  UINTN CpuBootloaderAddress,
  OUT UINTN *Base,
  OUT UINTN *Size
);

/**
  Retrieve Active Boot Chain Information

**/
EFI_STATUS
EFIAPI
T234GetActiveBootChain(
  IN  UINTN   CpuBootloaderAddress,
  OUT UINT32  *BootChain
);

/**
  Validate Active Boot Chain

**/
EFI_STATUS
EFIAPI
T234ValidateActiveBootChain(
  IN  UINTN   CpuBootloaderAddress
);

/**
  Get Platform Resource Information

**/
EFI_STATUS
EFIAPI
T234GetPlatformResourceInformation(
  IN UINTN                        CpuBootloaderAddress,
  IN TEGRA_PLATFORM_RESOURCE_INFO *PlatformResourceInfo
);

#endif //__T234_RESOURCE_CONFIG_H__
