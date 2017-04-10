#pragma once

/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ×未実装
OpenGL    PC Cg   ×未実装

*/

namespace sky { namespace lib { namespace graphic {

class   CBillboardBeam;
typedef SmartPointer< CBillboardBeam >		CBillboardBeamPtr;			//スマートポインタ型定義

struct CBillboardBeamMember;

/// <summary>
/// ビームチャージエフェクトクラス
/// </summary>
class CBillboardBeam : public CEffectObject
{
	friend class CBillboardBeamUtil;
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CBillboardBeam();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void vDraw();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize( vrtxtype nodeNum , dectype width , const CTexturePtr &spTexture );

	/// <summary>
	/// 末期化
	/// </summary>
	void Dispose();

	/// <summary>
	/// ノード座標セット
	/// </summary>
	void SetNodePos( u32 no , const math::CBasicVector3 &pos );

	/// <summary>
	/// 作成
	/// </summary>
	static CBillboardBeamPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CBillboardBeamPtr spBillboardBeamt( NEW__( CBillboardBeam , CBillboardBeam() ) );
		if ( name.IsEmpty() == skyFalse ) spBillboardBeamt->Name.SetName( name );
		return spBillboardBeamt;
	}

private:

	/// <summary>
	///メンバ変数
	/// </summary>
	CBillboardBeamMember *m_pMember;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CBillboardBeam();

	/// <summary>
	///描画（環境ごとに実装）
	/// </summary>
	void DrawPass1();
};

/// <summary>
/// ビームチャージエフェクト便利処理群
/// </summary>
class CBillboardBeamUtil
{

public:

	/// <summary>
	/// 雷の動き
	/// </summary>
	static void Move_Thunder( const CBillboardBeamPtr &spEffect );

	/// <summary>
	/// 回転
	/// </summary>
	static void Move_Rot( const CBillboardBeamPtr &spEffect );
};


} } }