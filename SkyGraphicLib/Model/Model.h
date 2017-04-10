#pragma once

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// モデルクラス
/// リソースファイルから描画する３Ｄオブジェクトはこのクラスが管理します。
/// </summary>
class CModel : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual           ~CModel(){};

    /// <summary>
	/// レンダリング処理
    /// </summary>
	virtual void       vRendering          ( CRenderingParam &param );

    /// <summary>
	///  レンダリング処理実装関数
    /// </summary>
	void               RenderingImple      ( CRenderingParam &param , const CShaderObjectPtr &shader );

	/// <summary>
	/// マテリアルの取得
	/// </summary>
	CMaterialPtr       GetMaterial( u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// 幾何学形状情報のDifuseMapを指定の色に設定する
	/// </summary>
	void               AttachDiffuseMap( const graphic::CTexturePtr &spTexture , u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// 幾何学形状情報のDifuseカラーを指定の色に設定する
	/// </summary>
	void               SetDiffuse( const CColor &color , u32 meshNo = 0 , u32 materialNo = 0 );

	/// <summary>
	/// アルファ値の設定
	/// </summary>
	void               SetAlphaValue( f32 alpha , u32 meshNo = 0 , u32 materialNo = 0 );

//---Collision

	/// <summary>
	/// 軸沿いボックスの算出
	/// </summary>
	void               ComputeAABB3();

	/// <summary>
	/// 有向境界ボックスの算出
	/// </summary>
	void               ComputeOBB3();

	/// <summary>
	/// 球の算出
	/// </summary>
	void               ComputeBS();

	/// <summary>
	///  軸沿いボックスの取得（ローカル座標）
	/// </summary>
	inline const math::CAABB3 &GetAABB3() const;

	/// <summary>
	///  有向境界ボックスの取得（ローカル座標）
	/// </summary>
	inline const math::COBB3 &GetOBB3() const;

	/// <summary>
	///  球の取得（ローカル座標）
	/// </summary>
	inline const math::CBasicSphere &GetOBS() const;

	/// <summary>
	///  現在のジョイントで計算した軸沿いボックスの取得（ワールド座標）（並行移動のみ反映）
	///  ※衝突判定を行うならこれを使用する。
	/// </summary>
	inline const math::CAABB3 &GetAABB3ComputeJoint();

	/// <summary>
	///  現在のジョイントで計算した有向境界ボックスの取得（ワールド座標）（並行移動と回転を計算）
	///  ※衝突判定を行うならこれを使用する。
	/// </summary>
	inline const math::COBB3  &GetOBB3ComputeJoint();

	/// <summary>
	///  現在のジョイントで計算した球の取得（ワールド座標）（並行移動のみ反映）
	///  ※衝突判定を行うならこれを使用する。
	/// </summary>
	inline const math::CBasicSphere &GetOBSComputeJoint();

	/// <summary>
	///  モデルのローカル座標系における各種サイズ、位置情報。変換後の値
	/// </summary>
	void                         ComputeSizeData();
	inline math::CBasicVector3   GetLocalSize();
	inline math::CBasicVector3   GetLocalHalfSize();
	inline f32                   GetLocalYUnderLength();


//----Branch

    /// <summary>
	///  Meshの関連付け／取得
    /// </summary>
	void				  AttachMeshContena( const CMeshContenaPtr &spMesh );
	CMeshContenaPtr		  GetMeshContena( skyBool isRequired = skyTrue );

//----Branch

//----Leaf

    /// <summary>
	///  シェーダーを関連付け／関連付け解除をする
    /// </summary>
	void            AttachShader( const CShaderObjectPtr &effectPtr );
	void            ClearAndAttachShader( const CShaderObjectPtr &effectPtr ); //シェーダーを一旦クリアしアタッチする
	void            DetachShader();

//----Leaf

//---Serialize

	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	virtual skyBool SaveBinary( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	virtual skyBool LoadBinary( const stream::IStreamPtr& stream );

	/// <summary>
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする（各ノードでオーバーライドする）
	/// </summary>
	serialize::CSerializeInterfacePtr 	Clone();
	serialize::CSerializeInterfacePtr 	CloneSharing();

	/// <summary>
	/// 実行可能状態にする
	/// </summary>
	virtual void Activate();

	/// <summary>
	/// 実行可能状態を解除する
	/// </summary>
	virtual void Deactivate();

#ifdef SW_SKYLIB_DEBUG_ON

//---Debug

	/// <summary>
	/// 法線を描画する
	/// </summary>
	void SetDebugDisplayNormalOn( skyBool isOn , float length = 1.0f );

	/// <summary>
	/// デバッグに使用するワーク
	/// </summary>
	math::CJointPtr m_spDebugJoint;

#endif

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CModelPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		
		CModelPtr spModel( NEW__( CModel , CModel() ) );
		if ( name.IsEmpty() == skyFalse ) spModel->Name.SetName( name );
		return spModel;
	}

protected:

	/// <summary>
	/// モデルの大きさ情報
	/// </summary>
	math::CBasicVector3  m_LocalSize;         //ローカル座標系における各軸の長さ
	math::CBasicVector3  m_LocalHalfSize;     //ローカル座標系における各軸の半分の長さ
	f32                  m_LocalYUnderLength; //ローカル座標系におけるＹ軸の最小頂点位置とモデルの中心位置との差。モデルを地面の上にのせるオフセット距離に使う。
	                                          //モデルがローカル座標系において、モデルの中心点に座標系の中心が合った場合、この値は
	                                          //m_LocalHalfSize.Yと一致する。ローカル座標系においてもY軸方向にややプラスの位置、足
	                                          //が地面につくような形になっていた場合、この値は限りなく０に近い値になる。

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CModel();
	CModel( CSceneGraphNode &spNode );
	void Constractor();

	/// <summary>
	/// 軸沿いボックス３Ｄ
	/// </summary>
	math::CAABB3			     m_AABB3;
	math::CAABB3			     m_AABB3ComputeJoint; //その時点でのモデルの拡大縮小、回転、平行移動を考慮した値

	/// <summary>
	/// 有向境界ボックス３Ｄ
	/// </summary>
	math::COBB3                  m_OBB3;
	math::COBB3                  m_OBB3ComputeJoint; //その時点でのモデルの拡大縮小、回転、平行移動を考慮した値

	/// <summary>
	/// 球（バウンディングスフィア）３Ｄ
	/// </summary>
	math::CBasicSphere           m_BS;
	math::CBasicSphere           m_BSComputeJoint;  //その時点でのモデルの拡大縮小、回転、平行移動を考慮した値

	/// <summary>
	/// デバッグ法線表示ＯＮ
	/// </summary>
	skyBool                      m_IsDebugNormalDispOn;
};

#define CModelCreate_()				sky::lib::graphic::CModel::Create()
#define CModelCreateName_( name )	sky::lib::graphic::CModel::Create( name )

} } }

#include "Model.inl"