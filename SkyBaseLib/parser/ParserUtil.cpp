#include "StdAfx.h"
#include "SkyBaseLib/Parser.h"

namespace sky { namespace lib { namespace parser {

void CParserUtil::ParseStringPtn0( const skyMString *src , string::CStringList &destlist )
{
	if ( CStringUtil_::Len( src ) == 0 ) return;

	u32 pointer = 0;
	u32 newTokenPointer = 0;
	skyMString token[ TOKEN_LENGTH_MAX ];
	for( ;; )
	{
		switch ( src[ pointer ] )
		{
            case '\0': //������I��
			{
				if ( newTokenPointer > 0 )
				{
                    //���݂̃g�[�N���̖����ɏI�[�q������
                    token[ newTokenPointer ] = '\0';

                    //���݂̃g�[�N�������X�g�ɒǉ�
                    destlist.AddTail( token ); //�����CString�ɕϊ����Ċi�[���Ă����
				}
				goto END;

				break;
			}
            case ' ':	 //��؂�
			{
                //���p�X�y�[�X���A���ő��݂����ꍇ�̓X�L�b�v����
                if ( newTokenPointer > 0 )
				{
                    //���݂̃g�[�N���̖����ɏI�[�q������
                    token[ newTokenPointer ] = '\0';

                    //���݂̃g�[�N�������X�g�ɒǉ�
                    destlist.AddTail( token );

                    //�����𓪂���i�[
                    newTokenPointer = 0;
				}
				break;
			}
            default: //��؂�ȊO(V0001���̒l�ƒf�肷��
			{
				SKY_ASSERT_MSG( newTokenPointer < TOKEN_LENGTH_MAX, _T( "The length of the token is invalid. " ) ); //�g�[�N���̒����������ł��B

				token[ newTokenPointer ] = src[ pointer ];
				newTokenPointer++;

				break;
			}
		}
		pointer++;
	}
END:
;
}

void CParserUtil::ParseStringPtn0( const skyWString *src , string::CStringList &destlist )
{
    //������̒����������ꍇ�͂��̂܂ܕԂ�
    if ( CStringUtil_::Len( src ) == 0 ) return;

	skyMString *cValue = CStringUtil_::CreateMString( src );
	ParseStringPtn0( cValue , destlist );
	CStringUtil_::DeleteMString( cValue );
}

void CParserUtil::ParseStringPtn1( const skyMString *src , CValues32Map &destmap )
{
    //������̒����������ꍇ�͂��̂܂ܕԂ�
    if ( CStringUtil_::Len( src ) == 0 ) return;

	skyMString  tokenTbl[ TOKEN_NUM_MAX ][ TOKEN_LENGTH_MAX ];
	u32         tokenCount = 0;

//--�p�[�X��2�i�K�ɕ����Ă��邪�A�����̌��₷�����d�����������ł��B�����s�����ɂȂ����ꍇ�p�[�X��1�i�K�ɂ����ق��������Ǝv���܂�

//----------------------��͈�i�K��----------------------------------��

	//��������w_�x��؂�Ńg�[�N���ɕ�������
	//��������e��m_Argv�Ɋi�[����
	u32 pointer = 0;
	u32 newTokenPointer = 0;
	for( ;; )
	{
		switch ( src[ pointer ] )
		{
			case '\0':		//������I��
			{
				if ( newTokenPointer > 0 )
				{
					//���݂̃g�[�N���̖����ɏI�[�q������
					tokenTbl[ tokenCount ][ newTokenPointer ] = '\0';

					//���̃g�[�N���ֈڍs
					tokenCount++;
				}
				goto END;
			}
			case '_':	//��؂�
			{
				//�A���_�[�o�[���A���ő��݂����ꍇ�̓X�L�b�v����
				if ( newTokenPointer > 0 )
				{
					//���݂̃g�[�N���̖����ɏI�[�q������
					tokenTbl[ tokenCount ][ newTokenPointer ] = '\0';

					//���̃g�[�N���ֈڍs
					tokenCount++;

					//�����𓪂���i�[
					newTokenPointer = 0;
				}
				break;
			}
			default: //��؂�ȊO(V0001���̒l�ƒf�肷��
			{
				SKY_ASSERT_MSG( newTokenPointer < TOKEN_LENGTH_MAX, _T( "The length of the token is invalid. " ) ); //�g�[�N���̒����������ł��B

				tokenTbl[ tokenCount ][ newTokenPointer ] = src[ pointer ];
				newTokenPointer++;

				break;
			}
		}
		pointer++;
	}
END:

//----------------------��͓�i�K��----------------------------------��

	//���̃g�[�N���𕶎��Ɛ����ɕ������違�������L�[�ɁA������l�ɂ��}�b�v�ɓo�^����
	skyMString key[ KEY_LENGHT_MAX ];
	u32 keyPointer;
	skyMString value[ VALUE_LENGHT_MAX ];
	u32 valuePointer;
	skyMString value_i[ VALUE_LENGHT_MAX ];
	u32 valuePointer_i;
	skyBool isValueMode;
	for ( u32 i = 0 ; i < tokenCount ; i++ )
	{
		//��̃g�[�N������͂���
		keyPointer = 0;
		valuePointer = 0;
		valuePointer_i = 0;
		newTokenPointer = 0;
		isValueMode = skyFalse;
		for( ;; )
		{
			//���l����������l���[�h�̃t���O��On�ɂ���
			if ( CStringUtil_::IsNum( tokenTbl[ i ][ newTokenPointer ] ) )
			{
				isValueMode = skyTrue;
			}

			//�I�[�q�̏ꍇ�I��
			if ( tokenTbl[ i ][ newTokenPointer ] == '\0' )
			{
				key[ keyPointer ] = '\0';
				value[ valuePointer ] = '\0';
				value_i[ valuePointer_i ] = '\0';
				break;
			}
			//�l���[�h�łȂ��ꍇ�̓L�[�l�Ƃ݂Ȃ�
			else if ( isValueMode == skyFalse )
			{
				key[ keyPointer ] = tokenTbl[ i ][ newTokenPointer ];
				keyPointer++;
			}
			//�l���[�h�̏ꍇ�͒l�Ƃ݂Ȃ�
			else if ( isValueMode == skyTrue )
			{
				//�l���i�[����
				value[ valuePointer ] = tokenTbl[ i ][ newTokenPointer ];
				valuePointer++;

				//�����Ȑ����̒l���i�[����
				if ( isdigit( tokenTbl[ i ][ newTokenPointer ] ) )
				{
					value_i[ valuePointer_i ] = tokenTbl[ i ][ newTokenPointer ];
					valuePointer_i++;
				}
			}

			//���݂̃g�[�N���̎��̕�����
			newTokenPointer++;
		}

		//�}�b�v�ɒl�𗭂߂�
		s32 m = 1;
		if( CStringUtil_::Strchr( value, 'm' ) )
		{
			// �l��-�̏ꍇ
			m = -1;
		}
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
		skyWString *cKey = CStringUtil_::CreateWString( key );
		destmap.Set( cKey ,  m * CStringUtil_::ConvStringTos32( value_i ) );
		CStringUtil_::DeleteWString( cKey );
#else
		destmap.Set( key ,  m * CStringUtil_::ConvStringTos32( value_i ) );
#endif
	}
}

void CParserUtil::ParseStringPtn1( const skyWString *src , CValues32Map &destmap )
{
	//������̒����������ꍇ�͂��̂܂ܕԂ�
	if ( CStringUtil_::Len( src ) == 0 ) return;

	skyMString *cValue = CStringUtil_::CreateMString( src );
	ParseStringPtn1( cValue , destmap );
	CStringUtil_::DeleteMString( cValue );
}

void CParserUtil::ParseStringPtn2( const skyMString *src , const skyMString *keyWord , CValueBoolList &destlist )
{
	//������̒����������ꍇ�͂��̂܂ܕԂ�
	if ( CStringUtil_::Len( src ) == 0 ) return;

	u32 srcLen = CStringUtil_::Len( src );
	u32 pointer = 0;
	u32 keywordLen = CStringUtil_::Len( keyWord );
	skyBool valueSwitch = skyFalse;
	u32 distance = 0;
	while( srcLen > pointer )
	{
		if ( valueSwitch == skyFalse )
		{//�L�[���[�h�T��

			if ( ( distance = CStringUtil_::StrstrDistance( src , keyWord  ) ) < srcLen )
			{//�L�[���[�h������
				pointer += distance + keywordLen;
				valueSwitch = skyTrue;
				continue;
			}
		}
		else
		{//�l�T��
			if ( CStringUtil_::IsNum( src[ pointer ] ) )
			{//���l���������̂ŁASkyBool�^�ɂ���
				destlist.AddTail( CStringUtil_::ConvertNum( src[ pointer ] ) != 0 );
			}
			else
			{//�I���
				goto END;
			}
		}
		pointer++;
	}
END:
;
}

void CParserUtil::ParseStringPtn2( const skyWString *src , const skyWString *keyWord , CValueBoolList &destlist )
{
	//������̒����������ꍇ�͂��̂܂ܕԂ�
	if ( CStringUtil_::Len( src ) == 0 ) return;
	if ( CStringUtil_::Len( keyWord ) == 0 ) return;

	skyMString *cValue = CStringUtil_::CreateMString( src );
	skyMString *cKeyWord = CStringUtil_::CreateMString( keyWord );
	ParseStringPtn2( cValue , cKeyWord , destlist );
	CStringUtil_::DeleteMString( cValue );
	CStringUtil_::DeleteMString( cKeyWord );
}

void CParserUtil::ParseStringPtn3( const skyMString *src , const skyMString *keyWord , CValueu8List &destlist )
{
	//������̒����������ꍇ�͂��̂܂ܕԂ�
	if ( CStringUtil_::Len( src ) == 0 ) return;
	if ( CStringUtil_::Len( keyWord ) == 0 ) return;

	u32 srcLen = CStringUtil_::Len( src );
	u32 pointer = 0;
	u32 keywordLen = CStringUtil_::Len( keyWord );
	skyBool valueSwitch = skyFalse;
	u32 distance = 0;
	while( srcLen > pointer )
	{
		if ( valueSwitch == skyFalse )
		{//�L�[���[�h�T��

			if ( ( distance = CStringUtil_::StrstrDistance( src , keyWord  ) ) < srcLen )
			{//�L�[���[�h������
				pointer += distance + keywordLen;
				valueSwitch = skyTrue;
				continue;
			}
		}
		else
		{//�l�T��
			if ( CStringUtil_::IsNum( src[ pointer ] ) )
			{//���l���������̂ŁASkyBool�^�ɂ���
				destlist.AddTail( ( const u8 )CStringUtil_::ConvertNum( src[ pointer ] ) );
			}
			else
			{//�I���
				goto END;
			}
		}
		pointer++;
	}
END:
;
}

void CParserUtil::ParseStringPtn3( const skyWString *src , const skyWString *keyWord , CValueu8List &destlist )
{
	//������̒����������ꍇ�͂��̂܂ܕԂ�
	if ( CStringUtil_::Len( src ) == 0 ) return;
	if ( CStringUtil_::Len( keyWord ) == 0 ) return;

	skyMString *cValue = CStringUtil_::CreateMString( src );
	skyMString *cKeyWord = CStringUtil_::CreateMString( keyWord );
	ParseStringPtn3( cValue , cKeyWord , destlist );
	CStringUtil_::DeleteMString( cValue );
	CStringUtil_::DeleteMString( cKeyWord );
}

} } }
