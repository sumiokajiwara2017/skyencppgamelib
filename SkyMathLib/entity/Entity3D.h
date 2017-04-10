#pragma once

namespace sky { namespace lib { namespace math {

class CEntity3D;
typedef SmartPointer< CEntity3D >       CEntity3DPtr; //スマートポインタ型定義
typedef kfm::CFCurve< CEntity3D >       CEntity3DFCurve;
typedef kfm::CFCurvePlayer< CEntity3D > CEntity3DFCurvePlayer;

/// <summary>
/// 三次元情報集合クラス
/// CJointに役割を奪われた感あり。過去の遺産です。
/// </summary>
class CEntity3D : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
    virtual ~CEntity3D(){ SAFE_DELETE( Before ); }

	/// <summary>
	/// 拡大縮小
	/// </summary>
	inline void SetSc( const CBasicVector3 &sc )
	{
		m_IsUpdate = skyTrue;
		m_Sc = sc;
	}
	inline void SetSc_X( dectype scX )
	{
		m_IsUpdate = skyTrue;
		m_Sc.X = scX;
	}
	inline void SetSc_Y( dectype scY )
	{
		m_IsUpdate = skyTrue;
		m_Sc.Y = scY;
	}
	inline void SetSc_Z( dectype scZ )
	{
		m_IsUpdate = skyTrue;
		m_Sc.Z = scZ;
	}
	inline void AddSc( const CBasicVector3 &sc )
	{
		m_IsUpdate = skyTrue;
		m_Sc += sc;
	}
	inline void AddSc_X( dectype scX )
	{
		m_IsUpdate = skyTrue;
		m_Sc.X += scX;
	}
	inline void AddSc_Y( dectype scY )
	{
		m_IsUpdate = skyTrue;
		m_Sc.Y += scY;
	}
	inline void AddSc_Z( dectype scZ )
	{
		m_IsUpdate = skyTrue;
		m_Sc.Z += scZ;
	}
	inline const CBasicVector3 &GetSc()  { return m_Sc; }
	inline const dectype        GetSc_X(){ return m_Sc.X; }
	inline const dectype        GetSc_Y(){ return m_Sc.Y; }
	inline const dectype        GetSc_Z(){ return m_Sc.Z; }

	/// <summary>
	/// 回転（度で設定）
	/// </summary>
	inline void SetRot( const CBasicAngleVector3 &rot )
	{
		m_IsUpdate = skyTrue;
		m_Rot = rot;
	}
	inline void SetRot_X( CBasicAngle rotX )
	{
		m_IsUpdate = skyTrue;
		m_Rot.X() = rotX;
	}
	inline void SetRot_Y( CBasicAngle rotY )
	{
		m_IsUpdate = skyTrue;
		m_Rot.Y() = rotY;
	}
	inline void SetRot_Z( CBasicAngle rotZ )
	{
		m_IsUpdate = skyTrue;
		m_Rot.Z() = rotZ;
	}
	inline void AddRot( const CBasicAngleVector3 &rot )
	{
		m_IsUpdate = skyTrue;
		m_Rot += rot;
	}
	inline void AddRot_X( CBasicAngle rotX )
	{
		m_IsUpdate = skyTrue;
		m_Rot.X() += rotX;
	}
	inline void AddRot_Y( CBasicAngle rotY )
	{
		m_IsUpdate = skyTrue;
		m_Rot.Y() += rotY;
	}
	inline void AddRot_Z( CBasicAngle rotZ )
	{
		m_IsUpdate = skyTrue;
		m_Rot.Z() += rotZ;
	}
	inline CBasicAngleVector3 &GetRot()  { return m_Rot; }
	inline CBasicAngle         GetRot_X(){ return m_Rot.X(); }
	inline CBasicAngle         GetRot_Y(){ return m_Rot.Y(); }
	inline CBasicAngle         GetRot_Z(){ return m_Rot.Z(); }

