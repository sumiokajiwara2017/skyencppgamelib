#pragma once

/*

【 実装状況 】
DirectX11 PC HLSL ○実装済
OpenGL    PC GLSL ×未実装
OpenGL    PC Cg   ×未実装

*/

namespace sky { namespace lib { namespace graphic {

class   CDecal;
typedef SmartPointer< CDecal >		CDecalPtr;			//スマートポインタ型定義
typedef CList< u32 , CDecalPtr >	CDecalPtrList;		//リスト定義

struct CDecalMember;

/// <summary>
/// デカールエフェクト（ポリゴンにシールポリゴンを貼る）
/// </summary>
class CDecal : public CEffectObject
{
	friend class CDecalManager;
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CDecal();

	/// <summary>
	/// テクスチャ関連付け
	/// </summary>
	void AttachTexture( const CTexturePtr &spTexture );

	/// <summary>
	/// 破棄時間設定
	/// </summary>
	void SetDestoryTime( updateTime destoryTime );

	/// <summary>
	/// 破棄判定
	/// </summary>
	skyBool IsDestroy();

	/// <summary>
	/// 作成
	/// </summary>
	void Create( const math::CBasicVector3 &pos , const math::CBasicVector3 &norm , const math::CBasicVector3 &tang , f32 w , f32 h , f32 d , const CMeshPtr &spMesh );

	/// <summary>
	/// 更新
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// 描画
	/// </summary>
	virtual void vDraw();

private:

	/// <summary>
	/// 作成
	/// </summary>
	static CDecalPtr   Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CDecalPtr spDecalt( NEW__( CDecal , CDecal() ) );
		if ( name.IsEmpty() == skyFalse ) spDecalt->Name.SetName( name );
		return spDecalt;
	}

	/// <summary>
	///メンバ変数
	/// </summary>
	CDecalMember *m_pMember;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CDecal();

	/// <summary>
	///描画（環境ごとに実装）
	/// </summary>
	void DrawPass1();
};


} } }