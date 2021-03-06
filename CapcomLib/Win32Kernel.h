#pragma once

// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/wdm/ne-wdm-_pool_type
typedef enum _POOL_TYPE {
	NonPagedPool,
	NonPagedPoolExecute = NonPagedPool,
	PagedPool,
	NonPagedPoolMustSucceed = NonPagedPool + 2,
	DontUseThisType,
	NonPagedPoolCacheAligned = NonPagedPool + 4,
	PagedPoolCacheAligned,
	NonPagedPoolCacheAlignedMustS = NonPagedPool + 6,
	MaxPoolType,
	NonPagedPoolBase = 0,
	NonPagedPoolBaseMustSucceed = NonPagedPoolBase + 2,
	NonPagedPoolBaseCacheAligned = NonPagedPoolBase + 4,
	NonPagedPoolBaseCacheAlignedMustS = NonPagedPoolBase + 6,
	NonPagedPoolSession = 32,
	PagedPoolSession = NonPagedPoolSession + 1,
	NonPagedPoolMustSucceedSession = PagedPoolSession + 1,
	DontUseThisTypeSession = NonPagedPoolMustSucceedSession + 1,
	NonPagedPoolCacheAlignedSession = DontUseThisTypeSession + 1,
	PagedPoolCacheAlignedSession = NonPagedPoolCacheAlignedSession + 1,
	NonPagedPoolCacheAlignedMustSSession = PagedPoolCacheAlignedSession + 1,
	NonPagedPoolNx = 512,
	NonPagedPoolNxCacheAligned = NonPagedPoolNx + 4,
	NonPagedPoolSessionNx = NonPagedPoolNx + 32
} POOL_TYPE;

/// Defines a static UNICODE_STRING
#define DECLARE_UNICODE_STRING(_var, _string) \
	WCHAR _var ## _buffer[] = _string; \
	__pragma(warning(push)) \
	__pragma(warning(disable:4221)) __pragma(warning(disable:4204)) \
	UNICODE_STRING _var = { sizeof(_string)-sizeof(WCHAR), sizeof(_string), (PWCH)_var ## _buffer } \
	__pragma(warning(pop))

//// https://forum.sysinternals.com/using-rtlinitunicodestring-in-cpp-code_topic20109.html
//inline VOID RtlInitUnicodeString(PUNICODE_STRING DestinationString, const WCHAR * SourceString)
//{
//	DestinationString->Buffer = (PWCHAR)(SourceString);
//	DestinationString->MaximumLength = DestinationString->Length = (USHORT)(wcslen(SourceString) * sizeof(WCHAR));
//}

/// https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/content/wdm/nf-wdm-exallocatepoolwithtag
typedef PVOID(NTAPI *ExAllocatePoolWithTagFunc)(
	POOL_TYPE    PoolType,
	SIZE_T    NumberOfBytes,
	ULONG     Tag);

/// MMGetSystemRoutine is the GetProcAddress of the kernel. Capcom.sys passes us the address
/// of this function as the first argument
typedef PVOID(NTAPI *MmGetSystemRoutineFunc)(PUNICODE_STRING SystemRoutineName);

// From Process Hacker for NtQuerySystemInformation
typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
	HANDLE Section;
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	CHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;


using modules_map = std::unordered_map<std::string, RTL_PROCESS_MODULE_INFORMATION>;

#define SystemModuleInformation (SYSTEM_INFORMATION_CLASS)11
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004
