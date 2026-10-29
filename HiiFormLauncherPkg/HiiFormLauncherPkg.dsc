[Defines]
  PLATFORM_NAME                  = HiiFormLauncherPkg
  PLATFORM_VERSION               = 0.1
  PLATFORM_GUID                  = 7E5F8EA4-6CFB-4A4D-8D8E-6FBEA88EE110
  DSC_SPECIFICATION               = 0x00010005
  OUTPUT_DIRECTORY                = Build/HiiFormLauncherPkg
  SUPPORTED_ARCHITECTURES         = X64
  BUILD_TARGETS                   = DEBUG|RELEASE
  SKUID_IDENTIFIER                = DEFAULT

[Packages]
  MdePkg/MdePkg.dec
  MdeModulePkg/MdeModulePkg.dec
  ShellPkg/ShellPkg.dec

[LibraryClasses]
  BaseLib                         | MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib                   | MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  DebugLib                        | MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  DevicePathLib                   | MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  UefiApplicationEntryPoint       | MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  HiiLib                          | MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  MemoryAllocationLib             | MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PcdLib                          | MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  RegisterFilterLib               | MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
  StackCheckLib                   | MdePkg/Library/StackCheckLibNull/StackCheckLibNull.inf
  ShellCEntryLib                  | ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  SafeIntLib                      | MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
  PrintLib                        | MdePkg/Library/BasePrintLib/BasePrintLib.inf
  UefiBootServicesTableLib        | MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiHiiServicesLib              | MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  UefiLib                         | MdePkg/Library/UefiLib/UefiLib.inf
  UefiRuntimeServicesTableLib     | MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf

[Components]
  HiiFormLauncherPkg/HiiFormLauncher.inf