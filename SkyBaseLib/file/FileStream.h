#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------------< 実装 >---------------------------------------------//

class CFileStream;
typedef SmartPointer< CFileStream >	CFileStreamPtr;
typedef CHash< CFileStreamPtr >		CFileStreamPtrHash;

/// <summary>
/// メモリ上に読み込んだファイルデータ（メモリストリームとやることはほぼ同じ）
/// </summary>
class CFileStream : public memory::CMemoryStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// コンストラクタ
	/// <summary>
	CFileStream( const skyString *path , void* pData , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize );
	CFileStream( const skyString *path , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize );

	/// <summary>
	/// １行読込
	/// <summary>
	skyBool vReadLine( skyString *pBuf , u32 bufLen , u32 offset = 0 );

	/// <summary>
	/// １行書き込み
	/// <summary>
	void vWriteLine( const skyString *pBuf , u32 len = 0 );

	/// <summary>
	/// 
	/// <summary>
	void vPrintf(){ SKY_PRINTF( _T( "CFileStream : %s \n" ) , m_pData ); }

};

} } }