	/// <summary>
	/// 位置
	/// </summary>
	inline void SetPos( const CBasicVector3 &pos )
	{
		m_IsUpdate = skyTrue;
		m_Pos = pos;
	}
	inline void SetPos_X( dectype posX )
	{
		m_IsUpdate = skyTrue;
		m_Pos.X = posX;
	}
	inline void SetPos_Y( dectype posY )
	{
		m_IsUpdate = skyTrue;
		m_Pos.Y = posY;
	}
	inline void SetPos_Z( dectype posZ )
	{
		m_IsUpdate = skyTrue;
		m_Pos.Z = posZ;
	}
	inline void AddPos( const CBasicVector3 &pos )
	{
		m_IsUpdate = skyTrue;
		m_Pos += pos;
	}
	inline void AddPos_X( dectype posX )
	{
		m_IsUpdate = skyTrue;
		m_Pos.X += posX;
	}
	inline void AddPos_Y( dectype posY )
	{
		m_IsUpdate = skyTrue;
		m_Pos.Y += posY;
	}
	inline void AddPos_Z( dectype posZ )
	{
		m_IsUpdate = skyTrue;
		m_Pos.Z += posZ;
	}
	inline const CBasicVector3 &GetPos()  { return m_Pos; }
	inline const dectype        GetPos_X(){ return m_Pos.X; }
	inline const dectype        GetPos_Y(){ return m_Pos.Y; }
	inline const dectype        GetPos_Z(){ return m_Pos.Z; }

	/// <summary>
	/// 内容を行列にして取得
	/// </summary>
	inline const CMatrix4x3 &ColMatrix()
	{
		//更新されたら計算しなおす
		if ( m_IsUpdate == skyTrue )
		{
			//恒等行列(初期化みたいなもの）
			m_LocalMatrix.Identity();

//----各現在値からワールドマトリックスを作成する

			//拡縮情報設定（処理負荷低い）
			m_LocalMatrix.SetupScale( m_Sc );

			//回転（処理負荷高い）
			CMatrix4x3 rot;
			m_LocalMatrix *= m_Rot.ToMatrixYXZ( rot );

			//移動設定（処理負荷低い）
			m_LocalMatrix.SetTranslation( m_Pos );

			//更新フラグをオフに
			m_IsUpdate = skyFalse;
		}

		//拡縮＊回転＊移動マトリックスを返す
		return m_LocalMatrix;
	}

	/// <summary>
	/// リセット
	/// </summary>
	void Reset()
	{
		m_LocalMatrix.Identity();

		m_Sc  = CBasicVector3( 1.0f , 1.0f , 1.0f );
		m_Rot = CBasicAngleVector3( 0.0f , 0.0f , 0.0f );
		m_Pos = CBasicVector3( 0.0f , 0.0f , 0.0f );
	}

	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CEntity3D& operator =( const CEntity3D& src )
	{
        m_Pos      = src.m_Pos;
        m_Sc       = src.m_Sc;
        m_Rot      = src.m_Rot;
		m_IsUpdate = skyTrue;
        return *this;
	}

	/// <summary>
	/// ==演算子オーバーライド
	/// </summary>
	skyBool operator ==( const CEntity3D& src ) const
	{
		return (
        m_Pos      == src.m_Pos &&
        m_Sc       == src.m_Sc  &&
        m_Rot      == src.m_Rot );
	}

	/// <summary>
	/// *演算子オーバーライド
	/// </summary>
	CEntity3D operator *( dectype src ) const
	{
		return CEntity3D( m_Pos * src , m_Rot * src , m_Sc * src );
	}
	CEntity3D operator *( const CEntity3D &src ) const
	{
		return CEntity3D( m_Pos * src.m_Pos , m_Rot * src.m_Rot , m_Sc * src.m_Sc );
	}

