#pragma once

/*

【 実装状況 】
DirectX11 PC HLSL ×移植失敗（一応ソースは残す）
OpenGL    PC GLSL ×未実装
OpenGL    PC Cg   ×未実装

*/

namespace sky { namespace lib { namespace graphic {

class   CBeamChargeEffect;
typedef SmartPointer< CBeamChargeEffect >		CBeamChargeEffectPtr;			//スマートポインタ型定義

struct CBeamChargeEffectMember;

/// <summary>
/// ビームチャージエフェクトクラス
/// </summary>
class CBeamChargeEffect : public CEffectObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CBeamChargeEffect();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void vDraw();

	/// <summary>
	/// リソースの読み込み（２重読み込みは内部でスルーします）
	/// </summary>
	void Load();                //インスタンス毎に必要なリソース用
	static void LoadStatic();   //クラス毎に必要なリソース用

	/// <summary>
	/// リソースの破棄
	/// </summary>
	void Destroy();
	static void DestroyStatic();

	/// <summary>
	/// 作成
	/// </summary>
	static CBeamChargeEffectPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CBeamChargeEffectPtr spBeamChargeEffectt( NEW__( CBeamChargeEffect , CBeamChargeEffect() ) );
		if ( name.IsEmpty() == skyFalse ) spBeamChargeEffectt->Name.SetName( name );
		return spBeamChargeEffectt;
	}

private:

	/// <summary>
	///メンバ変数
	/// </summary>
	CBeamChargeEffectMember *m_pMember;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CBeamChargeEffect();

	/// <summary>
	///描画（環境ごとに実装）
	/// </summary>
	void DrawPass1();
};

} } }

#define CBeamChargeEffectObjectCreate_()		sky::lib::graphic::CBeamChargeEffect::Create()
#define CBeamChargeEffectCreateName_( name )	sky::lib::graphic::CBeamChargeEffect::Create( name )
