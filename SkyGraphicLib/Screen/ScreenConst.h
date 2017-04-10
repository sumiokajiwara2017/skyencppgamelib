#pragma once

namespace sky { namespace lib { namespace graphic { namespace screen {

/// <summary>
/// screen�@�\���ʒ萔
/// </summary>
static const dectype VIRTUAL_SCREEN_W   = 1280.0;	                             //���z�𑜓x�̕�
static const dectype VIRTUAL_SCREEN_H   = 720.0;	                             //���z�𑜓x�̍���
const static dectype SCREEN_ASPECT_RATE = VIRTUAL_SCREEN_W / VIRTUAL_SCREEN_H;	 //���z�𑜓x�̃A�X�y�N�g��
static const u32     LAYER_STACK_NUM    = 8;

/// <summary>
/// �X�N���[���n�b�V���T�C�Y
/// </summary>
static const u32    HASH_SIZE = 19;

/// <summary>
/// �X�N���[���������
/// </summary>
enum eSplit
{
	eSplit_LR = 0 ,
	eSplit_TB ,
	eSplit_None 
};

} } } }