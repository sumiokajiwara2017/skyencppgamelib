#pragma once

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// D3Dに特化した共通処理群
/// </summary>
namespace D3DUtil
{

	/// <summary>
	/// ４×４マトリックスのＤ３Ｄ用マトリックスに変換する
	/// </summary>
	inline void Convert4x3Matrix( const math::CMatrix4x3& src , XMMATRIX& dest );
	inline void Convert4x3Matrix( const sky::lib::math::CMatrix4x3 &src , XMFLOAT4X4 &dest );

	/// <summary>
	/// Ｄ３Ｄ用マトリックスから４×４に変換する
	/// </summary>
	inline void Convert4x3MatrixRv( const XMMATRIX& src , math::CMatrix4x3& dest );

	/// <summary>
	/// Ｄ３Ｄ用マトリックスから回転に変換する
	/// </summary>
	inline void ConvertRotationMatrixRv( const XMMATRIX& src , math::CRotationMatrix& dest );

	/// <summary>
	/// Ｄ３Ｄ行列をダンプする
	/// </summary>
	inline void PrintD3DMatrix( const XMMATRIX& src );

	/// <summary>
	/// Ｄ３Ｄ行列をダンプする
	/// </summary>
	inline void PrintD3DMatrix( const XMMATRIX& src );

	/// <summary>
	/// CBasicVector3クラスの内容をXMFLOAT3に変換する
	/// </summary>
	inline void ConvertColorVector4( const math::CBasicVector3 &src , XMFLOAT3 &dest );

	/// <summary>
	/// Colorクラスの内容をXMFLOAT4に変換する
	/// </summary>
	inline void ConvertColorVector4( const CColor &src , XMFLOAT4 &dest );

	/// <summary>
	/// 処理結果を判別する
	/// </summary>
	inline void PrintD3D11Result( HRESULT hr );

};

} } }

#endif

#include "D3DUtil.inl"