	/// <summary>
	/// -演算子オーバーライド
	/// </summary>
	CEntity3D operator -( const CEntity3D& src ) const
	{
		return CEntity3D( m_Pos - src.m_Pos , m_Rot - src.m_Rot , m_Sc - src.m_Sc );
	}

	/// <summary>
	/// +演算子オーバーライド
	/// </summary>
	CEntity3D operator +( const CEntity3D& src ) const
	{
		return CEntity3D( m_Pos + src.m_Pos , m_Rot + src.m_Rot , m_Sc + src.m_Sc );
	}

	/// <summary>
	/// 過去の全情報（使用する場合は設定する。通常１フレーム前。これと現在の値によりコリジョン判定を行う）
	/// </summary>
    CEntity3D *Before;

	/// <summary>
	/// 値を過去の情報に保存する
	/// </summary>
    void Save(){ *Before = *this; }

	/// <summary>
	/// 作成
	/// </summary>
	static CEntity3DPtr Create(
		const CBasicVector3 &pos = CBasicVector3( 0.0f , 0.0f , 0.0f ) , 
		const CBasicAngleVector3 &rot = CBasicAngleVector3( 0 , 0.0f , 0.0f , 0.0f ) ,
		const CBasicVector3 &sc = CBasicVector3( 1.0f , 1.0f , 1.0f ) )
	{
		
		return CEntity3DPtr( NEW__( CEntity3D , CEntity3D( pos , rot , sc ) ) );
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
    CEntity3D() : Before( skyNull ) , m_IsUpdate( skyTrue ){ Reset(); }
    CEntity3D( 
		const CBasicVector3 &pos , 
		const CBasicAngleVector3 &rot = CBasicAngleVector3( 0 , 0.0f , 0.0f , 0.0f ) ,
		const CBasicVector3 &sc = CBasicVector3( 1.0f , 1.0f , 1.0f ) )
		: Before( skyNull ) , m_Pos( pos ) , m_Sc( sc ) , m_Rot( rot ) , m_IsUpdate( skyTrue ){ m_LocalMatrix.Identity(); }

	//---Systemで使用
    //※取得関数だが念のため値が更新されたことにしておく。
	inline CBasicVector3 &GetScUnSafe(){ m_IsUpdate = skyTrue; return m_Sc; }
	inline dectype       &GetScUnSafe_X(){ m_IsUpdate = skyTrue; return m_Sc.X; }
	inline dectype       &GetScUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Sc.Y; }
	inline dectype       &GetScUnSafe_Z(){ m_IsUpdate = skyTrue; return m_Sc.Z; }
	inline CBasicAngleVector3 &GetRotUnSafe(){ m_IsUpdate = skyTrue; return m_Rot; }
	inline CBasicAngle        &GetRotUnSafe_X(){ m_IsUpdate = skyTrue; return m_Rot.X(); }
	inline CBasicAngle        &GetRotUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Rot.Y(); }
	inline CBasicAngle        &GetRotUnSafe_Z(){ m_IsUpdate = skyTrue; return m_Rot.Z(); }
	inline CBasicVector3 &GetPosUnSafe(){ m_IsUpdate = skyTrue; return m_Pos; }
	inline dectype       &GetPosUnSafe_X(){ m_IsUpdate = skyTrue; return m_Pos.X; }
	inline dectype       &GetPosUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Pos.Y; }
	inline dectype       &GetPosUnSafe_Z(){ m_IsUpdate = skyTrue; return m_Pos.Z; }

private:

	/// <summary>
	/// ローカルマトリックス
	/// </summary>
	CMatrix4x3	m_LocalMatrix;

	/// <summary>
	/// 更新フラグ
	/// </summary>
	skyBool         m_IsUpdate;

	/// <summary>
	/// 拡大縮小
	/// </summary>
	CBasicVector3	m_Sc;

	/// <summary>
	/// 回転
	/// </summary>
	CBasicAngleVector3	m_Rot;

	/// <summary>
	/// 位置
	/// </summary>
	CBasicVector3	m_Pos;
};

} } }
