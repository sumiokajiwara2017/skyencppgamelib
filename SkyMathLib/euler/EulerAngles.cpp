#include "StdAfx.h"
#include "SkyMathLib/Euler.h"

const sky::lib::math::CEulerAngles skyCEulerAnglesIdentity(0.0f , 0.0f , 0.0f);

namespace sky { namespace lib { namespace math {

void	CEulerAngles::Canonize()
{
    //ピッチ範囲を-180度～180度にラップ(-180度～180度範囲表現に変換）する

	Pitch = CMathUtil::WrapPi( Pitch );

    //正準範囲 -90度...90度の外側で行列pitchの裏側をチェックする
    //ピッチ < 90度

	if (Pitch < -util::KPIOVER2)
	{
		Pitch = -util::KPI - Pitch;		//-180度 - ピッチ
		Heading += util::KPI;				//ヘディング += 180度
		Bank += util::KPI;				//バンク += 180度
	}
	else if (Pitch > util::KPIOVER2)
	{
		Pitch = util::KPI - Pitch;		//180度 - ピッチ
		Heading += util::KPI;				//ヘディング += 180度
		Bank += util::KPI;				//バンク
	}

     //ピッチ > 90度以上

	// ここでジンバルロックのケースをチェックする（許容範囲を少し(1e-4)持つ）
	if ( CMathUtil::FAbs( Pitch ) > util::KPIOVER2 - 1e-4 )
	{

        //ジンバルロックしている
        //垂直軸に関する全ての回転をヘディングに割り当てる（Z軸回転をX軸回転に割り当てる）
		Heading += Bank;
		Bank = 0.0f;

	} else {

        //ジンバルロックにない
        //バンク角を正準範囲にラップ(-180度～180度範囲表現に変換）する
		Bank = CMathUtil::WrapPi(Bank);
	}

	//ヘディングを正準範囲にラップ(-180度～180度範囲表現に変換）する
	Heading = CMathUtil::WrapPi(Heading);
}

void	CEulerAngles::FromObjectToInertialQuaternion(const CQuaternion &q) {

    //sin(pitch)を取り出す。

	dectype sp = -2.0f * (q.Y*q.Z - q.W*q.X);

    //ジンバルロックチェックをする
    //不正確さのため許容範囲を少し与える
	if (CMathUtil::FAbs(sp) > 0.9999f) {

        //真上か真下を向いている
		Pitch = util::KPIOVER2 * sp;

	    //ヘディングを計算し、バンクを０に設定する
		Heading = CMathUtil::Atan2(-q.X*q.Z + q.W*q.Y, 0.5f - q.Y*q.Y - q.Z*q.Z);
		Bank = 0.0f;

	} else {

        //角度を計算する
        //完全なasin関数を用いる必要はない
        //ジンバルロックチェック時に範囲をすでにチェックしているからである。

		Pitch	= CMathUtil::Asin(sp);
		Heading	= CMathUtil::Atan2(q.X*q.Z + q.W*q.Y, 0.5f - q.X*q.X - q.Y*q.Y);
		Bank	= CMathUtil::Atan2(q.X*q.Y + q.W*q.Z, 0.5f - q.X*q.X - q.Z*q.Z);
	}
}

void	CEulerAngles::FromInertialToObjectQuaternion(const CQuaternion &q) {

    //sin(pitch)を取り出す。
	dectype sp = -2.0f * (q.Y*q.Z + q.W*q.X);

    //ジンバルロックをチェックする
    //数字の不正確さのためにわずかな許容範囲を与える
	if (CMathUtil::FAbs(sp) > 0.9999f) {

        //真下を向いている
		Pitch = util::KPIOVER2 * sp;

        //ヘディングを計算し、バンクを0に設定する
		Heading = CMathUtil::Atan2(-q.X*q.Z - q.W*q.Y, 0.5f - q.Y*q.Y - q.Z*q.Z);
		Bank = 0.0f;

	} else {

        //角度を計算する
        //安全なasin関数を用いる必要はない
        //ジンバルロックチェック時に葉にをすでにチェックしているからである。

		Pitch	= CMathUtil::Asin(sp);
		Heading	= CMathUtil::Atan2(q.X*q.Z - q.W*q.Y, 0.5f - q.X*q.X - q.Y*q.Y);
		Bank	= CMathUtil::Atan2(q.X*q.Y - q.W*q.Z, 0.5f - q.X*q.X - q.Z*q.Z);
	}
}

void	CEulerAngles::FromObjectToWorldMatrix(const CMatrix4x3 &m) {

    //m32 から sin(pitch)を取り出す。

	dectype	sp = -m.M32;

    //ジンバルロックをチェックする
	
	if (CMathUtil::FAbs(sp) > 9.99999f) {

        //ジンバルロックをチェックする

		Pitch = util::KPIOVER2 * sp;

        // 真上か真下を向いている

		Heading = CMathUtil::Atan2(-m.M23, m.M11);
		Bank = 0.0f;

	}
	else
	{
        //角度を計算する
        //安全なasin関数を用いる必要はない
        //ジンバルロックチェック時に範囲をすでにチェックしているからである
 
		Heading = CMathUtil::Atan2(m.M31, m.M33);
		Pitch = CMathUtil::Asin(sp);
		Bank = CMathUtil::Atan2(m.M12, m.M22);
	}
}

void	CEulerAngles::FromWorldToObjectMatrix(const CMatrix4x3 &m) {

    //m23からsin(pitch)を取り出す。
	dectype	sp = -m.M23;

    //ジンバルロックをチェックする
	if (CMathUtil::FAbs(sp) > 9.99999f) {

        //真上か真下を向いている

		Pitch = util::KPIOVER2 * sp;

        //ヘディングを計算し、バンクを0に設定する

		Heading = CMathUtil::Atan2(-m.M31, m.M11);
		Bank = 0.0f;

	} else {

        //角度を計算する
        //安全なasin関数を用いる必要はない
        //ジンバルロックチェック時に範囲をすでにチェックしているからである

		Heading = CMathUtil::Atan2(m.M13, m.M33);
		Pitch = CMathUtil::Asin(sp);
		Bank = CMathUtil::Atan2(m.M21, m.M22);
	}
}

void	CEulerAngles::FromRotationMatrix(const CRotationMatrix &m) {

    //m23からsin(pitch)を取り出す
	dectype	sp = -m.M23;

    //ジンバルロックをチェックする
	if (CMathUtil::FAbs(sp) > 9.99999f) {

        //真上か真下を向いている
		Pitch = util::KPIOVER2 * sp;

        //ヘディングを計算し、バンクを0に設定する
		Heading = CMathUtil::Atan2(-m.M31, m.M11);
		Bank = 0.0f;

	} else {

        //角度を計算する
        //安全なasin関数を用いる必要はない
        //ジンバルロックチェック時に範囲をすでにチェックしているからである

		Heading = CMathUtil::Atan2(m.M13, m.M33);
		Pitch = CMathUtil::Asin(sp);
		Bank = CMathUtil::Atan2(m.M21, m.M22);
	}
}

} } }