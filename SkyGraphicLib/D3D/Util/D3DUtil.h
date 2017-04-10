#pragma once

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// D3D�ɓ����������ʏ����Q
/// </summary>
namespace D3DUtil
{

	/// <summary>
	/// �S�~�S�}�g���b�N�X�̂c�R�c�p�}�g���b�N�X�ɕϊ�����
	/// </summary>
	inline void Convert4x3Matrix( const math::CMatrix4x3& src , XMMATRIX& dest );
	inline void Convert4x3Matrix( const sky::lib::math::CMatrix4x3 &src , XMFLOAT4X4 &dest );

	/// <summary>
	/// �c�R�c�p�}�g���b�N�X����S�~�S�ɕϊ�����
	/// </summary>
	inline void Convert4x3MatrixRv( const XMMATRIX& src , math::CMatrix4x3& dest );

	/// <summary>
	/// �c�R�c�p�}�g���b�N�X�����]�ɕϊ�����
	/// </summary>
	inline void ConvertRotationMatrixRv( const XMMATRIX& src , math::CRotationMatrix& dest );

	/// <summary>
	/// �c�R�c�s����_���v����
	/// </summary>
	inline void PrintD3DMatrix( const XMMATRIX& src );

	/// <summary>
	/// �c�R�c�s����_���v����
	/// </summary>
	inline void PrintD3DMatrix( const XMMATRIX& src );

	/// <summary>
	/// CBasicVector3�N���X�̓��e��XMFLOAT3�ɕϊ�����
	/// </summary>
	inline void ConvertColorVector4( const math::CBasicVector3 &src , XMFLOAT3 &dest );

	/// <summary>
	/// Color�N���X�̓��e��XMFLOAT4�ɕϊ�����
	/// </summary>
	inline void ConvertColorVector4( const CColor &src , XMFLOAT4 &dest );

	/// <summary>
	/// �������ʂ𔻕ʂ���
	/// </summary>
	inline void PrintD3D11Result( HRESULT hr );

};

} } }

#endif

#include "D3DUtil.inl"