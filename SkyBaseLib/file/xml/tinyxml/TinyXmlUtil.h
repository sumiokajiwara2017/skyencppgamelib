#pragma once

namespace sky { namespace lib { namespace file {

typedef SmartPointer< TiXmlDocument >	TiXmlDocumentPtr;

/// <summary>
/// TinyXml�̃��b�p�[
/// �����g��Ȃ��Ǝg������Y���̂�Util������BUnicode������̈����̃N�b�V�����ɂ��Ȃ�B
/// �{���N���X�����b�s���O���Ē��ۉ������ق���������������Ȃ����I�[�o�[�w�b�h�ƍH�����C�ɂȂ�̂ŕۗ��B
/// </summary>
namespace TinyXmlUtil
{

/// <summary>
/// �w�l�k�h�L�������g�m�[�h�̍쐬
/// </summary>
inline TiXmlDocumentPtr Create( const skyString *pFilePath )
{
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE

	//wchar��char�ɕϊ�
	skyMString *cFilePath = CStringUtil_::CreateMString( pFilePath );
	TiXmlDocumentPtr spDoc( NEW__( TiXmlDocument , TiXmlDocument( cFilePath ) ) );
	CStringUtil_::DeleteMString( cFilePath );

#else

	TiXmlDocumentPtr  spDoc( NEW__( TiXmlDocument , TiXmlDocument( pFilePath ) );

#endif

	return spDoc;
}

/// <summary>
/// �w�l�k�h�L�������g�̃t�@�C�����[�h
/// </summary>
inline void Load( TiXmlDocumentPtr &spDoc )
{
	spDoc->LoadFile();
}

/// <summary>
/// �w�l�k�h�L�������g�̃t�@�C���Z�[�u
/// </summary>
inline void Save( TiXmlDocumentPtr &spDoc )
{
	spDoc->SaveFile();
}

/// <summary>
/// �G�������g�̖��O����
/// </summary>
inline skyBool ElementComp( TiXmlElement *el , const skyMString *value )
{
    return string::CStringUtil::Comp( el->Value() , value );
}

/// <summary>
/// �c�n�l�c���[�̃��[�g�m�[�h�̎擾�iDOM�̓c���[�\���ɂȂ��Ă���̂Ń��[�g����g���o�[�X���đ��삷��j
/// ����(root)�̎q�����[�v�擾����ɂ͈ȉ��̂悤�ɋL�q����
/// for( TiXmlElement *el = el->FirstChildElement() ; el ; el = el->NextSiblingElement() ){}
/// ����(root)�̌Z������[�v�擾����ɂ͈ȉ��̂悤�ɋL�q����
/// for( TiXmlElement *el = root ; el ; el = el->NextSiblingElement() )
/// </summary>
inline TiXmlElement *GetRootElement( TiXmlDocumentPtr &spDoc )
{
	return spDoc->RootElement();
}

/// <summary>
/// �w��̖��O�ƒl�Ŏw��̃G�������g�ɃA�g���r���[�g�𕶎��Ƃ��Đݒ�
/// </summary>
inline void SetAttribute_String( TiXmlElement& kElement , const skyMString *pName , const skyString *pValue )
{

#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE

	//wchar��char�ɕϊ�
	skyMString *cValue = CStringUtil_::CreateMString( pValue );
	kElement.SetAttribute( pName , cValue );
	CStringUtil_::DeleteMString( cValue );

#else

	kElement.SetAttribute( pName , pValue );

#endif
}

/// <summary>
/// �w��̖��O�ƒl�Ŏw��̃G�������g�ɃA�g���r���[�g��d64�Ƃ��Đݒ�
/// </summary>
inline void SetAttribute_d64( TiXmlElement& kElement , const skyMString *pName , d64 value )
{
	kElement.SetDoubleAttribute( pName , value );
}

/// <summary>
/// �w��̖��O�ƒl�Ŏw��̃G�������g�ɃA�g���r���[�g��s32�Ƃ��Đݒ�
/// </summary>
inline void SetAttribute_s32( TiXmlElement& kElement , const skyMString *pName , s32 value )
{
	kElement.SetAttribute( pName , value );
}

/// <summary>
/// �w��̖��O�ƒl�Ŏw��̃G�������g�ɃA�g���r���[�g���T�C�Y�Őݒ�
/// </summary>
inline void SetAttribute_Size( TiXmlElement& kElement , const skyMString *pName , u8 *value , u32 size )
{
	u8 *buf = NEW u8[ size + 1 ];
	CMemoryUtil_::Copy( buf , value , size );
	buf[ size ] = MEOT; //�����I�[�q��t����

	//�w��̃G�������g�ɕ����Ƃ��Đݒ肷��
	SetAttribute_String( kElement , pName , ( const skyString *)buf );

	DEL buf;
}

/// <summary>
/// �w��̔ėp�l���X�g�̒�����l�����o���Ȃ���w��̃A�g���r���[�g�ɐݒ肵�Ă���
/// </summary>
inline void SetAttribute_ValueList( TiXmlElement& kElement , const skyMString *pName, ValueList &valuelist )
{
	string::CStringBuffer buf;

    u32 i = 0;	
	FOREACH( ValueList , it , valuelist )
	{
		if ( i > 0 ) buf += _T( ' ' );
		( *it ).AddBuffer( buf );
	    i++;
	}

	//�w��̃G�������g�ɕ����Ƃ��Đݒ肷��
	SetAttribute_String( kElement , pName , ( const skyString *)buf );
}

/// <summary>
/// Color�̐ݒ菈��
/// </summary>
inline void SetAttribute_Color( TiXmlElement& kElement , const skyMString *pName, graphic::CColor &value )
{
	ValueList valuelist;
	valuelist.AddTail( Value( ( f32 )value.r ) );
	valuelist.AddTail( Value( ( f32 )value.g ) );
	valuelist.AddTail( Value( ( f32 )value.b ) );
	valuelist.AddTail( Value( ( f32 )value.a ) );

	SetAttribute_ValueList( kElement , pName, valuelist );
}

/// <summary>
/// u8�z��̐ݒ菈��
/// </summary>
inline void SetAttribute_u8Array( TiXmlElement& kElement , const skyMString *pName , u8 *value , u32 num )
{
	ValueList valuelist;

	for ( u32 i = 0 ; i < num ; i++ )
	{
		valuelist.AddTail( Value( ( u32 )value[ i ] ) );
	}

	SetAttribute_ValueList( kElement , pName, valuelist );
}

/// <summary>
/// u16�z��̐ݒ菈��
/// </summary>
inline void SetAttribute_u16Array( TiXmlElement& kElement , const skyMString *pName , u16 *value , u32 num )
{
	ValueList valuelist;

	for ( u32 i = 0 ; i < num ; i++ )
	{
		valuelist.AddTail( Value( ( u32 )value[ i ] ) );
	}

	SetAttribute_ValueList( kElement , pName, valuelist );
}

/// <summary>
/// u32�z��̐ݒ菈��
/// </summary>
inline void SetAttribute_u32Array( TiXmlElement& kElement , const skyMString *pName , u32 *value , u32 num )
{
	ValueList valuelist;

	for ( u32 i = 0 ; i < num ; i++ )
	{
		valuelist.AddTail( Value( ( u32 )value[ i ] ) );
	}

	SetAttribute_ValueList( kElement , pName, valuelist );
}

/// <summary>
/// f32�z��̐ݒ菈��
/// </summary>
inline void SetAttribute_f32Array( TiXmlElement& kElement , const skyMString *pName , f32 *value , u32 num )
{
	ValueList valuelist;

	for ( u32 i = 0 ; i < num ; i++ )
	{
		valuelist.AddTail( Value( ( f32 )value[ i ] ) );
	}

	SetAttribute_ValueList( kElement , pName, valuelist );
}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g�𕶎��Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_String( const TiXmlElement& kElement , const skyMString *pName , string::CString &value , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	const skyMString *cDestName = kElement.Attribute( pName );
	if ( cDestName == skyNull )
	{
		result = skyFalse;
	}
	else
	{
		value = CStringUtil_::CreateWString( cDestName );
	}

	if ( result == skyFalse && isRequired == skyTrue )
	{
		SKY_PANIC_MSG( "%s is required." , pName );
	}

	return result;

}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g��d64�Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_d64( const TiXmlElement& kElement , const skyMString *pName, d64 &value , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	const skyMString *cDestName = kElement.Attribute( pName, &value );

	if ( cDestName == skyNull )
	{
		result = skyFalse;
	}

	if ( result == skyFalse && isRequired == skyTrue )
	{
		SKY_PANIC_MSG( "%s is required." , pName );
	}

	return result;

}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g��s32�Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_s32( const TiXmlElement& kElement , const skyMString *pName, s32 &value , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	const skyMString *cDestName = kElement.Attribute( pName, ( int * )&value );

	if ( cDestName == skyNull )
	{
		result = skyFalse;
	}

	if ( result == skyFalse && isRequired == skyTrue )
	{
		SKY_PANIC_MSG( "%s is required." , pName );
	}

	return skyTrue;
}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g���T�C�Y�Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_Size( const TiXmlElement& kElement , const skyMString *pName, u8* &value , u32 size , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	const skyMString *cDestName = kElement.Attribute( pName );
	if ( cDestName == skyNull )
	{
		result = skyFalse;
	}
	else
	{
		CMemoryUtil_::Copy( value , cDestName , size );
	}

	if ( result == skyFalse && isRequired == skyTrue )
	{
		SKY_PANIC_MSG( "%s is required." , pName );
	}

	return skyTrue;
}


/// <summary>
/// �w��̔ėp�l���X�g�ɒl�����o���Ȃ���w��̃A�g���r���[�g���擾���Ă���
/// </summary>
inline skyBool GetAttribute_ValueList( const TiXmlElement& kElement , const skyMString *pName, ValueList &valuelist , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	//�܂�������𕶎��g�[�N���ɕ�������
	const skyMString *cDestName = kElement.Attribute( pName );
	if ( cDestName == skyNull )
	{
		result = skyFalse;
	}
	else
	{
		string::CStringList list;
		parser::CParserUtil::ParseStringPtn0( cDestName , list );

		u32 i = 0;
		FOREACH( ValueList , it , valuelist )
		{
			if( list.GetNum() <= i ) break;

			( *it ).SetConvertString( list[ i ] );
			i++;
		}
	}

	if ( result == skyFalse && isRequired == skyTrue )
	{
		SKY_PANIC_MSG( "%s is required." , pName );
	}

	return result;
}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g��Color�Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_Color( const TiXmlElement& kElement , const skyMString *pName, graphic::CColor &value , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	ValueList valuelist;
	SetUpValueList( valuelist , Value::eType_Float , 4 );
	result = GetAttribute_ValueList( kElement , pName, valuelist , isRequired );
	if ( result == skyTrue )
	{
		value.r = ( colortype )( f32 )valuelist[ 0 ];
		value.g = ( colortype )( f32 )valuelist[ 1 ];
		value.b = ( colortype )( f32 )valuelist[ 2 ];
		value.a = ( colortype )( f32 )valuelist[ 3 ];
	}

	return result;
}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g��u16�z��Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_u8Array( const TiXmlElement& kElement , const skyMString *pName , u8 *value , u32 num , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	ValueList valuelist;
	SetUpValueList( valuelist , Value::eType_UInt , num );
	result = GetAttribute_ValueList( kElement , pName, valuelist , isRequired );

	for ( u32 i = 0 ; i < num ; i++ )
	{
        value[ i ] = ( u8 )( u32 )valuelist[ i ];
	}

	return result;
}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g��u16�z��Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_u16Array( const TiXmlElement& kElement , const skyMString *pName , u16 *value , u32 num , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	ValueList valuelist;
	SetUpValueList( valuelist , Value::eType_UInt , num );
	result = GetAttribute_ValueList( kElement , pName, valuelist , isRequired );

	for ( u32 i = 0 ; i < num ; i++ )
	{
        value[ i ] = ( u16 )( u32 )valuelist[ i ];
	}

	return result;
}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g��u32�z��Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_u32Array( const TiXmlElement& kElement , const skyMString *pName , u32 *value , u32 num , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	ValueList valuelist;
	SetUpValueList( valuelist , Value::eType_UInt , num );
	result = GetAttribute_ValueList( kElement , pName, valuelist , isRequired );

	for ( u32 i = 0 ; i < num ; i++ )
	{
        value[ i ] = ( u32 )valuelist[ i ];
	}

	return result;
}

/// <summary>
/// �w��̖��O�̎w��̃G�������g�̃A�g���r���[�g��f32�z��Ƃ��Ď擾
/// </summary>
inline skyBool GetAttribute_f32Array( const TiXmlElement& kElement , const skyMString *pName , f32 *value , u32 num , skyBool isRequired = skyFalse )
{
	skyBool result = skyTrue;

	ValueList valuelist;
	SetUpValueList( valuelist , Value::eType_Float , num );
	result = GetAttribute_ValueList( kElement , pName, valuelist , isRequired );

	for ( u32 i = 0 ; i < num ; i++ )
	{
        value[ i ] = ( f32 )valuelist[ i ];
	}

	return result;
}


/// <summary>
/// �����̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_String( const TiXmlElement& kElement , const skyMString *pName , string::CString &value , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_String( kElement , pName , value , isRequired );
	}
	else
	{
		SetAttribute_String( *const_cast< TiXmlElement * >( &kElement ) , pName , value.Get() );
	}

