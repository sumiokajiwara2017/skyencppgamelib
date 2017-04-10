#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// モーションキーフレームデータ（行列）
/// </summary>
struct CAnimationKeyFrameMatrix
{
	/// <summary>
	/// キー時間
	/// </summary>
	u32    m_Time;

	/// <summary>
	/// キーフレームデータ
	/// </summary>
	math::CMatrix4x3  m_Matrix;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAnimationKeyFrameMatrix()
	{
		m_Time = 0;
	}
	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CAnimationKeyFrameMatrix& operator =( const CAnimationKeyFrameMatrix& src )
	{
		m_Time   = src.m_Time;
		m_Matrix = src.m_Matrix;
		return *this;
	}
};
typedef CVector< CAnimationKeyFrameMatrix >      CAnimationKeyFrameMatrixArray;
typedef CVector< CAnimationKeyFrameMatrixArray > CAnimationKeyFrameMatrixArrayVector;

/// <summary>
/// モーションキーフレームデータ（拡縮）
/// </summary>
struct CAnimationKeyFrameScale
{
	/// <summary>
	/// キー時間
	/// </summary>
	u32    m_Time;

	/// <summary>
	/// キーフレームデータ
	/// </summary>
	math::CBasicVector3  m_Scale;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAnimationKeyFrameScale()
	{
		m_Time = 0;
	}
	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CAnimationKeyFrameScale& operator =( const CAnimationKeyFrameScale& src )
	{
		m_Time  = src.m_Time;
		m_Scale = src.m_Scale;

		return *this;
	}
};
typedef CVector< CAnimationKeyFrameScale >      CAnimationKeyFrameScaleArray;
typedef CVector< CAnimationKeyFrameScaleArray > CAnimationKeyFrameScaleArrayVector;

/// <summary>
/// モーションキーフレームデータ（移動）
/// </summary>
struct CAnimationKeyFrameTrans
{
	/// <summary>
	/// キー時間
	/// </summary>
	u32    m_Time;

	/// <summary>
	/// キーフレームデータ
	/// </summary>
	math::CBasicVector3  m_Trans;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAnimationKeyFrameTrans()
	{
		m_Time = 0;
	}
	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CAnimationKeyFrameTrans& operator =( const CAnimationKeyFrameTrans& src )
	{
		m_Time  = src.m_Time;
		m_Trans = src.m_Trans;

		return *this;
	}
};
typedef CVector< CAnimationKeyFrameTrans >      CAnimationKeyFrameTransArray;
typedef CVector< CAnimationKeyFrameTransArray > CAnimationKeyFrameTransArrayVector;

/// <summary>
/// モーションキーフレームデータ（回転）
/// </summary>
struct CAnimationKeyFrameRot
{
	/// <summary>
	/// キー時間
	/// </summary>
	u32    m_Time;

	/// <summary>
	/// キーフレームデータ
	/// </summary>
	math::CQuaternion m_Rot;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAnimationKeyFrameRot()
	{
		m_Time = 0;
	}
	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CAnimationKeyFrameRot& operator =( const CAnimationKeyFrameRot& src )
	{
		m_Time = src.m_Time;
		m_Rot  = src.m_Rot;

		return *this;
	}
};
typedef CVector< CAnimationKeyFrameRot >      CAnimationKeyFrameRotArray;
typedef CVector< CAnimationKeyFrameRotArray > CAnimationKeyFrameRotArrayVector;


/// <summary>
/// モーションデータ
/// </summary>
struct CAnimationData
{
	/// <summary>
	/// 名前
	/// </summary>
	CName Name;

	/// <summary>
	/// 対応ジョイント名
	/// </summary>
	hash::CHashStringPtr m_JointName;

	/// <summary>
	/// キーフレームデータ配列（行列）
	/// </summary>
	CAnimationKeyFrameMatrixArray m_KeyFrameArrayMatrix;

