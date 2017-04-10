#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------------< ���� >---------------------------------------------//

class CFileStream;
typedef SmartPointer< CFileStream >	CFileStreamPtr;
typedef CHash< CFileStreamPtr >		CFileStreamPtrHash;

/// <summary>
/// ��������ɓǂݍ��񂾃t�@�C���f�[�^�i�������X�g���[���Ƃ�邱�Ƃ͂قړ����j
/// </summary>
class CFileStream : public memory::CMemoryStream
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// <summary>
	CFileStream( const skyString *path , void* pData , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize );
	CFileStream( const skyString *path , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize );

	/// <summary>
	/// �P�s�Ǎ�
	/// <summary>
	skyBool vReadLine( skyString *pBuf , u32 bufLen , u32 offset = 0 );

	/// <summary>
	/// �P�s��������
	/// <summary>
	void vWriteLine( const skyString *pBuf , u32 len = 0 );

	/// <summary>
	/// 
	/// <summary>
	void vPrintf(){ SKY_PRINTF( _T( "CFileStream : %s \n" ) , m_pData ); }

};

} } }