	return result;
}

/// <summary>
/// d64�̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_d64( const TiXmlElement& kElement , const skyMString *pName, d64 &value , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_d64( kElement , pName, value , isRequired );
	}
	else
	{
		SetAttribute_d64( *const_cast< TiXmlElement * >( &kElement ) , pName , value );
	}

	return result;
}
inline skyBool GetSetAttribute_d64( const TiXmlElement& kElement , const skyMString *pName, d64 *pValue , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	d64 value = *pValue;
	skyBool result = GetSetAttribute_d64( kElement , pName, value , isRequired , isGet );
	*pValue = value;
	return result;
}

/// <summary>
/// f32�̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_f32( const TiXmlElement& kElement , const skyMString *pName, f32 &value , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	d64 value_d64 = value;
	skyBool result = GetSetAttribute_d64( kElement , pName, value_d64 , isRequired , isGet );
	value = ( f32 )value_d64;
	return result;
}

/// <summary>
/// s32�̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_s32( const TiXmlElement& kElement , const skyMString *pName, s32 &value , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_s32( kElement , pName , value , isRequired );
	}
	else
	{
        SetAttribute_s32( *const_cast< TiXmlElement * >( &kElement ) , pName , value );
	}

	return result;
}
inline skyBool GetSetAttribute_s32( const TiXmlElement& kElement , const skyMString *pName, s32 *pValue , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	s32 value = *pValue;
	skyBool result = GetSetAttribute_s32( kElement , pName, value , isRequired , isGet );
	*pValue = value;
	return result;
}

