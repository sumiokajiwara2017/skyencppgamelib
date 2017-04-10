#pragma once

namespace sky { namespace lib { namespace graphic {

class   CShaderObject;
typedef SmartPointer< CShaderObject >		CShaderObjectPtr;			//スマートポインタ型定義
typedef CList< dectype , CShaderObjectPtr >	CShaderObjectPtrList;		//スマートポインタリスト型

//-----------------------------------< 抽象実装 >-------------------------------------------//

/// <summary>
/// シェーダーの基本クラス
/// すべてのシェーダークラスはこのクラスを継承すること。
/// </summary>
class CShaderObject : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void			Initialize();

	/// <summary>
	/// 初期化
	/// </summary>
	static void			Dispose();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CShaderObject(){};

	/// <summary>
	/// パス数の取得
	/// </summary>
	inline u32 GetPassNum(){ return m_PassNum; };

	/// <summary>
	/// レンダリングパス番号設定
	/// </summary>
	inline void       SetRenderingPass( u32 passNo ){ m_RenderingPassNo = passNo; }

	/// <summary>
	/// 頂点座標変換行列を設定する（どのシェーダーにも頂点座標変換は必ず実装する）
	/// </summary>
	inline void       SetWorldMatrix ( const math::CMatrix4x3 &w )  { m_WorldMatrix = w; };

	/// <summary>
	/// カメラ情報を設定
	/// </summary>
	inline void       SetCamera      ( const CameraPtr &spCamera ) { m_spCamera = spCamera;}

	/// <summary>
	/// メッシュを設定
	/// </summary>
	inline void       SetMesh        ( const CMeshPtr   &spMesh )   { m_spMesh = spMesh; }

	/// <summary>
	/// メッシュを設定
	/// </summary>
	inline void       SetExtraData   ( const CExtraDataPtr   &spExtra )   { m_spExtraData = spExtra; }

	/// <summary>
	/// ライトを設定
	/// </summary>
	inline void       SetLight   ( const CLightPtr   &spLight )   { m_spLight = spLight; }

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	virtual void	  vRendering	 ( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	virtual void              vPostRendering  ( CRenderingParam &param );

	/// <summary>
	/// 描画（各シェーダーが実装）
	/// </summary>
	virtual void              vDraw     () = 0;

protected:

	/// <summary>
	/// パス数
	/// </summary>
	u32                       m_PassNum;

//---実行時に生成されるバッファ変数（保存の必要なし）

	/// <summary>
	/// パス数
	/// </summary>
	u32                       m_RenderingPassNo;

	/// <summary>
	/// ワールドマトリックス
	/// </summary>
	math::CMatrix4x3		  m_WorldMatrix;

	/// <summary>
	/// カメラ情報
	/// </summary>
    CameraPtr                 m_spCamera;

	/// <summary>
	/// メッシュ情報
	/// </summary>
    CMeshPtr                  m_spMesh;

	/// <summary>
	/// エキストラデータ情報
	/// </summary>
    CExtraDataPtr             m_spExtraData;

	/// <summary>
	/// ライト情報
	/// </summary>
    CLightPtr                 m_spLight;

    /// <summary>
	///  現在使用しているシェーダーを示すＩＤ
    /// </summary>
	u32						  m_ShaderId;

    /// <summary>
	/// シェーダー情報
    /// </summary>
	CShaderInfoPtr            m_spShaderInfo;

//---実行時に生成されるバッファ変数（保存の必要なし）

    /// <summary>
	/// コンストラクタ
    /// </summary>
	CShaderObject();
};

/// <summary>
/// シェーダー情報（このシェーダーを割り当てられた場合何も表示しません）
/// </summary>
class CNullShader : public CShaderObject
{

public:

	//描画
	virtual void vDraw(){};

	//作成
	static CShaderObjectPtr Create()
	{
		return CShaderObjectPtr( NEW CNullShader() );
	}
};

#define CNullShaderCreate_() sky::lib::graphic::CNullShader::Create()

} } }