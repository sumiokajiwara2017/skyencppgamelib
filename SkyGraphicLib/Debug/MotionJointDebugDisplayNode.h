#pragma once

#ifdef SW_SKYLIB_DEBUG_ON

namespace sky { namespace lib { namespace graphic {

class CMotionJointDebugDisplayNode;
typedef SmartPointer< CMotionJointDebugDisplayNode >	CMotionJointDebugDisplayNodePtr;			//スマートポインタ版定義

/// <summary>
/// モーションのデバッグ表示クラス。
/// </summary>
class CMotionJointDebugDisplayNode : public CSceneGraphNode
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// シーングラフノードとしての動作
	/// </summary>
	virtual void   vRendering	   ( CRenderingParam &param );

	/// <summary>
	/// 描画オン
	/// </summary>
	void SetDrawOn( skyBool isOn = skyTrue );

	/// <summary>
	/// 基本スケール設定
	/// </summary>
	void SetBaseScale( const math::CBasicVector3 &scale );

	/// <summary>
	/// 基本回転設定
	/// </summary>
	void SetBaseRot( math::eAxis axis , dectype theta );

	/// <summary>
	/// 代入演算子オーバーライド
	/// </summary>
	virtual CMotionJointDebugDisplayNode& operator =( const CMotionJointDebugDisplayNode& src )
	{
		SKY_UNUSED_ARG( src );

		//このクラスは代入できん
		SKY_ASSERT( skyFalse );
        return *this;
	}

//---Static

	/// <summary>
	/// 作成
	/// </summary>
	static CMotionJointDebugDisplayNodePtr Create( const math::CMotionController &pMotionController )
	{
		return CMotionJointDebugDisplayNodePtr( NEW CMotionJointDebugDisplayNode( pMotionController ) );
	}

private:

	/// <summary>
	/// 描画フラグ
	/// </summary>
	skyBool m_IsDrawOn;

	/// <summary>
	/// 基本スケール
	/// </summary>
	math::CBasicVector3 m_BaseScale;

	/// <summary>
	/// 基本スケール
	/// </summary>
	math::CRotationMatrix m_BaseRot;

	/// <summary>
	/// モデルツリールート
	/// </summary>
	CSceneGraphNodePtr m_spModelTreeRoot;

	/// <summary>
	/// モーションコントローラーの参照
	/// </summary>
	const math::CMotionController &m_pMotionController;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CMotionJointDebugDisplayNode( const math::CMotionController &m_pMotionController );

	/// <summary>
	/// モデルツリーの作成
	/// </summary>
	static void CreateTreeModelFromJointTree( const math::CJointPtr &spJoint , const CSceneGraphNodePtr &spModelTree , const math::CBasicVector3 &basicScale , const math::CRotationMatrix &rot );

	/// <summary>
	/// モデルツリーにジョイントの計算結果を設定していく。
	/// </summary>
	static void SetTreeModelFromJointTree( const math::CMotionController &pMotionController , const CSceneGraphNodePtr &spModelTree );
};

} } }

#define CMotionJointDebugDisplayNodeCreate_( controller )  sky::lib::graphic::CMotionJointDebugDisplayNode::Create( controller );

#endif