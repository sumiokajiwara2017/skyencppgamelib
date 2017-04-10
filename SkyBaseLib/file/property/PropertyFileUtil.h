#pragma once

namespace sky { namespace lib { namespace file {

/// <summary>
/// プロパティファイル操作クラス
/// プロパティファイルのフォーマットは下記の通り
///---------------< propertyfile >---------------
/// #コメントはシャープ文字
/// key1=value1						//□値は基本キーと値で書く。
/// key2=value1,value2,value3		//□値は複数持てる。
///
/// key1=value2						//■キーがかぶるとエラーになる。
///
///
/// key3=value						//□空白行は何行挟んでも構わない。
/// #key4=value						//□これはもちろんコメントになる
///
/// ほげほげ						//■コメントでもなく『キー=値』のフォーマットでもないものはエラーになる。
///
///---------------< propertyfile >---------------
/// < Memo >
/// ※1バイナリ保存する際にキーをHash値にしてから保存しようと考えたが、元の文字列が失われると色々デメリットがあるので一旦保留にした
/// </summary>
class CPropertyFileUtil : base::SkyObject
{

public:

	/// <summary>
	/// プロパティファイルロード
	/// </summary>
	static void Load( stream::IStreamPtr &src , hash::CHashStringPtrListPtrHash &dest );

	/// <summary>
	/// プロパティファイルコンパイル（バイナリ化する）
	/// </summary>
	static void Compile( stream::IStreamPtr &src ,  stream::IStreamPtr &dest );

	/// <summary>
	/// キーとインデックスで値を文字として取得
	/// </summary>
	static const hash::CHashStringPtr &GetString( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index = 0 );

	/// <summary>
	/// キーとインデックスで値を整数値として取得
	/// </summary>
	static s32 Gets32( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index = 0 );

	/// <summary>
	/// キーとインデックスで値を少数値として取得
	/// </summary>
	static d64 Getd64( const hash::CHashStringPtr &key , hash::CHashStringPtrListPtrHash &dest , u32 index = 0 );

	/// <summary>
	/// プロパティファイルの内容をダンプする
	/// </summary>
	static void Dump( hash::CHashStringPtrListPtrHash &src , stream::IStreamPtr &dest );

};

#define CPropertyFileUtil_					sky::lib::file::CPropertyFileUtil

} } }