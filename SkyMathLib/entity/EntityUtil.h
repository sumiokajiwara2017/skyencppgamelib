#pragma once

namespace sky { namespace lib { namespace math {

namespace camera {
	static const f32 DEFAULT_TRANS_SPEED = 1.0f;
	static const f32 DEFAULT_ROT_SPEED   = 2.0f;
	static const f32 DEFAULT_SC_SPEED    = 0.5f;
}

/// <summary>
/// エンティティ便利関数群
/// </summary>
class CEntityUtil
{

public:

	/// <summary>
	/// エンティティキーボード操作基本入力受付（デバッグとかに使える）
	/// </summary>
	static void DefaultInput_Entity2D( const CEntity2DPtr &entity2d , eSkyKeybordKeyKind mainKey , dectype transSpeed = camera::DEFAULT_TRANS_SPEED , dectype rotSpeed = camera::DEFAULT_ROT_SPEED , dectype scSpeed = camera::DEFAULT_SC_SPEED );

	/// <summary>
	/// エンティティキーボード操作基本入力受付（デバッグとかに使える）
	/// </summary>
	static void DefaultInput_Entity3D( const CEntity3DPtr &entity3d , eSkyKeybordKeyKind mainKey , dectype transSpeed = camera::DEFAULT_TRANS_SPEED , dectype rotSpeed = camera::DEFAULT_ROT_SPEED , dectype scSpeed = camera::DEFAULT_SC_SPEED );
};

} } }