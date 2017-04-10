#pragma once

namespace sky { namespace lib { 

/// <summary>
/// ストリーム関連型定義
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
/// 文字リスト
/// </summary>
namespace string {

typedef CList< u32 , CString >	CStringList;

}

/// <summary>
///プリミティブ型のスマートポインタ
/// </summary>
typedef SmartPointer2< skyBool > skyBoolPtr;

/// <summary>
/// 汎用型コンテナ
/// </summary>
typedef CHash< s32 >            CValues32Map;
typedef CList< u32 , skyBool >  CValueBoolList;
typedef CList< u32 , u8 >       CValueu8List;

static const skyString *NAME_NONE = _T( "none" );

/// <summary>
/// 名前保持クラス
/// 名前を持ちたいクラスはこのクラスをpublicなメンバとして持つ
/// </summary>
class CName : public base::SkyObject
{

public:

	/// <summary>
	/// 名前のSetter/Getter（高速化のためにinlineにしてある）
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
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	CName& operator =( const CName& v1 )
	{
		m_Name = v1.m_Name;
		return *this;
	}

	/// <summary>
	/// ==演算子オーバーライド（要素の代入）
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
	/// 名前
	/// </summary>
	hash::CHashStringPtr m_Name;
};

/// <summary>
/// 汎用値構造体
/// （注意　文字はポインタのみ保持しているので、ポインタの指し先が消失しないように扱ってください）
/// </summary>
struct Value
{
	/// <summary>
	/// 汎用値の型
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
	/// コンストラクタ
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
	/// 値型
	/// </summary>
	eType m_eType;

	/// <summary>
	/// 値
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
	/// 設定(設定と同時に型を設定する）
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
	/// 代入演算子オーバーライド
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
	/// キャスト演算子オーバーライド
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
	/// 指定の文字を型に合わせて変換して格納する
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
/// 指定の文字を型を指定の個数でセットアップする
/// </summary>
static inline void SetUpValueList( ValueList &list , Value::eType eType , u32 num )
{
	for( u32 i = 0 ; i < num ; i++ )
	{
		list.AddTail( Value( eType ) );
	}
}

} }