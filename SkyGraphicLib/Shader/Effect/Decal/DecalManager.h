#pragma once

namespace sky { namespace lib { namespace graphic {

class   CDecalManager;
typedef SmartPointer< CDecalManager >		CDecalManagerPtr;			//スマートポインタ型定義

struct CDecalManagerMember;

/// <summary>
/// デカールエフェクト管理
/// </summary>
class CDecalManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CDecalManager();

	/// <summary>
	/// デカールを貼りつける対象のモデルを登録
	/// </summary>
	void AttachModel( const CModelPtr &spModel );

	/// <summary>
	/// デカールを貼りつける対象のモデルを登録破棄
	/// </summary>
	void DetachModel( const CModelPtr &spModel );

	/// <summary>
	/// デカール作成
	/// tangは接ベクトル。normが視線ならtangは視線方向に対して真横を示すベクトルを設定してください
	/// </summary>
	void CreateDecal( const CTexturePtr &spTexture , const math::CBasicVector3 &pos , const math::CBasicVector3 &norm , math::CBasicVector3 &tang , f32 distance , f32 width , f32 height , f32 depth , updateTime destoryTime );

	/// <summary>
	/// デカール破棄
	/// </summary>
	void DestoryDecal( const CDecalPtr &spDecal );

	/// <summary>
	/// デカール全破棄
	/// </summary>
	void DestoryAllDecal();

	/// <summary>
	/// 貼りつけ処理
	/// </summary>
	void DrawDecal( CRenderingParam &param );

	/// <summary>
	/// 作成
	/// </summary>
	static CDecalManagerPtr   Create()
	{
		CDecalManagerPtr spDecalt( NEW__( CDecalManager , CDecalManager() ) );
		return spDecalt;
	}

private:

	/// <summary>
	///メンバ変数
	/// </summary>
	CDecalManagerMember *m_pMember;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CDecalManager();
};

} } }