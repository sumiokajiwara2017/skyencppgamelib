#pragma once

namespace sky { namespace lib { namespace file { 

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

/// <summary>
/// �t�@�C�����N���X�i�����͂قƂ�Ǌe�����j
/// </summary>
class IFileInfo : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	u32		fileSize;

};
typedef SmartPointer< IFileInfo >	IFileInfoPtr;

} } }