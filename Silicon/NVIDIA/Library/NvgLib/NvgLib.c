/** @file

  NVG Library

  Copyright (c) 2021-2022, NVIDIA CORPORATION & AFFILIATES. All rights reserved.

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/DebugLib.h>
#include <Library/NvgLib.h>
#include <Library/PcdLib.h>

#define TEGRA_NVG_CHANNEL_NUM_CORES_CMD        20
#define TEGRA_NVG_CHANNEL_LOGICAL_TO_MPIDR_CMD 23

#define AA64_MRS(reg, var)  do { \
  asm volatile ("mrs %0, "#reg : "=r"(var) : : "memory", "cc"); \
} while (FALSE)

static inline void WriteNvgChannelIdx(UINT64 Channel)
{
  asm volatile ("msr s3_0_c15_c1_2, %0" : : "r"(Channel) : "memory", "cc");
}

static inline void WriteNvgChannelData(UINT64 Data)
{
  asm volatile ("msr s3_0_c15_c1_3, %0" : : "r"(Data) : "memory", "cc");
}

static inline UINT64 ReadNvgChannelData(void)
{
  UINT64 Reg;
  AA64_MRS(s3_0_c15_c1_3, Reg);
  return Reg;
}

UINT32
EFIAPI
NvgGetNumberOfEnabledCpuCores (
  VOID
  )
{
  UINT64 Data;

  WriteNvgChannelIdx(TEGRA_NVG_CHANNEL_NUM_CORES_CMD);
  Data = ReadNvgChannelData();

  return (Data & 0xF);
}

EFI_STATUS
EFIAPI
NvgConvertCpuLogicalToMpidr (
  IN  UINT32 LogicalCore,
  OUT UINT64 *Mpidr
  )
{
  UINT32 NumCores;
  UINT64 Data = 0;

  NumCores = NvgGetNumberOfEnabledCpuCores();
  if (LogicalCore < NumCores) {
    WriteNvgChannelIdx (TEGRA_NVG_CHANNEL_LOGICAL_TO_MPIDR_CMD);

    /* Write the logical core id */
    WriteNvgChannelData (LogicalCore);

    /* Read-back the MPIDR */
    Data = ReadNvgChannelData ();
    *Mpidr = (Data & 0xFFFFFFFF);

    DEBUG ((DEBUG_INFO, "NVG: Logical CPU: %u; MPIDR: 0x%x\n", LogicalCore, *Mpidr));
  } else {
    DEBUG ((DEBUG_ERROR, "Core: %u is not present\r\n", LogicalCore));
    *Mpidr = 0;
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  Fills in bit map of enabled cores

**/
EFI_STATUS
EFIAPI
NvgGetEnabledCoresBitMap (
  IN  UINT64    *EnabledCoresBitMap
  )
{
  UINTN     CpuCount;

  CpuCount = NvgGetNumberOfEnabledCpuCores ();
  ASSERT (CpuCount <= 64);

  EnabledCoresBitMap[0] = (1ULL << CpuCount) - 1;

  return EFI_SUCCESS;
}
