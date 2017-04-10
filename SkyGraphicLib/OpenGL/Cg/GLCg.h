#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

//プラットフォーム毎のメンバ変数(pimpl)
struct CCgMember;

/// <summary>
/// Cgプログラム情報
/// </summary>
struct CCgProgram
{
    CGprogram Program;
    CGprofile Profile;
};

/// <summary>
/// Cg結合プログラム情報
/// </summary>
struct CCgCombineProgram
{
    CGprogram Program;
    CGprofile Profile1;
    CGprofile Profile2;
};

/// <summary>
/// GLSLクラス
/// </summary>
class CCg : base::SkyObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize	();

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose		();

	/// <summary>
	/// コンテキストの取得
	/// </summary>
	static CGcontext GetContext	();

	/// <summary>
	/// 頂点シェーダーの読み込み
	/// </summary>
	static CCgProgram &CreateVertexShaderFromFile( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName );
	static void        CreateVertexShaderFromFileImple( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName );

	/// <summary>
	/// フラグメントシェーダーの読み込み
	/// </summary>
	static CCgProgram &CreateFragmentShaderFromFile( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName );
	static void        CreateFragmentShaderFromFileImple( CCgProgram &dest , const skyMString *pFilePath , const skyMString *pFuncName );

	/// <summary>
	/// 結合プログラムを作成する
	//　※注意
	//　頂点シェーダーとフラグメントシェーダ―を結合した場合、頂点シェーダーとフラグメントシェーダ―で同じグローバル変数を定義すると
	//　ピクセルシェーダー側のグローバル変数に値が入りません。つまり、頂点シェーダーとフラグメントシェーダーのそれぞれのファイル内で同名の
	//　グローバル変数を定義してはいけません。
	/// </summary>
	static CCgCombineProgram &CreateVertexFragmentCombineShaderFromFile( CCgCombineProgram &dest , 
		                                            const skyMString *pFilePath1 , const skyMString *pFuncName1 ,
		                                            const skyMString *pFilePath2 , const skyMString *pFuncName2 );

	/// <summary>
	/// エラーチェック
	/// </summary>
    static void CheckForCgError( const skyMString *situation );

	/// <summary>
	/// シェーダーの内容をダンプ
	/// </summary>
    static void DumpCompiledPrograms( CGprogram program );

private:

	/// <summary>
	/// メンバ変数群
	/// </summary>
	static CCgMember *m_pMember;
};

} } } 

#define CCgInitialize_()	sky::lib::graphic::CGraphicLibController::Initialize()
#define CCg_				sky::lib::graphic::CGraphicLibController
#define CCgDispose_()		sky::lib::graphic::CGraphicLibController::Dispose()

#endif

#endif
