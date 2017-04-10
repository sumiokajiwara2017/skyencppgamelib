# pragma once


namespace sky { namespace lib { namespace math {

/// <summary>
/// 定数
/// <summary>
namespace matrix3x2stack_const
{
	static const u32 STACK_SIZE	= 32;
}

typedef CStack< CMatrix3x2 >	CMatrix3x2Stack_;

/// <summary>
/// ４ｘ４行列スタック
/// </summary>
class CMatrix3x2Stack : base::SkyObject
{

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
	CMatrix3x2Stack				( u32 size = matrix3x2stack_const::STACK_SIZE );

    /// <summary>
    /// デストラクタ
    /// </summary>
	virtual ~CMatrix3x2Stack		();

    /// <summary>
    /// 格納されている数を返す
    /// </summary>
	s32 GetNum				(){ return m_Stack.GetNum(); };

    /// <summary>
    /// 行列の一番上を返す
    /// </summary>
	CMatrix3x2 &GetTop			();

    /// <summary>
    /// 一番上の行列を単位行列にする
    /// </summary>
	void LoadIdentity			();

    /// <summary>
    /// 一番上の行列を指定の行列にする
    /// </summary>
	void LoadMatrix				( const CMatrix3x2 &mat );

    /// <summary>
    /// スタックの一番上に行列を追加する
    /// </summary>
	void Push					();

    /// <summary>
    /// スタックの一番上の行列を削除する
    /// </summary>
	void Pop					();

    /// <summary>
    /// stacktop = stacktop * mat　の計算を行う。
    /// </summary>
	void MultMatrix				( const CMatrix3x2 &mat );

    /// <summary>
    /// stacktop = mat * stacktop の計算を行う。( matが子のLoack行列、stackが親のワールド行列、という場合の子のワールド座標をstackに算出する等に使う）
    /// </summary>
	void MultMatrixLocal		( const CMatrix3x2 &mat );

    /// <summary>
    /// stacktop = stacktop - 1 * matの計算を行う。
    /// </summary>
	void LoadAndMultBeforeMatrix( const CMatrix3x2 &mat );

    /// <summary>
    /// stacktop = mat * stacktop - 1の計算を行う。
    /// </summary>
	void LoadAndMultLocalBeforeMatrix( const CMatrix3x2 &mat );

private:

    /// <summary>
    /// スタック
    /// </summary>
	CMatrix3x2Stack_				m_Stack;

};

} } }