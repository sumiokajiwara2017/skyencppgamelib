#pragma once

#define INVALID_JOINT_INDEX ( -1 )

namespace sky { namespace lib { namespace math {

class CJoint;
typedef SmartPointer< CJoint >   CJointPtr;
typedef CList< u32 , CJointPtr > CJointPtrList;
typedef CVector< CJoint >        CJointVector;
typedef CHash< CJointPtr >       CJointPtrHash;
typedef CStack< CJointPtr >      CJointPtrStack;

/// <summary>
/// ジョイントクラス
/// </summary>
class CJoint : public serialize::CSerializeInterface
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CJoint();

	/// <summary>
	/// 名前
	/// </summary>
    CName Name;

	/// <summary>
	/// インデックス
	/// </summary>
    s32 Index;

	/// <summary>
	/// ローカル行列を単位行列にする
	/// </summary>
	inline void Reset();

//-----------並行移動

	//※Set … 値を上書きする（完全に値を置き換える）※置き換える対象の行列が単位行列であることを前提とします。行列が初期状態でない場合はReset()を使用してから設定してください。
	//  Add … 値を加算する（元々の値を生かす）
	//  Mul … 値を乗算する（元々の値を生かす）

	/// <summary>
	/// X軸
	/// </summary>
	inline void SetPosition_X( dectype value );
	inline void AddPosition_X( dectype value );

	/// <summary>
	/// Y軸
	/// </summary>
	inline void SetPosition_Y( dectype value );
	inline void AddPosition_Y( dectype value );

	/// <summary>
	/// Z軸
	/// </summary>
	inline void SetPosition_Z( dectype value );
	inline void AddPosition_Z( dectype value );

	/// <summary>
	/// XY軸
	/// </summary>
	inline void SetPosition_XY( const CBasicVector2 &vec );
	inline void AddPosition_XY( const CBasicVector2 &vec );

	/// <summary>
	/// XZ軸
	/// </summary>
	inline void SetPosition_XZ( const CBasicVector2 &vec );
	inline void AddPosition_XZ( const CBasicVector2 &vec );

	/// <summary>
	/// YZ軸
	/// </summary>
	inline void SetPosition_YZ( const CBasicVector2 &vec );
	inline void AddPosition_YZ( const CBasicVector2 &vec );

	/// <summary>
	/// XYZ軸
	/// </summary>
	inline void SetPosition_XYZ( const CBasicVector3 &vec );
	inline void AddPosition_XYZ( const CBasicVector3 &vec );

//-----------拡大縮小

	/// <summary>
	/// X軸
	/// </summary>
	inline void SetScale_X( dectype value );
	inline void MulScale_X( dectype value );

	/// <summary>
	/// Y軸
	/// </summary>
	inline void SetScale_Y( dectype value );
	inline void MulScale_Y( dectype value );

	/// <summary>
	/// Z軸
	/// </summary>
	inline void SetScale_Z( dectype value );
	inline void MulScale_Z( dectype value );

	/// <summary>
	/// XZ軸
	/// </summary>
	inline void SetScale_XZ( const math::CBasicVector2 &vec );
	inline void AddScale_XZ( const math::CBasicVector2 &vec );

	/// <summary>
	/// YZ軸
	/// </summary>
	inline void SetScale_YZ( const math::CBasicVector2 &vec );
	inline void AddScale_YZ( const math::CBasicVector2 &vec );

	/// <summary>
	/// XY軸
	/// </summary>
	inline void SetScale_XY( const math::CBasicVector2 &vec );
	inline void MulScale_XY( const math::CBasicVector2 &vec );

	/// <summary>
	/// XYZ軸
	/// </summary>
	inline void SetScale_XYZ( const math::CBasicVector3 &vec );
	inline void MulScale_XYZ( const math::CBasicVector3 &vec );
	inline void SetScale_XYZ( dectype sc );
	inline void MulScale_XYZ( dectype sc );

//-----------回転

	/// <summary>
	/// X軸
	/// </summary>
	inline void SetRotation_X( dectype radian );
	inline void MulRotation_X( dectype radian );

	/// <summary>
	/// Y軸
	/// </summary>
	inline void SetRotation_Y( dectype radian );
	inline void MulRotation_Y( dectype radian );

	/// <summary>
	/// Z軸
	/// </summary>
	inline void SetRotation_Z( dectype radian );
	inline void MulRotation_Z( dectype radian );

	/// <summary>
	/// XY軸
	/// </summary>
	inline void MulRotation_XY( const CBasicVector2 &vec );
	inline void MulRotation_YX( const CBasicVector2 &vec );

	/// <summary>
	/// XZ軸
	/// </summary>
	inline void MulRotation_XZ( const CBasicVector2 &vec );
	inline void MulRotation_ZX( const CBasicVector2 &vec );

	/// <summary>
	/// YZ軸
	/// </summary>
	inline void MulRotation_YZ( const CBasicVector2 &vec );
	inline void MulRotation_ZY( const CBasicVector2 &vec );

