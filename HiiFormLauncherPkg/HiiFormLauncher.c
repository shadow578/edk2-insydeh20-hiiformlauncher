#include <Uefi.h>
#include <Protocol/FormBrowser2.h>
#include <Uefi/UefiInternalFormRepresentation.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/HiiLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ShellCEntryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

// InsydeH20 AMD CBS setup
// B04535E3-3004-4946-9EB7-149428983053; 0x7000
STATIC EFI_GUID gH20CbsFormSetGuid =
    {0xB04535E3, 0x3004, 0x4946, {0x9E, 0xB7, 0x14, 0x94, 0x28, 0x98, 0x30, 0x53}};

// InsydeH20 AMD PBS setup
// B863B959-0EC6-4033-99C1-8FD89F040222; 0xB
STATIC EFI_GUID gH20PbsFormSetGuid =
    {0xB863B959, 0x0EC6, 0x4033, {0x99, 0xC1, 0x8F, 0xD8, 0x9F, 0x04, 0x02, 0x22}};

// InsydeH20 Boot Maintenance Manager
// 642237C7-35D4-472D-8365-12E0CCF27A22; 0x1001
STATIC EFI_GUID gH20BootMaintenanceManagerFormSetGuid = 
    {0x642237C7, 0x35D4, 0x472D, {0x83, 0x65, 0x12, 0xE0, 0xCC, 0xF2, 0x7A, 0x22}};

// InsydeH20 Front Page
// 9E0C30BC-3F06-4BA6-8288-09179B855DBE; 0x1000
STATIC EFI_GUID gH20FrontPageFormSetGuid = 
    {0x9E0C30BC, 0x3F06, 0x4BA6, {0x82, 0x88, 0x09, 0x17, 0x9B, 0x85, 0x5D, 0xBE}};

// InsydeH20 H20Ove Configuration
// D1D7208F-84EC-411A-9BED-98E000BA7495; 0x470
STATIC EFI_GUID gH20OveConfigurationFormSetGuid = 
    {0xD1D7208F, 0x84EC, 0x411A, {0x9B, 0xED, 0x98, 0xE0, 0x00, 0xBA, 0x74, 0x95}};

INTN EFIAPI ShellAppMain(IN UINTN Argc, IN CHAR16 **Argv)
{
    EFI_FORM_BROWSER2_PROTOCOL *Browser;
    EFI_HII_HANDLE *Handles;
    EFI_HII_HANDLE MatchedHandle;
    EFI_IFR_FORM_SET *FormSet;
    UINTN FormSetSize;
    EFI_BROWSER_ACTION_REQUEST ActionRequest;
    EFI_GUID *FormSetGuid;
    EFI_GUID GenericFormSetGuid;
    CHAR16 *EndPointer;
    UINTN ParsedFormId;
    UINT16 FormId;
    UINTN HandleCount;
    EFI_STATUS Status;

    if (Argc < 2)
    {
        Print(L"HII Form Launcher\n");
        Print(L"Usage: HiiFormLauncher.efi <formset-guid> <form-id-hex>\n");
        Print(L"       HiiFormLauncher.efi <alias>\n");
        Print(L"\n");
        Print(L"Aliases (may not be available):\n");
        Print(L"  H20CBS - InsydeH20 AMD CBS (GUID: %g, ID: 0x7000)\n", &gH20CbsFormSetGuid);
        Print(L"  H20PBS - InsydeH20 AMD PBS (GUID: %g, ID: 0x000B)\n", &gH20PbsFormSetGuid);
        Print(L"  H20BMM - InsydeH20 Boot Maintenance Manager (GUID: %g, ID: 0x1001)\n", &gH20BootMaintenanceManagerFormSetGuid);
        Print(L"  H20FP  - InsydeH20 Front Page (GUID: %g, ID: 0x1000)\n", &gH20FrontPageFormSetGuid);
        Print(L"  H20Ove - InsydeH20 OVE Configuration (GUID: %g, ID: 0x0470)\n", &gH20OveConfigurationFormSetGuid);
        return EFI_INVALID_PARAMETER;
    }

    if (StrCmp(Argv[1], L"H20CBS") == 0)
    {
        FormSetGuid = &gH20CbsFormSetGuid;
        FormId = 0x7000;
    }
    else if (StrCmp(Argv[1], L"H20PBS") == 0)
    {
        FormSetGuid = &gH20PbsFormSetGuid;
        FormId = 0x000B;
    }
    else if (StrCmp(Argv[1], L"H20BMM") == 0)
    {
        FormSetGuid = &gH20BootMaintenanceManagerFormSetGuid;
        FormId = 0x1001;
    }
    else if (StrCmp(Argv[1], L"H20FP") == 0)
    {
        FormSetGuid = &gH20FrontPageFormSetGuid;
        FormId = 0x1000;
    }
    else if (StrCmp(Argv[1], L"H20Ove") == 0)
    {
        FormSetGuid = &gH20OveConfigurationFormSetGuid;
        FormId = 0x0470;
    }
    else if (Argc == 3)
    {
        Status = StrToGuid(Argv[1], &GenericFormSetGuid);
        if (EFI_ERROR(Status))
        {
            Print(L"Invalid formset GUID: %s\n", Argv[1]);
            return EFI_INVALID_PARAMETER;
        }

        EndPointer = NULL;
        Status = StrHexToUintnS(Argv[2], &EndPointer, &ParsedFormId);
        if (EFI_ERROR(Status) || EndPointer == Argv[2] || *EndPointer != L'\0' || ParsedFormId > 0xFFFF)
        {
            Print(L"Invalid form ID: %s\n", Argv[2]);
            return EFI_INVALID_PARAMETER;
        }

        FormSetGuid = &GenericFormSetGuid;
        FormId = (UINT16)ParsedFormId;
    }
    else
    {
        Print(L"Unknown formset or incorrect argument count\n");
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->LocateProtocol(
        &gEfiFormBrowser2ProtocolGuid,
        NULL,
        (VOID **)&Browser);
    if (EFI_ERROR(Status))
    {
        Print(L"FormBrowser2 unavailable: %r\n", Status);
        return Status;
    }

    Handles = HiiGetHiiHandles(NULL);
    if (Handles == NULL)
    {
        Print(L"No HII handles found\n");
        return EFI_NOT_FOUND;
    }

    MatchedHandle = NULL;
    for (HandleCount = 0; Handles[HandleCount] != NULL; ++HandleCount)
    {
        FormSet = NULL;
        FormSetSize = 0;
        Status = HiiGetFormSetFromHiiHandle(
            Handles[HandleCount],
            &FormSet,
            &FormSetSize);
        if (!EFI_ERROR(Status) && FormSet != NULL)
        {
            if (CompareGuid(&FormSet->Guid, FormSetGuid))
            {
                MatchedHandle = Handles[HandleCount];
                FreePool(FormSet);
                break;
            }
            FreePool(FormSet);
        }
    }

    if (MatchedHandle == NULL)
    {
        Print(L"Formset not found: %g\n", FormSetGuid);
        FreePool(Handles);
        return EFI_NOT_FOUND;
    }

    Print(L"Formset found on HII handle %p\n", MatchedHandle);

    ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;

    Status = Browser->SendForm(
        Browser,
        &MatchedHandle,
        1,
        FormSetGuid,
        FormId,
        NULL,
        &ActionRequest);

    FreePool(Handles);
    Print(L"SendForm returned: %r\n", Status);
    return Status;
}