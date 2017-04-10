#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// カメラスマートポインタ型
/// </summary>
class CameraManager;
typedef SmartPointer< CameraManager > CameraManagerPtr;	//スマートポインタ版定義

/// <summary>
/// カメラ管理クラス
/// インスタンスはビューポートの数だけあるので、シングルトンではない。
/// </summary>
class CameraManager : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CameraManager(){};

	/// <summary>
	/// カメラを登録
	/// </summary>
	void ResisterCamera( const CameraPtr &spCamera );

	/// <summary>
	/// カメラを登録解除
	/// </summary>
	void UnResisterCamera( const CameraPtr &spCamera );

	/// <summary>
	/// カメラを切り替え
	/// </summary>
	void NextCameraRequest( updateTime time = CAMERAMANAGER_DEFAULT_INTERP_TIME , eInterpolateType eInterpType = eInterpolateType_None );

	/// <summary>
	/// カメラを切り替え中判定（補間中）
	/// </summary>
	skyBool IsInterpolate();

	/// <summary>
	/// 現在のカメラの設定
	/// </summary>
	void SetCurrentCameraRequest( const CameraPtr &spCamera , updateTime time = CAMERAMANAGER_DEFAULT_INTERP_TIME , eInterpolateType eInterpType = eInterpolateType_None );
	void SetCurrentCameraRequest( const hash::CHashStringPtr &name , updateTime time = CAMERAMANAGER_DEFAULT_INTERP_TIME , eInterpolateType eInterpType = eInterpolateType_None );

	/// <summary>
	/// 現在のカメラの取得
	/// </summary>
	CameraPtr GetCurrentCamera();

	/// <summary>
	/// カメラ管理の更新
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// キーボードイベントデリゲート（キーボードイベントハンドラに登録する）
	/// </summary>
	delegate::IDelegateObjectPtr m_spKeyBordEventDelegate;
	void KeyBordEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// デフォルトの入力操作ＯＮ／ＯＦＦ
	/// </summary>
	void SetDefaultInputOn( skyBool isDefaultInputOn ){ m_IsDefaultInputOn = isDefaultInputOn; }

//---Serialize

	/// <summary>
	/// 複製をする
	/// </summary>
	const serialize::CSerializeInterfacePtr&  Copy( const serialize::CSerializeInterfacePtr &spContainer );
	const serialize::CSerializeInterfacePtr&  CopySharing( const serialize::CSerializeInterfacePtr &spContainer );

	/// <summary>
	/// 複製をする
	/// </summary>
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

//---Static

	/// <summary>
	/// 作成
	/// </summary>
	static CameraManagerPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CameraManagerPtr spResult = CameraManagerPtr( NEW__( CameraManager , CameraManager() ) );
		if ( name.IsEmpty() == skyFalse ) spResult->Name.SetName( name );
		return spResult;
	}

private:

	/// <summary>
	/// 補間要求データ
	/// </summary>
	struct CInterpRequestData
	{
		/// <summary>
		/// 補間中移動先カメラ視点
		/// </summary>
		math::CBasicVector3 m_FromEye;
		math::CBasicVector3 m_ToEye;

		/// <summary>
		/// 補間中移動先カメラ注視点
		/// </summary>
		math::CBasicVector3 m_FromTag;
		math::CBasicVector3 m_ToTag;

		/// <summary>
		/// 補間時間
		/// </summary>
		updateTime m_Time;

		/// <summary>
		/// 補間経過時間
		/// </summary>
		updateTime m_ElapsedTime;

		/// <summary>
		/// 補間タイプ
		/// </summary>
		eInterpolateType m_eInterpType;
	};
	CFifo< CInterpRequestData > m_InterpQueue;

	/// <summary>
	/// デフォルト入力ON
	/// </summary>
	skyBool	m_IsDefaultInputOn;

//-----------State Function-----------------▽

	/// <summary>
	/// ステート種別
	/// </summary>
	enum eState
	{
		eState_Start = 0 ,
		eState_Wait ,
		eState_Interpolate ,
		eState_End ,
	};

	/// <summary>
	/// ステートマシン機能を付加
	/// </summary>
	state::CFunctionStateManager< CameraManager , eState > m_FunctionState;

//-----StateFunction-----▽

	skyBool State_Start( const fps::CFpsController &fps );
	skyBool State_Wait( const fps::CFpsController &fps );
	skyBool State_Interpolate( const fps::CFpsController &fps );
	skyBool State_End( const fps::CFpsController &fps );

//-----StateFunction-----△

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CameraManager();
};

} } }

#define CameraManagerCreate_()               sky::lib::graphic::CameraManager::Create()
#define CameraManagerCreateName_( name )     sky::lib::graphic::CameraManager::Create( name )
