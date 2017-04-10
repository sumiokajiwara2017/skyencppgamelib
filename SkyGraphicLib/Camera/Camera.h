#pragma once

namespace sky { namespace lib { namespace graphic {

class Camera;
typedef SmartPointer< Camera >	CameraPtr;					//スマートポインタ版定義

/// <summary>
/// カメラデータ構造体
/// </summary>
struct CameraData
{
	/// <summary>
	/// カメラの座標
	/// </summary>
	math::CBasicVector3      m_Eye;

	/// <summary>
	/// カメラの注視点
	/// </summary>
	math::CBasicVector3      m_Tag;

	/// <summary>
	/// 視線ベクトル
	/// </summary>
	math::CBasicVector3      m_ViewVec;

	/// <summary>
	/// 上方向定義ベクトル
	/// </summary>
	math::CBasicVector3      m_UpVec;

	/// <summary>
	/// 右方向定義ベクトル
	/// </summary>
	math::CBasicVector3      m_RightVec;

	/// <summary>
	/// 視体積の近平面
	/// </summary>
    dectype                  m_Near;

	/// <summary>
	/// 遠平面
	/// </summary>
    dectype                  m_Far;

	/// <summary>
	/// 視野角
	/// </summary>
	dectype                  m_Fov;

	/// <summary>
	/// 正射影拡大縮小値
	/// </summary>
	dectype                  m_Zoom;

	/// <summary>
	/// アスペクト比
	/// </summary>
	dectype                  m_Aspect;

	/// <summary>
	/// カメラと被写体の距離
	/// </summary>
	dectype                  m_Distance;

	/// <summary>
	/// 回転角度（カメラの種別によって意味が変わってくる）
	/// </summary>
	math::CBasicAngleVector3 m_Angle;

	/// <summary>
	/// 傾き角度（正射影モードでは意味は無い）
	/// </summary>
	math::CBasicAngleVector3 m_InclinationAngle;

	/// <summary>
	/// カメラの描画矩形
	/// </summary>
	math::CBasicSize         m_ViewSize;
};

/// <summary>
/// カメラクラス
/// </summary>
class Camera : public CSceneGraphNode //最終的にはこっち
{
    SkyTypeDefRTTI;
	friend class CameraDebugDispNode;
	friend class CameraManager;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Camera(){};

	/// <summary>
	/// カメラデータの取得
	/// </summary>
	inline CameraData &GetData(){ return m_Data; }

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	void vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// シーングラフノードとしての動作後処理
	/// </summary>
	void vPostRendering   ( CRenderingParam &param );

	/// <summary>
	/// カメラ管理の更新
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// 初期情報の設定
	/// </summary>
	void SetInitInfo( const math::CBasicVector3 &eye , 
		              const math::CBasicVector3 &tag , 
					  const math::CBasicVector3 &upVec , 
					  const math::CBasicVector3 &rightVec , 
					  dectype fov , 
					  dectype nearValue , 
					  dectype farValue , 
					  dectype width , 
					  dectype height , 
					  const math::CBasicAngleVector3 &angle = math::CBasicAngleVector3( 0 , 0 , 0 , 0 ) , 
					  const math::CBasicAngleVector3 &inclinationangle = math::CBasicAngleVector3( 0 , 0 , 0 , 0 ) , 
					  dectype distance = 10.0f , 
					  dectype zoom = 1.0f );

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 画面サイズの設定
	/// </summary>
	void SetDisplaySize( dectype width , dectype height );

	/// <summary>
	/// 距離の設定
	/// </summary>
	void SetDistance( dectype distance );

	/// <summary>
	/// 角度の設定
	/// </summary>
	void SetAngle( const math::CBasicAngleVector3 &angle );
	const math::CBasicAngleVector3 &GetAngle();

	/// <summary>
	/// カメラの位置の設定／取得
	/// </summary>
	void SetEye  ( const math::CBasicVector3 &eye );

	/// <summary>
	/// 対象位置の設定／取得
	/// </summary>
	void SetTag( const math::CBasicVector3 &tag );

	/// <summary>
	/// ビュー情報の更新（取得はGetViewMatrix()で）
	/// </summary>
	void UpdateViewInfo( const math::CBasicVector3 &eye , const math::CBasicVector3 &tag , const math::CBasicVector3 &upVec );

	/// <summary>
	/// 透視投射情報の更新（取得はGetProjectionMatrix()で）
	/// </summary>
	void UpdateProjectionInfo( dectype fov , dectype width , dectype height , dectype nearValue , dectype farValue );
	void UpdateProjectionInfo_Fov( dectype fov );
	void UpdateProjectionInfo_Size( dectype width , dectype height );
	void UpdateProjectionInfo_NearFar( dectype nearValue , dectype farValue );

	/// <summary>
	/// 正射影情報の更新（取得はGetOrthoMatrix()で）
	/// </summary>
	void UpdateOrthoInfo( dectype width , dectype height , dectype nearValue , dectype farValue , dectype zoom = 1.0f );
	void UpdateOrthoInfo_Size( dectype width , dectype height );
	void UpdateOrthoInfo_NearFar( dectype nearValue , dectype farValue );
	void UpdateOrthoInfo_Zoom( dectype zoom );

	/// <summary>
	/// ワールドマトリックスの取得
	/// </summary>
	const math::CMatrix4x3 &GetWorldMatrix();

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	const math::CMatrix4x3 &GetViewMatrix();

