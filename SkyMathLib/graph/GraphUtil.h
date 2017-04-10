#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �l�X�ȃO���t��Y�l��X�l���w�肷�邱�ƂŎ擾����
/// X�͈̔͂Ɛ��x���w�肷�邱�ƂŃO���t���v���b�g�����_�W����Ԃ�
/// </summary>
namespace CGraphUtil
{
	/// <summary>
	/// ���� y = ax + b�ɂ����āAx�͈̔͂��w�肵 , a , b�̒l���w�肵�āA0.0f�`1.0f�͈͓���now���w�肷�邱�Ƃł��̈ʒu�ł�x��y��Ԃ��B
	/// </summary>
	inline CBasicVector3 &GetStraightLinePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a = 1 , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// �����̃v���b�g�l��Ԃ��Baccuracy�l���Ⴏ��ΒႢ�قǃO���t�̐��x�������B
	/// </summary>
	inline CBasicVector3Vector &DotsStraightLine( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy = 0.1f , dectype a = 1 , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 2���Ȑ� y = ax~2 + b�ɂ����āAx�͈̔͂��w�肵 , a , b�̒l���w�肵�āA0.0f�`1.0f�͈͓���now���w�肷�邱�Ƃł��̈ʒu�ł�x��y��Ԃ��B
	/// </summary>
	inline CBasicVector3 &GetQuadraticCurvePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a = 1 , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 2���Ȑ��̃v���b�g�l��Ԃ��Baccuracy�l���Ⴏ��ΒႢ�قǃO���t�̐��x�������B
	/// </summary>
	inline CBasicVector3Vector &DotsQuadraticCurve( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy = 0.001f , dectype a = -0.1f , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 3���Ȑ� y = ax~3 + b�ɂ����āAx�͈̔͂��w�肵 , a , b�̒l���w�肵�āA0.0f�`1.0f�͈͓���now���w�肷�邱�Ƃł��̈ʒu�ł�x��y��Ԃ��B
	/// </summary>
	inline CBasicVector3 &GetCubicCurvePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a = 1 , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 3���Ȑ��̃v���b�g�l��Ԃ��Baccuracy�l���Ⴏ��ΒႢ�قǃO���t�̐��x�������B
	/// </summary>
	inline CBasicVector3Vector &DotsCubicCurveCurve( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy = 0.001f , dectype a = -0.001f , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// �~ x = r * cos�� , y = r * sin�� r=�~�̔��a�ɂ����Ĕ��ar�A�p�x�Ɓi���W�A���j�͈̔͂��w�肵��0.0f�`1.0f�͈͓���now���w�肷�邱�Ƃł��̈ʒu�ł�x��y��Ԃ��B
	/// </summary>
	inline CBasicVector3 &GetCirclePoint( CBasicVector3 &dest , dectype r , dectype now , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// �~�̃v���b�g�l��Ԃ��Baccuracy�l���Ⴏ��ΒႢ�قǃO���t�̐��x�������B
	/// </summary>
	inline CBasicVector3Vector &DotsCircle( CBasicVector3Vector &dest , dectype r , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

	/// <summary>
	/// �ȉ~ x = ar * cos�� , y = br sin�� ar=x�����a br=y�����a �p�x�Ɓi���W�A���j�͈̔͂��w�肵��0.0f�`1.0f�͈͓���now���w�肷�邱�Ƃł��̈ʒu�ł�x��y��Ԃ��B
	/// </summary>
	inline CBasicVector3 &GetEllipticalPoint( CBasicVector3 &dest , dectype ar , dectype br , dectype now , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// �ȉ~�̃v���b�g�l��Ԃ��Baccuracy�l���Ⴏ��ΒႢ�قǃO���t�̐��x�������B
	/// </summary>
	inline CBasicVector3Vector &DotsElliptical( CBasicVector3Vector &dest , dectype ar , dectype br , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

	/// <summary>
	/// �o�Ȑ� x = a / cos�� y = b * tan�� �p�x�Ɓi���W�A���j�͈̔͂��w�肵��0.0f�`1.0f�͈͓���now���w�肷�邱�Ƃł��̈ʒu�ł�x��y��Ԃ��B
	/// </summary>
	inline CBasicVector3 &GetHyperbolaPoint( CBasicVector3 &dest , dectype a , dectype b , dectype now , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// �o�Ȑ��̃v���b�g�l��Ԃ��Baccuracy�l���Ⴏ��ΒႢ�قǃO���t�̐��x�������B
	/// </summary>
	inline CBasicVector3Vector &DotsHyperbola( CBasicVector3Vector &dest , dectype a , dectype b , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

	/// <summary>
	/// �g���R�C�h�Ȑ� x = rm * �� - rd * sin(��) , y = rm - rd * cos(��) ���~���arm���w�肵�A�`��_���ard���w�肵�A�p�x�Ɓi���W�A���j�͈̔͂��w�肵��0.0f�`1.0f�͈͓���now���w�肷�邱�Ƃł��̈ʒu�ł�x��y��Ԃ��B
	/// rm < rd�̂Ƃ��A1��̉�]��x����2������B
	/// rm = rd�̂Ƃ��A1��̉�]��x����1��ڂ��A�Ȑ��̓T�C�N���C�h�ƂȂ�B
	/// rm > rd�̂Ƃ��Ax���ƌ����Ȃ��B
	/// </summary>
	inline CBasicVector3 &GetTrochoidPoint( CBasicVector3 &dest , dectype rm , dectype rd , dectype now , dectype beginRad = math::CMathUtil::DegToRad( -360 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// �g���R�C�h�Ȑ��̃v���b�g�l��Ԃ��Baccuracy�l���Ⴏ��ΒႢ�قǃO���t�̐��x�������B
	/// </summary>
	inline CBasicVector3Vector &DotsTrochoid( CBasicVector3Vector &dest , dectype rm , dectype rd , dectype beginRad = math::CMathUtil::DegToRad( -360 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

	/// <summary>
	/// �N���\�C�h�Ȑ� x = r * cos( ��~2 / 2 ) * �� , y = r * sin( ��~2 / 2 ) * �� ���ar���w�肵�A�p�x�Ɓi���W�A���j�͈̔͂��w�肵��0.0f�`1.0f�͈͓���now���w�肷�邱�Ƃł��̈ʒu�ł�x��y��Ԃ��B
	/// </summary>
	inline CBasicVector3 &GetClothoidPoint( CBasicVector3 &dest , dectype r , dectype now , dectype beginRad = math::CMathUtil::DegToRad( -360 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// �N���\�C�h�Ȑ��̃v���b�g�l��Ԃ��Baccuracy�l���Ⴏ��ΒႢ�قǃO���t�̐��x�������B
	/// </summary>
	inline CBasicVector3Vector &DotsClothoid( CBasicVector3Vector &dest , dectype r , dectype beginRad = math::CMathUtil::DegToRad( -360 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

//-----�O���t�Ƃ͊֌W�Ȃ��ł����֗��֐�

	/// <summary>
	/// �ɍ��W(r,��)�𒼌����W(x,y)�֕ϊ�����B���ar �p�x�Ƃ�n���A���Wx,y�𓾂�B
	/// </summary>
	inline void ConvertFromPolarCoordinatesToRectangularCoordinate( CBasicVector2 &dest , const CBasicVector2 &src );

	/// <summary>
	/// �������W(x,y)���ɍ��W(r,��)�֕ϊ�����B���Wx,y��n���A���ar �p�x�Ƃ𓾂�B�i�����v���𐳂Ƃ���j
	/// �����̊p�x�Ȃǂ����߂�̂ɕ֗�
	/// </summary>
	inline void ConvertFromRectangularCoordinateToPolarCoordinates( CBasicVector2 &dest , const CBasicVector2 &src );

	/// <summary>
	/// �ɍ��W (r,��,��)�𒼌����W (x,y,z) �֕ϊ�����B���ar�AXY���ʉ�]�p�x�ƁA�p�ӂ�n���A���Wx,y,z�𓾂�(�ӂ̈�������ʓI�ȋɍ��W�Ƃ͈قȂ�j
	/// </summary>
	inline void ConvertFromPolarCoordinatesToRectangularCoordinate( CBasicVector3 &dest , const CBasicVector3 &src );

	/// <summary>
	/// �������W (x,y,z)���ɍ��W (r,��,��)�� �֕ϊ�����B(�ӂ̈�������ʓI�ȋɍ��W�Ƃ͈قȂ�j
	/// </summary>
	inline void ConvertFromRectangularCoordinateToPolarCoordinates( CBasicVector3 &dest , const CBasicVector3 &src );

};

} } }

#include "GraphUtil.inl"