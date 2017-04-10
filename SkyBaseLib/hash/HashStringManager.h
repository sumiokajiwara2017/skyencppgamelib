#pragma once

namespace sky { namespace lib { namespace hash {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// Hash文字管理クラス
/// すべてのHash文字をこのマネージャー経由で生成することで、インスタンスの重複生成を抑制します。
/// CHashStringはキーの役割しかないので、読み取り専用なので1つの文字に対するインスタンスは1つでいい。
/// </summary>
class CHashStringManager : public base::SkyObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize( u32 hashSize = C_S32( hash_HASH_STR_MAN_HASH_SIZE ) );

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose();

	/// <summary>
	/// Hash文字作成
	/// </summary>
	static hash::CHashStringPtr GetPtr( const skyString  *pKey , skyBool isNoCreateOn = skyFalse ); 
	static hash::CHashStringPtr Get( const skyWString *pKey , skyBool isNoCreateOn = skyFalse ); 
	static hash::CHashStringPtr Get( const skyMString *pKey , skyBool isNoCreateOn = skyFalse ); 

	/// <summary>
	/// Hash文字全破棄
	/// </summary>
	static void Clear();

	/// <summary>
	/// マネージャーしか参照していない値を消す
	/// </summary>
	static void GC();

};

} } }

//インスタンスアクセス文字列
#define CHashStringManagerInitialize_()		sky::lib::hash::CHashStringManager::Initialize()
#define CHashStringManager_					sky::lib::hash::CHashStringManager
#define CHashStringManagerDispose_()		sky::lib::hash::CHashStringManager::Dispose()

//この処理はかなり処理負荷があります。マイフレーム行う処理には絶対に使用しないでください。
#define CHS( str )							CHashStringManager_::Get( str )
#define CHSC( str )							CHashStringManager_::Get( str , skyTrue ) //文字列のバッファを内部で取らない。アドレスだけ保持。static const等で指定されている文字列はこっちでいい。消えることがないから。
