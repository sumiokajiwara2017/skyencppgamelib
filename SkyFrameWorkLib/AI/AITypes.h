#pragma once

namespace sky { namespace lib { namespace framework {

/// <summary>
/// �ړ��^�C�v�i�ړ��^�C�v���Ɉ����s�����̂ł͂Ȃ��A���͗D�揇�ʂ������^�C�v�̑g�ݍ��킹�Ŏg�p���邱�ƂɂȂ�j
/// </summary>
enum eAIMoveType
{
	eAIMoveType_Seek                 = 1 << 0  , //�y�T���s���z�w��̃^�[�Q�b�g�ɋ߂Â�
	eAIMoveType_Flee                 = 1 << 1  , //�y�����s���z�w��̃^�[�Q�b�g���牓������
	eAIMoveType_Arrive               = 1 << 2  , //�y�����s���z�w��̃^�[�Q�b�g�ɋ߂Â��A���B������~�܂�
	eAIMoveType_Pursuit              = 1 << 3  , //�y�ǐՍs���z�w��̃^�[�Q�b�g�ɋ߂Â��BSeek�Ƃ̈Ⴂ�͌��݂̑���̈ʒu�Ɍ������̂ł͂Ȃ�����̑��x�����ē��B�\�z�n�_��z�肵�A�����Ɉړ�����
	eAIMoveType_Evade                = 1 << 4  , //�y�����s���z�w��̃^�[�Q�b�g���牓������Flee�Ƃ̈Ⴂ�͑���̑��x���݂đ���̓��B�n�_���瓦����
	eAIMoveType_Wander               = 1 << 5  , //�y�p�j�s���z�����_���Ō��ʓI�ɜp�j���Ă��邩�̂悤�Ɏ��R�Ɉړ�����
    eAIMoveType_ObstacleAvoidance    = 1 << 6  , //�y��Q����s���z�o�H��ɑ��݂����Q��������Ĉړ�����
	eAIMoveType_WallAvoidance        = 1 << 7  , //�y�ǉ���s���z���ʂƂ̏Փ˂�����Ĉړ�����
	eAIMoveType_Interpose            = 1 << 8  , //�y����s���z�Q�̃G�[�W�F���g�A�������͂Q�̒n�_�A�������̓G�[�W�F���g�ƂP�̓_�̐^�񒆂Ɉړ�����
	eAIMoveType_Hide                 = 1 << 9  , //�y�B�g�s���z��Q���������Ƒ���Ƃ̊Ԃɏ�ɑ��݂���悤�ɍs������
	eAIMoveType_FollowPath           = 1 << 10 , //�y�o�H�Ǐ]�s���z�w��p�X�i���W�Q�j�ɉ����Ĉړ�����
	eAIMoveType_OffsetPursuit        = 1 << 11   //�y�I�t�Z�b�g�ǐՍs���z�ڕW�����苗���͂Ȃ�ĒǐՂ���
};

} } }