#pragma once

namespace sky { namespace lib { namespace graphic {

namespace font {

//�t�H���g�̎��
enum eFontStyleType
{
	eFontStyleType_Gosic11 = 0 ,
	eFontStyleType_Gosic13 ,
//	eFontStyleType_Gosic16 ,
//	eFontStyleType_Mincho11 ,
//	eFontStyleType_Mincho13 ,
//	eFontStyleType_Mincho16 ,
};

static const u32        STYLE_HASH_SIZE   = 11; //�X�^�C���̎�ނ�肿����Ə�̒l�̑f���ɂ���
static const u32        FONT_HASH_SIZE    = 691;//�P�X�^�C�����̕�������肿����Ə�̒l�̑f���ɂ���

}

} } }