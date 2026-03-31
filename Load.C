LSTATUS __fastcall sub_140027CD0(__int64 a1, BYTE *a2, HANDLE *a3)
{
  HANDLE CurrentProcess; // rax
  LSTATUS result; // eax
  __int64 v7; // rax
  HMODULE ModuleHandleW; // rax
  NTSTATUS (__stdcall *NtLoadDriver)(PUNICODE_STRING); // rax
  unsigned int v10; // ebx
  ULONG (__stdcall *RtlNtStatusToDosError)(NTSTATUS); // rax
  HMODULE v12; // rax
  DWORD v13; // eax
  BYTE Data[8]; // [rsp+30h] [rbp-D0h] BYREF
  HKEY phkResult; // [rsp+38h] [rbp-C8h] BYREF
  void *TokenHandle; // [rsp+40h] [rbp-C0h] BYREF
  struct _LUID Luid; // [rsp+48h] [rbp-B8h] BYREF
  __int16 v18; // [rsp+50h] [rbp-B0h] BYREF
  const wchar_t *v19; // [rsp+58h] [rbp-A8h]
  struct _TOKEN_PRIVILEGES NewState; // [rsp+60h] [rbp-A0h] BYREF
  wchar_t v21[264]; // [rsp+70h] [rbp-90h] BYREF

  CurrentProcess = GetCurrentProcess();
  if ( OpenProcessToken(CurrentProcess, 0x28u, &TokenHandle)
    && LookupPrivilegeValueW(0, L"SeLoadDriverPrivilege", &Luid) )
  {
    NewState.Privileges[0].Luid = Luid;
    NewState.PrivilegeCount = 1;
    NewState.Privileges[0].Attributes = 2;
    if ( AdjustTokenPrivileges(TokenHandle, 0, &NewState, 0x10u, 0, 0) )
      GetLastError();
    CloseHandle(TokenHandle);
  }
  result = RegCreateKeyW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Services\\PROCMON24", &phkResult); // Create reg
  if ( !result )
  {
    *(_DWORD *)Data = 2;
    RegSetValueExW(phkResult, L"Type", 0, 4u, Data, 4u);
    *(_DWORD *)Data = 1;
    RegSetValueExW(phkResult, L"ErrorControl", 0, 4u, Data, 4u);
    *(_DWORD *)Data = 3;
    RegSetValueExW(phkResult, L"Start", 0, 4u, Data, 4u);
    sub_140003120(v21, (wchar_t *)L"\\??\\%s");
    v7 = -1;
    do
      ++v7;
    while ( v21[v7] );
    RegSetValueExW(phkResult, L"ImagePath", 0, 1u, (const BYTE *)v21, 2 * v7);
    sub_140026A90(phkResult, a2);
    ModuleHandleW = GetModuleHandleW(L"ntdll.dll");
    NtLoadDriver = (NTSTATUS (__stdcall *)(PUNICODE_STRING))GetProcAddress(ModuleHandleW, "NtLoadDriver");
    v19 = L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\PROCMON24";
    v18 = 122;
    v10 = ((__int64 (__fastcall *)(__int16 *))NtLoadDriver)(&v18);
    RegDeleteKeyW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Services\\PROCMON24\\Enum");
    RegDeleteKeyW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Services\\PROCMON24\\Security");
    RegDeleteKeyW(HKEY_LOCAL_MACHINE, L"System\\CurrentControlSet\\Services\\PROCMON24\\Parameters");
    RegDeleteValueW(phkResult, L"Type");
    RegDeleteValueW(phkResult, L"ErrorControl");
    RegDeleteValueW(phkResult, L"Start");
    RegDeleteValueW(phkResult, L"ImagePath");
    RegCloseKey(phkResult);
    if ( !v10 || v10 == -1073741554 )
    {
      return sub_140028000(a3);
    }
    else
    {
      RtlNtStatusToDosError = (ULONG (__stdcall *)(NTSTATUS))qword_1401422A8;
      if ( !qword_1401422A8 )
      {
        v12 = GetModuleHandleW(L"ntdll.dll");
        RtlNtStatusToDosError = (ULONG (__stdcall *)(NTSTATUS))GetProcAddress(v12, "RtlNtStatusToDosError");
        qword_1401422A8 = (__int64)RtlNtStatusToDosError;
      }
      v13 = ((__int64 (__fastcall *)(_QWORD))RtlNtStatusToDosError)(v10);
      SetLastError(v13);
      return GetLastError();
    }
  }
  return result;
}
