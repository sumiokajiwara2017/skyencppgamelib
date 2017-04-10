#include "StdAfx.h"
#include "SkyMathLib/Joint.h"

namespace sky { namespace lib { namespace math {

namespace InverseKinematicsUtil
{

void SolveCcd( const CSkeletonPtr &skeleton, const CJointPtr &effector, const math::CBasicVector3 &targetPos, s32 numMaxIteration, dectype errToleranceSq)
{
	CBasicVector3 localTargetPos   = math::CBasicVector3_ZERO;
    CBasicVector3 localEffectorPos = math::CBasicVector3_ZERO;

    for ( s32 i = 0; i < numMaxIteration; ++i )
    {
		for ( CJointPtr spJoint = effector->Parent() ; spJoint.IsEmpty() == skyFalse && spJoint->Index != INVALID_JOINT_INDEX; spJoint = spJoint->Parent() )
        {
            // エフェクタの位置の取得
            CBasicVector3 effectorPos = skeleton->GetGlobalPosition( effector->Index );

            // 注目ノードの位置の取得
            CBasicVector3 jointPos = skeleton->GetGlobalPosition(spJoint->Index);

            // ワールド座標系から注目ノードの局所座標系への変換
            CMatrix4x3 invCoord =  Invert( skeleton->GetGlobalCoordinate( spJoint->Index ) );

            // エフェクタと到達目標のローカル位置
            localEffectorPos = Transform( effectorPos, invCoord );

            localTargetPos = Transform( targetPos, invCoord );

            // (1) 基準関節→エフェクタ位置への方向ベクトル
			CBasicVector3 basis2Effector = localEffectorPos.Normalize();

            // (2) 基準関節→目標位置への方向ベクトル
			CBasicVector3 basis2Target = localTargetPos.Normalize();

            // ベクトル (1) を (2) に一致させるための最短回転量（Axis-Angle）
            //
            // 回転角
			dectype rotationDotProduct = basis2Effector.Dot( basis2Target );
            dectype rotationAngle = ( dectype )CMathUtil::Acos( rotationDotProduct );

            if (rotationAngle > 1.0e-5f)
            {
                // 回転軸
				CBasicVector3 rotationAxis = basis2Effector.Cross( basis2Target );
				if ( rotationAxis.Length() > 0 )
				{
					rotationAxis.Normalize();

					// 関節回転量の補正
					CQuaternion rot;
					spJoint->GetRotation() = rot.SetToRotateAboutAxis( rotationAxis, rotationAngle ) * spJoint->GetRotation();
				}
            }
        }

        if ( ( localEffectorPos - localTargetPos ).QuaredLength() < errToleranceSq )
        {
            return;
        }
    }
}

/// <summary>
/// パーティクル群へのフィギュアの当てはめ
/// </summary>
/// <param name="skeleton">スケルトン</param>
/// <param name="particles">パーティクル位置</param>
/// <param name="joints">関節リスト</param>
static void FitFigureToParticle( const CSkeletonPtr &skeleton , const CBasicVector3List &particles, const CJointPtrList &joints )
{
	for ( u32 jid = joints.GetNum() - 1; jid > 0; --jid)
    {
        // 注目関節の局所座標系行列の逆行列
        CMatrix4x3 invCoord = Invert4x4( skeleton->GetGlobalCoordinate( joints[ jid ]->Index ) );

        CBasicVector3 p1 = Transform(particles[jid - 1], invCoord);
        CBasicVector3 p0 = Transform(skeleton->GetGlobalPosition(joints[jid]->Index), invCoord);
		CBasicVector3 dv = (p1 - p0).Normalize();
		CBasicVector3 offset = (joints[jid - 1]->GetOffset()).Normalize();

		dectype rotationAngle = (dectype)CMathUtil::Acos( offset.Dot( dv ) );
        if (rotationAngle > 1.0e-5f)
        {
			CBasicVector3 rotationAxis = offset.Cross( dv ).Normalize();

			if ( rotationAxis.Length() > 0 )
			{
				CQuaternion rot;
				joints[jid]->GetRotation() = rot.SetToRotateAboutAxis(rotationAxis, rotationAngle);
			}
        }
    }
}

