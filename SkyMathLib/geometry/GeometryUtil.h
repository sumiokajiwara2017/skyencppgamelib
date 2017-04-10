#pragma once

namespace sky { namespace lib { namespace math {


/// <summary>
/// �􉽊w�֗������Q
/// </summary>
namespace GeometryUtil {

/// <summary>
/// ���S���W�̍��W�����㌴�_���W�ɕϊ�����i�n���ꂽ�Q�c���W���l�p�`�|���S���̒��S���W���Ɖ��肵���l��Ԃ��j
/// </summary>
inline void ConvertCPosToLTPos( CBasicVector2 &dest , const CBasicVector2 &src , const CBasicSize &srcScreenSize ); 

/// <summary>
/// ���S���W�̍��W�����㌴�_���W�ɕϊ�����i�n���ꂽ�Q�c���W���l�p�`�|���S���̍��㒸�_���W���Ɖ��肵���l��Ԃ��j
/// ��L�֐��Ƃ̈Ⴂ�́A�n���ꂽ���W�𒆐S�Ƃ�����`�̍���p�����㌴�_�ɍ��킹�邱�ƁB
/// </summary>
inline void ConvertCPosToLTPos(  CBasicVector2 &dest , const CBasicVector2 &src , const CBasicSize &srcPolSize , const CBasicSize &srcScreenSize ); 

/// <summary>
/// �w��̋�Ԕ͈͓��ő��݂��镨�̂����[�v����悤�ɍ��W��ϊ�����
/// ���Ɍ��E�܂Ői�񂾂�E�ɏo��B��Ɍ��E�܂Ői�񂾂牺�ɏo��悤�ȍ��W�ϊ�
/// </summary>
inline CBasicVector3 ConvertRoopPos( const CBasicVector3 &max , const CBasicVector3 &min , const CBasicVector3 &pos );

/// <summary>
/// �Q�c�x�N�g����ʏ���W���瓯�����W�ɕϊ�����
/// </summary>
inline CBasicVector3 ConvertToHomogeneous( const CBasicVector2 &src );

/// <summary>
/// �Q�c�x�N�g���𓯎����W����ʏ���W�ɕϊ�����
/// </summary>
inline CBasicVector2 ConvertFromHomogeneous( const CBasicVector3 &src );

/// <summary>
/// �R�c�x�N�g����ʏ���W���瓯�����W�ɕϊ�����
/// </summary>
inline CBasicVector4 ConvertToHomogeneous( const CBasicVector3 &src );

/// <summary>
/// �R�c�x�N�g���𓯎����W����ʏ���W�ɕϊ�����
/// </summary>
inline CBasicVector3 ConvertFromHomogeneous( const CBasicVector4 &src );

}

} } }

#include "GeometryUtil.inl"
