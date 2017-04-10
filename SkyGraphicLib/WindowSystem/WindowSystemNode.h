#pragma once

namespace sky { namespace lib { namespace graphic {

static const s32 WINDOW_SYSTEM_INVALID_NODE_ID = -1;
static const dectype WINDOW_SYSTEM_DEFAULT_NODE_WIDTH = 100;
static const dectype WINDOW_SYSTEM_DEFAULT_NODE_HEIGHT = 30;

class CWindowSystemNode;

typedef SmartPointer< CWindowSystemNode >   CWindowSystemNodePtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CWindowSystemNodePtr > CWindowSystemNodePtrList;	//���X�g��`

/// <summary>
/// Window�m�[�h
/// </summary>
class CWindowSystemNode : public CSprite
{

friend class CWindowSystemManager;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CWindowSystemNode();

    /// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
    /// </summary>
	virtual void vRendering	( CRenderingParam &param );
		
	/// <summary>
	/// �����\���v�f�Ƃ��ăZ�b�g�A�b�v
	/// </summary>
	void SetUpFontDispMode( const skyString *headerfont );

	/// <summary>
	/// ������ݒ�
	/// </summary>
	void SetFont( const skyString *font );

	/// <summary>
	/// �����A�h���X�ƃo�C���f�B���O
	/// </summary>
	void BindFontAddress( const skyString *font );

	/// <summary>
	/// f32�\���v�f�Ƃ��ăZ�b�g�A�b�v
	/// </summary>
	void SetUpf32DispMode( const skyString *headerfont , f32 initValue );

	/// <summary>
	/// f32�l�ݒ�
	/// </summary>
	void Setf32Value( f32 value );

	/// <summary>
	/// f32�A�h���X�ƃo�C���f�B���O
	/// </summary>
	void Bindf32Address( f32 *pValue );

	/// <summary>
	/// s32�\���v�f�Ƃ��ăZ�b�g�A�b�v
	/// </summary>
	void SetUps32DispMode( const skyString *headerfont , s32 initValue );

	/// <summary>
	/// s32�l�ݒ�
	/// </summary>
	void Sets32Value( s32 value );

	/// <summary>
	/// s32�A�h���X�ƃo�C���f�B���O
	/// </summary>
	void Binds32Address( s32 *pValue );

	/// <summary>
	/// �q�m�[�h���֘A�t��
	/// </summary>
	void AttachChild( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// �q�m�[�h���֘A�t������
	/// </summary>
	void DetachChild( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// �Z��m�[�h�֘A�t��(�w��̃m�[�h�������̏�̌Z��ɂȂ�B������̌Z�킪�����炻�̏�̌Z��ɂȂ�B�����E�E�i�����j�j
	/// </summary>
	void AttachBeforeBrotherNodeList( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// �Z��m�[�h�֘A�t��(�w��̃m�[�h�������̉��̌Z��ɂȂ�B�������̌Z�킪�����炻�̉��̌Z��ɂȂ�B�����E�E�i�����j�j
	/// </summary>
	void AttachAfterBrotherNodeList( const CWindowSystemNodePtr &spNode );

	/// <summary>
	/// �Z��m�[�h�֘A�t�������i�������Z�탊�X�g���甲����j
	/// </summary>
	void DetachBrotherNodeList();

	/// <summary>
	/// �R���X�g���N�^�i�ʒu�w��̓��[�g�ȊO�͌��ʓI�ɖ����ɂȂ�܂��j
	/// </summary>
	CWindowSystemNode( dectype posX = 0 , dectype posY = 0 , dectype width = WINDOW_SYSTEM_DEFAULT_NODE_WIDTH , dectype height = WINDOW_SYSTEM_DEFAULT_NODE_HEIGHT );

	/// <summary>
	/// ���j�[�N�h�c
	/// </summary>
	s32 m_UniqId;

	/// <summary>
	/// �t�H�[�J�X�t���O
	/// </summary>
	skyBool m_IsOnFocus;

	/// <summary>
	/// �ʒu
	/// </summary>
	math::CBasicVector2 m_Pos;

	/// <summary>
	/// ������
	/// </summary>
	math::CBasicSize m_Size;

	/// <summary>
	/// �e�̎Q��
	/// </summary>
	CWindowSystemNodePtr m_Parent;

	/// <summary>
	/// �q���X�g
	/// </summary>
	CWindowSystemNodePtrList m_ChildList;

	/// <summary>
	/// ���̌Z��̎Q��
	/// </summary>
	CWindowSystemNodePtr m_BeforeBrotherNode;
	CWindowSystemNodePtr m_AfterBrotherNode;
};

} } }
