#include "StdAfx.h"
#include "SkyMathLib/Entity.h"

namespace sky { namespace lib { namespace math {

void CEntityUtil::DefaultInput_Entity2D( const CEntity2DPtr &entity2d , eSkyKeybordKeyKind mainKey , dectype transSpeed , dectype rotSpeed , dectype scSpeed )
{
	//‰ñ“]
	if ( Ckeyboard_->IsKeyOn( mainKey ) && Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_LEFT ) )
	{
		entity2d->AddRot( -rotSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_RIGHT ) )
	{
		entity2d->AddRot( rotSpeed );
	}

	//ˆÚ“®
	if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_LEFT ) )
	{
		entity2d->AddPos_X( -transSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_RIGHT ) )
	{
		entity2d->AddPos_X( transSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_UP ) )
	{
		entity2d->AddPos_Y( transSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
	{
		entity2d->AddPos_Y( -transSpeed );
	}

	//Šg‘åk¬
	if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_LEFT ) )
	{
		entity2d->AddSc_X( -scSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_RIGHT ) )
	{
		entity2d->AddSc_X( scSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_UP ) )
	{
		entity2d->AddSc_Y( scSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
	{
		entity2d->AddSc_Y( -scSpeed );
	}

	//ƒŠƒZƒbƒg
	if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyPress( KEY_HOME ) )
	{
		entity2d->Reset();
	}
}

void CEntityUtil::DefaultInput_Entity3D( const CEntity3DPtr &entity3d , eSkyKeybordKeyKind mainKey , dectype transSpeed , dectype rotSpeed , dectype scSpeed )
{
	//‰ñ“]
	if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_PAGE_UP ) )
	{
		entity3d->AddRot_Z( -rotSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_PAGE_DOWN ) )
	{
		entity3d->AddRot_Z( rotSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_LEFT ) )
	{
		entity3d->AddRot_Y( -rotSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_RIGHT ) )
	{
		entity3d->AddRot_Y( rotSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_UP ) )
	{
		entity3d->AddRot_X( -rotSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
	{
		entity3d->AddRot_X( rotSpeed );
	}

	//ˆÚ“®
	if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_PAGE_UP ) )
	{
		entity3d->AddPos_Z( transSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_PAGE_DOWN ) )
	{
		entity3d->AddPos_Z( -transSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_LEFT ) )
	{
		entity3d->AddPos_X( -transSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_RIGHT ) )
	{
		entity3d->AddPos_X( transSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_UP ) )
	{
		entity3d->AddPos_Y( transSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
	{
		entity3d->AddPos_Y( -transSpeed );
	}

	//Šg‘åk¬
	if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_PAGE_UP ) )
	{
		entity3d->AddSc_Z( scSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_PAGE_DOWN ) )
	{
		entity3d->AddSc_Z( -scSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_LEFT ) )
	{
		entity3d->AddSc_X( -scSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_RIGHT ) )
	{
		entity3d->AddSc_X( scSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_UP ) )
	{
		entity3d->AddSc_Y( scSpeed );
	}
	else if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyOn( KEY_S ) && Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
	{
		entity3d->AddSc_Y( -scSpeed );
	}

	//ƒŠƒZƒbƒg
	if ( Ckeyboard_->IsKeyOn( mainKey ) &&  Ckeyboard_->IsKeyPress( KEY_HOME ) )
	{
		entity3d->Reset();
	}
}

} } }