	/// <summary>
	/// キーフレームデータ配列（拡縮）
	/// </summary>
	CAnimationKeyFrameScaleArray m_KeyFrameArrayScale;

	/// <summary>
	/// キーフレームデータ配列（移動）
	/// </summary>
	CAnimationKeyFrameTransArray m_KeyFrameArrayTrans;

	/// <summary>
	/// キーフレームデータ配列（回転）
	/// </summary>
	CAnimationKeyFrameRotArray   m_KeyFrameArrayRot;

	/// <summary>
	/// キーフレームデータ配列を設定する
	/// </summary>
	void SetKeyFrameMatrixArray( const CAnimationKeyFrameMatrixArray &keyFrameArray )
	{
		m_KeyFrameArrayMatrix.Clear();
		for( u32 i = 0 ; i < keyFrameArray.GetNum() ; i++ )
		{
			m_KeyFrameArrayMatrix.Add( keyFrameArray[ i ] );
		}
	}
	void SetKeyFrameScaleArray( const CAnimationKeyFrameScaleArray &keyFrameArray )
	{
		m_KeyFrameArrayScale.Clear();
		for( u32 i = 0 ; i < keyFrameArray.GetNum() ; i++ )
		{
			m_KeyFrameArrayScale.Add( keyFrameArray[ i ] );
		}
	}
	void SetKeyFrameTransArray( const CAnimationKeyFrameTransArray &keyFrameArray )
	{
		m_KeyFrameArrayTrans.Clear();
		for( u32 i = 0 ; i < keyFrameArray.GetNum() ; i++ )
		{
			m_KeyFrameArrayTrans.Add( keyFrameArray[ i ] );
		}
	}
	void SetKeyFrameRotArray( const CAnimationKeyFrameRotArray &keyFrameArray )
	{
		m_KeyFrameArrayRot.Clear();
		for( u32 i = 0 ; i < keyFrameArray.GetNum() ; i++ )
		{
			m_KeyFrameArrayRot.Add( keyFrameArray[ i ] );
		}
	}

	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CAnimationData& operator =( const CAnimationData& src )
	{
		Name                 = src.Name;
		m_JointName          = src.m_JointName;

		//キーフレームデータ配列
		m_KeyFrameArrayMatrix.Clear();
		for( u32 i = 0 ; i < src.m_KeyFrameArrayMatrix.GetNum() ; i++ )
		{
			m_KeyFrameArrayMatrix.Add( src.m_KeyFrameArrayMatrix[ i ] );
		}

		m_KeyFrameArrayScale.Clear();
		for( u32 i = 0 ; i < src.m_KeyFrameArrayScale.GetNum() ; i++ )
		{
			m_KeyFrameArrayScale.Add( src.m_KeyFrameArrayScale[ i ] );
		}

		m_KeyFrameArrayTrans.Clear();
		for( u32 i = 0 ; i < src.m_KeyFrameArrayTrans.GetNum() ; i++ )
		{
			m_KeyFrameArrayTrans.Add( src.m_KeyFrameArrayTrans[ i ] );
		}

		m_KeyFrameArrayRot.Clear();
		for( u32 i = 0 ; i < src.m_KeyFrameArrayRot.GetNum() ; i++ )
		{
			m_KeyFrameArrayRot.Add( src.m_KeyFrameArrayRot[ i ] );
		}
		return *this;
	}
};
typedef CVector< CAnimationData > CAnimationDataPallet;

/// <summary>
/// 型定義
/// </summary>
class CMotionData;
typedef SmartPointer< CMotionData >	 CMotionDataPtr;					//スマートポインタ版定義
typedef CList< u32 , CMotionDataPtr > CMotionDataPtrList;
typedef CHash< CMotionDataPtr >       CMotionDataPtrHash;
typedef CVector< CMotionDataPtr >     CMotionDataPtrPallet;

