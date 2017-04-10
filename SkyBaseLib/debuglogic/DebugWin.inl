#ifdef SKYLIB_COMPILER_VC10

#include <dbghelp.h>
#include <tlhelp32.h>
#include <string>
#include <vector>

#pragma comment(lib, "Dbghelp.lib") 

using namespace sky::lib;

static const u32 BUFFER_LEN = 512;

class LineInfo
{
public:
	std::string strFilePath;
	std::string strSymbol;
	DWORD ulLine;
	DWORD64 address;
	void Reset(){
		strFilePath.clear();
		strSymbol.clear();
		ulLine = 0;
		address = 0;
	};
};

typedef std::vector<LineInfo> LineInfoArray;

bool LoadModules(HANDLE hProcess, DWORD dwProcessId)
{
	HANDLE hSnap;

	hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);

	if (hSnap == (HANDLE)(-1))
	{
	    return false;
	}

    BOOL loop;
    MODULEENTRY32 entry;
    entry.dwSize = sizeof(entry);

    // モジュールを取得
    loop = Module32First(hSnap, &entry);

    while (loop)
    {
        TCHAR* image;
        TCHAR* module;

        image = _tcsdup(entry.szExePath);
        module = _tcsdup(entry.szModule);

        SymLoadModule64(hProcess, NULL, (PSTR)(image), (PSTR)(module),
                        (DWORD64)(entry.modBaseAddr),
                        static_cast<DWORD>(entry.modBaseSize));

        loop = Module32Next(hSnap, &entry);
    }
    CloseHandle(hSnap);

	return true;
}


