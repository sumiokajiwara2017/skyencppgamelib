#pragma once

namespace sky { namespace lib { 

/// <summary>
/// �X�g���[���֘A�^��`
/// </summary>
namespace stream {

typedef SmartPointer< IStream >		IStreamPtr;
typedef CList< u32 , IStreamPtr >	IStreamPtrList;

}
namespace memory {

typedef SmartPointer< CMemoryStream >   CMemoryStreamPtr;
typedef CList< u32 , CMemoryStreamPtr >	CMemoryStreamPtrList;

}

/// <summary>
/// �������X�g
/// </summary>
namespace string {

typedef CList< u32 , CString >	CStringList;

}

/// <summary>
///�v���~�e�B�u�^�̃X�}�[�g�|�C���^
/// </summary>
typedef SmartPointer2< skyBool > skyBoolPtr;

/// <summary>
/// �ėp�^�R���e�i
/// </summary>
typedef CHash< s32 >            CValues32Map;
typedef CList< u32 , skyBool >  CValueBoolList;
typedef CList< u32 , u8 >       CValueu8List;

static const skyString *NAME_NONE = _T( "none" );

/// <summary>
/// ���O�ێ��N���X
/// ���O�����������N���X�͂��̃N���X��public�ȃ����o�Ƃ��Ď���
/// </summary>
class CName : public base::SkyObject
{

public:

	/// <summary>
	/// ���O��Setter/Getter�i�������̂��߂�inline�ɂ��Ă���j
	/// </summary>
	inline void                       SetName( const hash::CHashStringPtr &name ) { m_Name = name; }
	inline const hash::CHashStringPtr GetName() const                             { return m_Name; }
	inline u32                        GetNameSize() const                         { return m_Name->GetSize(); }
	inline const skyString*           GetString()
	{
		if( m_Name.IsEmpty() )
		{
			return NAME_NONE;
		}
		else
		{
			return m_Name->GetString();
		}
	}

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	CName& operator =( const CName& v1 )
	{
		m_Name = v1.m_Name;
		return *this;
	}

	/// <summary>
	/// ==���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	skyBool operator ==( const CName& v1 ) const
	{
		return m_Name->Get() == v1.m_Name->Get();
	}
	skyBool operator ==( const hash::CHashStringPtr& v1 ) const
	{
		return m_Name->Get() == v1->Get();
	}

	/// <summary>
	/// ���O
	/// </summary>
	hash::CHashStringPtr m_Name;
};

/// <summary>
/// �ėp�l�\����
/// �i���Ӂ@�����̓|�C���^�̂ݕێ����Ă���̂ŁA�|�C���^�̎w���悪�������Ȃ��悤�Ɉ����Ă��������j
/// </summary>
struct Value
{
	/// <summary>
	/// �ėp�l�̌^
	/// </summary>
	enum eType
	{
		eType_None = 0 ,
		eType_Char ,
		eType_Int ,
		eType_UInt ,
		eType_Float ,
		eType_String ,
		eType_HashString ,
		eType_Bool ,
		eType_Double ,
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Value()
	{
		m_Int = 0;
		m_eType = eType_None;
	}
	Value( eType eType )
	{
		m_eType = eType;
	}
	Value( u8 value )
	{
		SetChar( value );
	}
	Value( s32 value )
	{
		SetInt( value );
	}
	Value( u32 value )
	{
		SetUInt( value );
	}
	Value( f32 value )
	{
		SetFloat( value );
	}
	Value( const skyString* value )
	{
		SetString( value );
	}
	Value( const hash::CHashStringPtr* value )
	{
		SetHashString( value );
	}
	Value( skyBool value )
	{
		SetBool( value );
	}
	Value( d64 value )
	{
		SetDouble( value );
	}

	/// <summary>
	/// �l�^
	/// </summary>
	eType m_eType;

	/// <summary>
	/// �l
	/// </summary>
	union
	{
		u8                          m_Char;
		s32                         m_Int;
		u32                         m_UInt;
		f32                         m_Float;
		const skyString*            m_String;
		const hash::CHashStringPtr* m_HashString;
		skyBool                     m_Bool;
		d64                         m_Double;
	};