/// <summary>
/// CStaticメッシュのモーションデータクラスです
/// このインスタンス１個につき１モーション（歩く、走る、蹴る、殴る等いずれか一つ）
/// </summary>
class CMotionData : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// 名前
	/// </summary>
    CName Name;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CMotionData();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMotionData();

	/// <summary>
	/// モーションパレットを設定
	/// </summary>
	void SetAnimationPallet( const CAnimationDataPallet &pallet );

	/// <summary>
	/// モーションデータ数を返す（通常ジョイントの数とおなじになるはず）
	/// </summary>
	u32 GetAnimationNum() const;

	/// <summary>
	///  モーションデータの取得
	/// </summary>
	const CAnimationData &GetAnimationData( u32 index ) const;

	/// <summary>
	/// 最大時間
	/// </summary>
	void SetMaxTime( f32 maxTime );
	f32  GetMaxTime() const;

//---Serialize
//※モーションバイナリデータはシーングラフのバイナリに含めない。
//  独自のバイナリとして保存する　

	/// <summary>
	/// バイナリにセーブ
	/// </summary>
	skyBool SaveBinary_( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// バイナリからロード
	/// </summary>
	skyBool LoadBinary_( const stream::IStreamPtr& stream );

//---Static

	/// <summary>
	/// 作成
	/// </summary>
	static CMotionDataPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CMotionDataPtr spResult = CMotionDataPtr( NEW__( CMotionData , CMotionData() ) );
		if ( name.IsEmpty() == skyFalse ) spResult->Name.SetName( name );
		return spResult;
	}
			
private:

	/// <summary>
	/// 最大時間
	/// </summary>
	f32 m_MaxTime;

	/// <summary>
	/// モーションデータパレット
	/// 配列のインデックスはジョイントパレットのジョイントインデックスと対応
	/// つまりジョイント毎のモーションを持っている
	/// </summary>
	CAnimationDataPallet m_AnimationDataPallet;
};

#define CMotionDataCreate_()           sky::lib::math::CMotionData::Create()
#define CMotionDataCreateName_( name ) sky::lib::math::CMotionData::Create( name )

//-----------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// 型定義
/// </summary>
class CMotionDataContena;
typedef SmartPointer< CMotionDataContena >	CMotionDataContenaPtr;					//スマートポインタ版定義
typedef CList< u32 , CMotionDataContenaPtr > CMotionDataContenaPtrList;

/// <summary>
/// モーションデータコンテナ
/// 複数のモーション（歩く、走る、蹴る、殴る）を格納するコンテナクラス
/// </summary>
class CMotionDataContena : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// 名前
	/// </summary>
    CName Name;

	/// <summary>
	/// モーションの追加（追加の順番が取得時のインデックスになります）
	/// </summary>
	void AddAnimation( const CMotionDataPtr &spAnimation );

	/// <summary>
	/// モーション数の取得
	/// </summary>
	u32 GetAnimationNum();

	/// <summary>
	/// モーションをインデックスで取得
	/// </summary>
	CMotionDataPtr GetAnimation( u32 index );

	/// <summary>
	/// モーションを名前で取得
	/// </summary>
	CMotionDataPtr GetAnimation( const hash::CHashStringPtr &name );

//---Static

	/// <summary>
	/// 作成
	/// </summary>
	static CMotionDataContenaPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CMotionDataContenaPtr spResult = CMotionDataContenaPtr( NEW__( CMotionDataContena , CMotionDataContena() ) );
		if ( name.IsEmpty() == skyFalse ) spResult->Name.SetName( name );
		return spResult;
	}

private:

	/// <summary>
	/// モーションデータ配列
	/// </summary>
	CMotionDataPtrPallet m_AnimationPallet;

	/// <summary>
	/// モーション名前コンテナ
	/// </summary>
	CMotionDataPtrHash   m_AnimationHash;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMotionDataContena();
};

} } }

#define CMotionDataContenaCreate_()           sky::lib::math::CMotionDataContena::Create()
#define CMotionDataContenaCreateName_( name ) sky::lib::math::CMotionDataContena::Create( name )
