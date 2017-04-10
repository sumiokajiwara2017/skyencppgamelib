# pragma once


namespace sky { namespace lib { namespace math {

/// <summary>
/// 定数
/// <summary>
namespace matrix4x3stack_const
{
	static const u32 STACK_SIZE	= 32;
}

typedef CStack< CMatrix4x3 >	CMatrix4x3Stack_;

/// <summary>
/// ４ｘ４行列スタック
/// </summary>
class CMatrix4x3Stack : base::SkyObject
{

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
	inline CMatrix4x3Stack				( u32 size = matrix4x3stack_const::STACK_SIZE );

    /// <summary>
    /// デストラクタ
    /// </summary>
	inline virtual ~CMatrix4x3Stack		();

    /// <summary>
    /// 格納されている数を返す
    /// </summary>
	inline s32 GetNum				(){ return m_Stack.GetNum(); };

    /// <summary>
    /// 行列の一番上を返す
    /// </summary>
	inline CMatrix4x3 &GetTop			();

    /// <summary>
    /// 一番上の行列を単位行列にする
    /// </summary>
	inline void LoadIdentity			();

    /// <summary>
    /// 一番上の行列を指定の行列にする
    /// </summary>
	inline void LoadMatrix				( const CMatrix4x3 &mat );

    /// <summary>
    /// スタックの一番上に行列を追加する
    /// </summary>
	inline void Push					();

    /// <summary>
    /// スタックの一番上の行列を削除する
    /// </summary>
	inline void Pop					();

    /// <summary>
    /// stacktop = stacktop * mat　の計算を行う。
    /// </summary>
	inline void MultMatrix				( const CMatrix4x3 &mat );

    /// <summary>
    /// stacktop = mat * stacktop の計算を行う。( matが子のLoack行列、stackが親のワールド行列、という場合の子のワールド座標をstackに算出する等に使う）
    /// </summary>
	inline void MultMatrixLocal		( const CMatrix4x3 &mat );

    /// <summary>
    /// stacktop = stacktop - 1 * matの計算を行う。
    /// </summary>
	inline void LoadAndMultBeforeMatrix( const CMatrix4x3 &mat );

    /// <summary>
    /// stacktop = mat * stacktop - 1の計算を行う。
    /// </summary>
	inline void LoadAndMultLocalBeforeMatrix( const CMatrix4x3 &mat );

    /// <summary>
    /// stacktop = mat * stacktop - 1の計算を行う。
    /// </summary>
	inline void LoadAndMultLocalBeforeMatrixTransOnly( const CMatrix4x3 &mat );

private:

    /// <summary>
    /// スタック
    /// </summary>
	CMatrix4x3Stack_				m_Stack;

};

} } }

#include "Matrix4x3Stack.inl"