	/// <summary>
	/// �ݒ�(�ݒ�Ɠ����Ɍ^��ݒ肷��j
	/// </summary>
	void SetChar( u8 value )
	{
		m_Char = value;
		m_eType = eType_Char;
	}
	void SetInt( s32 value )
	{
		m_Int = value;
		m_eType = eType_Int;
	}
	void SetUInt( u32 value )
	{
		m_UInt = value;
		m_eType = eType_UInt;
	}
	void SetFloat( f32 value )
	{
		m_Float = value;
		m_eType = eType_Float;
	}
	void SetString( const skyString* value )
	{
		m_String = value;
		m_eType = eType_String;
	}
	void SetHashString( const hash::CHashStringPtr* value )
	{
		m_HashString = value;
		m_eType = eType_HashString;
	}
	void SetBool( skyBool value )
	{
		m_Bool = value;
		m_eType = eType_Bool;
	}
	void SetDouble( d64 value )
	{
		m_Double = value;
		m_eType = eType_Double;
	}

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	Value& operator =( u8 value )
	{
		SetChar( value );
		return *this;
	}
	Value& operator =( s32 value )
	{
		SetInt( value );
		return *this;
	}
	Value& operator =( u32 value )
	{
		SetUInt( value );
		return *this;
	}
	Value& operator =( f32 value )
	{
		SetFloat( value );
		return *this;
	}
	Value& operator =( const skyString* value )
	{
		SetString( value );
		return *this;
	}
	Value& operator =( const hash::CHashStringPtr* value )
	{
		SetHashString( value );
		return *this;
	}
	Value& operator =( skyBool value )
	{
		SetBool( value );
		return *this;
	}
	Value& operator =( d64 value )
	{
		SetDouble( value );
		return *this;
	}

	/// <summary>
	/// �L���X�g���Z�q�I�[�o�[���C�h
	/// </summary>
    operator u8 () const
	{
		SKY_ASSERT( m_eType == eType_Char ); //The type is different.
		return m_Char;
	}
    operator s32 () const
	{
		SKY_ASSERT( m_eType == eType_Int ); //The type is different.
		return m_Int;
	}
    operator u32 () const
	{
		SKY_ASSERT( m_eType == eType_UInt ); //The type is different.
		return m_UInt;
	}
    operator f32 () const
	{
		SKY_ASSERT( m_eType == eType_Float ); //The type is different.
		return m_Float;
	}
    operator const skyString* () const
	{
		SKY_ASSERT( m_eType == eType_String ); //The type is different.
		return m_String;
	}
    operator const hash::CHashStringPtr* () const
	{
		SKY_ASSERT( m_eType == eType_HashString ); //The type is different.
		return m_HashString;
	}
    operator skyBool () const
	{
		SKY_ASSERT( m_eType == eType_Bool ); //The type is different.
		return m_Bool;
	}
    operator d64 () const
	{
		SKY_ASSERT( m_eType == eType_Double ); //The type is different.
		return m_Double;
	}

	/// <summary>
	/// �w��̕������^�ɍ��킹�ĕϊ����Ċi�[����
	/// </summary>
	void SetConvertString( const skyString *value )
	{
		switch( m_eType )
		{
		case eType_Char:
			m_Char = ( u8 )CStringUtil_::ConvStringTou32( value );
			break;
		case eType_Int:
			m_Int = CStringUtil_::ConvStringTos32( value );
			break;
		case eType_UInt:
			m_UInt = CStringUtil_::ConvStringTou32( value );
			break;
		case eType_Float:
			m_Float = CStringUtil_::ConvStringTof32( value );
			break;
		case eType_String:
			m_String = value;
			break;
		case eType_Bool:
			m_Bool = CStringUtil_::ConvStringToBool( value );
			break;
		case eType_Double:
			m_Double = CStringUtil_::ConvStringTod64( value );
			break;
		default:
			SKY_PANIC(); //This Type is no support.
		}
	}
    void AddBuffer( string::CStringBuffer &buf )
	{
		switch( m_eType )
		{
		case eType_Char:
			buf += m_Char;
			break;
		case eType_Int:
			buf += m_Int;
			break;
		case eType_UInt:
			buf += m_UInt;
			break;
		case eType_Float:
			buf += m_Float;
			break;
		case eType_String:
			buf += m_String;
			break;
		case eType_Bool:
			buf += m_Bool;
			break;
		case eType_Double:
			buf += m_Double;
			break;
		default:
			SKY_PANIC(); //This Type is no support.
		}		
	}
};
typedef CList< u32 , Value >	ValueList;  

/// <summary>
/// �w��̕������^���w��̌��ŃZ�b�g�A�b�v����
/// </summary>
static inline void SetUpValueList( ValueList &list , Value::eType eType , u32 num )
{
	for( u32 i = 0 ; i < num ; i++ )
	{
		list.AddTail( Value( eType ) );
	}
}

} }