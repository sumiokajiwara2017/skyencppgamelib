#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �g���h�c�ő吔
/// </summary>
const u32 COLLISION_EXTRAID_NUM = 8;
const s32 COLLISION_KIND_ID_ALL = -1;

/// <summary>
/// �R���W�����f�[�^
/// </summary>
class CollisionData : public base::SkyRefObject
{

friend class CollisionManager;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// �Փ˔�����s����f���Q�[�g���w�肵�Ȃ��i�f�t�H���g�����j�ꍇ��isStopSameKindCollision�̔���O�̂��ׂẴR���W�����f�[�^�Ƃ̏Փ˔�����s���܂�
	/// kindId = -1 �͂��ׂĂ̎�ʂƂ����Ӗ��ŗ\�񂳂�Ă���̂Ń��[�U�[�͐ݒ肵�Ȃ����ƁB
	/// </summary>
	CollisionData();
	CollisionData( s32 kindId , skyBool isStopSameKindCollision , const CGeometryData* pGeometry , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate = ICollisionCBDelegateObjectPtr() , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject = IExecDetectionDelegateObjectPtr() );
	CollisionData( s32 kindId , skyBool isStopSameKindCollision , const IGeometoryGetDelegateObjectPtr &spGeometoryGetDelegate , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate = ICollisionCBDelegateObjectPtr() , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject = IExecDetectionDelegateObjectPtr() );

	/// <summary>
	/// ������
	/// �Փ˔�����s����f���Q�[�g���w�肵�Ȃ��i�f�t�H���g�����j�ꍇ��isStopSameKindCollision�̔���O�̂��ׂẴR���W�����f�[�^�Ƃ̏Փ˔�����s���܂�
	/// </summary>
	void Initialize( s32 kindId , skyBool isStopSameKindCollision , const CGeometryData* pGeometry , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate = ICollisionCBDelegateObjectPtr() , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject = IExecDetectionDelegateObjectPtr() );
	void Initialize( s32 kindId , skyBool isStopSameKindCollision , const IGeometoryGetDelegateObjectPtr &spGeometoryGetDelegate , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate = ICollisionCBDelegateObjectPtr() , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject = IExecDetectionDelegateObjectPtr() );

	/// <summary>
	/// �R���W�������擾�f���Q�[�g�ݒ�
	/// �ݒ肵�Ȃ������ƂŃR���W������r���ŕω������邱�Ƃ��o����
	/// </summary>
	void SetGeometoryGetDelegate( const IGeometoryGetDelegateObjectPtr &spDelegate );

	/// <summary>
	/// �Փ˔�����s����f���Q�[�g�ݒ�
	/// �ݒ肵�Ȃ������ƂŏՓ˔�����s���g�ݍ��킹��ω������邱�Ƃ��ł���
	/// </summary>
	void SetExecDetectionDelegate( const IExecDetectionDelegateObjectPtr &spDelegate );

	/// <summary>
	/// �R���W���������R�[���o�b�N�f���Q�[�g�ݒ�
	/// �ݒ肵�Ȃ������ƂŃR���W���������̒ʒm��ω������邱�Ƃ��o����
	/// </summary>
	void SetCollisionCBDelegate( const ICollisionCBDelegateObjectPtr &spDelegate );

	/// <summary>
	/// �R���W���������~��
	/// </summary>
	void SetStop( skyBool isStop );

	/// <summary>
	/// �����������m�̏Փ˔����~�ݒ�
	/// </summary>
	void SetStopSameKindCollision( skyBool isStop );

	/// <summary>
	/// ��ʂh�c�̎擾�^�ݒ�
	/// </summary>
	s32  GetKindId() const;
	void SetKindId( s32 id );

	/// <summary>
	/// �g���h�c�擾�^�ݒ�
	/// </summary>
	s32  GetExtraId( u32 index ) const;
	void SetExtraId( u32 index , s32 id );

