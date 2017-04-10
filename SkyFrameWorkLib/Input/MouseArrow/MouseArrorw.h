#pragma once

namespace sky { namespace lib { namespace framework {

class CMouseArrow;
typedef SmartPointer< CMouseArrow >	CMouseArrowPtr;				//�X�}�[�g�|�C���^�Œ�`

/// <summary>
/// �}�E�X��󃂃f��
/// ���̃N���X�͕`��̂ݒS�����܂��B
/// �}�E�X�̊e��C�x���g��CMouse�N���X���g�p���Ď擾���Ă��������B
/// </summary>
class CMouseArrow : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CMouseArrow( wndType windowId , dectype prio = graphic::RENDERING_2D_PRIO_FAST  );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CMouseArrow( void );

	/// <summary>
	/// ���摜�`��X�v���C�g
	/// </summary>
	graphic::CSpritePtr m_spRootSprite;

private:

	/// <summary>
	/// �������������Ă���E�B���h�E�̎���ID
	/// </summary>
	wndType m_WindowId;

	/// <summary>
	/// �}�E�X�ړ��C�x���g�f���Q�[�g
	/// </summary>
	delegate::IDelegateObjectPtr m_spMouseEventDelegate;
	void MouseEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args );
};

} } }