// ****************************************************
/*!	@fn	void DebugSymbolUtil::AddressToLine(DWORD64 address, LineInfo &outInfo)
	@brief	アドレスを対応する関数名,ファイル名,行数に変換する
	@author	S.Matsumoto
	@date	2010/10/01
	@note	filename : SymbolManager.cpp
	@param	[in]  DWORD64  address
	@param	[out] LineInfo &outInfo
*/
// *****************************************************
void AddressToLine(DWORD64 address, LineInfo &outInfo)
{
    HANDLE hProcess = GetCurrentProcess();
    DWORD dwProcessId = GetCurrentProcessId();

	outInfo.Reset();
    // モジュール読み込み
    LoadModules(hProcess, dwProcessId);

    // ファイル名、行数、メソッド名を取得
    DWORD64 displacement64;
    ULONG64 buffer[(sizeof(SYMBOL_INFO) +
        MAX_SYM_NAME * sizeof(TCHAR) +
        sizeof(ULONG64) - 1) /
        sizeof(ULONG64)];
    PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;

    pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    pSymbol->MaxNameLen = MAX_SYM_NAME;

    if (SymFromAddr(hProcess, address, &displacement64, pSymbol))
    {
        DWORD displacement;
        IMAGEHLP_LINE64 Line;
        memset(&Line, 0, sizeof(Line));
        Line.SizeOfStruct = sizeof(Line);

		outInfo.strSymbol = pSymbol->Name;
        if (SymGetLineFromAddr64(hProcess, address, &displacement, &Line))
        {
			outInfo.strFilePath = Line.FileName;
			outInfo.ulLine = Line.LineNumber;
			outInfo.address = Line.Address;
        }
        else
        {
			char buf[ BUFFER_LEN ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
			sprintf_s( buf , BUFFER_LEN ,  "??? (---): %s\n", pSymbol->Name );
#else
			sprintf( buf ,  "??? (---): %s\n", pSymbol->Name );
#endif
            printf( buf );
			OutputDebugStringA( buf );
        }
    }
    else
    {
		char buf[ BUFFER_LEN ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
		sprintf_s( buf , BUFFER_LEN ,  "Failed to retrieve symbol information: 0x%08x\n", address );
#else
		sprintf( buf ,  "Failed to retrieve symbol information: 0x%08x\n", address );
#endif
        printf( buf );
		OutputDebugStringA( buf );
    }
}

// ****************************************************
/*!	@fn	void DebugSymbolUtil::DebugPrint( int index , DWORD64 address , LineInfo *lpLineInfo )
	@brief	
	@date	2010/10/11
	@note	filename : DebugSymbolUtil.cpp
*/
// *****************************************************
void DebugPrint( int index , DWORD64 address , LineInfo *lpLineInfo ) {

	const char* nazo = "???";
	const char* symbol; 
	const char* file;

	if( address && lpLineInfo ){
		AddressToLine( address , *lpLineInfo );
		if( lpLineInfo->strSymbol.empty() ) symbol = nazo;
		else                                symbol = lpLineInfo->strSymbol.c_str();
		if( lpLineInfo->strFilePath.empty() ) file = nazo;
		else                                  file = lpLineInfo->strFilePath.c_str();
		char buf[ BUFFER_LEN ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
		sprintf_s( buf , BUFFER_LEN ,  "%d: 0x%08x, %s() : %s(%u)\n", index, (unsigned int)lpLineInfo->address, symbol, file, lpLineInfo->ulLine );
#else
		sprintf( buf ,  "%d: 0x%08x, %s() : %s(%u)\n", index, (unsigned int)lpLineInfo->address, symbol, file, lpLineInfo->ulLine );
#endif
        printf( buf );
		OutputDebugStringA( buf );
	}
}

// ****************************************************
/*!	@fn	void DebugSymbolUtil::OutputCurrentStackFrameToDebugStream()
	@brief	呼び出された時点のスタックフレームをダンプします
	@author	S.Matsumoto
	@date	2010/10/06
	@note	filename : DebugSymbolUtil.cpp
*/
// *****************************************************
void OutputCurrentStackFrameToDebugStream()
{
    HANDLE hProcess = GetCurrentProcess();
//    DWORD dwProcessId = GetCurrentProcessId();

    // Context 取得
    HANDLE hThread = GetCurrentThread();
    CONTEXT context;
    memset(&context, 0, sizeof(context));
    context.ContextFlags = CONTEXT_FULL;

#if defined(_M_IX86)
	// ContextのIA32環境での正しい取得方法が謎だったのでレジスタから直接取得
    __asm    call(x);
    __asm x: pop eax;
    __asm    mov context.Eip, eax;
    __asm    mov context.Ebp, ebp;
    __asm    mov context.Esp, esp;
#else
    // 64-bit環境ではRtlCaptureContextが使えるらしい(TEST出来る環境がないのでまた未検証)
    RtlCaptureContext(&context);
#endif

    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(stackFrame));

#if defined(_M_IX86)
    stackFrame.AddrPC.Offset = context.Eip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context.Ebp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context.Esp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
    DWORD machineType = IMAGE_FILE_MACHINE_I386;
#elif defined(_M_AMD64)
    stackFrame.AddrPC.Offset = context.Rip;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Offset = context.Rbp;
    stackFrame.AddrFrame.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context.Rsp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
    DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
#elif defined(_M_IA64)
    stackFrame.AddrPC.Offset = context.StIIP;
    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrStack.Offset = context.IntSp;
    stackFrame.AddrStack.Mode = AddrModeFlat;
    stackFrame.AddrBStore.Offset = context.RsBSP;
    stackFrame.AddrBStore.Mode = AddrModeFlat;
    DWORD machineType = IMAGE_FILE_MACHINE_IA64;
#else
#error "Not Supported."
#endif
	#define ARRAY_NUM 64
	DWORD64	akReturnAddress[ARRAY_NUM];
	memset( akReturnAddress, 0, sizeof( akReturnAddress ) );
	for(int i=0; i<ARRAY_NUM; i++){
		// スタックフレームを取得
		if (!StackWalk64(machineType, hProcess, hThread, &stackFrame, &context, NULL, NULL, NULL, NULL)){
		    break;
		}
		// スタックフレームを検証
		if (stackFrame.AddrPC.Offset == stackFrame.AddrReturn.Offset)break;		// エンドレスになるので終わり
		if (stackFrame.AddrPC.Offset == 0)break;								// 不正なスタックフレーム
		if (stackFrame.AddrReturn.Offset == 0)break;							// 最後のスタックフレーム
		akReturnAddress[i] = stackFrame.AddrReturn.Offset;
	}
	
	LineInfo lineInfo;
	for( int i=0; i<ARRAY_NUM; i++ ){

		if( akReturnAddress[i] ){
			DebugPrint( i , akReturnAddress[i] , &lineInfo );
		}
	}
}

#endif
