#pragma once

namespace sky { namespace lib { namespace stream {

/// <summary>
/// �X�g���[���̎��
/// </summary>
enum eStreamKind
{
	eStreamKind_memory = 0, //��������̒l���X�g���[���Ƃ��Ă݂Ă�i�قƂ�ǂ��ꂩ���j
	eStreamKind_file,
	eStreamKind_net,
	eStreamKind_console,
};

} } }