	/// <summary>
	/// XYZ軸
	/// </summary>
	inline void MulRotation_XYZ( const CBasicVector3 &vec );
	inline void MulRotation_XZY( const CBasicVector3 &vec );
	inline void MulRotation_YXZ( const CBasicVector3 &vec );
	inline void MulRotation_YZX( const CBasicVector3 &vec );
	inline void MulRotation_ZXY( const CBasicVector3 &vec );
	inline void MulRotation_ZYX( const CBasicVector3 &vec );

	/// <summary>
	/// 軸指定で回転
	/// </summary>
	inline void SetRotation( const CBasicVector3 &axis , dectype radian );
	inline void MulRotation( const CBasicVector3 &axis , dectype radian );

	/// <summary>
	/// 現在の回転情報を取得
	/// </summary>
	inline const CQuaternion &GetRotation() const;
	inline CQuaternion       &GetRotation();
	inline CRotationMatrix    GetRotationMatrix() const;
	inline CMatrix4x3         GetRotationMatrix4x3() const;

//----------クオータニオン

	inline void MulQuaternion( const CQuaternion &q );

//-----------値取得(行列からは下記のような上方が取れる）

	/// <summary>
	/// 位置情報
	/// </summary>
	inline dectype GetPosition_X() const;
	inline dectype GetPosition_Y() const;
	inline dectype GetPosition_Z() const;
	inline CBasicVector2 GetPosition_XY() const;
	inline CBasicVector2 GetPosition_XZ() const;
	inline CBasicVector2 GetPosition_YZ() const;
	inline CBasicVector3 GetPosition_XYZ() const;

	/// <summary>
	/// スケール情報
	/// </summary>
	inline dectype GetScale_X() const;
	inline dectype GetScale_Y() const;
	inline dectype GetScale_Z() const;
	inline CBasicVector2 GetScale_XY() const;
	inline CBasicVector2 GetScale_XZ() const;
	inline CBasicVector2 GetScale_YZ() const;
	inline CBasicVector3 GetScale_XYZ() const;

	/// <summary>
	/// このジョイントの空間マトリックスをかえす（主に描画に使用）
	/// 一番処理負荷が軽いのはシステム内は全てGetLocalMatrixを使って、
	/// アプリは各種トランスフォームを行った後必ずComputeLocalMatrix()を呼ぶようにする。
	/// これで変化があった時だけComputeLocalMatrix()が呼ばれるようになり余計な負荷がへるはず。
	/// </summary>
	inline const CMatrix4x3 &ComputeLocalMatrix(); //内部値を計算して返す
	inline const CMatrix4x3 &GetLocalMatrix() const;     //バッファの参照を返す
	inline void              SetLocalMatrix( const CMatrix4x3 &matrix );

	/// <summary>
	/// 現在のジョイントのローカル座標のX軸（これを軸にクオータニオンで回転したりこの前方に進むためにベクトルとして使用したりすると便利）
	/// せん断等ローカル座標系の軸自体が歪む幾何変換が行われていた場合、歪んだ軸が取得されてしまう
	/// </summary>
	inline CBasicVector3 GetAxisX() const;

	/// <summary>
	/// 現在のジョイントのローカル座標のY軸（これを軸にクオータニオンで回転したりこの前方に進むためにベクトルとして使用したりすると便利）
	/// せん断等ローカル座標系の軸自体が歪む幾何変換が行われていた場合、歪んだ軸が取得されてしまう
	/// </summary>
	inline CBasicVector3 GetAxisY() const;

	/// <summary>
	/// 現在のジョイントのローカル座標のZ軸（これを軸にクオータニオンで回転したりこの前方に進むためにベクトルとして使用したりすると便利）
	/// せん断等ローカル座標系の軸自体が歪む幾何変換が行われていた場合、歪んだ軸が取得されてしまう
	/// </summary>
	inline CBasicVector3 GetAxisZ() const;

	/// <summary>
	/// ローカル行列の計算をＯＮ／ＯＦＦする（なんらかの理由で外部からSetLocalMatrixで設定した値を使用したい場合等）
	/// </summary>
	inline void SetComputeLocalMatrixOn( skyBool isComputeOn = skyTrue );

	/// <summary>
	/// ローカル行列の計算にオフセット行列を使用するかどうか。
	/// </summary>
	inline void SetComputeOffsetMatrixToLocalMatrixOn( skyBool isComputeOn = skyTrue );

	/// <summary>
	/// 座標マトリックスの取得
	/// </summary>
	inline CMatrix4x3 GetPositionMatrix() const;

	/// <summary>
	/// ボーン変換行列（回転×オフセット行列×座標行列）を返す。
	/// IKに使用。モーションの場合キーフレームデータからこれを算出する。
	/// </summary>
	inline const CMatrix4x3 &ComputeBoneTransForm(); //内部値から計算
	inline const CMatrix4x3 &GetJointTransForm() const;
	inline void SetJointTransForm( const CMatrix4x3 &matrix ); //MotionControllerがキー情報から算出した値など

	/// <summary>
	/// 初期姿勢行列取得／設定
	/// </summary>
	inline const CMatrix4x3 &GetInitialTransform() const;
	inline void SetInitialTransform( const CMatrix4x3 &matrix );

