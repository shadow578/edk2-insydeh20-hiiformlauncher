# InsydeH20 HII Form Launcher


this project adds a single EFI application that allows launching arbitrary HII forms by specifying the formset GUID and form ID in an EFI shell.
some aliases are provided.

## Building

1. git clone --recurse-submodules https://github.com/shadow578/edk2-insydeh20-hiiformlauncher.git
2. open this project in a devcontainer, using vscode
3. make -C BaseTools
4. source edksetup.sh
5. build -a X64 -t GCC -p HiiFormLauncherPkg/HiiFormLauncherPkg.dsc -b RELEASE

the build output will be in Build/HiiFormLauncherPkg/DEBUG_GCC/X64/HiiFormLauncher.efi

## Usage

place the build .efi file on a FAT32 formatted USB drive, and boot into the EFI shell. 
if your mainboard does not provide a shell, you can build the shell package from (this) edk2 project, OR download a prebuild efi binary from https://github.com/pbatard/UEFI-Shell.

once in the shell, use `fs0:` to switch to the USB drive (may be different for you!).
now, use `HiiFormLauncher.efi <formset GUID> <form ID>` to launch a form.
alternatively, you can use the aliases.
e.g. to launch into AMD CBS, use `HiiFormLauncher.efi H20CBS`.


## Finding Forms

to find more formsets, extract PE image body using UEFITool, then use ifrextractor.exe to extract the IFR.
at the top, you'll find the formset GUIDs and form IDs.

for example for H2OUveConfigUtilDxe (B60D67C6-9546-4C1D-B1DA-681B001C53C0), you'll get this output.
to launch the form, you'd use CbsLaunch.efi D1D7208F-84EC-411A-9BED-98E000BA7495 0x470 (1st ist the FormSet Guid: <>; 2nd is the Form FormId: <>).

```
Program version: 1.6.1, Extraction mode: UEFI, SHA256: daef721a250b899a91824d07414dc377de0fbcfbc02e40e85861ec161f73e166
FormSet Guid: D1D7208F-84EC-411A-9BED-98E000BA7495, Title: "H2OUve Configuration", Help: "Show H2OUve Configuration"
	DefaultStore DefaultId: 0x0, Name: ""
	DefaultStore DefaultId: 0x1, Name: ""
	VarStore Guid: EF0849B6-FAD0-40E9-9107-974AEB8787A2, VarStoreId: 0x46F, Size: 0x4, Name: "VarEditFunDis"
	Form FormId: 0x470, Title: "H2OUve Setup"
		Subtitle Prompt: "H2OUve Setup", Help: "", Flags: 0x0
		End 
		Subtitle Prompt: " ", Help: "", Flags: 0x0
		End 
		OneOf Prompt: "H2OUVE Support", Help: "Enable/Disable interface of settings of SCU for H2OUVE tool.", QuestionFlags: 0x0, QuestionId: 0x1, VarStoreId: 0x46F, VarOffset: 0x0, Flags: 0x12, Size: 32, Min: 0x0, Max: 0x1, Step: 0x0
			OneOfOption Option: "Disabled" Value: 1
			OneOfOption Option: "Enabled" Value: 0, Default
		End 
	End 
End 
```

