#pragma once

namespace sky { namespace lib { namespace math {

class CSkeleton;
typedef SmartPointer< CSkeleton >   CSkeletonPtr;

/// <summary>
/// スケルトンクラス
/// Jointのツリー構造を持つ。
/// </summary>
class CSkeleton : public base::SkyRefObject
{

public:

	/// <summary>
	/// ジョイントツリー
	/// ジョイントの親子関係を保存する
	/// </summary>
	CJointPtr m_spJointRoot;

	/// <summary>
	/// ジョイントパレット
	/// ジョイントを配列状に管理する。シェーダーにウェイトの計算をさせるときなどは配列でジョイントをシェーダーに渡す。
	/// このテーブルの存在意義は、スキニングのためだけ。
	/// </summary>
	CJointPalletPtr m_spJointPallet;

	/// <summary>
	/// リンク系木構造オブジェクトからの関節リスト生成
	/// </summary>
	/// <param name="srcNode">注目ノード</param>
	/// <param name="parentInList">リスト内ノード</param>
	inline void SerializeJoints( const CJointPtr &spNode );

    /// <summary>
    /// 初期化関数
    /// </summary>
    /// <param name="root">多関節リンク系のルート</param>
    inline void Initialize( const CJointPtr &root );

    /// <summary>
    /// 関節のルートローカル座標の取得
    /// </summary>
    /// <param name="jid">関節インデクス</param>
    /// <returns>ルートローカル座標</returns>
    inline CBasicVector3 GetLocalPosition( s32 jid ) const;

    /// <summary>
    /// 関節のグローバル座標の取得
    /// </summary>
    /// <param name="jid">関節インデクス</param>
    /// <returns>グローバル座標</returns>
    inline CBasicVector3 GetGlobalPosition( s32 jid ) const;

    /// <summary>
    /// 関節のグローバルトランスフォームの取得
    /// </summary>
    /// <param name="jid">関節インデクス</param>
    /// <returns>グローバルトランスフォーム行列</returns>
    inline CMatrix4x3 GetGlobalCoordinate( s32 jid ) const;

    /// <summary>
    /// 関節のルートローカルトランスフォームの取得
    /// </summary>
    /// <param name="jid">関節インデクス</param>
    /// <returns>ルートローカルトランスフォーム行列</returns>
    inline CMatrix4x3 GetLocalCoordinate( s32 jid ) const;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CSkeleton( const CJointPtr &spJoint = CJointPtr() );

#ifdef SW_SKYLIB_DEBUG_ON

	/// <summary>
	/// 座標プリント
	/// </summary>
	void PrintPosition();

#endif

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CSkeletonPtr Create( const CJointPtr &spJoint )
	{
		CSkeletonPtr spSkeleton( NEW__( CSkeleton , CSkeleton( spJoint ) ) );
		return spSkeleton;
	}
};

} } }

#define CSkeletonCreate_( root )	          sky::lib::math::CSkeleton::Create( root )

#include "Skeleton.inl"