	/// <summary>
	/// オフセット行列取得／設定
	/// </summary>
	inline const CMatrix4x3 &GetOffsetMatrix() const;
	inline void              SetOffsetMatrix( const CMatrix4x3 &matrix );
	inline void              SetOffset( const CBasicVector3 &offset );
	inline CBasicVector3     GetOffset() const;

	/// <summary>
	/// オフセットベクトルを取得／設定
	/// 内部ではオフセット行列としてもつ
	/// </summary>
	inline CBasicVector3 GetOffsetVector() const;
	inline void SetOffsetVector( const CBasicVector3 &vector );

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
	/// 複製をする
	/// </summary>
	serialize::CSerializeInterfacePtr Clone();
	serialize::CSerializeInterfacePtr CloneSharing();

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CJointPtr Create( const CBasicVector3 offset = CBasicVector3( 0 , 0 , 0 ) , const CJointPtr &spParent = CJointPtr() )
	{
		CJointPtr spJoint( NEW__( CJoint , CJoint() ) );

		spJoint->SetOffset( offset );
		spJoint->AttachParent( spParent );

		return spJoint;
	}

protected:

	/// <summary>
	/// ローカルマトリックス
	/// </summary>
	CMatrix4x3  m_LocalMatrix; //※これはポインタにして使用する場面のみインスタンス化する

	/// <summary>
	/// ローカルマトリックスを計算で出すフラグ
	/// </summary>
	skyBool m_IsLocalMatrixComputeOn;

	/// <summary>
	/// ローカルマトリックスの計算にオフセット行列も使用するフラグ
	/// </summary>
	skyBool m_IsComputeOffsetMatrixToLocalMatrixOn;

	/// <summary>
	/// 拡大縮小
	/// </summary>
	math::CBasicVector3 m_Scale;

	/// <summary>
	/// 回転情報クオータニオン
	/// </summary>
	CQuaternion m_Rotation;

	/// <summary>
	/// 位置
	/// </summary>
	CBasicVector3 m_Postion;

//------以下はモーション時にしか使用しない-----▽

	/// <summary>
	/// ボーン変換行列
	/// </summary>
	CMatrix4x3  m_JointTransformMatrix; //※これはポインタにして使用する場面のみインスタンス化する

	/// <summary>
	/// 初期ボーントランスフォーム行列
	/// </summary>
	CMatrix4x3* m_pInitialTransform; //※これはポインタにして使用する場面のみインスタンス化する

	/// <summary>
	/// オフセット行列（親Jointからどれだけ変化しているかという情報）
	/// </summary>
	CMatrix4x3  m_OffsetMatrix; //※これはポインタにして使用する場面のみインスタンス化する

//------以下はアニメーション時にしか使用しない-----△

//-------------親子関係関連-------------------//

public:

	/// <summary>
	/// 最上位かどうか判定する
	/// </summary>
	skyBool IsRoot();

	/// <summary>
	/// 子が存在するか確認する
	/// </summary>
	skyBool IsChildOn();

	/// <summary>
	/// 親の関連付け
	/// </summary>
	void AttachParent( const CJointPtr &spParent );

	/// <summary>
	/// 親の関連付け解除
	/// </summary>
	void DetachParent();

	/// <summary>
	/// 親の取得
	/// </summary>
	const CJointPtr &Parent();

	/// <summary>
	/// 子のリストを返す
	/// </summary>
	CJointPtrList &ChildList();

	/// <summary>
	/// 子の関連付け
	/// </summary>
	CJointPtr AttachChild     ( u32 key, const CJointPtr &spChild , const CBasicVector3 &pos = CBasicVector3_ZERO );

	/// <summary>
	/// 子の関連付け
	/// </summary>
	CJointPtr AttachChild     ( const CJointPtr &spChild , const CBasicVector3 &pos = CBasicVector3_ZERO );

	/// <summary>
	/// 子の関連付け
	/// </summary>
	CJointPtr AttachChildFront( const CJointPtr &spChild , const CBasicVector3 &pos = CBasicVector3_ZERO );

	/// <summary>
	/// 子の関連付け解除
	/// </summary>
	void               DetachChild     ( const CJointPtr &spChild );

	/// <summary>
	/// 子の関連付け解除
	/// </summary>
	void               DetachAllChild  ();

	/// <summary>
	/// 子を名前で検索
	/// </summary>
	CJointPtr SearchChild( const hash::CHashStringPtr &name );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CJoint( const CJointPtr &spParent = CJointPtr() );

protected:

	/// <summary>
	/// 親
	/// </summary>
	CJointPtr   m_Parent;

	/// <summary>
	/// 子リスト
	/// </summary>
	CJointPtrList	m_ChildList;

//-------------親子関係関連-------------------//


};

} } }

#define CJointCreate_()	                            sky::lib::math::CJoint::Create()
#define CJointCreateWithOffset_( offset , parent )  sky::lib::math::CJoint::Create( offset , parent )

#include "Joint.inl"