	/// <summary>
	/// プロジェクション行列の取得
	/// </summary>
	const math::CMatrix4x3 &GetProjectionMatrix();

	/// <summary>
	/// ビュープロジェクション行列の取得
	/// </summary>
	const math::CMatrix4x3 &GetViewProjection();

	/// <summary>
	/// カメラに正面を向くビルボード回転マトリックスを計算
	/// </summary>
	const math::CRotationMatrix &ComputeBillboardRotationMatrix();

	/// <summary>
	/// 正射影行列ＯＮ／ＯＦＦ
	/// </summary>
	void SetOrtho( skyBool isOrthoOn );

	/// <summary>
	/// 正射影行列判定
	/// </summary>
	skyBool IsOrthoOn();

	/// <summary>
	/// 正射影行列の取得
	/// </summary>
	const math::CMatrix4x3 &GetOrthoMatrix();

	/// <summary>
	/// カメラ変換マトリクスを作成
	/// 正射影モードならGetOrthoMatrix()の値を返す
	/// それ以外ならGetViewMatrix()の値を返す
	/// </summary>
	const math::CMatrix4x3 &GetCameraMatrix();

	/// <summary>
	/// カメラの傾き角度を設定する
	/// </summary>
	void SetInitInclinationAngle( const math::CBasicAngleVector3 &angle );

	/// <summary>
	/// 左上原点フラグ（正射影のみ有効。これがＯＮのときは各スプライトが左上原点座標となるように位置を変換を行う）
	/// </summary>
	void SetLeftTopCenter( skyBool isLeftTopCenterOn );

	/// <summary>
	/// 左上原点フラグチェック
	/// </summary>
	skyBool IsLeftTopCenter();

	/// <summary>
	/// デフォルト入力ＯＮ／ＯＦＦ
	/// </summary>
	void SetDefaultInput( skyBool isOn = skyTrue );

	/// <summary>
	/// デフォルト入力ＯＮ／ＯＦＦ
	/// </summary>
	math::CBasicVector3 ComputeDistancePos( dectype distance );

//---Static

	/// <summary>
	/// 作成
	/// </summary>
	static CameraPtr Create( eCameraKind ekind = eCameraKind_Satellite , const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CameraPtr spResult = CameraPtr( NEW__( Camera , Camera( ekind ) ) );
		if ( name.IsEmpty() == skyFalse ) spResult->Name.SetName( name );
		return spResult;
	}

protected:

	/// <summary>
	/// カメラデータ
	/// </summary>
	 CameraData              m_Data;

	/// <summary>
	/// カメラ初期データ
	/// </summary>
	 CameraData              m_InitData;

	/// <summary>
	/// 正射影フラグ
	/// </summary>
	skyBool                  m_IsOrthoOn;

	/// <summary>
	/// 標準入力受付フラグ
	/// </summary>
	skyBool                  m_IsDefaultInputOn;

	/// <summary>
	/// マウス左ボタン押下フラグ
	/// </summary>
	skyBool                  m_IsMouseLBtnClick;

	/// <summary>
	/// マウス右ボタン押下フラグ
	/// </summary>
	skyBool                  m_IsMouseRBtnClick;

	/// <summary>
	/// マウスボタン押下座標
	/// </summary>
	math::CBasicVector2      m_MouseClickPos;

	/// <summary>
	/// カレントフラグＯＮ
	/// </summary>
	skyBool                  m_IsCurrentOn;

	/// <summary>
	/// カメラ種別
	/// </summary>
	eCameraKind              m_eKind;

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	math::CMatrix4x3         m_WorldMatrix;

	/// <summary>
	/// ビュー行列の取得
	/// </summary>
	math::CMatrix4x3         m_ViewMatrix;

	/// <summary>
	/// プロジェクション行列の取得
	/// </summary>
	math::CMatrix4x3         m_ProjectionMatrix;

	/// <summary>
	/// ビュープロジェクション行列の取得
	/// </summary>
	math::CMatrix4x3         m_ViewProjectionMatrix;

	/// <summary>
	/// 正射影行列の取得
	/// </summary>
	math::CMatrix4x3         m_OrthoMatrix;

	/// <summary>
	/// ビルボード行列
	/// </summary>
	math::CRotationMatrix    m_BillBoardRotationMatrix;

	/// <summary>
	/// 視野台クラス
	/// </summary>
	CFrustum                 m_Frustum;

	/// <summary>
	/// 左上原点フラグ
	/// </summary>
	skyBool                  m_IsLeftTopCenterFlg;

	/// <summary>
	/// マウスイベントデリゲート（マウスイベントハンドラに登録する）
	/// </summary>
	delegate::IDelegateObjectPtr m_spMouseEventDelegate;
	void MouseEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// キーボードイベントデリゲート（キーボードイベントハンドラに登録する）
	/// </summary>
	delegate::IDelegateObjectPtr m_spKeyBoradEventDelegate;
	void KeyBoradEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Camera( eCameraKind ekind );

	/// <summary>
	/// 現在のカメラにする
	/// </summary>
	void SetCurrentOn( skyBool isCurrentOn );

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
};

} } }

#define CameraCreate_( ekind )            sky::lib::graphic::Camera::Create( ekind )
#define CameraCreateName_( ekind , name ) sky::lib::graphic::Camera::Create( ekind , name )
