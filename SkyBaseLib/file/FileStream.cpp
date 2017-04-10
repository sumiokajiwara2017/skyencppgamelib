#include "StdAfx.h"
#include "SkyBaseLib/File.h"

SkyImplementRTTI( sky::lib::file::CFileStream , sky::lib::memory::CMemoryStream );

namespace sky { namespace lib { namespace file { 

CFileStream::CFileStream( const skyString *path , void* pData , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize )
: CMemoryStream( path , pData , size , isEnableDelete , isReadOnly , isEnableChengeSize )
{
}
CFileStream::CFileStream( const skyString *path , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize )
: CMemoryStream( path , size , isEnableDelete , isReadOnly , isEnableChengeSize )
{
}

skyBool CFileStream::vReadLine( skyString *pBuf , u32 bufLen , u32 offset )
{
	SKY_ASSERT_MSG( m_pData != skyNull , _T( "There is no data yet. " ) ); //�܂��f�[�^������܂���B

	//�P�����ڂ��I�[�q�ɂ��Ă����i�����ǂ߂Ȃ������ꍇ�A�󔒕������Ԃ�悤�ɂ���j
	CMemoryUtil_::Copy( pBuf , &EOT , sizeof ( skyString ) );

	//�V�[�N�ʒu�����łɃt�@�C���I�[�������ꍇ�����ǂ܂��ɕԂ�
	if ( m_Seek == m_Size - 1 )
	{
		return skyFalse;
	}

	//�w��̃I�t�Z�b�g�l���v���X����
	m_Seek += offset;

	//���̉��s�R�[�h�܂ł̒�����}��
	u32 readLen; 
	skyBool isFound = CStringUtil_::StrchrDistance( ( skyString * )&m_pData[ m_Seek ] , EC , readLen );
	
	//�O�������ꍇ�A�����ǂ܂��ɕԂ�
	if ( readLen == 0 )
	{
		return skyFalse;
	}

	if ( isFound == skyTrue )
	{
		//�����������s�R�[�h�͌��ʂɕԂ��Ȃ�
		readLen -= 1;
	}

	//����+1�͏I�[�q�i�[��
    SKY_ASSERT_MSG( ( readLen + 1 ) * sizeof( skyString ) <= bufLen , _T( "Buffer over run. dataSize=[ %d ] bufLen=[ %d ]" ), m_Size , bufLen );

	//�ǂݍ��݌��ʂƖ����ɏI�[�q���t���ĕԂ�
	CMemoryUtil_::Copy( pBuf , ( skyString * )&m_pData[ m_Seek ] , readLen * sizeof ( skyString ) );
	CMemoryUtil_::Copy( pBuf + readLen , &EOT, sizeof ( skyString ) ); //�I�[�q������

	//�ǂݍ��񂾕������V�[�N�������߂�
	m_Seek =  m_Seek + ( ( readLen + 2 ) * sizeof ( skyString ) ); //+2�͏I�[�q�ȍ~�ɃV�[�N���邽�߁B

	//�J�[�\�����f�[�^�̏I�[���z���Ă��܂�����f�[�^�̏I�[�܂Ŗ߂�
	if ( m_Seek >= m_Size )
	{
		m_Seek = m_Size - 1;
	}

	return skyTrue;
}

void CFileStream::vWriteLine( const skyString *pBuf , u32 len )
{
	SKY_ASSERT_MSG( m_pData != skyNull , _T( "There is no data yet. " ) ); //�܂��f�[�^������܂���B

	//���̉��s�R�[�h�܂ł̒�����}��
	u32 readLen = len;

	if ( len == 0 )
	{
		skyBool isFound = CStringUtil_::StrchrDistance( pBuf , EC , readLen );
	
		//�O�������ꍇ�A�����ǂ܂��ɕԂ�
		if ( readLen == 0 )
		{
			return;
		}

		if ( isFound == skyTrue )
		{
			//�����������s�R�[�h������
			readLen -= 1;
		}
	}

	//���s�R�[�h�܂ł̒������T�C�Y�ɂ���
	u32 bufSize = readLen * sizeof( skyString );

	//���s�R�[�h�܂ł̕�������������
	vWriteStream( ( u8 * )pBuf , bufSize );
	vWriteStream( ( u8 * )&EC , sizeof ( skyString ) ); //���s�R�[�h������
}

} } }