	/// <summary>
	/// �g���f�[�^�擾�^�ݒ�
	/// </summary>
	const CommonSmartPointer &GetExtraData() const;
	void  SetExtraData( const CommonSmartPointer &spExtraData );
	
	/// <summary>
	/// �^�O�r�b�g�̎擾�A�N���A
	/// </summary>
	u32     GetTagBit();
	void    TagBitOn( u32 tagBit );
	void    TagBitOff( u32 tagBit );
	skyBool IsTagBitOn( u32 tagBit );
	void    ClearTagBit();
	
	/// <summary>
	/// �Փˉ����f�[�^���󂯎��
	/// </summary>
	skyBool IsHitResultGet();
	void    SetHitResultSet( skyBool isHitResult );

	/// <summary>
	/// �W�I���g���擾
	/// </summary>
	const CGeometryData* GetGeometry();

private:

	/// <summary>
	/// �^�O�i�^�O�̓r�b�g�l�Ƃ��Ĉ�����̂ŁA������̓r�b�g���Z������j
	/// </summary>
	u32 m_TagBit;

	/// <summary>
	/// �R���W���������~�t���O
	/// ���̃t���O���n�m�̏ꍇ�͏Փ˔���͍s���Ȃ��B�J�����̎���p�ɓ����Ă��Ȃ����̗��R�ŕ`����K�v���Ȃ��ꍇ��
	/// ���̃t���O�𗧂ĂĂ������ƁB
	/// </summary>
	skyBool m_IsStop;

	/// <summary>
	/// ����ʂh�c�Փ˔����~�t���O
	/// </summary>
	skyBool m_IsStopSameKindCollision;

	/// <summary>
	/// ��ʂh�c�i�v���C���[�E�G�E�n�`�E�M�~�b�N���̎�ʂh�c�j
	/// </summary>
	s32 m_KindID;

	/// <summary>
	/// �􉽌`��f�[�^�|�C���^
	/// </summary>
	const CGeometryData* m_pGeometry;

	/// <summary>
	/// �R���W�������擾�f���Q�[�g�i�􉽌`��f�[�^�|�C���^�����݂��Ȃ��ꍇ���̃f���Q�[�g���Ăԁj
	/// CollisionManager�͂��̃f���Q�[�g���R�[�����邱�Ƃ�CGeometryData*���󂯎��Փ˔�����s��
	/// </summary>
	IGeometoryGetDelegateObjectPtr m_spGeometoryGetDelegate;

	/// <summary>
	/// �Փ˔�����s����f���Q�[�g
	/// CollisionManager�͂��̃f���Q�[�g�̖߂�l����Ŏ��ۂɏՓ˔�������s���邩�ǂ������肷��
	/// </summary>
	IExecDetectionDelegateObjectPtr m_spExecDetectionDelegate;

	/// <summary>
	/// �R���W���������R�[���o�b�N�f���Q�[�g
	/// CollisionManager�͂��̃f���Q�[�g���R�[�����邱�ƂŏՓ˂����������Ƃ�ʒm����
	/// </summary>
	ICollisionCBDelegateObjectPtr m_spCollisionCBDelegate;

	/// <summary>
	/// �g���h�c(�A�v���P�[�V�������ɍD���ȃf�[�^���i�[�ł���j
	/// </summary>
	s32 m_ExtraId[ COLLISION_EXTRAID_NUM ];

	/// <summary>
	/// �g���f�[�^(SkyRefObject���p�����Ă���f�[�^�Ȃ牽�ł�����|�C���^�B�A�v���P�[�V�������ɍD���ȃf�[�^���i�[�ł���j
	/// </summary>
	CommonSmartPointer m_spExtraData;

	/// <summary>
	/// �폜�t���O
	/// </summary>
	skyBool m_IsDelete;

	/// <summary>
	/// �q�b�g���ʃf�[�^���󂯎�邩�t���O
	/// </summary>
	skyBool m_IsHitResultGet;

};

} } }