/// <summary>
/// u32�̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_u32( const TiXmlElement& kElement , const skyMString *pName, u32 &value , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	s32 value_s32 = value;
	skyBool result = GetSetAttribute_s32( kElement , pName, value_s32 , isRequired , isGet );
	value = ( u32 )value_s32;
	return result;
}

/// <summary>
/// u16�̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_u16( const TiXmlElement& kElement , const skyMString *pName, u16 &value , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	s32 value_s32 = value;
	skyBool result = GetSetAttribute_s32( kElement , pName, value_s32 , isRequired , isGet );
	value = ( u16 )value_s32;
	return result;
}

/// <summary>
/// u8�̔z��擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_u8Array( const TiXmlElement& kElement , const skyMString *pName, u8 *value , u32 num , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_u8Array( kElement , pName , value , num , isRequired );
	}
	else
	{
		SetAttribute_u8Array( *const_cast< TiXmlElement * >( &kElement ) , pName , value , num );
	}

	return result;
}

/// <summary>
/// u16�̔z��擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_u16Array( const TiXmlElement& kElement , const skyMString *pName, u16 *value , u32 num , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_u16Array( kElement , pName , value , num , isRequired );
	}
	else
	{
		SetAttribute_u16Array( *const_cast< TiXmlElement * >( &kElement ) , pName , value , num );
	}

	return result;
}

