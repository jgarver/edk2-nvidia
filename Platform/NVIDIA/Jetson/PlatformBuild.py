# Copyright (c) 2021-2022 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
#
# SPDX-License-Identifier: BSD-2-Clause-Patent


###############################################################################
# Stuart build for NVIDIA Jetson UEFI firmware
#
# Run with:
# $ build_uefi.sh -c \
#      edk2-nvidia/Platform/NVIDIA/Jetson/PlatformBuild.py


import os
from pathlib import Path
from edk2nv.stuart import NVIDIASettingsManager, NVIDIAPlatformBuilder
import glob
import shutil


class JetsonSettingsManager(NVIDIASettingsManager):
    ''' SettingsManager for NVIDIA's Jetson platform. '''

    def GetName(self):
        return "Jetson"

    def GetActiveScopes(self):
        return super().GetActiveScopes() + ["jetson"]

    def GetFirmwareVersionBase(self):
        fvb = os.getenv("FIRMWARE_VERSION_BASE")
        if not fvb:
            fvb = "r35.0"
        return fvb

    def GetFirmwareVolume(self):
        return "FV/UEFI_NS.Fv"

    def GetBootAppName(self):
        return "AARCH64/L4TLauncher.efi"

    def GetDscName(self):
        return "edk2-nvidia/Platform/NVIDIA/Jetson/Jetson.dsc"

    def GetDtbPath(self):
        return "AARCH64/Silicon/NVIDIA/Tegra/DeviceTree/DeviceTree/OUTPUT"

    def GetVariablesDescFile(self):
        return "edk2-nvidia/Platform/NVIDIA/Jetson/JetsonVariablesDesc.json"


class PlatformBuilder(NVIDIAPlatformBuilder):
    ''' PlatformBuilder for NVIDIA's Jetson. '''
    SettingsManager = JetsonSettingsManager

    def PlatformPostBuild(self):
        ''' Additional build steps for Jetson platform. '''
        ret = super().PlatformPostBuild()
        if ret != 0:
            return ret

        build_dir = Path(self.env.GetValue("BUILD_OUTPUT_BASE"))
        dtb_path = self.settings.GetDtbPath()
        target = self.settings.GetTarget()

        dtbs = (build_dir / dtb_path).glob("*.dtb")

        for src_dtb in dtbs:
            dest_dtb = Path("images") / f"{src_dtb.stem}_Jetson_{target}.dtbo"
            shutil.copyfile(src_dtb, dest_dtb)

        return 0