	CBasicVector3List &SolveParticle( CBasicVector3List &dest , const CSkeletonPtr &skeleton , const CJointPtr &effector, const math::CBasicVector3 &targetPos , s32 numMaxIteration )
{
	dest.DeleteAll();

	CList< u32 , dectype > constraints;
	CJointPtrList joints;

	// パーティクルの作成
	for ( CJointPtr j = effector; j->Parent().IsEmpty() != skyTrue && j->Parent()->Index != INVALID_JOINT_INDEX; j = j->Parent() )
	{
		dest.AddTail( skeleton->GetGlobalPosition( j->Index ) );
		constraints.AddTail( j->GetOffset().Length() );
		joints.AddTail( j );
	}
	joints.AddTail( joints[ joints.GetNum() - 1 ]->Parent() );
	CBasicVector3 rootPos = skeleton->m_spJointRoot->GetPosition_XYZ();

	// パーティクル位置の更新
	for ( s32 it = 0 ; it < numMaxIteration; ++it)
	{
		CBasicVector3 dv = targetPos - dest[ 0 ];
		dest[ 0 ] += dv;

		for ( u32 pid = 0; pid < dest.GetNum() - 1 ; ++pid )
		{
			dv = dest[pid + 1] - dest[pid];
			dv *= 0.5f - constraints[pid] / dv.Length() * 0.5f;
			dest[pid] += dv;
			dest[pid + 1] -= dv;
		}

		dv = rootPos - dest[dest.GetNum() - 1];
		dv *= 1.0f - constraints[constraints.GetNum() - 1] / dv.Length();
		dest[dest.GetNum() - 1] += dv;
	}

	FitFigureToParticle( skeleton , dest , joints );

	return dest;
}

/// <summary>
/// クォータニオンの対数
/// </summary>
/// <param name="q">クォータニオン</param>
/// <returns>対数クォータニオン</returns>
static CQuaternion QuatLn( const CQuaternion &q )
{
    CQuaternion n = q;
	n.Normalize();

    if ( CMathUtil::Abs(n.W) == 1.0f)
    {
        return CQuaternion(0, 0, 0, 0);
    }
    d64 theta = CMathUtil::Acos(n.W);
    d64 invSinTheta = theta / CMathUtil::Sin(( dectype )theta);
    CQuaternion qe = n * ( dectype )invSinTheta;
    qe.W = 0.0f;

    return qe;
}

/// <summary>
/// 対数クォータニオン ln(Q) の各成分に関するクォータニオン Q の微分
/// </summary>
/// <param name="dqx">dQ/ln(Q).x</param>
/// <param name="dqy">dQ/ln(Q).y</param>
/// <param name="dqz">dQ/ln(Q).z</param>
/// <param name="lnq">対数クォータニオン ln(Q)</param>
static void CalcDqDlq( CQuaternion &dqx, CQuaternion &dqy, CQuaternion &dqz , const CQuaternion &lnq)
{
	d64 a = lnq.X, b = lnq.Y, c = lnq.Z;
	d64 theta = CMathUtil::Sqrt(( dectype )(a * a + b * b + c * c));
	d64 sint = CMathUtil::Sin(( dectype )theta), cost = CMathUtil::Cos(( dectype )theta);
	d64 itheta = 1.0 / theta;
	d64 it2c_it3s = cost / (theta * theta) - sint / (theta * theta * theta);

    dqx.W = (dectype)(-a * itheta * sint);
	dqx.X = (dectype)(a * a * it2c_it3s + itheta * sint);
	dqx.Y = (dectype)(a * b * it2c_it3s);
	dqx.Z = (dectype)(a * c * it2c_it3s);

	dqy.W = (dectype)(-b * itheta * sint);
	dqy.X = (dectype)(b * a * it2c_it3s);
	dqy.Y = (dectype)(b * b * it2c_it3s + itheta * sint);
	dqy.Z = (dectype)(b * c * it2c_it3s);

	dqz.W = (dectype)(-c * itheta * sint);
	dqz.X = (dectype)(c * a * it2c_it3s);
	dqz.Y = (dectype)(c * b * it2c_it3s);
	dqz.Z = (dectype)(c * c * it2c_it3s + itheta * sint);
}

/// <summary>
/// クォータニオン Q の各成分に関する回転行列 R の微分 dR/dQ
/// </summary>
/// <param name="q">回転クォータニオン</param>
/// <param name="dq">微分クォータニオン</param>
/// <returns>微分回転行列</returns>
static CMatrix4x3 CalcDrDq( const CQuaternion &q, const CQuaternion &dq)
{
    CMatrix4x3 m(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    m.M11 = -4.0f * (q.Y * dq.Y + q.Z * dq.Z);
    m.M12 =  2.0f * (q.X * dq.Y + q.Y * dq.X + q.W * dq.Z + q.Z * dq.W);
    m.M13 =  2.0f * (q.X * dq.Z + q.Z * dq.X - q.W * dq.Y - q.Y * dq.W);
    m.M21 =  2.0f * (q.X * dq.Y + q.Y * dq.X - q.W * dq.Z - q.Z * dq.W);
    m.M22 = -4.0f * (q.X * dq.X + q.Z * dq.Z);
    m.M23 =  2.0f * (q.Y * dq.Z + q.Z * dq.Y + q.W * dq.X + q.X * dq.W);
    m.M31 =  2.0f * (q.X * dq.Z + q.Z * dq.X + q.W * dq.Y + q.Y * dq.W);
    m.M32 =  2.0f * (q.Y * dq.Z + q.Z * dq.Y - q.W * dq.X - q.X * dq.W);
    m.M33 = -4.0f * (q.X * dq.X + q.Y * dq.Y);
    return m;
}

/// <summary>
/// 位置に関する FK
/// </summary>
/// <param name="effector">エフェクタ関節</param>
/// <param name="joints">関節リスト</param>
/// <param name="pose">姿勢データ</param>
/// <returns>ワールド座標位置ベクトル</returns>
static CBasicVector3 CalcFkPosition( const CJointPtr effector , CJointPtrList &joints, CMatrix4x3Array &pose)
{
	CMatrix4x3 m = effector->ComputeBoneTransForm();
	for (u32 jid = 0; jid < joints.GetNum(); ++jid)
	{
		m *= pose[jid] * joints[jid]->GetOffsetMatrix() * joints[jid]->GetPositionMatrix();
	}
	return Transform( CBasicVector3_ZERO, m);
}

/// <summary>
/// 対数クォータニオンヤコビアン行列の計算
/// </summary>
/// <param name="effector">エフェクタ関節</param>
/// <param name="joints">関節リスト</param>
/// <returns>ヤコビアン行列</returns>
static  CDictionaryDec CreateQuaternionJacobian( const CJointPtr &effector, CJointPtrList &joints)
{
    CDictionaryDec jm;

    CMatrix4x3Array pose;
	pose.Reset( joints.GetNum() );

	for ( u32 jid = 0; jid < joints.GetNum(); ++jid)
    {
		for ( u32 i = 0; i < joints.GetNum(); ++i)
        {
            pose[i].SetupRotate( joints[i]->GetRotation() );
        }

        CQuaternionVector dq;
		dq.Reset( 3 );

        CalcDqDlq( dq[0], dq[1],  dq[2], QuatLn(joints[jid]->GetRotation()));

        for ( u32 i = 0; i < 3; ++i)
        {
            pose[jid] = CalcDrDq(joints[jid]->GetRotation(), dq[i]);
            CBasicVector3 dp = CalcFkPosition(effector, joints, pose);
			jm.Set( dp.X , 3 * jid + i, 0 );
			jm.Set( dp.Y , 3 * jid + i, 1 );
			jm.Set( dp.Z , 3 * jid + i, 2 );
        }
    }
	jm.DebugPrint();
    return jm;
}

/// <summary>
/// 転置行列
/// </summary>
/// <param name="m">行列</param>
/// <returns>転置行列</returns>
static CDictionaryDec Transpose( const CDictionaryDec &m )
{
	CDictionaryDec dest( m.GetColNum(), m.GetRowNum() );
	for ( u32 r = 0; r < m.GetRowNum(); ++r)
    {
		for ( u32 c = 0; c < m.GetColNum(); ++c)
        {
			dest.Get( c, r ) = m.Get( r, c );
        }
    }
    return dest;
}

/// <summary>
/// 行列の乗算 m0 x m1
/// </summary>
/// <param name="m0">行列0</param>
/// <param name="m1">行列1</param>
/// <returns>乗算結果</returns>
static CDictionaryDec Multiply( const CDictionaryDec &m0, const CDictionaryDec &m1)
{
    CDictionaryDec m( m0.GetRowNum(), m1.GetColNum() );

	for ( u32 r = 0; r < m.GetRowNum(); ++r)
    {
		for ( u32 c = 0; c < m.GetColNum(); ++c)
        {
			m.Set( 0 , r , c );
			for ( u32 i = 0; i < m0.GetColNum(); ++i)
            {
				m.Set( m.Get( r , c ) + m0.Get( r , i ) * m1.Get( i, c ) , r, c );
            }
        }
    }
    return m;
}

/// <summary>
/// 3x3行列の逆行列
/// </summary>
/// <param name="m">3x3行列</param>
/// <returns>逆行列</returns>
static CDictionaryDec Inverse3x3( CDictionaryDec &m )
{
    CDictionaryDec inv( 3 , 3 );

    dectype a, b, c, d, e, f, g, h, i;
	a = m.Get(0, 0);
    b = m.Get(0, 1);
    c = m.Get(0, 2);
    d = m.Get(1, 0);
    e = m.Get(1, 1);
    f = m.Get(1, 2);
    g = m.Get(2, 0);
    h = m.Get(2, 1);
    i = m.Get(2, 2);

    inv.Get(0, 0) = e * i - f * h;
    inv.Get(0, 1) = c * h - b * i;
    inv.Get(0, 2) = b * f - c * e;
    inv.Get(1, 0) = f * g - d * i;
    inv.Get(1, 1) = a * i - c * g;
    inv.Get(1, 2) = c * d - a * f;
    inv.Get(2, 0) = d * h - e * g;
    inv.Get(2, 1) = b * g - a * h;
    inv.Get(2, 2) = a * e - b * d;

	for ( u32 i0 = 0; i0 < inv.GetRowNum(); ++i0)
    {
		for ( u32 i1 = 0; i1 < inv.GetColNum(); ++i1)
        {
			inv.Get(i0, i1) *= 1.0f / ((a * e * i + b * f * g + c * d * h) - (a * f * h + b * d * i + c * e * g));
        }
    }

    return inv;
}

/// <summary>
/// 疑似逆行列
/// </summary>
/// <param name="m">行列（行数 > 列数）</param>
/// <returns>疑似逆行列</returns>
static CDictionaryDec PseudoInverse( const CDictionaryDec &m )
{

    CDictionaryDec tmp = Multiply( Transpose(m), m);
    tmp = Inverse3x3( tmp );
    return Multiply(tmp, Transpose(m));
}

/// <summary>
/// 対数クォータニオンの指数
/// </summary>
/// <param name="lnq">対数クォータニオン</param>
/// <returns>クォータニオン</returns>
static CQuaternion QuatExp( const CQuaternion &lnq)
{
    d64 theta = CMathUtil::Sqrt( lnq.X * lnq.X + lnq.Y * lnq.Y + lnq.Z * lnq.Z );
    if (theta == 0)
    {
		CQuaternion q;
		q.Identity();
        return q;
    }

    d64 sinTheta = ( d64 )( CMathUtil::Sin((dectype)theta) / theta );
    CQuaternion q = lnq * (dectype)sinTheta;
    q.W = (dectype)CMathUtil::Cos(( dectype )theta);
	return q.Normalize();
}

void SolveEulerJacobian( const CSkeletonPtr &skeleton , const CJointPtr &effector , const math::CBasicVector3 &targetPos , s32 numMaxIteration, dectype step )
{
    CJointPtrList joints;
    for ( CJointPtr j = effector; j->Parent().IsEmpty() != skyTrue && j->Parent()->Index != INVALID_JOINT_INDEX; j = j->Parent() )
    {
        joints.AddTail( j );
    }

    for ( s32 it = 0 ; it < numMaxIteration ; ++it )
    {
        CBasicVector3 pv = targetPos - skeleton->GetGlobalPosition(effector->Index);
        dectype len = pv.Length();
        if (len < step)
        {
            break;
        }
        pv *= step / len;

		CDictionaryDec dv( 1 , 3 );
		dv.Set( pv.X , 0 , 0 );
		dv.Set( pv.Y , 0 , 1 );
		dv.Set( pv.Z , 0 , 2 );

        dv = Multiply(dv, PseudoInverse(CreateQuaternionJacobian(effector, joints)));

		for (u32 jid = 0; jid < joints.GetNum(); ++jid)
        {
			CQuaternion lnq = QuatLn(joints[jid]->GetRotation());
			lnq += CQuaternion( dv.Get( 0, jid * 3), dv.Get(0, jid * 3 + 1), dv.Get(0, jid * 3 + 2), 0);

            CQuaternion q = QuatExp(lnq);
            if (CMathUtil::Abs(q.W) < 1.0f - 1.0e-6f)
            {
				joints[jid]->GetRotation() = q;
            }
       }
    }
}

}

} } }