#pragma once

/*
【スキニングの原理「一旦、原点に戻してから回転などの変換を加え、また元の位置に戻す】
スキニングの原理は一旦、原点に戻してから回転などの変換を加え、元の位置に戻すでした。
一旦、原点に戻すには『オフセット行列』を使います。オフセット行列は座標原点から見た絶対位置の初期姿勢を原点に移動する行列です。
『初期姿勢行列』は移動も含めた行列で、オフセット行列をかけると回転情報だけになる。

	//これでいいはず。
	変換後座標 = 座標 * ボーンオフセット行列 *（ 初期姿勢行列（ただし親子関係で計算済みのこと） * アニメーション行列 )
    //ただし初期姿勢織り込み済みの場合はこうなる
	変換後座標 = 座標 * ボーンオフセット行列 * アニメーション行列
*/

namespace sky { namespace lib { namespace graphic {

class CMotionJointDebugDisplayNode;

} } }

namespace sky { namespace lib { namespace math {

/// <summary>
/// モーションコントローラー（順運動学（フォワードキネマティクス）で作られているデータを再生。親リンクから子リンクへと計算していく）
/// 未実装
/// ・モーション補間
/// ・モーションブレンド
/// ・デュアルクオータニオン対応
/// </summary>
class CMotionController
{
	//デバッグ表示機能はお友達
	friend class graphic::CMotionJointDebugDisplayNode;

public:

	/// <summary>
	/// ジョイントパレットの関連付け
	/// Meshの持つジョイントパレットを設定することで、メッシュにモーションをさせる
	/// </summary>
	void AttachSkeleton( const CSkeletonPtr &spSkeleton );

	/// <summary>
	/// モーションデータ配列※を設定
	/// 複数のモーション（歩く、走る、殴る、蹴るなど）が入ったモーションコンテナ
	/// </summary>
	void SetAnimationContena( const CMotionDataContenaPtr &spAnimationContena );

	/// <summary>
	/// モーションデータの再生
	/// </summary>
	void Play( u32 motionNo , updateTime blendTime = 0 );

	/// <summary>
	/// 現在再生中のモーションデータを取得
	/// </summary>
	CMotionDataPtr GetCurrentAnimationData();

	/// <summary>
	/// 時間の更新（戻り値は再生されたモーションのフレーム番号）
	/// </summary>
	void Update( updateTime time , skyBool relativeToCurTime = skyTrue );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMotionController();

private:

	/// <summary>
	/// モーションコンテナ
	/// </summary>
	CMotionDataContenaPtr m_spAnimationContena;

	/// <summary>
	/// スケルトン
	/// </summary>
	CSkeletonPtr m_spSkeleton;

	/// <summary>
	/// 現在の時間
	/// </summary>
	updateTime m_CurrentTime;

	/// <summary>
	/// 現在のモーション番号
	/// </summary>
	u32 m_CurrentAnimationNo;

	/// <summary>
	/// 現在のモーションデータ
	/// </summary>
	CMotionDataPtr m_CurrentAnimationData; 

	/// <summary>
	/// ブレンド時間
	/// </summary>
	updateTime m_BlendTime;

//------計算バッファ----▽

	/// <summary>
	/// ジョイントスタック
	/// </summary>
	CJointPtrStack m_JointStack;

	/// <summary>
	/// ワールド変換行列配列
	/// </summary>
	math::CMatrix4x3Array m_WorldTransforms;

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// デバッグバッファ
	/// </summary>
	math::CMatrix4x3Array m_DebugJointTransforms;

#endif	

//------計算バッファ----△

	/// <summary>
	/// 骨の更新
	/// </summary>
    void UpdateJointTransform( updateTime time , skyBool relativeToCurTime );
	static void CMotionController::UpdateWorldTransformRecursive( const CJointPtr &spJoint , const CJointPtr &spParentJoint );

	/// <summary>
	/// ワールド座標位置の更新
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// スキン座標の更新
	/// </summary>
    void UpdateSkinTransform();

	/// <summary>
	/// ジョイント変換行列の初期化
	/// </summary>
	void InitJointTransformsMatrix();
};

} } }