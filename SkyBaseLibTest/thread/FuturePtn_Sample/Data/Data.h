/// <summary>
/// �z�X�g�N���X
/// </summary>
class Data
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Data( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Data( void );

	/// <summary>
	/// �f�[�^�̐ݒ�
	/// </summary>
	void SetData( const skyWString *pStr );

	/// <summary>
	/// �f�[�^�̎擾
	/// </summary>
	const skyWString *GetData( void );

protected:

	/// <summary>
	/// �f�[�^���������t���O
	/// </summary>
	skyBool							m_IsDataSet;

	/// <summary>
	/// �f�[�^
	/// </summary>
	sky::lib::WString				m_Data;

	/// <summary>
	/// ���b�N
	/// </summary>
	sky::lib::thread::ILockObject	*m_pLock;

};

typedef sky::lib::SmartPointerMT< Data >			DataSmPtr;	//SmartPointer�^
