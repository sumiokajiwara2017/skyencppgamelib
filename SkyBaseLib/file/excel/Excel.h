#pragma once

#ifdef SKYLIB_COMPILER_VC10

//tlb�̃t�@�C���������ł��Btlb�܂ł̃p�X��include�f�B���N�g���̎w��ōs���Ă��������B
#ifdef _DEBUG
#import "../../../../../skycslib/dlls/Debug/skycslib.tlb" raw_interfaces_only //COM�N���X���Ăяo���ۂ̃w�b�_�[�̂悤�Ȃ��́B�����namespace�̊O�ɋ��Ȃ���namespace�̉e�����E�P�� 
#else
#import "../../../../../skycslib/dlls/Release/skycslib.tlb" raw_interfaces_only //COM�N���X���Ăяo���ۂ̃w�b�_�[�̂悤�Ȃ��́B�����namespace�̊O�ɋ��Ȃ���namespace�̉e�����E�P�� 
#endif

namespace sky { namespace lib { namespace file {

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

class IExcel;
typedef SmartPointer2< IExcel >	IExcelPtr;

/// <summary>
/// Excel����N���X
/// </summary>
class IExcel
{

public:

    /// <summary>
    /// �v���p�e�B
    /// <summary>
    virtual s32 getRow() = 0;
    virtual void setRow( s32 row ) = 0;
    virtual s32 getCol() = 0;
    virtual void setCol( s32 col ) = 0;

    /// <summary>
    /// ���[�N�u�b�N���J��
    /// <summary>
    virtual void Open( skyString *filePath ) = 0;

    /// <summary>
    /// ����
    /// <summary>
    virtual void Close() = 0;

    /// <summary>
    /// �V�[�g���̑��݃`�F�b�N
    /// <summary>
    virtual skyBool IsSheet( skyString *sheetName ) = 0;

    /// <summary>
    /// ������ŃV�[�g�I��
    /// <summary>
    virtual void SelSheet( skyString *sheetName ) = 0;

    /// <summary>
    /// �V�[�g�I��
    /// <summary>
    virtual void SelSheet(s32 sheetNo ) = 0;

    /// <summary>
    /// �Z���I��
    /// <summary>
    virtual void SelCell(s32 rowNo, s32 colNo) = 0;

    /// <summary>
    /// �Z���ɓ����Ă��镶���񂪎w��̕�����Ɠ������ǂ������肷��
    /// <summary>
    virtual skyBool EqualsCell( skyString *key) = 0;

    /// <summary>
    /// �Z���ɓ����Ă��镶����Ɏw��̕����������Ă��邩�`�F�b�N
    /// <summary>
    virtual skyBool IndexOfCell( skyString *key) = 0;

    /// <summary>
    /// �Z���Ƀf�[�^�����݂��邩���肷��
    /// <summary>
    virtual skyBool IsCellEmpty() = 0;

    /// <summary>
    /// ��I��
    /// <summary>
    virtual void NextCol() = 0;

    /// <summary>
    /// �s�I��
    /// <summary>
    virtual void NextRow() = 0;

    /// <summary>
    /// �l�𕶎���Ŏ擾
    /// <summary>
    virtual const skyString *GetValue_String() = 0;

    /// <summary>
    /// �l�𕶎���Ŏ擾
    /// <summary>
    virtual s32 GetValue_Int() = 0;

    /// <summary>
    /// �l�𕶎���Ŏ擾
    /// <summary>
    virtual void SetValue_String( skyString *value) = 0;

    /// <summary>
    /// �l�𕶎���Ŏ擾
    /// <summary>
    virtual void SetValue_Int(s32 value) = 0;

    /// <summary>
    /// �ۑ�
    /// <summary>
    virtual void Save() = 0;

    /// <summary>
    /// �ʖ��ۑ�
    /// <summary>
    virtual void SaveAs( skyString *filePath ) = 0;

	/// <summary>
	/// �C���X�^���X��Ԃ�
	/// </summary>
	static IExcelPtr						CreateInstance	();
};

//-------------------------------------< ���� >---------------------------------------------//

class CExcelWin32 : public IExcel
{

//�C���^�[�t�F�[�X�̂݃A�N�Z�X�\
friend class IExcel;

public:

	/// <summary>
	///  �C���^�t�F�[�X�������z�֐��̎����i�����̓C���^�t�F�[�X�̒�`�����Ă��������j
	/// </summary>
    s32 getRow			();
    void setRow			( s32 row );
    s32 getCol			();
    void setCol			( s32 col );
    void Open			( skyString *filePath);
    void Close			();
    skyBool IsSheet		( skyString *sheetName);
    void SelSheet		( skyString *sheetName);
    void SelSheet		(s32 sheetNo);
    void SelCell		(s32 rowNo, s32 colNo);
    skyBool EqualsCell	( skyString *key);
    skyBool IndexOfCell	( skyString *key);
    skyBool IsCellEmpty	();
    void NextCol		();
    void NextRow		();
    const skyString *GetValue_String();
    s32 GetValue_Int();
    void SetValue_String( skyString *value);
    void SetValue_Int(s32 value);
    void Save();
    void SaveAs( skyString *filePath );

private:

	skycslib::IExcelPtr	m_pComExcel;

	//�e�@�\����
	CExcelWin32			();									//�R���X�g���N�^
	CExcelWin32			( const CExcelWin32& ){};					//�R�s�[�R���X�g���N�^
	virtual ~CExcelWin32	();								//�f�X�g���N�^
	CExcelWin32 &operator=	(const CExcelWin32& ){ return *this; };	//������Z�q�I�[�o�[���[�h

};

} } }

#endif
