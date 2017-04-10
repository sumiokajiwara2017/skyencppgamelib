#pragma once

namespace sky { namespace lib { namespace math {

class CEntity2D;
typedef SmartPointer< CEntity2D >       CEntity2DPtr; //スマートポインタ型定義
typedef kfm::CFCurve< CEntity2D >       CEntity2DFCurve;
typedef kfm::CFCurvePlayer< CEntity2D > CEntity2DFCurvePlayer;

/// <summary>
/// 二次元情報集合クラス
/// CJointに役割を奪われた感あり。過去の遺産です。
/// </summary>
class CEntity2D : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
    virtual ~CEntity2D(){ SAFE_DELETE( Before ); }

	/// <summary>
	/// 拡大縮小
	/// </summary>
	inline void SetSc( const CBasicVector2 &sc )
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
	inline void AddSc( const CBasicVector2 &sc )
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
	inline const CBasicVector2 &GetSc(){ return m_Sc; }
	inline const dectype        GetSc_X(){ return m_Sc.X; }
	inline const dectype        GetSc_Y(){ return m_Sc.Y; }

	/// <summary>
	/// 回転
	/// </summary>
	inline void SetRot( CBasicAngle angle )
	{
		m_IsUpdate = skyTrue;
		m_Rot = angle;
	}
	inline void AddRot( CBasicAngle angle )
	{
		m_IsUpdate = skyTrue;
		m_Rot += angle;
	}
	inline const CBasicAngle &GetRot(){ return m_Rot.Rot(); }

	/// <summary>
	/// 位置
	/// </summary>
	inline void SetPos( const CBasicVector2 &pos )
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
	inline void AddPos( const CBasicVector2 &pos )
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
	inline const CBasicVector2 &GetPos(){ return m_Pos; }
	inline const dectype        GetPos_X(){ return m_Pos.X; }
	inline const dectype        GetPos_Y(){ return m_Pos.Y; }

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
			m_LocalMatrix.SetupScale( CBasicVector3( m_Sc.X , m_Sc.Y , 0.0f ) );

			//回転（処理負荷高い）
			CMatrix4x3 rotMat;
			m_Rot.ToMatrix( rotMat );
			m_LocalMatrix *= rotMat;

			//移動設定（処理負荷低い）
			m_LocalMatrix.SetTranslation( CBasicVector3( m_Pos.X , m_Pos.Y , 0.0f ) );

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

		m_Pos = CBasicVector2( 0.0f , 0.0f );
		m_Sc  = CBasicVector2( 1.0f , 1.0f );
		m_Rot = 0.0f;
	}

	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CEntity2D& operator =( const CEntity2D& src )
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
	skyBool operator ==( const CEntity2D& src ) const
	{
		return ( 
		m_Pos      == src.m_Pos &&
		m_Sc       == src.m_Sc  &&
		m_Rot      == src.m_Rot );
	}

	/// <summary>
	/// *演算子オーバーライド
	/// </summary>
	CEntity2D operator *( dectype src ) const
	{
		return CEntity2D( m_Pos * src , m_Rot * src , m_Sc * src );
	}
	CEntity2D operator *( const CEntity2D &src ) const
	{
		return CEntity2D( m_Pos * src.m_Pos , m_Rot * src.m_Rot , m_Sc * src.m_Sc );
	}

	/// <summary>
	/// -演算子オーバーライド
	/// </summary>
	CEntity2D operator -( const CEntity2D& src ) const
	{
		return CEntity2D( m_Pos - src.m_Pos , m_Rot - src.m_Rot , m_Sc - src.m_Sc );
	}

	/// <summary>
	/// +演算子オーバーライド
	/// </summary>
	CEntity2D operator +( const CEntity2D& src ) const
	{
		return CEntity2D( m_Pos + src.m_Pos , m_Rot + src.m_Rot , m_Sc + src.m_Sc );
	}

	/// <summary>
	/// 過去の全情報（通常１フレーム前。これと現在の値によりコリジョン判定を行う）
	/// </summary>
	CEntity2D *Before;

	/// <summary>
	/// ３Ｄエンティティの情報を作成する
	/// この処理を使用するのはCvertexBufferのセットアップ等特殊な場合に限定した方がいい。
	/// 計算量を減らすために２Ｄでやっているのに早い段階で３Ｄに戻したらもともこもないので。
	/// </summary>
	CEntity3DPtr Create3D() const
	{
		return CEntity3D::Create
        (
		    CBasicVector3( m_Pos.X , m_Pos.Y , 0.0f ) ,
			CBasicAngleVector3( 0.0f , 0.0f , m_Rot.Rot() ) ,
			CBasicVector3( m_Sc.X , m_Sc.Y , 1.0f )
        );
	}

	/// <summary>
	/// 値を過去の情報に保存する
	/// </summary>
	void Save(){ *Before = *this; }

	/// <summary>
	/// 作成
	/// </summary>
	static CEntity2DPtr Create(
		const CBasicVector2 &pos = CBasicVector2( 0 , 0 ) , 
		const dectype rot = 0.0f ,
		const CBasicVector2 &sc = CBasicVector2( 1.0f , 1.0f ) )
	{
		
		return CEntity2DPtr( NEW__( CEntity2D , CEntity2D( pos , rot , sc ) ) );
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
    CEntity2D() : Before( skyNull ) , m_IsUpdate( skyFalse ){ Reset(); }
    CEntity2D( 
		const CBasicVector2 &pos , 
		const CBasicAngleVector2 rot = 0.0f ,
		const CBasicVector2 &sc = CBasicVector2( 1.0f , 1.0f ) )
		: Before( skyNull ) , m_Pos( pos ) , m_Sc( sc ) , m_Rot( rot ) , m_IsUpdate( skyTrue ){ m_LocalMatrix.Identity(); }

	//---Systemで使用
    //※取得関数だが念のため値が更新されたことにしておく。
	inline CBasicVector2      &GetScUnSafe(){ m_IsUpdate = skyTrue; return m_Sc; }
	inline dectype            &GetScUnSafe_X(){ m_IsUpdate = skyTrue; return m_Sc.X; }
	inline dectype            &GetScUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Sc.Y; }
	inline CBasicAngleVector2 &GetRotUnSafe(){ m_IsUpdate = skyTrue; return m_Rot; }
	inline CBasicVector2      &GetPosUnSafe(){ m_IsUpdate = skyTrue; return m_Pos; }
	inline dectype            &GetPosUnSafe_X(){ m_IsUpdate = skyTrue; return m_Pos.X; }
	inline dectype            &GetPosUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Pos.Y; }
	inline void UpdateFlagOn(){ m_IsUpdate = skyTrue; }

private:

	/// <summary>
	/// ローカルマトリックス
	/// </summary>
	CMatrix4x3	m_LocalMatrix;

	/// <summary>
	/// 更新フラグ
	/// </summary>
	skyBool             m_IsUpdate;

	/// <summary>
	/// 位置
	/// </summary>
	CBasicVector2	m_Pos;

	/// <summary>
	/// 拡大縮小
	/// </summary>
	CBasicVector2	m_Sc;

	/// <summary>
	/// 回転
	/// </summary>
	CBasicAngleVector2	m_Rot;
};

} } }