/// <summary>
/// u32�̔z��擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_u32Array( const TiXmlElement& kElement , const skyMString *pName, u32 *value , u32 num , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_u32Array( kElement , pName , value , num , isRequired );
	}
	else
	{
		SetAttribute_u32Array( *const_cast< TiXmlElement * >( &kElement ) , pName , value , num );
	}

	return result;
}

/// <summary>
/// f32�̔z��擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_f32Array( const TiXmlElement& kElement , const skyMString *pName, f32 *value , u32 num , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_f32Array( kElement , pName , value , num , isRequired );
	}
	else
	{
		SetAttribute_f32Array( *const_cast< TiXmlElement * >( &kElement ) , pName , value , num );
	}

	return result;
}

/// <summary>
/// skyBool�̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_Bool( const TiXmlElement& kElement , const skyMString *pName, skyBool &value , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	s32 value_s32 = ( s32 )value;
	skyBool result = GetSetAttribute_s32( kElement , pName, value_s32 , isRequired , isGet );
	value = ( skyBool )value_s32;
	return result;
}

/// <summary>
/// ���ʒl���X�g�̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_ValueList( const TiXmlElement& kElement , const skyMString *pName, ValueList &valuelist , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_ValueList( kElement , pName, valuelist , isRequired );
	}
	else
	{
		SetAttribute_ValueList( *const_cast< TiXmlElement * >( &kElement ) , pName , valuelist );
	}

	return result;
}

/// <summary>
/// Color�̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_Color( const TiXmlElement& kElement , const skyMString *pName, graphic::CColor &value , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_Color( kElement , pName, value , isRequired );
	}
	else
	{
		SetAttribute_Color( *const_cast< TiXmlElement * >( &kElement ) , pName , value );
	}

	return result;
}

/// <summary>
/// �T�C�Y�w��̎擾�ݒ菈��
/// </summary>
inline skyBool GetSetAttribute_Size( const TiXmlElement& kElement , const skyMString *pName, u8* &value , u32 size , skyBool isRequired = skyFalse , skyBool isGet = skyTrue )
{
	skyBool result = skyTrue;

	if ( isGet == skyTrue )
	{
		result = GetAttribute_Size( kElement , pName , value , size , isRequired );
	}
	else
	{
        SetAttribute_Size( *const_cast< TiXmlElement * >( &kElement ) , pName , value , size );
	}

	return result;
}


}

} } }