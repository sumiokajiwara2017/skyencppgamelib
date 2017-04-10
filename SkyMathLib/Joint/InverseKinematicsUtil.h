#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 逆運動学（インバースキネマティクス）を実現する機能実装群
/// </summary>
namespace InverseKinematicsUtil
{
    /// <summary>
    /// Cyclic-Coordinate Descent IK ソルバ
    /// </summary>
    /// <param name="skeleton">スケルトン</param>
    /// <param name="effector">エフェクタ関節</param>
    /// <param name="targetPos">目標位置</param>
    /// <param name="numMaxIteration">最大反復回数</param>
    /// <param name="errToleranceSq">誤差許容値の二乗</param>
    void SolveCcd( const CSkeletonPtr &skeleton , const CJointPtr &effector , const math::CBasicVector3 &targetPos , s32 numMaxIteration , dectype errToleranceSq);

	/// <summary>
	/// パーティクル IK ソルバ
	/// </summary>
	/// <param name="skeleton">スケルトン</param>
	/// <param name="effector">エフェクタ関節</param>
	/// <param name="targetPos">目標位置</param>
	/// <param name="numMaxIteration">最大反復回数</param>
	/// <returns>パーティクル位置</returns>
	CBasicVector3List &SolveParticle( CBasicVector3List &dest , const CSkeletonPtr &skeleton , const CJointPtr &effector , const math::CBasicVector3 &targetPos , s32 numMaxIteration );

    /// <summary>
    /// ヨー・ピッチ・ロールヤコビアン IK ソルバ
    /// </summary>
    /// <param name="skeleton">スケルトン</param>
    /// <param name="effector">エフェクタ関節</param>
    /// <param name="dest">目標位置</param>
    /// <param name="numMaxIteration">最大反復回数</param>
    /// <param name="step">計算刻み幅</param>
    void SolveEulerJacobian( const CSkeletonPtr &skeleton , const CJointPtr &effector , const math::CBasicVector3 &targetPos , s32 numMaxIteration, dectype step );

};

} } }