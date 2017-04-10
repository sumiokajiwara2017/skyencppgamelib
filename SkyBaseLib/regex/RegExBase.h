#pragma once

namespace sky { namespace lib { namespace regex {

//-------------------------------< インターフェース >----------------------------------------//

typedef CList< u32 , string::CString > RegResultList;

/// <summary>
/// 正規表現処理クラス
/// </summary>
class IRegEx : public base::SkyObject
{

public:

	/// <summary>
	/// 正規表現文と処理対象文字を引数にして処理結果リストを返す
	/// </summary>
	virtual void Execute( const skyString *pRegEx , const skyString *pSrc , RegResultList &dest ) = 0;		

	/// <summary>
	/// 正規表現文と処理対象文字を引数にして該当数を返す
	/// </summary>
	virtual u32 Execute( const skyString *pRegEx , const skyString *pSrc ) = 0;		

	/// <summary>
	/// インスタンスを返す／消す
	/// </summary>
	static void					CreateInstance		();
	static IRegEx				*GetInstance		();
	static void					DeleteInstance		();
};

//インスタンスアクセス文字列
#define IRegExCreate_()	sky::lib::regex::IRegEx::CreateInstance()
#define IRegEx_			sky::lib::regex::IRegEx::GetInstance()
#define IRegExDelete_()	sky::lib::regex::IRegEx::DeleteInstance()

//-----------------------------------< 抽象実装 >--------------------------------------------//

class AbsRegEx : public IRegEx
{
};

//-------------------------------------< 実装 >---------------------------------------------//

#ifdef SKYLIB_COMPILER_VC10

class CRegExWin32 : public AbsRegEx
{

friend class IRegEx;

public:

	/// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
	/// </summary>
	void Execute	( const skyString *pRegEx , const skyString *pSrc , RegResultList &dest );
	u32  Execute	( const skyString *pRegEx , const skyString *pSrc );


	//シングルトン機能を付属
	friend class Singleton< CRegExWin32 >;
	static Singleton< CRegExWin32 > Singleton;

private:

	//各機能封印
	CRegExWin32(){};												//コンストラクタ
	CRegExWin32( const CRegExWin32& ){};							//コピーコンストラクタ
	virtual ~CRegExWin32(){};										//デストラクタ
	CRegExWin32 &operator=(const CRegExWin32& ){ return *this; };	//代入演算子オーバーロード

};

#endif

} } }
