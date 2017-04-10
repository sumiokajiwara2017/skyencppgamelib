#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#include "SkyGraphicLib/OpenGL/SkyGraphicLibGL.h"
#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/Loader.h"

#ifdef SKY_GRAPHIC_LIB_OPENGL

using namespace sky::lib;

#ifdef TEST_OPENGL

#if OFF_

// ---------------------------------------------------------------------------
// ���̃I�u�W�F�N�g
// ---------------------------------------------------------------------------
static GLfloat CubeNormals[6][3] = 
 {  
  {+1.0,  0.0,  0.0}, {0.0, +1.0, 0.0}, {-1.0, 0.0, 0.0},
  { 0.0, -1.0,  0.0}, {0.0, 0.0,+1.0}, {0.0, 0.0, -1.0} 
 };

static GLint CubeFaces[6][4] = 
 {  
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} 
 };

static GLfloat CubeVertices[8][3];  

static void InitializeCube(GLfloat v[8][3])
{
	v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
	v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
	v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
	v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
	v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
	v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;
}

//------- ���_�f�[�^-----------//
//�6��ʤ�4����_�1���_��x,y,z�̢3��v�f
GLfloat vertexAry[6][4][3] =
{
	//1�߂̖�(v0-v1-v2-v3)
	{ 
		{ 0.f , 0.f , 0.f } , { 1.f ,0.f , 0.f},
		{ 1.f , 1.f , 0.f } , { 0.f ,1.f , 0.f} 
	},
	//2�߂̖�(v1-v5-v6-v2)
	{ 
		{ 1.f , 0.f , 0.f }  , { 1.f ,0.f , -1.f },
		{ 1.f , 1.f , -1.f } , { 1.f ,1.f , 0.f  } 
	},
	//3�߂̖�(v4-v7-v6-v5)
	{ 
		{ 0.f , 0.f , -1.f } , { 0.f , 1.f , -1.f },
		{ 1.f , 1.f , -1.f } , { 1.f , 0.f , -1.f } 
	},
	//4�߂̖�(v4-v0-v3-v7)
	{ 
		{ 0.f , 0.f ,-1.f } , { 0.f , 0.f , 0.f },
		{ 0.f , 1.f ,0.f }  , { 0.f , 1.f , -1.f } 
	},
	//5�߂̖�(v3-v2-v6-v7)
	{ 
		{ 0.f , 1.f , 0.f } ,{ 1.f ,1.f ,0.f },
		{ 1.f , 1.f , -1.f } ,{ 0.f ,1.f ,-1.f } 
	},
	//6�߂̖�(v0-v4-v5-v1)
	{ 
		{0.f,0.f,0.f},{0.f,0.f,-1.f},
		{1.f,0.f,-1.f},{1.f,0.f,0.f} 
	}
};

//�@���f�[�^
GLfloat normalAry[6][4][3] =
{
	//1�߂̖�(v0-v1-v2-v3)
	{ 
		{0.f,0.f,1.f},{0.f,0.f,1.f},
		{0.f,0.f,1.f},{0.f,0.f,1.f} 
	},
	//2�߂̖�(v1-v5-v6-v2)
	{ 
		{1.f,0.f,0.f},{1.f,0.f,0.f},
		{1.f,0.f,0.f},{1.f,0.f,0.f} 
	},
	//3�߂̖�(v4-v7-v6-v5)
	{ 
		{0.f,0.f,-1.f},{0.f,0.f,-1.f},
		{0.f,0.f,-1.f},{0.f,0.f,-1.f} 
	},
	//4�߂̖�(v4-v0-v3-v7)
	{ 
		{-1.f,0.f,0.f},{-1.f,0.f,0.f},
		{-1.f,0.f,0.f},{-1.f,0.f,0.f} 
	},
	//5�߂̖�(v3-v2-v6-v7)
	{ 
		{0.f,1.f,0.f},{0.f,1.f,0.f},
		{0.f,1.f,0.f},{0.f,1.f,0.f} 
	},
	//6�߂̖�(v0-v4-v5-v1)
	{ 
		{0.f,-1.f,0.f},{0.f,-1.f,0.f}, 
		{0.f,-1.f,0.f},{0.f,-1.f,0.f} 
	}
};

//�F�f�[�^
GLfloat colorAry[6][4][3] =
{
	//1�߂̖�(v0-v1-v2-v3)
	{ 
		{0.f,0.f,1.f},{0.f,0.f,1.f},
		{0.f,0.f,1.f},{0.f,0.f,1.f} 
	},
	//2�߂̖�(v1-v5-v6-v2)
	{ 
		{1.f,0.f,0.f},{1.f,0.f,0.f},
		{1.f,0.f,0.f},{1.f,0.f,0.f} 
	},
	//3�߂̖�(v4-v7-v6-v5)
	{ 
		{0.f,1.f,1.f},{0.f,1.f,1.f},
		{0.f,1.f,1.f},{0.f,1.f,1.f} 
	},
	//4�߂̖�(v4-v0-v3-v7)
	{ 
		{1.f,1.f,0.f},{1.f,1.f,0.f},
		{1.f,1.f,0.f},{1.f,1.f,0.f} 
	},
	//5�߂̖�(v3-v2-v6-v7)
	{ 
		{0.f,1.f,0.f},{0.f,1.f,0.f},
		{0.f,1.f,0.f},{0.f,1.f,0.f} 
	},
	//6�߂̖�(v0-v4-v5-v1)
	{ 
		{1.f,0.f,1.f},{1.f,0.f,1.f},
		{1.f,0.f,1.f},{1.f,0.f,1.f} 
	}
};

//�C���f�b�N�X
GLubyte indexAry[]=
{
	0,1,2,3,
	4,5,6,7,
	8,9,10,11,
	12,13,14,15,
	16,17,18,19,
	20,21,22,23
};

// ��/180�̒l
const float PI_OVER_180 = 0.0174532925f;

//VBO�pID
GLuint VboId[3];//�R��
GLuint VboIndex;//�C���f�b�N�X

static const skyString* window_name = _T( "testwindow" );

static skyBool bExit = skyFalse;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		bExit = skyTrue;
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

TEST( opengl , 1 ) 
{
    SKY_LIB_GRAPHIC_INIT();

	//Window�̍쐬
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= 0;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= window_name;
    wcex.lpszClassName	= window_name;
    wcex.hIconSm		= 0;
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(window_name, window_name, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if ( !hWnd )
    {
		SKY_PANIC_MSG( _T( "Window�쐬�̎��s \n" ) );
    }

   ShowWindow( hWnd , SW_SHOWDEFAULT );
   UpdateWindow( hWnd );

	//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
	RECT clientArea;
	GetClientRect( hWnd, &clientArea );
	int width  = clientArea.right - clientArea.left;
	int height = clientArea.bottom -clientArea.top;

   HDC hDC = NULL;
   HGLRC hRC = NULL;
	// �f�o�C�X�R���e�L�X�g�̎擾�A�ݒ�
	hDC = GetDC( hWnd );

	if( NULL == hDC )
	{
		SKY_PANIC_MSG( _T( "�f�o�C�X�R���e�L�X�g�̍쐬�̎��s \n" ) );
	}

	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( hDC, &pfd ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	if( TRUE != ::SetPixelFormat( hDC, iPixelFormat, &pfd ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g�̍쐬
	if( 0 == ( hRC = ::wglCreateContext( hDC ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g���J�����g�̃f�o�C�X�R���e�L�X�g�ɐݒ�
	if( TRUE != ::wglMakeCurrent( hDC, hRC ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

   // GLEW������
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        SKY_PANIC_MSG(_T( "Error: %s\n" ), glewGetErrorString( err ) );
    }

	//�g���`�F�b�N
	if(!glewIsExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
        SKY_PANIC_MSG(_T( "you Can't use VBO \n" ), glewGetErrorString( err ) );
	}

	// �V�F�[�_�̓ǂݍ���
	GLuint hProgramObject;
	sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(hProgramObject ,_T( "data/test/glsl/GLSL.vert"), _T("data/test/glsl/GLSL.frag") );
	
	// �I�u�W�F�N�g�̃��P�[�V�����̏���
	GLint locLightPos = glGetUniformLocation(hProgramObject, "LightPos");

//--------------------VBO�̍쐬--------------------------------------��

	glGenBuffers(3,&VboId[0]);//���W��@����F�̂R��
	
	//���_
	glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexAry),
				vertexAry,GL_DYNAMIC_DRAW);//�f�[�^�ύX����

	//�@��
	glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(normalAry),
				normalAry,GL_DYNAMIC_DRAW);//�f�[�^�ύX����

	//�F
	glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colorAry),
					colorAry,GL_STREAM_DRAW);//�f�[�^�ύX�Ȃ�

	//�C���f�b�N�X
	glGenBuffers(1,&VboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexAry),
					indexAry,GL_STATIC_DRAW);//�f�[�^�ύX�Ȃ�

//--------------------VBO�̍쐬--------------------------------------��

	// �N���A�J���[�̐ݒ�
	::glClearColor( 0, 0, 0, 0 );	// ��

	// �f�v�X�o�b�t�@�̃N���A
	::glClearDepth( 1.0f );

	// �f�v�X�e�X�g��L���ɂ���
	::glEnable( GL_DEPTH_TEST );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	::glDepthFunc( GL_LEQUAL );

	//�J�����O��L���ɂ���
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	//�r���[�|�[�g
	::glViewport(clientArea.left,clientArea.top,width,height);

	// ����p����ݒ�
	::glMatrixMode( GL_PROJECTION );
	::glLoadIdentity();
#if OFF_
	::glOrtho(	- width * 0.5 / 10.0,
				  width * 0.5 / 10.0,
				- height * 0.5 / 10.0,
				  height * 0.5 / 10.0,
				  0.1,
				  1000.0 );
#else
    gluPerspective(30.0, (double)width / (double)height, 1.0f, 1000.0f);
#endif

	// ���_���W�ϊ�
	::glMatrixMode( GL_MODELVIEW );
	::glLoadIdentity();
	::gluLookAt (	0.0, 0.0, 10.0,	// ���_
					0.0, 0.0,   0.0,	// �����_
					0.0, 1.0,   0.0 );	// ��x�N�g��

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//���͊Ǘ��X�V
		Ckeyboard_->Update( 0 );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		// �h���C�o�Ƀo�O�̂���O���t�B�b�N�J�[�h�ɂ����Ă�SDI�̍ŏ��̕`��ƁA
		// ���ׂẴO���t�B�b�N�J�[�h�ɂ����Ă�MDI�`��ɂ�����
		// �ȉ��̖��߂́A���ʂ𔭊�����B
		::wglMakeCurrent( hDC, hRC );

		//�J���[�o�b�t�@�Ɛ[�x�o�b�t�@�̃N���A
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// �f�v�X�o�b�t�@�̃N���A
		::glClearDepth( 1.0f );

//--------------------VBO�̕`��--------------------------------------��
#if ON_

		static int anglex = 0;
		glPushMatrix();
		glRotatef(float(anglex),1.f,0.f,0.f);//x����]

		GLfloat *clientPtr;//�N���C�A���g���p
		GLfloat tmp[3];
		int idloop;
		int loop;
		static float angle = 2*PI_OVER_180;

		//�f�[�^�̏ꏊ��m�点��
		//���W
		glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		//�@��
		glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
		glNormalPointer(GL_FLOAT, 0, 0);
		//�F
		glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
		glColorPointer(3,GL_FLOAT, 0, 0);

		//�C���f�b�N�X���o�C���h
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndex);
 
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_INDEX_ARRAY);

		// �V�F�[�_�̐؂�ւ�
		glUseProgram(hProgramObject);

		// ���C�g�ʒu�̐ݒ�
		float lightPosition[3] = {2.0f, 8.0f, -10.0f };
		glUniform3fv(locLightPos, 1, &lightPosition[0]);

		//�`��
		glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,0);
	
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		//���W�Ɩ@������]������
		for(idloop = 0; idloop < 2;++idloop)
		{
			//idloop�Ԗڂ̃o�b�t�@�I�u�W�F�N�g�ɒ���
			glBindBuffer(GL_ARRAY_BUFFER,VboId[idloop]);
		
			//�Ή��t��
			clientPtr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,
													GL_READ_WRITE);
			if(clientPtr != NULL)
			{
				//24���_*3���W
				 for(loop = 0; loop < 24*3;loop += 3)	{
					//�ǂݏo��(READ)
					tmp[0] = clientPtr[loop];
					tmp[1] = clientPtr[loop+1];
					tmp[2] = clientPtr[loop+2];
					//��������(WRITE)
					clientPtr[loop] = cos(angle)*tmp[0]
										+ sin(angle)*tmp[2];
					clientPtr[loop+1] = tmp[1];
					clientPtr[loop+2] = -sin(angle)*tmp[0]
											+ cos(angle)*tmp[2];
				 }
				glUnmapBuffer(GL_ARRAY_BUFFER);//�Ή��t���̉��
			}
		}
		if(++anglex >= 360) anglex = 0;

		glPopMatrix();
		glPushMatrix();

		glTranslatef( 0 , 0 , -10 );

		//�N���C�A���g���ɖ߂�
		glBindBuffer(GL_ARRAY_BUFFER,0);

		//�f�[�^�̏ꏊ��m�点��
		//���W
		glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		//�@��
		glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
		glNormalPointer(GL_FLOAT, 0, 0);
		//�F
		glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
		glColorPointer(3,GL_FLOAT, 0, 0);

		//�C���f�b�N�X���o�C���h
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndex);
        glIndexPointer(GL_FLOAT, 4, 0); 

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_INDEX_ARRAY);

		//�`��
		glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,0);
	
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glPopMatrix();
#endif
//--------------------VBO�̕`��--------------------------------------��
//--------------------����̕`��-----------------------------------��
#if OFF_

		float	points[4][3] = { {  0.0,  0.0,  0.0 },
								 { 10.0,  0.0,  0.0 },
								 {  0.0, 10.0,  0.0 },
								 {  0.0,  0.0, 10.0 } };

		::glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );	// �F | �����

		::glLineWidth( 2.0 );
		::glBegin( GL_LINES );
			::glColor3ub( 255, 0, 0 );
			::glVertex3fv( points[0] );
			::glVertex3fv( points[1] );
			::glColor3ub( 0, 255, 0 );
			::glVertex3fv( points[0] );
			::glVertex3fv( points[2] );
			::glColor3ub( 0, 0, 255 );
			::glVertex3fv( points[0] );
			::glVertex3fv( points[3] );
		::glEnd();

		::glPopAttrib();

#endif
//--------------------����̕`��-----------------------------------��
//--------------------�O�p���ƕ��s�l�ӌ`-----------------------------��
#if OFF_
		::glPushMatrix();

		//�O�p��
			::glScalef(0.3f,0.3f,0.3f);
			::glTranslatef(0.0f,0.0f,-5.0f);

			::glBegin(GL_TRIANGLES);
			::glColor3f(0.0f,0.0f,1.0f);
			::glVertex3f( 0.0f, 1.0f, 0.0f);
			::glVertex3f(-1.0f,-1.0f, 1.0f);
			::glVertex3f( 1.0f,-1.0f, 1.0f);

			::glColor3f(0.0f,1.0f,0.0f);
			::glVertex3f( 0.0f, 1.0f, 0.0f);
			::glVertex3f( 1.0f,-1.0f, 1.0f);
			::glVertex3f( 1.0f,-1.0f, -1.0f);
			::glEnd();

		::glPopMatrix();
#endif
#if OFF_

		::glPushMatrix();
		//�O�p��
			::glTranslatef(0.0f,0.0f,-7.0f);

			::glBegin(GL_TRIANGLES);
			::glColor3f(1.0f,0.0f,0.0f);
			::glVertex3f( 0.0f, 1.0f, 0.0f);
			::glVertex3f(-1.0f,-1.0f, 1.0f);
			::glVertex3f( 1.0f,-1.0f, 1.0f);

			::glColor3f(0.0f,1.0f,0.0f);
			::glVertex3f( 0.0f, 1.0f, 0.0f);
			::glVertex3f( 1.0f,-1.0f, 1.0f);
			::glVertex3f( 1.0f,-1.0f, -1.0f);

			::glEnd();
		::glPopMatrix();
#endif
#if OFF_
		::glPushMatrix();
		//�l�p�`�X�v���C�g
			::glTranslatef(0.0f,0.0f,-6.0f);

			::glBegin(GL_QUADS);
			::glColor3f(1.0f,0.0f,0.0f);
			::glVertex3f( 1.0f, 1.0f, 1.0f);
			::glColor3f(0.0f,1.0f,0.0f);
			::glVertex3f(-1.0f, 1.0f, 1.0f);
			::glColor3f(0.0f,0.0f,1.0f);
			::glVertex3f(-1.0f,-1.0f, 1.0f);
			::glColor3f(1.0f,1.0f,0.0f);
			::glVertex3f( 1.0f,-1.0f, 1.0f);
			::glEnd();

		::glPopMatrix();
#endif
//--------------------�O�p���ƕ��s�l�ӌ`-----------------------------��
//--------------------�V�F�[�_�[�ł̕`��-----------------------------��
#if OFF_

		static int init=0;
		if(!init){init=1;
			// ���̏�����
			InitializeCube(CubeVertices);
		}

		static FLOAT      m_fTime = 0;
		static FLOAT      m_fElapsedTime = 0;

		// ���Ԃ��v��
		FLOAT time = 0.001f*(FLOAT)timeGetTime();
		m_fElapsedTime = (FLOAT)(time-m_fTime);
		m_fTime = time;
		FLOAT  rot = time;

		// �V�F�[�_�̐؂�ւ�
		glUseProgramObjectARB(hProgramObject);

		// ���C�g�ʒu�̐ݒ�
		float lightPosition[3] = {2.0f, 8.0f, -10.0f };
		glUniform3fvARB(locLightPos, 1, &lightPosition[0]);

		// ���̕`��
		glPushMatrix();
		glRotated(100*rot, 0.0, 1.0, 0.0);

		for(int i = 0; i < 6; i++)
		{
			glBegin(GL_QUADS);	// �l�p�`�̕`��

			glNormal3fv(&CubeNormals[i][0]);							// �@��
			glColor3d(CubeVertices[CubeFaces[i][0]][0]*0.5+0.5
					, CubeVertices[CubeFaces[i][0]][1]*0.5+0.5
					, CubeVertices[CubeFaces[i][0]][2]*0.5+0.5);		// ���_�F
			glVertex3fv(&CubeVertices[CubeFaces[i][0]][0]);				// ���W
			glColor3d(CubeVertices[CubeFaces[i][1]][0]*0.5+0.5
					, CubeVertices[CubeFaces[i][1]][1]*0.5+0.5
					, CubeVertices[CubeFaces[i][1]][2]*0.5+0.5);		// ���_�F
			glVertex3fv(&CubeVertices[CubeFaces[i][1]][0]);
			glColor3d(CubeVertices[CubeFaces[i][2]][0]*0.5+0.5
					, CubeVertices[CubeFaces[i][2]][1]*0.5+0.5
					, CubeVertices[CubeFaces[i][2]][2]*0.5+0.5);		// ���_�F
			glVertex3fv(&CubeVertices[CubeFaces[i][2]][0]);
			glColor3d(CubeVertices[CubeFaces[i][3]][0]*0.5+0.5
					, CubeVertices[CubeFaces[i][3]][1]*0.5+0.5
					, CubeVertices[CubeFaces[i][3]][2]*0.5+0.5);		// ���_�F
			glVertex3fv(&CubeVertices[CubeFaces[i][3]][0]);
    
			glEnd();	// �l�p�`�I���
		}
	
		glPopMatrix();

#endif
//--------------------�V�F�[�_�[�ł̕`��-----------------------------��

		//�`��R�}���h��S���]��
		::glFinish();

		//�o�b�N�o�b�t�@���t�����g��
		::SwapBuffers( hDC );
	}
	GAME_LOOP_END

	::wglMakeCurrent(hDC,0);
	::wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );

	DestroyWindow( hWnd );
	UnregisterClass( window_name , hInstance );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

//================================================================================================================

#if OFF_

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "testcube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "testcone.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "testplane10x10.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_4 = _T( "testsphere.obj" );

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 

//�J�������
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA_DEFAULT_FAR		= 1000;
static const dectype			 _CAMERA_DEFAULT_FOV		= math::CMathUtil::DegToRad( 30.0f );
static const math::CBasicVector3 _CAMERA_DEFAULT_EYE		= math::CBasicVector3( 0.0f, 0.0f,-30.0f );

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera3DDebugPtr  g_spCamera;

//�e�X�g�p���f��
static graphic::CModelPtr     g_TestCube;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;

TEST( opengl , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m�R�[���o�b�N��o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
	g_spCamera = Camera3DDebugCreate_();
	g_spCamera->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA_DEFAULT_FAR );
	g_spCamera->SetFov( _CAMERA_DEFAULT_FOV );
	g_spCamera->SetInitPos( _CAMERA_DEFAULT_EYE );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera;                           //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = IRenderStatCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen = CScreenObject_::Create( screenData );

	g_TestCube = graphic::CModelUtil::CreateTestCube();

	//���f�����V�[���O���t�ɓo�^
	g_spScreen->GetScene()->m_spSceneGraph = CSceneGraphNodeCreate_();
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestCube );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//�J�����̍X�V
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

		//���f���ړ��ʒu���f
		graphic::CModelUtil::SetEntity3D( g_TestCube );

		//���Z�b�g
		if ( Ckeyboard_->IsKeyPress( KEY_HOME ) )
		{
			g_TestCube->GetProperty().ResetLocal();
			g_TestCube->GetProperty().ResetParent();
			g_TestCube->GetProperty().ResetWorld();
		}

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spScreen->Rendering();

			//�R�}���h�o�b�t�@�̒ǉ�
			CRenderCommandManager_::PushBuffer();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//�`��p�X���b�h�̒�~
	CRenderCommandManager_::StopThread();

	g_spCamera.Delete();
	g_TestCube.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

//================================================================================================================

#if OFF_

//------- ���_�f�[�^-----------//
//�6��ʤ�4����_�1���_��x,y,z�̢3��v�f
GLfloat vertexAry[6][4][3] =
{
	//1�߂̖�(v0-v1-v2-v3)
	{ 
		{ 0.f , 0.f , 0.f } , { 1.f ,0.f , 0.f},
		{ 1.f , 1.f , 0.f } , { 0.f ,1.f , 0.f} 
	},
	//2�߂̖�(v1-v5-v6-v2)
	{ 
		{ 1.f , 0.f , 0.f }  , { 1.f ,0.f , -1.f },
		{ 1.f , 1.f , -1.f } , { 1.f ,1.f , 0.f  } 
	},
	//3�߂̖�(v4-v7-v6-v5)
	{ 
		{ 0.f , 0.f , -1.f } , { 0.f , 1.f , -1.f },
		{ 1.f , 1.f , -1.f } , { 1.f , 0.f , -1.f } 
	},
	//4�߂̖�(v4-v0-v3-v7)
	{ 
		{ 0.f , 0.f ,-1.f } , { 0.f , 0.f , 0.f },
		{ 0.f , 1.f ,0.f }  , { 0.f , 1.f , -1.f } 
	},
	//5�߂̖�(v3-v2-v6-v7)
	{ 
		{ 0.f , 1.f , 0.f } ,{ 1.f ,1.f ,0.f },
		{ 1.f , 1.f , -1.f } ,{ 0.f ,1.f ,-1.f } 
	},
	//6�߂̖�(v0-v4-v5-v1)
	{ 
		{0.f,0.f,0.f},{0.f,0.f,-1.f},
		{1.f,0.f,-1.f},{1.f,0.f,0.f} 
	}
};

//�@���f�[�^
GLfloat normalAry[6][4][3] =
{
	//1�߂̖�(v0-v1-v2-v3)
	{ 
		{0.f,0.f,1.f},{0.f,0.f,1.f},
		{0.f,0.f,1.f},{0.f,0.f,1.f} 
	},
	//2�߂̖�(v1-v5-v6-v2)
	{ 
		{1.f,0.f,0.f},{1.f,0.f,0.f},
		{1.f,0.f,0.f},{1.f,0.f,0.f} 
	},
	//3�߂̖�(v4-v7-v6-v5)
	{ 
		{0.f,0.f,-1.f},{0.f,0.f,-1.f},
		{0.f,0.f,-1.f},{0.f,0.f,-1.f} 
	},
	//4�߂̖�(v4-v0-v3-v7)
	{ 
		{-1.f,0.f,0.f},{-1.f,0.f,0.f},
		{-1.f,0.f,0.f},{-1.f,0.f,0.f} 
	},
	//5�߂̖�(v3-v2-v6-v7)
	{ 
		{0.f,1.f,0.f},{0.f,1.f,0.f},
		{0.f,1.f,0.f},{0.f,1.f,0.f} 
	},
	//6�߂̖�(v0-v4-v5-v1)
	{ 
		{0.f,-1.f,0.f},{0.f,-1.f,0.f}, 
		{0.f,-1.f,0.f},{0.f,-1.f,0.f} 
	}
};

//�F�f�[�^
GLfloat colorAry[6][4][3] =
{
	//1�߂̖�(v0-v1-v2-v3)
	{ 
		{0.f,0.f,1.f},{0.f,0.f,1.f},
		{0.f,0.f,1.f},{0.f,0.f,1.f} 
	},
	//2�߂̖�(v1-v5-v6-v2)
	{ 
		{1.f,0.f,0.f},{1.f,0.f,0.f},
		{1.f,0.f,0.f},{1.f,0.f,0.f} 
	},
	//3�߂̖�(v4-v7-v6-v5)
	{ 
		{0.f,1.f,1.f},{0.f,1.f,1.f},
		{0.f,1.f,1.f},{0.f,1.f,1.f} 
	},
	//4�߂̖�(v4-v0-v3-v7)
	{ 
		{1.f,1.f,0.f},{1.f,1.f,0.f},
		{1.f,1.f,0.f},{1.f,1.f,0.f} 
	},
	//5�߂̖�(v3-v2-v6-v7)
	{ 
		{0.f,1.f,0.f},{0.f,1.f,0.f},
		{0.f,1.f,0.f},{0.f,1.f,0.f} 
	},
	//6�߂̖�(v0-v4-v5-v1)
	{ 
		{1.f,0.f,1.f},{1.f,0.f,1.f},
		{1.f,0.f,1.f},{1.f,0.f,1.f} 
	}
};

//�C���f�b�N�X
GLubyte indexAry[]=
{
	0,1,2,3,
	4,5,6,7,
	8,9,10,11,
	12,13,14,15,
	16,17,18,19,
	20,21,22,23
};

static skyBool bExit = skyFalse;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		bExit = skyTrue;
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

// ��/180�̒l
const float PI_OVER_180 = 0.0174532925f;

//VBO�pID
GLuint VboId[3];//�R��
GLuint VboIndex;//�C���f�b�N�X

HDC   m_hDC;
HGLRC m_hRC;
HWND  m_hWnd;
static const skyString* window_name = _T( "testwindow" );

/// <summary>
/// �R�}���h���s�p�X���b�h
/// </summary>
static skyBool s_isLoop = skyTrue;
class CTestThread : public thread::CThread
{

public:

	enum eState {
		eState_Init = 0 ,
		eState_Update ,
	};

	eState m_State;

	// �V�F�[�_�̓ǂݍ���
	GLuint m_hProgramObject;
	
	// �I�u�W�F�N�g�̃��P�[�V�����̏���
	GLint m_locLightPos;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CTestThread(){ m_State = eState_Init; }

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CTestThread(){};

	/// <summary>
	/// �X���b�h���s�֐�
	/// </summary>
	virtual void ThreadMain()
	{
		while( s_isLoop )
		{
			switch( m_State )
			{
			case eState_Init:
			{

				//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
				RECT clientArea;
				GetClientRect( m_hWnd, &clientArea );
				int width  = clientArea.right - clientArea.left;
				int height = clientArea.bottom -clientArea.top;

			   m_hDC = NULL;
			   m_hRC = NULL;
				// �f�o�C�X�R���e�L�X�g�̎擾�A�ݒ�
				m_hDC = GetDC( m_hWnd );

				if( NULL == m_hDC )
				{
					SKY_PANIC_MSG( _T( "�f�o�C�X�R���e�L�X�g�̍쐬�̎��s \n" ) );
				}

				static PIXELFORMATDESCRIPTOR pfd = { 
					sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
					1,
					PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
					PFD_TYPE_RGBA,
					24,
					0, 0, 0, 0, 0, 0,
					0,
					0,
					0,
					0, 0, 0, 0,
					32,
					0,
					0,
					PFD_MAIN_PLANE,
					0,
					0, 0, 0
				};

				int iPixelFormat;
				if( 0 == ( iPixelFormat = ::ChoosePixelFormat( m_hDC, &pfd ) ) )
				{
					SKY_PANIC_MSG( _T( "\n" ) );
				}

				if( TRUE != ::SetPixelFormat( m_hDC, iPixelFormat, &pfd ) )
				{
					SKY_PANIC_MSG( _T( "\n" ) );
				}

				// �����_�����O�R���e�L�X�g�̍쐬
				if( 0 == ( m_hRC = ::wglCreateContext( m_hDC ) ) )
				{
					SKY_PANIC_MSG( _T( "\n" ) );
				}

				// �����_�����O�R���e�L�X�g���J�����g�̃f�o�C�X�R���e�L�X�g�ɐݒ�
				if( TRUE != ::wglMakeCurrent( m_hDC, m_hRC ) )
				{
					SKY_PANIC_MSG( _T( "\n" ) );
				}

			   // GLEW������
				GLenum err = glewInit();
				if (err != GLEW_OK)
				{
					SKY_PANIC_MSG(_T( "Error: %s\n" ), glewGetErrorString( err ) );
				}

				//�g���`�F�b�N
				if(!glewIsExtensionSupported("GL_ARB_vertex_buffer_object"))
				{
					SKY_PANIC_MSG(_T( "you Can't use VBO \n" ), glewGetErrorString( err ) );
				}

				// �V�F�[�_�̓ǂݍ���
				sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(m_hProgramObject ,_T( "data/test/glsl/GLSL.vert"), _T("data/test/glsl/GLSL.frag") );
	
				// �I�u�W�F�N�g�̃��P�[�V�����̏���
				m_locLightPos = glGetUniformLocation(m_hProgramObject, "LightPos");

			//--------------------VBO�̍쐬--------------------------------------��

				glGenBuffers(3,&VboId[0]);//���W��@����F�̂R��
	
				//���_
				glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
				glBufferData(GL_ARRAY_BUFFER,sizeof(vertexAry),
							vertexAry,GL_DYNAMIC_DRAW);//�f�[�^�ύX����

				//�@��
				glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
				glBufferData(GL_ARRAY_BUFFER,sizeof(normalAry),
							normalAry,GL_DYNAMIC_DRAW);//�f�[�^�ύX����

				//�F
				glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
				glBufferData(GL_ARRAY_BUFFER,sizeof(colorAry),
								colorAry,GL_STREAM_DRAW);//�f�[�^�ύX�Ȃ�

				//�C���f�b�N�X
				glGenBuffers(1,&VboIndex);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VboIndex);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexAry),
								indexAry,GL_STATIC_DRAW);//�f�[�^�ύX�Ȃ�

			//--------------------VBO�̍쐬--------------------------------------��

				// �N���A�J���[�̐ݒ�
				::glClearColor( 0, 0, 0, 0 );	// ��

				// �f�v�X�o�b�t�@�̃N���A
				::glClearDepth( 1.0f );

				// �f�v�X�e�X�g��L���ɂ���
				::glEnable( GL_DEPTH_TEST );

				//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
				::glDepthFunc( GL_LEQUAL );

				//�J�����O��L���ɂ���
				glEnable(GL_CULL_FACE);
				glCullFace(GL_FRONT);

				//�r���[�|�[�g
				::glViewport(clientArea.left,clientArea.top,width,height);

				// ����p����ݒ�
				::glMatrixMode( GL_PROJECTION );
				::glLoadIdentity();
			#if OFF_
				::glOrtho(	- width * 0.5 / 10.0,
							  width * 0.5 / 10.0,
							- height * 0.5 / 10.0,
							  height * 0.5 / 10.0,
							  0.1,
							  1000.0 );
			#else
				gluPerspective(30.0, (double)width / (double)height, 1.0f, 1000.0f);
			#endif

				// ���_���W�ϊ�
				::glMatrixMode( GL_MODELVIEW );
				::glLoadIdentity();
				::gluLookAt (	0.0, 0.0, 10.0,	// ���_
								0.0, 0.0,   0.0,	// �����_
								0.0, 1.0,   0.0 );	// ��x�N�g��

				m_State = eState_Update;

				break;
			}
			case eState_Update :

				//�Q�[�����[�v
				GAME_LOOP_START
				{

				// �h���C�o�Ƀo�O�̂���O���t�B�b�N�J�[�h�ɂ����Ă�SDI�̍ŏ��̕`��ƁA
				// ���ׂẴO���t�B�b�N�J�[�h�ɂ����Ă�MDI�`��ɂ�����
				// �ȉ��̖��߂́A���ʂ𔭊�����B
				::wglMakeCurrent( m_hDC, m_hRC );

				//�J���[�o�b�t�@�Ɛ[�x�o�b�t�@�̃N���A
				::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				// �f�v�X�o�b�t�@�̃N���A
				::glClearDepth( 1.0f );

//--------------------VBO�̕`��--------------------------------------��
#if ON_
				static int anglex = 0;
				glPushMatrix();
				glRotatef(float(anglex),1.f,0.f,0.f);//x����]

				GLfloat *clientPtr;//�N���C�A���g���p
				GLfloat tmp[3];
				int idloop;
				int loop;
				static float angle = 2*PI_OVER_180;

				//�f�[�^�̏ꏊ��m�点��
				//���W
				glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
				glVertexPointer(3, GL_FLOAT, 0, 0);
				//�@��
				glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
				glNormalPointer(GL_FLOAT, 0, 0);
				//�F
				glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
				glColorPointer(3,GL_FLOAT, 0, 0);

				//�C���f�b�N�X���o�C���h
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndex);
 
				glEnableClientState(GL_VERTEX_ARRAY);
				glEnableClientState(GL_NORMAL_ARRAY);
				glEnableClientState(GL_COLOR_ARRAY);
				glEnableClientState(GL_INDEX_ARRAY);

				// �V�F�[�_�̐؂�ւ�
				glUseProgram(m_hProgramObject);

				// ���C�g�ʒu�̐ݒ�
				float lightPosition[3] = {2.0f, 8.0f, -10.0f };
				glUniform3fv(m_locLightPos, 1, &lightPosition[0]);

				//�`��
				glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,0);
	
				glDisableClientState(GL_INDEX_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);

				//���W�Ɩ@������]������
				for(idloop = 0; idloop < 2;++idloop)
				{
					//idloop�Ԗڂ̃o�b�t�@�I�u�W�F�N�g�ɒ���
					glBindBuffer(GL_ARRAY_BUFFER,VboId[idloop]);
		
					//�Ή��t��
					clientPtr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,
															GL_READ_WRITE);
					if(clientPtr != NULL)
					{
						//24���_*3���W
						 for(loop = 0; loop < 24*3;loop += 3)	{
							//�ǂݏo��(READ)
							tmp[0] = clientPtr[loop];
							tmp[1] = clientPtr[loop+1];
							tmp[2] = clientPtr[loop+2];
							//��������(WRITE)
							clientPtr[loop] = cos(angle)*tmp[0]
												+ sin(angle)*tmp[2];
							clientPtr[loop+1] = tmp[1];
							clientPtr[loop+2] = -sin(angle)*tmp[0]
													+ cos(angle)*tmp[2];
						 }
						glUnmapBuffer(GL_ARRAY_BUFFER);//�Ή��t���̉��
					}
				}
				if(++anglex >= 360) anglex = 0;

				glPopMatrix();
#endif
//--------------------VBO�̕`��--------------------------------------��

				//�`��R�}���h��S���]��
				glFinish();

				//�o�b�N�o�b�t�@���t�����g��
				SwapBuffers( m_hDC );

				}
				GAME_LOOP_END


				break;
    		}

		}
	}
};
static CTestThread *s_Thread = skyNull;

TEST( opengl , 3 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//Window�̍쐬
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= window_name;
	wcex.lpszClassName	= window_name;
	wcex.hIconSm		= 0;
	RegisterClassEx(&wcex);

	m_hWnd = CreateWindow(window_name, window_name, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if ( !m_hWnd )
	{
		SKY_PANIC_MSG( _T( "Window�쐬�̎��s \n" ) );
	}

	ShowWindow( m_hWnd , SW_SHOWDEFAULT );
	UpdateWindow( m_hWnd );

	s_Thread = NEW CTestThread();
	CThreadManager_::CreateThread( s_Thread );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//��u�҂i�X���b�h��؂�ւ���j
		CTimeUtil_::Wait( 1 );
	}
    GAME_LOOP_END

	//�`��p�X���b�h�̒�~
	CRenderCommandManager_::StopThread();

	DestroyWindow( m_hWnd );
	UnregisterClass( window_name , hInstance );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

//================================================================================================================

#if OFF_

//------- ���_�f�[�^-----------//
GLfloat vertexAry[][3] =
{
	//1�߂̖�(v0-v1-v2-v3)
	{ -0.5f ,  0.5f , 0.0f } , //0:���� 
	{  0.5f ,  0.5f , 0.0f } , //1:�E��
	{ -0.5f , -0.5f , 0.0f } , //2:����
	{  0.5f , -0.5f , 0.0f } , //3:�E��
};

//�@���f�[�^
GLfloat normalAry[][3] =
{
	{ 0.0f , 0.0f , -1.0f } , 
	{ 0.0f , 0.0f , -1.0f } , 
	{ 0.0f , 0.0f , -1.0f } , 
	{ 0.0f , 0.0f , -1.0f } , 
};

//�F�f�[�^
GLfloat colorAry[][3] =
{
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
};

//�t�u�f�[�^
GLfloat uvAry[][2] =
{
	{ 0.000000f , 1.000000f } , //����
	{ 1.000000f , 1.000000f } , //�E��
	{ 0.000000f , 0.000000f } , //����
	{ 1.000000f , 0.000000f } , //�E��
};

#define LH_

#ifdef LH_

//�C���f�b�N�X�i�����v����j������W�n
GLubyte indexAry[]=
{
   0 , 2 , 1 ,
   2 , 3 , 1
};

#else

//�C���f�b�N�X�i���v���j �E����W�n
GLubyte indexAry[]=
{
   0 , 1 , 2 ,
   2 , 1 , 3
};


#endif

// ��/180�̒l
const float PI_OVER_180 = 0.0174532925f;

//VBO�pID
GLuint VboId[4];//���W�A�@���A�F�A�t�u
GLuint Location[4];//���W�A�@���A�F�A�t�u
GLuint VboIndex;//�C���f�b�N�X
GLuint numIndices;
GLuint TexId[1]; //�e�N�X�`��
GLint textureLocation;

static const skyString* window_name = _T( "testwindow" );

static skyBool bExit = skyFalse;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		bExit = skyTrue;
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

TEST( opengl , 5 ) 
{
    SKY_LIB_GRAPHIC_INIT();

	//Window�̍쐬
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= 0;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= window_name;
    wcex.lpszClassName	= window_name;
    wcex.hIconSm		= 0;
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(window_name, window_name, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if ( !hWnd )
    {
		SKY_PANIC_MSG( _T( "Window�쐬�̎��s \n" ) );
    }

   ShowWindow( hWnd , SW_SHOWDEFAULT );
   UpdateWindow( hWnd );

	//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
	RECT clientArea;
	GetClientRect( hWnd, &clientArea );
	int width  = clientArea.right - clientArea.left;
	int height = clientArea.bottom -clientArea.top;

   HDC hDC = NULL;
   HGLRC hRC = NULL;
	// �f�o�C�X�R���e�L�X�g�̎擾�A�ݒ�
	hDC = GetDC( hWnd );

	if( NULL == hDC )
	{
		SKY_PANIC_MSG( _T( "�f�o�C�X�R���e�L�X�g�̍쐬�̎��s \n" ) );
	}

	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( hDC, &pfd ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	if( TRUE != ::SetPixelFormat( hDC, iPixelFormat, &pfd ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g�̍쐬
	if( 0 == ( hRC = ::wglCreateContext( hDC ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g���J�����g�̃f�o�C�X�R���e�L�X�g�ɐݒ�
	if( TRUE != ::wglMakeCurrent( hDC, hRC ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

   // GLEW������
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        SKY_PANIC_MSG(_T( "Error: %s\n" ), glewGetErrorString( err ) );
    }

	//�g���`�F�b�N
	if(!glewIsExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
        SKY_PANIC_MSG(_T( "you Can't use VBO \n" ), glewGetErrorString( err ) );
	}

	// �V�F�[�_�̓ǂݍ���
	GLuint hProgramObject;
	sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(hProgramObject ,_T( "data/test/glsl/GLSL.vert"), _T("data/test/glsl/GLSL.frag") );
	
	// �I�u�W�F�N�g�̃��P�[�V�����̏���
	GLint locLightPos = glGetUniformLocation(hProgramObject, "LightPos");

//--------------------VBO�̍쐬--------------------------------------��

	glGenBuffers(3,&VboId[0]);//���W��@����F�̂R��
	
	//���_
	glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexAry),
				vertexAry,GL_DYNAMIC_DRAW);//�f�[�^�ύX����

	//�@��
	glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(normalAry),
				normalAry,GL_DYNAMIC_DRAW);//�f�[�^�ύX����

	//�F
	glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colorAry),
					colorAry,GL_STREAM_DRAW);//�f�[�^�ύX�Ȃ�
	//�t�u
	glBindBuffer(GL_ARRAY_BUFFER,VboId[3]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(uvAry),
					colorAry,GL_STREAM_DRAW);//�f�[�^�ύX�Ȃ�

	//�C���f�b�N�X
	glGenBuffers(1,&VboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexAry),
					indexAry,GL_STATIC_DRAW);//�f�[�^�ύX�Ȃ�

//--------------------VBO�̍쐬--------------------------------------��

	// �N���A�J���[�̐ݒ�
	::glClearColor( 0, 0, 0, 0 );	// ��

	// �f�v�X�o�b�t�@�̃N���A
	::glClearDepth( 1.0f );

	// �f�v�X�e�X�g��L���ɂ���
	::glEnable( GL_DEPTH_TEST );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	::glDepthFunc( GL_LEQUAL );

	//�r���[�|�[�g
	::glViewport(clientArea.left,clientArea.top,width,height);

	//�\�����`��
	glEnable(GL_CULL_FACE); 
	glCullFace(GL_BACK);

#ifdef LH_
		glFrontFace(GL_CCW); //�����v���肪�\�i�f�t�H���g�j
#else
		glFrontFace(GL_CW);  //���v��肪�\
#endif

	// ����p����ݒ�
	::glMatrixMode( GL_PROJECTION );
	::glLoadIdentity();

#define DISP_2D
#ifdef DISP_2D
	::glOrtho(	- width * 0.5 / 10.0,
				  width * 0.5 / 10.0,
				- height * 0.5 / 10.0,
				  height * 0.5 / 10.0,
				  0.1,
				  1000.0 );
#else
    gluPerspective(30.0, (double)width / (double)height, 1.0f, 1000.0f);
#endif

	// ���_���W�ϊ�
	::glMatrixMode( GL_MODELVIEW );
	::glLoadIdentity();
	::gluLookAt (	0.0, 0.0, 10.0,	 // ���_
					0.0, 0.0, 0.0,	 // �����_
					0.0, 1.0, 0.0 ); // ��x�N�g��

//---------�e�N�X�`��

	IplImage *pTex = cvLoadImage("data/test/texture/OpenGLTexMap.jpg",CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if ( pTex == NULL )
	{
		SKY_PANIC_MSG( _T( "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B" ) );
	}

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//���͊Ǘ��X�V
		Ckeyboard_->Update( 0 );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		// �h���C�o�Ƀo�O�̂���O���t�B�b�N�J�[�h�ɂ����Ă�SDI�̍ŏ��̕`��ƁA
		// ���ׂẴO���t�B�b�N�J�[�h�ɂ����Ă�MDI�`��ɂ�����
		// �ȉ��̖��߂́A���ʂ𔭊�����B
		::wglMakeCurrent( hDC, hRC );

		//�J���[�o�b�t�@�Ɛ[�x�o�b�t�@�̃N���A
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// �f�v�X�o�b�t�@�̃N���A
		::glClearDepth( 1.0f );

//--------------------VBO�̕`��--------------------------------------��
#if ON_

		static int anglex = 0;
		glPushMatrix();
		glRotatef(float(anglex),1.f,0.f,0.f);//x����]

		GLfloat *clientPtr;//�N���C�A���g���p
		GLfloat tmp[3];
		int idloop;
		int loop;
		static float angle = 2*PI_OVER_180;

		//�f�[�^�̏ꏊ��m�点��
		//���W
		glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		//�@��
		glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
		glNormalPointer(GL_FLOAT, 0, 0);

		//�F
		glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
		glColorPointer(3,GL_FLOAT, 0, 0);

		//�C���f�b�N�X���o�C���h
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndex);
 
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// �V�F�[�_�̐؂�ւ�
		glUseProgram(hProgramObject);

		// ���C�g�ʒu�̐ݒ�
		float lightPosition[3] = {0.0f, 0.0f, 1.0f };
		glUniform3fv(locLightPos, 1, &lightPosition[0]);

		//�`��
		glDrawElements(GL_TRIANGLES,24,GL_UNSIGNED_BYTE,0);
	
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

/*
		//���W�Ɩ@������]������
		for(idloop = 0; idloop < 2;++idloop)
		{
			//idloop�Ԗڂ̃o�b�t�@�I�u�W�F�N�g�ɒ���
			glBindBuffer(GL_ARRAY_BUFFER,VboId[idloop]);
		
			//�Ή��t��
			clientPtr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,
													GL_READ_WRITE);
			if(clientPtr != NULL)
			{
				//24���_*3���W
				 for(loop = 0; loop < 4*3;loop += 3)	{
					//�ǂݏo��(READ)
					tmp[0] = clientPtr[loop];
					tmp[1] = clientPtr[loop+1];
					tmp[2] = clientPtr[loop+2];
					//��������(WRITE)
					clientPtr[loop] = cos(angle)*tmp[0]
										+ sin(angle)*tmp[2];
					clientPtr[loop+1] = tmp[1];
					clientPtr[loop+2] = -sin(angle)*tmp[0]
											+ cos(angle)*tmp[2];
				 }
				glUnmapBuffer(GL_ARRAY_BUFFER);//�Ή��t���̉��
			}
		}
		if(++anglex >= 360) anglex = 0;
*/

		glPopMatrix();
#endif
//--------------------VBO�̕`��--------------------------------------��

		//�`��R�}���h��S���]��
		::glFinish();

		//�o�b�N�o�b�t�@���t�����g��
		::SwapBuffers( hDC );
	}
	GAME_LOOP_END

	::wglMakeCurrent(hDC,0);
	::wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );

	DestroyWindow( hWnd );
	UnregisterClass( window_name , hInstance );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

//================================================================================================================

#if OFF_

GLfloat vertexAry[][3] =
{
	//1�߂̖�(v0-v1-v2-v3)
	{ 1.000000f , 1.000000f , -1.000000f } , 
	{ 1.000000f , -1.000000f , -1.000000f } , 
	{ -1.000000f , 1.000000f , -1.000000f } , 
	{ -1.000000f , -1.000000f , -1.000000f } , 
	{ -1.000000f , -1.000000f , 1.000000f } , 
	{ -1.000000f , 1.000000f , 1.000000f } , 
	{ -1.000000f , -1.000000f , -1.000000f } , 
	{ -1.000000f , 1.000000f , -1.000000f } , 
	{ 1.000000f , -1.000000f , 1.000000f } , 
	{ 0.999999f , 1.000000f , 1.000001f } , 
	{ -1.000000f , 1.000000f , 1.000000f } , 
	{ 1.000000f , -1.000000f , 1.000000f } , 
	{ -1.000000f , 1.000000f , 1.000000f } , 
	{ -1.000000f , -1.000000f , 1.000000f } , 
	{ 1.000000f , -1.000000f , -1.000000f } , 
	{ 1.000000f , 1.000000f , -1.000000f } , 
	{ 0.999999f , 1.000000f , 1.000001f } , 
	{ 1.000000f , -1.000000f , -1.000000f } , 
	{ 0.999999f , 1.000000f , 1.000001f } , 
	{ 1.000000f , -1.000000f , 1.000000f } , 
	{ 1.000000f , 1.000000f , -1.000000f } , 
	{ -1.000000f , 1.000000f , -1.000000f } , 
	{ -1.000000f , 1.000000f , 1.000000f } , 
	{ 0.999999f , 1.000000f , 1.000001f } , 
	{ 1.000000f , -1.000000f , -1.000000f } , 
	{ 1.000000f , -1.000000f , 1.000000f } , 
	{ -1.000000f , -1.000000f , -1.000000f } , 
	{ -1.000000f , -1.000000f , 1.000000f } , 
};

//�@���f�[�^
GLfloat normalAry[][3] =
{
	{ 0.000000f , 0.000000f , -1.000000f } , 
	{ 0.000000f , 0.000000f , -1.000000f } , 
	{ 0.000000f , 0.000000f , -1.000000f } , 
	{ 0.000000f , 0.000000f , -1.000000f } , 
	{ -1.000000f , -0.000000f , -0.000000f } , 
	{ -1.000000f , -0.000000f , -0.000000f } , 
	{ -1.000000f , -0.000000f , -0.000000f } , 
	{ -1.000000f , -0.000000f , -0.000000f } , 
	{ -0.000001f , -0.000000f , 1.000000f } , 
	{ -0.000001f , -0.000000f , 1.000000f } , 
	{ -0.000001f , -0.000000f , 1.000000f } , 
	{ -0.000000f , 0.000000f , 1.000000f } , 
	{ -0.000000f , 0.000000f , 1.000000f } , 
	{ -0.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , -0.000000f , 0.000001f } , 
	{ 1.000000f , -0.000000f , 0.000001f } , 
	{ 1.000000f , -0.000000f , 0.000001f } , 
	{ 1.000000f , 0.000000f , 0.000000f } , 
	{ 1.000000f , 0.000000f , 0.000000f } , 
	{ 1.000000f , 0.000000f , 0.000000f } , 
	{ 0.000000f , 1.000000f , -0.000000f } , 
	{ 0.000000f , 1.000000f , -0.000000f } , 
	{ 0.000000f , 1.000000f , -0.000000f } , 
	{ 0.000000f , 1.000000f , -0.000000f } , 
	{ -0.000000f , -1.000000f , 0.000000f } , 
	{ -0.000000f , -1.000000f , 0.000000f } , 
	{ -0.000000f , -1.000000f , 0.000000f } , 
	{ -0.000000f , -1.000000f , 0.000000f } , 
};

//�F�f�[�^
GLfloat colorAry[][3] =
{
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 1.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 0.000000f } , 
	{ 0.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 0.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000001f , 0.000000f , 1.000000f } , 
	{ 1.000001f , 0.000000f , 1.000000f } , 
	{ 1.000001f , 1.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 0.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000001f } , 
	{ 1.000000f , 0.000000f , 1.000001f } , 
	{ 1.000000f , 0.000000f , 1.000001f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
	{ 1.000000f , 0.000000f , 1.000000f } , 
};

//�C���f�b�N�X
GLubyte indexAry[]=
{
   0,1,2,
   1,3,2,
   4,5,6,
   5,7,6,
   8,9,10,
   11,12,13,
   14,15,16,
   17,18,19,
   20,21,22,
   20,22,23,
   24,25,26,
   25,27,26
};

// ��/180�̒l
const float PI_OVER_180 = 0.0174532925f;

//VBO�pID
GLuint VboId[3];//�R��
GLuint VboIndex;//�C���f�b�N�X

static const skyString* window_name = _T( "testwindow" );

static skyBool bExit = skyFalse;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		bExit = skyTrue;
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

TEST( opengl , 5 ) 
{
    SKY_LIB_GRAPHIC_INIT();

	//Window�̍쐬
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= 0;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= window_name;
    wcex.lpszClassName	= window_name;
    wcex.hIconSm		= 0;
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(window_name, window_name, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if ( !hWnd )
    {
		SKY_PANIC_MSG( _T( "Window�쐬�̎��s \n" ) );
    }

   ShowWindow( hWnd , SW_SHOWDEFAULT );
   UpdateWindow( hWnd );

	//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
	RECT clientArea;
	GetClientRect( hWnd, &clientArea );
	int width  = clientArea.right - clientArea.left;
	int height = clientArea.bottom -clientArea.top;

   HDC hDC = NULL;
   HGLRC hRC = NULL;
	// �f�o�C�X�R���e�L�X�g�̎擾�A�ݒ�
	hDC = GetDC( hWnd );

	if( NULL == hDC )
	{
		SKY_PANIC_MSG( _T( "�f�o�C�X�R���e�L�X�g�̍쐬�̎��s \n" ) );
	}

	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( hDC, &pfd ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	if( TRUE != ::SetPixelFormat( hDC, iPixelFormat, &pfd ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g�̍쐬
	if( 0 == ( hRC = ::wglCreateContext( hDC ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g���J�����g�̃f�o�C�X�R���e�L�X�g�ɐݒ�
	if( TRUE != ::wglMakeCurrent( hDC, hRC ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

   // GLEW������
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        SKY_PANIC_MSG(_T( "Error: %s\n" ), glewGetErrorString( err ) );
    }

	//�g���`�F�b�N
	if(!glewIsExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
        SKY_PANIC_MSG(_T( "you Can't use VBO \n" ), glewGetErrorString( err ) );
	}

	// �V�F�[�_�̓ǂݍ���
	GLuint hProgramObject;
	sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(hProgramObject ,_T( "data/test/glsl/GLSL.vert"), _T("data/test/glsl/GLSL.frag") );
	
	// �I�u�W�F�N�g�̃��P�[�V�����̏���
	GLint locLightPos = glGetUniformLocation(hProgramObject, "LightPos");

//--------------------VBO�̍쐬--------------------------------------��

	glGenBuffers(3,&VboId[0]);//���W��@����F�̂R��
	
	//���_
	glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexAry),
				vertexAry,GL_DYNAMIC_DRAW);//�f�[�^�ύX����

	//�@��
	glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(normalAry),
				normalAry,GL_DYNAMIC_DRAW);//�f�[�^�ύX����

	//�F
	glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(colorAry),
					colorAry,GL_STREAM_DRAW);//�f�[�^�ύX�Ȃ�

	//�C���f�b�N�X
	glGenBuffers(1,&VboIndex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,VboIndex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indexAry),
					indexAry,GL_STATIC_DRAW);//�f�[�^�ύX�Ȃ�

//--------------------VBO�̍쐬--------------------------------------��

	// �N���A�J���[�̐ݒ�
	::glClearColor( 0, 0, 0, 0 );	// ��

	// �f�v�X�o�b�t�@�̃N���A
	::glClearDepth( 1.0f );

	// �f�v�X�e�X�g��L���ɂ���
	::glEnable( GL_DEPTH_TEST );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	::glDepthFunc( GL_LEQUAL );

	//�r���[�|�[�g
	::glViewport(clientArea.left,clientArea.top,width,height);

#ifdef LH_
		glFrontFace(GL_CCW); //�����v���肪�\�i�f�t�H���g�j
#else
		glFrontFace(GL_CW);  //���v��肪�\
#endif

//	glEnable(GL_CULL_FACE); 
//	glCullFace(GL_BACK);

	// ����p����ݒ�
	::glMatrixMode( GL_PROJECTION );
	::glLoadIdentity();
#if OFF_
	::glOrtho(	- width * 0.5 / 10.0,
				  width * 0.5 / 10.0,
				- height * 0.5 / 10.0,
				  height * 0.5 / 10.0,
				  0.1,
				  1000.0 );
#else
    gluPerspective(30.0, (double)width / (double)height, 1.0f, 1000.0f);
#endif

	// ���_���W�ϊ�
	::glMatrixMode( GL_MODELVIEW );
	::glLoadIdentity();
	::gluLookAt (	0.0, 0.0, 10.0,	 // ���_
					0.0, 0.0, 0.0,	 // �����_
					0.0, 1.0, 0.0 ); // ��x�N�g��

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//���͊Ǘ��X�V
		Ckeyboard_->Update( 0 );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		// �h���C�o�Ƀo�O�̂���O���t�B�b�N�J�[�h�ɂ����Ă�SDI�̍ŏ��̕`��ƁA
		// ���ׂẴO���t�B�b�N�J�[�h�ɂ����Ă�MDI�`��ɂ�����
		// �ȉ��̖��߂́A���ʂ𔭊�����B
		::wglMakeCurrent( hDC, hRC );

		//�J���[�o�b�t�@�Ɛ[�x�o�b�t�@�̃N���A
		::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// �f�v�X�o�b�t�@�̃N���A
		::glClearDepth( 1.0f );

//--------------------VBO�̕`��--------------------------------------��
#if ON_

		static int anglex = 0;
		glPushMatrix();
		glRotatef(float(anglex),1.f,0.f,0.f);//x����]

		GLfloat *clientPtr;//�N���C�A���g���p
		GLfloat tmp[3];
		int idloop;
		int loop;
		static float angle = 2*PI_OVER_180;

		//�f�[�^�̏ꏊ��m�点��
		//���W
		glBindBuffer(GL_ARRAY_BUFFER,VboId[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		//�@��
		glBindBuffer(GL_ARRAY_BUFFER,VboId[1]);
		glNormalPointer(GL_FLOAT, 0, 0);
		//�F
		glBindBuffer(GL_ARRAY_BUFFER,VboId[2]);
		glColorPointer(3,GL_FLOAT, 0, 0);

		//�C���f�b�N�X���o�C���h
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboIndex);
 
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// �V�F�[�_�̐؂�ւ�
		glUseProgram(hProgramObject);

		// ���C�g�ʒu�̐ݒ�
		float lightPosition[3] = {0.0f, 0.0f, 1.0f };
		glUniform3fv(locLightPos, 1, &lightPosition[0]);

		//�`��
		glDrawElements(GL_TRIANGLES, array_sizeof( indexAry ) ,GL_UNSIGNED_BYTE,0);
	
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);


		//���W�Ɩ@������]������
		for(idloop = 0; idloop < 2;++idloop)
		{
			//idloop�Ԗڂ̃o�b�t�@�I�u�W�F�N�g�ɒ���
			glBindBuffer(GL_ARRAY_BUFFER,VboId[idloop]);
		
			//�Ή��t��
			clientPtr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,
													GL_READ_WRITE);
			if(clientPtr != NULL)
			{
				//24���_*3���W
				 for(loop = 0; loop < array_sizeof( vertexAry )*3;loop += 3)	{
					//�ǂݏo��(READ)
					tmp[0] = clientPtr[loop];
					tmp[1] = clientPtr[loop+1];
					tmp[2] = clientPtr[loop+2];
					//��������(WRITE)
					clientPtr[loop] = cos(angle)*tmp[0]
										+ sin(angle)*tmp[2];
					clientPtr[loop+1] = tmp[1];
					clientPtr[loop+2] = -sin(angle)*tmp[0]
											+ cos(angle)*tmp[2];
				 }
				glUnmapBuffer(GL_ARRAY_BUFFER);//�Ή��t���̉��
			}
		}
		if(++anglex >= 360) anglex = 0;


		glPopMatrix();
#endif
//--------------------VBO�̕`��--------------------------------------��

		//�`��R�}���h��S���]��
		::glFinish();

		//�o�b�N�o�b�t�@���t�����g��
		::SwapBuffers( hDC );
	}
	GAME_LOOP_END

	::wglMakeCurrent(hDC,0);
	::wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );

	DestroyWindow( hWnd );
	UnregisterClass( window_name , hInstance );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

//================================================================================================================

#if OFF_

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "glut/glut.h"

GLuint programObject;
GLuint vertexShader;
GLuint fragmentShader;

GLint modelLocation;
GLint viewLocation;
GLint projectionLocation;
GLfloat viewMatrix[16];
GLfloat projectionMatrix[16];

class Plane
{
private:
    GLuint vid[4];   // vertex, normal, texcoord, index
    GLint vertexLocation;
    GLint normalLocation;
    GLint texCoordLocation;
    GLuint texId[1];
    GLint textureLocation;
    GLuint numIndices;
 
    GLfloat matrix[16];
 
public:
    void setup(GLuint programObj)
    {
        // ���ʍ쐬
 
        GLuint numVertices;
        numVertices = 4;
        numIndices = 6;
 
        const GLfloat vertices[] = {
            -1.0f, 0.0f, -1.0f,
            +1.0f, 0.0f, -1.0f,
            +1.0f, 0.0f, +1.0f,
            -1.0f, 0.0f, +1.0f
        };
        const GLfloat normals[] = {
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
        };
        const GLfloat texCoords[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };
        const GLubyte indices[] = {
            0, 2, 1,
            0, 3, 2
        };
 
        ////////////////////////////////////////////////////////////////
 
        // in(attribute)�ϐ��ւ̊֘A�t��
        vertexLocation = glGetAttribLocation(programObj, "a_vertex");
        normalLocation = glGetAttribLocation(programObj, "a_normal");
        texCoordLocation = glGetAttribLocation(programObj, "a_texcoord");
 
        // uniform�ϐ��ւ̊֘A�t��
        textureLocation = glGetUniformLocation(programObj, "texture0");
 
        glGenBuffers(4, vid);
        // ���_�f�[�^��GPU���������֓]��
        glBindBuffer(GL_ARRAY_BUFFER, vid[0]);
        glBufferData(GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        // �@���f�[�^��GPU���������֓]��
        glBindBuffer(GL_ARRAY_BUFFER, vid[1]);
        glBufferData(GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), normals, GL_STATIC_DRAW);
        // �e�N�X�`�����W�f�[�^��GPU���������֓]��
        glBindBuffer(GL_ARRAY_BUFFER, vid[2]);
        glBufferData(GL_ARRAY_BUFFER, 2*numVertices*sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
        // ���_�ԍ��f�[�^��GPU���������֓]��
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vid[3]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLubyte), indices, GL_STATIC_DRAW);
 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
 
        ////////////////////////////////////////////////////////////////
        // texture
        const GLubyte imageData[] = { // RGBA
            255, 0, 0, 255,    // Red
            0, 255, 0, 255,    // Green
            0, 0, 255, 255,    // Blue
            255, 255, 0, 255,  // Yellow
        };
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 1�f�[�^��1�o�C�g����
 
        // �e�N�X�`���f�[�^��GPU���������֓]��
        glGenTextures(1, texId);
        glActiveTexture(GL_TEXTURE0); // �e�N�X�`�����j�b�g�ԍ� 0
        glBindTexture(GL_TEXTURE_2D, texId[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
        glBindTexture(GL_TEXTURE_2D, 0);
 
        ////////////////////////////////////////////////////////////////
 
        angle = 0.0;
    }
 
    void render()
    {
        // vertex
        glBindBuffer(GL_ARRAY_BUFFER, vid[0]);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(vertexLocation);
 
        // normal
        glBindBuffer(GL_ARRAY_BUFFER, vid[1]);
        glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
        glEnableVertexAttribArray(normalLocation);
 
        // texcoord
        glBindBuffer(GL_ARRAY_BUFFER, vid[2]);
        glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(texCoordLocation);
 
        // texture
        glUniform1i(textureLocation, 0); // �e�N�X�`�����j�b�g�ԍ� 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texId[0]);
 
        // index
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vid[3]);
 
        // draw
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_BYTE, 0);
    }
 
    float angle;
    void update() {
        graphic::sl_LoadIdentityf(matrix);
        graphic::sl_Scalef(matrix, 2.0f, 2.0f, 2.0f);   // 2�{�ɂ���
        graphic::sl_Translatef(matrix, 0.0f, 0.0f, 0.0f);
        graphic::sl_Rotatef(matrix, angle, 1.0f, 0.0f, 0.0f);
        angle += 1.0f;
        if (angle > 360.0f) angle = 0.0f;
    }
 
    GLfloat* m() {
        return matrix;
    }
};
Plane plane;

static const skyString* window_name = _T( "testwindow" );

static skyBool bExit = skyFalse;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:

		glViewport(0, 0, LOWORD( lParam ) , HIWORD( lParam ) ); 
 
		graphic::sl_LoadIdentityf(projectionMatrix);
		graphic::sl_Perspectivef(projectionMatrix, 30, (GLfloat)LOWORD( lParam )/(GLfloat)HIWORD( lParam ), 1.0f, 100.0f);

		break;

	case WM_DESTROY:
		bExit = skyTrue;
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

// prototype
void display();
void reshape(int w, int h);
void timer(int t);
void init();
bool initGlsl();
void setupParams();
GLuint LoadShader(GLenum type, const char *fileName);
 
 
void display()
{
    plane.update();
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


 
    glUseProgram(programObject);
 
    // transform
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
 
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, plane.m());
    plane.render();
 
    glUseProgram(0);
 
    glutSwapBuffers();
}
 
 
void reshape(int w, int h)
{
    glViewport(0, 0, w, h); 
 
    graphic::sl_LoadIdentityf(projectionMatrix);
    graphic::sl_Perspectivef(projectionMatrix, 30, (GLfloat)w/(GLfloat)h, 1.0f, 100.0f);
}
 
 
void timer(int t)
{
    glutPostRedisplay();
    glutTimerFunc(t, timer, 17);
}
 
 
void init()
{
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepth(1.0f);
 
    graphic::sl_LoadIdentityf(viewMatrix);
    graphic::sl_LookAtf(viewMatrix, 3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
 
 
bool initGlsl()
{
    programObject = glCreateProgram();
    if (programObject == 0) return false;
 
    vertexShader = LoadShader(GL_VERTEX_SHADER, "data/test/glsl/TextureMapping.vert");
    glAttachShader(programObject, vertexShader);
 
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, "data/test/glsl/TextureMapping.frag");
    glAttachShader(programObject, fragmentShader);
 
    GLint linked;
    glLinkProgram(programObject);
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
 
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char *infoLog = new char[sizeof(char)*infoLen];
 
            glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
            std::cerr << "Error linking program:\n" << infoLog << "\n";
            delete [] infoLog;
        }
        glDeleteProgram(programObject);
        return false;
    }
 
    return true;
}
 
void setupParams()
{
    // Fragment data
 
    glBindFragDataLocation(programObject, 0, "fragColor");
 
    // Transform Matrix
 
    modelLocation = glGetUniformLocation(programObject, "modelMatrix");
    viewLocation = glGetUniformLocation(programObject, "viewMatrix");
    projectionLocation = glGetUniformLocation(programObject, "projectionMatrix");
 
    // objects
 
    plane.setup(programObject);
}
 
GLuint LoadShader(GLenum type, const char *fileName)
{
    GLuint shader;
    GLint compiled;
 
    std::fstream inputFile(fileName);
    std::istreambuf_iterator<char> dataBegin(inputFile);
    std::istreambuf_iterator<char> dataEnd;
    std::string fileData(dataBegin, dataEnd);
    const char *file = fileData.c_str();
 
    shader = glCreateShader(type);
    if (shader == 0) return 0;
 
    glShaderSource(shader, 1, &file, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = new char[sizeof(char)*infoLen];
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cerr << "Error compiling shader: " << fileName << "\n" << infoLog << "\n";
            delete [] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
 
TEST( opengl , 6 ) 
{

#if ON_

    SKY_LIB_GRAPHIC_INIT();

	//Window�̍쐬
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= 0;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= window_name;
    wcex.lpszClassName	= window_name;
    wcex.hIconSm		= 0;
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(window_name, window_name, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if ( !hWnd )
    {
		SKY_PANIC_MSG( _T( "Window�쐬�̎��s \n" ) );
    }

   ShowWindow( hWnd , SW_SHOWDEFAULT );
   UpdateWindow( hWnd );

	//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
	RECT clientArea;
	GetClientRect( hWnd, &clientArea );
	int width  = clientArea.right - clientArea.left;
	int height = clientArea.bottom -clientArea.top;

   HDC hDC = NULL;
   HGLRC hRC = NULL;
	// �f�o�C�X�R���e�L�X�g�̎擾�A�ݒ�
	hDC = GetDC( hWnd );

	if( NULL == hDC )
	{
		SKY_PANIC_MSG( _T( "�f�o�C�X�R���e�L�X�g�̍쐬�̎��s \n" ) );
	}

	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( hDC, &pfd ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	if( TRUE != ::SetPixelFormat( hDC, iPixelFormat, &pfd ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g�̍쐬
	if( 0 == ( hRC = ::wglCreateContext( hDC ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g���J�����g�̃f�o�C�X�R���e�L�X�g�ɐݒ�
	if( TRUE != ::wglMakeCurrent( hDC, hRC ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

   // GLEW������
	glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        SKY_PANIC_MSG(_T( "Error: %s\n" ), glewGetErrorString( err ) );
    }

	//�g���`�F�b�N
	if(!glewIsExtensionSupported("GL_ARB_vertex_buffer_object"))
	{
        SKY_PANIC_MSG(_T( "you Can't use VBO \n" ), glewGetErrorString( err ) );
	}

	// �V�F�[�_�̓ǂݍ���
	sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(programObject ,_T( "data/test/glsl/TextureMapping.vert"), _T("data/test/glsl/TextureMapping.frag") );

    // Fragment data
 
    glBindFragDataLocation(programObject, 0, "fragColor");
 
    // Transform Matrix
 
    modelLocation = glGetUniformLocation(programObject, "modelMatrix");
    viewLocation = glGetUniformLocation(programObject, "viewMatrix");
    projectionLocation = glGetUniformLocation(programObject, "projectionMatrix");
 
    // objects
 
    plane.setup(programObject);

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepth(1.0f);

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	::glDepthFunc( GL_LEQUAL );
 
    graphic::sl_LoadIdentityf(viewMatrix);
    graphic::sl_LookAtf(viewMatrix, 3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glViewport(0, 0, width, height); 
 
    graphic::sl_LoadIdentityf(projectionMatrix);
    graphic::sl_Perspectivef(projectionMatrix, 30, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f);

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//���͊Ǘ��X�V
		Ckeyboard_->Update( 0 );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		// �h���C�o�Ƀo�O�̂���O���t�B�b�N�J�[�h�ɂ����Ă�SDI�̍ŏ��̕`��ƁA
		// ���ׂẴO���t�B�b�N�J�[�h�ɂ����Ă�MDI�`��ɂ�����
		// �ȉ��̖��߂́A���ʂ𔭊�����B
		::wglMakeCurrent( hDC, hRC );

		plane.update();
 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
		glUseProgram(programObject);
 
		// transform
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewMatrix);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix);
 
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, plane.m());
		plane.render();
 
		glUseProgram(0);
 
		//�`��R�}���h��S���]��
		::glFinish();

		//�o�b�N�o�b�t�@���t�����g��
		::SwapBuffers( hDC );
	}
	GAME_LOOP_END

#else

    int argc = 1;
	char *argv[] = { "" };
    glutInit(&argc, argv);
    glutInitWindowSize(600, 400);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("test");


    // GL�̃x���_�[�A�y�уo�[�W���� 
    const GLubyte *str = glGetString(GL_VENDOR);
    printf("VND:%s\n", str);
    str = glGetString(GL_RENDERER);
    printf("RND:%s\n",str);
    str = glGetString(GL_VERSION);
    printf("VER:%s\n", str);
 
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(100, timer, 17);
 
    GLenum err;
    glewExperimental = GL_TRUE;
    err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "GLEW error : " << glewGetErrorString(err) << "\n";
        std::exit(1);
    }   
 
    init();
    initGlsl();
    setupParams();
 
    glutMainLoop();

#endif
	SKY_LIB_GRAPHIC_TERM();
}

#endif

//================================================================================================================

#if OFF_

//------- ���_�f�[�^-----------//
GLfloat vertexAry[] =
{
	//1�߂̖�(v0-v1-v2-v3)
    -1.0f, 0.0f, -1.0f,
    +1.0f, 0.0f, -1.0f,
    +1.0f, 0.0f, +1.0f,
    -1.0f, 0.0f, +1.0f
};

//�@���f�[�^
GLfloat normalAry[] =
{
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f
};

//�F�f�[�^
GLfloat colorAry[] =
{
	1.0f , 0.0f , 1.0f , 1.0f , 
	0.0f , 1.0f , 0.0f , 1.0f , 
	0.0f , 0.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 0.0f , 1.0f 
};

//�t�u�f�[�^
GLfloat uvAry[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};

//#define LH_

#ifdef LH_

//�C���f�b�N�X�i�����v����j������W�n
GLubyte indexAry[]=
{
   0 , 2 , 1 ,
   2 , 3 , 1
};

#else

//�C���f�b�N�X�i���v���j �E����W�n
GLubyte indexAry[]=
{
    0, 2, 1,
    0, 3, 2
};


#endif

// ��/180�̒l
const float PI_OVER_180 = 0.0174532925f;

//VBO�pID
GLuint vid[5];   // vertex, normal, texcoord, index
GLint vertexLocation;
GLint normalLocation;
GLint colorLocation;
GLint texCoordLocation;
GLuint texId[1];
GLint textureLocation;

GLuint programObject;
GLuint vertexShader;
GLuint fragmentShader;

float angle;

GLuint globalparamUboId[1]; // material�p
GLuint globalparamIndex;
GLint  globalparamBlockSize;

HDC   hDC;
HGLRC hRC;

//�ގ�
struct GlobalParam
{
	GLfloat worldMatrix[16];
	GLfloat viewMatrix[16];
	GLfloat projectionMatrix[16];
} g_GlobalParam;

static const skyString* window_name = _T( "testwindow" );

static skyBool bExit = skyFalse;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:

		glViewport(0, 0, LOWORD( lParam ) , HIWORD( lParam ) ); 
 
		graphic::sl_LoadIdentityf(g_GlobalParam.projectionMatrix);
		graphic::sl_Perspectivef(g_GlobalParam.projectionMatrix, 30, (GLfloat)LOWORD( lParam )/(GLfloat)HIWORD( lParam ), 1.0f, 100.0f);
		break;
	case WM_DESTROY:
		bExit = skyTrue;
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

static void Rendering()
{

	// �h���C�o�Ƀo�O�̂���O���t�B�b�N�J�[�h�ɂ����Ă�SDI�̍ŏ��̕`��ƁA
	// ���ׂẴO���t�B�b�N�J�[�h�ɂ����Ă�MDI�`��ɂ�����
	// �ȉ��̖��߂́A���ʂ𔭊�����B
//	::wglMakeCurrent( hDC, hRC );

	//�J���[�o�b�t�@�Ɛ[�x�o�b�t�@�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//�X�V����

	graphic::sl_LoadIdentityf(g_GlobalParam.worldMatrix);
	graphic::sl_Scalef(g_GlobalParam.worldMatrix, 2.0f, 2.0f, 2.0f);   // 2�{�ɂ���
	graphic::sl_Rotatef(g_GlobalParam.worldMatrix, angle, 1.0f, 0.0f, 0.0f);
	graphic::sl_Translatef(g_GlobalParam.worldMatrix, 1.0f, 1.0f, 1.0f);

	//
	angle += 0.1f;
	if (angle > 360.0f) angle = 0.0f;

	//�V�F�[�_�[�̐ݒ�
	glUseProgram(programObject);

	//�e�X�g�o��
#if OFF_
    graphic::sl_PrintMatrix( g_GlobalParam.worldMatrix );
	graphic::sl_PrintMatrix( g_GlobalParam.projectionMatrix );
	graphic::sl_PrintMatrix( g_GlobalParam.viewMatrix );
#endif

	//UBO�̐ݒ�
	glBindBuffer(GL_UNIFORM_BUFFER, globalparamUboId[0]);
	glBufferData(GL_UNIFORM_BUFFER, globalparamBlockSize, &g_GlobalParam, GL_DYNAMIC_DRAW);

//UV�X�N���[���i�^�ʖڂɎ����͂��Ȃ��BGPU�������̒l���X�V����T���v���j
#if OFF_
	//UV���W�̃��b�N
	GLfloat *clientPtr;
	GLfloat tmp[3];
	glBindBuffer(GL_ARRAY_BUFFER,vid[2]);
	clientPtr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);

	//�ǂݏo��(READ)
	tmp[0] = clientPtr[0];
	tmp[1] = clientPtr[1];
	tmp[2] = clientPtr[2];

	tmp[0] += 0.01f;

	//��������(WRITE)
	clientPtr[0] = tmp[0];
	clientPtr[1] = tmp[1];
	clientPtr[2] = tmp[2];

	//UV���W�̃A�����b�N
	glUnmapBuffer(GL_ARRAY_BUFFER);
#endif
//--------------------VBO�̕`��--------------------------------------��

    // vertex
    glBindBuffer(GL_ARRAY_BUFFER, vid[0]);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(vertexLocation);
 
    // normal
    glBindBuffer(GL_ARRAY_BUFFER, vid[1]);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(normalLocation);
 
    // color
    glBindBuffer(GL_ARRAY_BUFFER, vid[2]);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(colorLocation);
 
    // texcoord
    glBindBuffer(GL_ARRAY_BUFFER, vid[3]);
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texCoordLocation);
 
    // texture
    glUniform1i(textureLocation, 0); // �e�N�X�`�����j�b�g�ԍ� 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId[0]);
 
    // index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vid[4]);
 
    // draw
    glDrawElements(GL_TRIANGLES, array_sizeof(indexAry), GL_UNSIGNED_BYTE, 0);

//--------------------VBO�̕`��--------------------------------------��

	//�V�F�[�_�[�̐ݒ����
	glUseProgram(0);
 
	//�`��R�}���h��S���]��
	::glFinish();

	//�o�b�N�o�b�t�@���t�����g��
	::SwapBuffers( hDC );
}

TEST( opengl , 5 ) 
{
    SKY_LIB_GRAPHIC_INIT();

	//Window�̍쐬
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= 0;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= window_name;
    wcex.lpszClassName	= window_name;
    wcex.hIconSm		= 0;
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(window_name, window_name, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if ( !hWnd )
    {
		SKY_PANIC_MSG( _T( "Window�쐬�̎��s \n" ) );
    }

   ShowWindow( hWnd , SW_SHOWDEFAULT );
   UpdateWindow( hWnd );

	//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
	RECT clientArea;
	GetClientRect( hWnd, &clientArea );
	int width  = clientArea.right - clientArea.left;
	int height = clientArea.bottom -clientArea.top;

	// �f�o�C�X�R���e�L�X�g�̎擾�A�ݒ�
	hDC = GetDC( hWnd );

	if( NULL == hDC )
	{
		SKY_PANIC_MSG( _T( "�f�o�C�X�R���e�L�X�g�̍쐬�̎��s \n" ) );
	}

	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( hDC, &pfd ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	if( TRUE != ::SetPixelFormat( hDC, iPixelFormat, &pfd ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g�̍쐬
	if( 0 == ( hRC = ::wglCreateContext( hDC ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g���J�����g�̃f�o�C�X�R���e�L�X�g�ɐݒ�
	if( TRUE != ::wglMakeCurrent( hDC, hRC ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

   // GLEW������
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        SKY_PANIC_MSG(_T( "Error: %s\n" ), glewGetErrorString( err ) );
    }

    // GL�̃x���_�[�A�y�уo�[�W���� 
    const GLubyte *str = glGetString(GL_VENDOR);
    printf("VND:%s\n", str);
    str = glGetString(GL_RENDERER);
    printf("RND:%s\n",str);
    str = glGetString(GL_VERSION);
    printf("VER:%s\n", str);

	// �V�F�[�_�̓ǂݍ���
	sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(programObject ,_T( "data/test/glsl/TextureMapping.vert"), _T("data/test/glsl/TextureMapping.frag") );

    // Fragment data
    glBindFragDataLocation(programObject, 0, "fragColor");
 	
//--------------------VBO�̍쐬--------------------------------------��

    // in(attribute)�ϐ��ւ̊֘A�t��
    vertexLocation = glGetAttribLocation(programObject, "a_vertex");
    normalLocation = glGetAttribLocation(programObject, "a_normal");
    colorLocation = glGetAttribLocation(programObject, "a_color");
    texCoordLocation = glGetAttribLocation(programObject, "a_texcoord");
 
    // uniform�ϐ��ւ̊֘A�t��
    textureLocation = glGetUniformLocation(programObject, "texture0");
 
    glGenBuffers(5, vid);
    // ���_�f�[�^��GPU���������֓]��
    glBindBuffer(GL_ARRAY_BUFFER, vid[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAry), vertexAry, GL_STATIC_DRAW);
    // �@���f�[�^��GPU���������֓]��
    glBindBuffer(GL_ARRAY_BUFFER, vid[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAry), normalAry, GL_STATIC_DRAW);
    // �F�f�[�^��GPU���������֓]��
    glBindBuffer(GL_ARRAY_BUFFER, vid[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorAry), colorAry, GL_STATIC_DRAW);
    // �e�N�X�`�����W�f�[�^��GPU���������֓]��
    glBindBuffer(GL_ARRAY_BUFFER, vid[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAry), uvAry, GL_STATIC_DRAW);
    // ���_�ԍ��f�[�^��GPU���������֓]��
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vid[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexAry), indexAry, GL_STATIC_DRAW);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
 
    ////////////////////////////////////////////////////////////////
    // texture

	IplImage *pTex = cvLoadImage("data/test/texture/OpenGLTexMap.jpg",CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if ( pTex == NULL )
	{
		SKY_PANIC_MSG( _T( "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B" ) );
	}

	cvCvtColor(pTex, pTex, CV_BGR2RGB);
	cvFlip(pTex,NULL,0);

	/******* �e�N�X�`���쐬 ****************/
	glGenTextures(1, &texId[0]);
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pTex->width,pTex->height, GL_RGB,GL_UNSIGNED_BYTE,pTex->imageData);

	cvReleaseImage( &pTex );
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    glBindTexture(GL_TEXTURE_2D, 0);

    //*******************UBO************************
    globalparamIndex = glGetUniformBlockIndex(programObject, "GlobalParam");
    glGetActiveUniformBlockiv(
            programObject, globalparamIndex,
            GL_UNIFORM_BLOCK_DATA_SIZE, &globalparamBlockSize);
 
    // UBO���g�p
    glGenBuffers(1, globalparamUboId);
    glBindBuffer(GL_UNIFORM_BUFFER, globalparamUboId[0]);
    glBufferData(GL_UNIFORM_BUFFER, 1, NULL, GL_DYNAMIC_DRAW);
 
	enum { UBO_INDEX_1=0 };
    glBindBufferBase(GL_UNIFORM_BUFFER, UBO_INDEX_1, globalparamUboId[0]);
    glUniformBlockBinding(programObject, globalparamIndex, UBO_INDEX_1);

//--------------------VBO�̍쐬--------------------------------------��

	// �N���A�J���[�̐ݒ�
	::glClearColor( 0, 0, 0, 0 );	// ��

	// �f�v�X�o�b�t�@�̃N���A
	::glClearDepth( 1.0f );

	// �f�v�X�e�X�g��L���ɂ���
	::glEnable( GL_DEPTH_TEST );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	::glDepthFunc( GL_LEQUAL );

	//�r���[�|�[�g
	::glViewport(0,0,width,height);

    glEnable( GL_SCISSOR_TEST );
	glScissor( 0 , 0 , width , height );

	//�\�����`��
//	glEnable(GL_CULL_FACE); 
//	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE); 

#ifdef LH_
		glFrontFace(GL_CCW); //�����v���肪�\�i�f�t�H���g�j
#else
		glFrontFace(GL_CW);  //���v��肪�\
#endif

	glMatrixMode(GL_PROJECTION);
	graphic::sl_LoadIdentityf(g_GlobalParam.viewMatrix);

//#define DISP_2D
#ifdef DISP_2D
	graphic::sl_Orthof( viewMatrix , 
		        - width * 0.5 / 10.0,
				  width * 0.5 / 10.0,
				- height * 0.5 / 10.0,
				  height * 0.5 / 10.0,
				  0.1,
				  1000.0 );
#else
    graphic::sl_LookAtf(g_GlobalParam.viewMatrix, 3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
#endif

	// ���_���W�ϊ�
    graphic::sl_LoadIdentityf(g_GlobalParam.projectionMatrix);
    graphic::sl_Perspectivef(g_GlobalParam.projectionMatrix, 30, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f);

	// FOB
	graphic::CFramebufferObject fob;
	fob.Initialize( width , height );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//���͊Ǘ��X�V
		Ckeyboard_->Update( 0 );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//�v�����g�X�N���[��
		if ( Ckeyboard_->IsKeyPress( KEY_P ) )
		{
			//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
			RECT clientArea;
			GetClientRect( hWnd, &clientArea );
			width  = clientArea.right - clientArea.left;
			height = clientArea.bottom -clientArea.top;

			graphic::GLUtil::ScreenShot( "data/out/GLScreen.bmp" , width , height );
		}

		//FBO�ɕ`���e�N�X�`���}�b�s���O�Ƃ��Ďg�p����
		if ( Ckeyboard_->IsKeyPress( KEY_F ) )
		{
			//FBO�ɕ`��
			fob.Render( Rendering );

			texId[0] = fob.texID;
		}

		Rendering();
	}
	GAME_LOOP_END

	::wglMakeCurrent(hDC,0);
	::wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );

	DestroyWindow( hWnd );
	UnregisterClass( window_name , hInstance );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

//================================================================================================================

#if OFF_

#include "SkyGraphicLib/Loader.h"

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "testcube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "testcone.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "testplane10x10.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_4 = _T( "testsphere.obj" );

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 

//�J�������
/*
static const math::CBasicVector3 _CAMERA3D_DEFAULT_UP_VECTOR= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA3D_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA3D_DEFAULT_FAR		= 1000;
static const dectype			 _CAMERA3D_DEFAULT_FOV		= math::CMathUtil::DegToRad( 30.0f );
static const math::CBasicVector3 _CAMERA3D_DEFAULT_EYE		= math::CBasicVector3( 0.0f, 3.0f, 10.0f );
static const math::CBasicVector3 _CAMERA3D_DEFAULT_TAG		= math::CBasicVector3( 0.0f, 0.0f, 0.0f );
*/
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA_DEFAULT_FAR		= 1000;
static const dectype			 _CAMERA_DEFAULT_FOV		= math::CMathUtil::DegToRad( 15.0f );


//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
//static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera3DSatellitePtr  g_spCamera;

//�e�X�g�p���f��
static graphic::CModelPtr     g_TestCube;
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;
static graphic::CModelPtr     g_TestSphere;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;

TEST( opengl , 6 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m�R�[���o�b�N��o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
/*
	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->SetUpVector( _CAMERA3D_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA3D_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA3D_DEFAULT_FAR );
	g_spCamera->SetFov( _CAMERA3D_DEFAULT_FOV );
	g_spCamera->SetEye( _CAMERA3D_DEFAULT_EYE );
	g_spCamera->SetTag( _CAMERA3D_DEFAULT_TAG ); 
*/
	g_spCamera = Camera3DSatelliteCreate_();
	g_spCamera->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA_DEFAULT_FAR );
	g_spCamera->SetFov( _CAMERA_DEFAULT_FOV );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera;                           //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = IRenderStatCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen = CScreenObject_::Create( screenData );

	g_TestCube = graphic::CModelUtil::CreateTestCube();
	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();
	g_TestSphere = graphic::CModelUtil::CreateTestSphere();

	//���f�����V�[���O���t�ɓo�^
	g_spScreen->GetScene()->m_spSceneGraph = CSceneGraphNodeCreate_();
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestCube );
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestCone ); g_TestCone->GetEntity3D().Pos.X = 4.0f;
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestSphere ); g_TestSphere->GetEntity3D().Pos.X = -4.0f;

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�ŏ��̃R�}���h�͎��s���Ă��܂�
	CRenderCommandManager_::FlashBuffer();

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//�J�����̍X�V
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

		//���f���ړ��ʒu���f
		graphic::CModelUtil::SetEntity3D( g_TestCube );
		graphic::CModelUtil::SetEntity3D( g_TestCone );
		graphic::CModelUtil::SetEntity3D( g_TestSphere );

		//���Z�b�g
		if ( Ckeyboard_->IsKeyPress( KEY_HOME ) )
		{
			g_TestCube->GetProperty().ResetLocal();
			g_TestCube->GetProperty().ResetParent();
			g_TestCube->GetProperty().ResetWorld();
		}

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spScreen->Rendering();

			//�R�}���h�o�b�t�@�̒ǉ�
			CRenderCommandManager_::PushBuffer();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//�`��p�X���b�h�̒�~
	CRenderCommandManager_::StopThread();

	g_spCamera.Delete();
	g_TestCube.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

//================================================================================================================

#if OFF_

//------- ���_�f�[�^-----------//
GLfloat vertexAry[] =
{
1.000000f , 1.000000f , -1.000000f, 
1.000000f , -1.000000f , -1.000000f, 
-1.000000f , 1.000000f , -1.000000f, 
-1.000000f , -1.000000f , -1.000000f, 
-1.000000f , -1.000000f , 1.000000f, 
-1.000000f , 1.000000f , 1.000000f, 
-1.000000f , -1.000000f , -1.000000f, 
-1.000000f , 1.000000f , -1.000000f, 
1.000000f , -1.000000f , 1.000000f, 
0.999999f , 1.000000f , 1.000001f, 
-1.000000f , 1.000000f , 1.000000f, 
1.000000f , -1.000000f , 1.000000f, 
-1.000000f , 1.000000f , 1.000000f, 
-1.000000f , -1.000000f , 1.000000f, 
1.000000f , -1.000000f , -1.000000f, 
1.000000f , 1.000000f , -1.000000f, 
0.999999f , 1.000000f , 1.000001f, 
1.000000f , -1.000000f , -1.000000f, 
0.999999f , 1.000000f , 1.000001f, 
1.000000f , -1.000000f , 1.000000f, 
1.000000f , 1.000000f , -1.000000f, 
-1.000000f , 1.000000f , -1.000000f, 
-1.000000f , 1.000000f , 1.000000f, 
0.999999f , 1.000000f , 1.000001f, 
1.000000f , -1.000000f , -1.000000f, 
1.000000f , -1.000000f , 1.000000f, 
-1.000000f , -1.000000f , -1.000000f, 
-1.000000f , -1.000000f , 1.000000f,
};

//�@���f�[�^
GLfloat normalAry[] =
{
0.000000f , 0.000000f , -1.000000f, 
0.000000f , 0.000000f , -1.000000f, 
0.000000f , 0.000000f , -1.000000f, 
0.000000f , 0.000000f , -1.000000f, 
-1.000000f , -0.000000f , -0.000000f, 
-1.000000f , -0.000000f , -0.000000f, 
-1.000000f , -0.000000f , -0.000000f, 
-1.000000f , -0.000000f , -0.000000f, 
-0.000001f , -0.000000f , 1.000000f, 
-0.000001f , -0.000000f , 1.000000f, 
-0.000001f , -0.000000f , 1.000000f, 
-0.000000f , 0.000000f , 1.000000f, 
-0.000000f , 0.000000f , 1.000000f, 
-0.000000f , 0.000000f , 1.000000f, 
1.000000f , -0.000000f , 0.000001f, 
1.000000f , -0.000000f , 0.000001f, 
1.000000f , -0.000000f , 0.000001f, 
1.000000f , 0.000000f , 0.000000f, 
1.000000f , 0.000000f , 0.000000f, 
1.000000f , 0.000000f , 0.000000f, 
0.000000f , 1.000000f , -0.000000f, 
0.000000f , 1.000000f , -0.000000f, 
0.000000f , 1.000000f , -0.000000f, 
0.000000f , 1.000000f , -0.000000f, 
-0.000000f , -1.000000f , 0.000000f, 
-0.000000f , -1.000000f , 0.000000f, 
-0.000000f , -1.000000f , 0.000000f, 
-0.000000f , -1.000000f , 0.000000f, 
};

//�F�f�[�^
GLfloat colorAry[] =
{
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 

	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 

	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 

	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 

	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 

	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 

	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
	1.0f , 1.0f , 1.0f , 1.0f , 
};

//�t�u�f�[�^
GLfloat uvAry[] =
{
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
};

//#define LH_

#ifdef LH_

//�C���f�b�N�X�i�����v����j������W�n
GLubyte indexAry[]=
{
   0 , 2 , 1 ,
   2 , 3 , 1
};

#else

//�C���f�b�N�X�i���v���j �E����W�n
GLubyte indexAry[]=
{
0,1,2,1,3,2,4,5,6,5,7,6,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,20,22,23,24,25,26,25,27,26
};


#endif

// ��/180�̒l
const float PI_OVER_180 = 0.0174532925f;

//VBO�pID
GLuint vid[5];   // vertex, normal, texcoord, index
GLint vertexLocation;
GLint normalLocation;
GLint colorLocation;
GLint texCoordLocation;
GLuint texId[1];
GLint textureLocation;

GLuint programObject;
GLuint vertexShader;
GLuint fragmentShader;

float angle;

GLuint globalparamUboId[1]; // material�p
GLuint globalparamIndex;
GLint  globalparamBlockSize;

HDC   hDC;
HGLRC hRC;

//�ގ�
struct GlobalParam
{
	GLfloat worldMatrix[16];
	GLfloat viewMatrix[16];
	GLfloat projectionMatrix[16];
} g_GlobalParam;

static const skyString* window_name = _T( "testwindow" );

static skyBool bExit = skyFalse;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SIZE:

		glViewport(0, 0, LOWORD( lParam ) , HIWORD( lParam ) ); 
 
		graphic::sl_LoadIdentityf(g_GlobalParam.projectionMatrix);
		graphic::sl_Perspectivef(g_GlobalParam.projectionMatrix, 30, (GLfloat)LOWORD( lParam )/(GLfloat)HIWORD( lParam ), 1.0f, 100.0f);
		break;
	case WM_DESTROY:
		bExit = skyTrue;
		break;
	}

	return DefWindowProc( hWnd, message, wParam, lParam );
}

static void Rendering()
{

	// �h���C�o�Ƀo�O�̂���O���t�B�b�N�J�[�h�ɂ����Ă�SDI�̍ŏ��̕`��ƁA
	// ���ׂẴO���t�B�b�N�J�[�h�ɂ����Ă�MDI�`��ɂ�����
	// �ȉ��̖��߂́A���ʂ𔭊�����B
//	::wglMakeCurrent( hDC, hRC );

	//�J���[�o�b�t�@�Ɛ[�x�o�b�t�@�̃N���A
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//�X�V����

	graphic::sl_LoadIdentityf(g_GlobalParam.worldMatrix);
	graphic::sl_Scalef(g_GlobalParam.worldMatrix, 2.0f, 2.0f, 2.0f);   // 2�{�ɂ���
	graphic::sl_Rotatef(g_GlobalParam.worldMatrix, angle, 1.0f, angle, 0.0f);
	graphic::sl_Translatef(g_GlobalParam.worldMatrix, 1.0f, 1.0f, 1.0f);

	//
	angle += 0.1f;
	if (angle > 360.0f) angle = 0.0f;

	//�V�F�[�_�[�̐ݒ�
	glUseProgram(programObject);

	//�e�X�g�o��
#if OFF_
    graphic::sl_PrintMatrix( g_GlobalParam.worldMatrix );
	graphic::sl_PrintMatrix( g_GlobalParam.projectionMatrix );
	graphic::sl_PrintMatrix( g_GlobalParam.viewMatrix );
#endif

	//UBO�̐ݒ�
	glBindBuffer(GL_UNIFORM_BUFFER, globalparamUboId[0]);
	glBufferData(GL_UNIFORM_BUFFER, globalparamBlockSize, &g_GlobalParam, GL_DYNAMIC_DRAW);

#if OFF_
	GLfloat *clientPtr;
	glBindBuffer(GL_ARRAY_BUFFER,vid[0]);
	clientPtr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);
	u32 point = 0;
	for( u32 i = 0 ;  i < array_sizeof( vertexAry ) / 3 ; i++ )
	{
        SKY_PRINTF( _T( "sample { %f , %f , %f } , \n" ) , clientPtr[ i + point ] , clientPtr[ i + point + 1 ] , clientPtr[ i + point + 2 ] );
		point += 3;
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);

#endif

//UV�X�N���[���i�^�ʖڂɎ����͂��Ȃ��BGPU�������̒l���X�V����T���v���j
#if OFF_
	//UV���W�̃��b�N
	GLfloat *clientPtr;
	GLfloat tmp[3];
	glBindBuffer(GL_ARRAY_BUFFER,vid[2]);
	clientPtr = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_WRITE);

	//�ǂݏo��(READ)
	tmp[0] = clientPtr[0];
	tmp[1] = clientPtr[1];
	tmp[2] = clientPtr[2];

	tmp[0] += 0.01f;

	//��������(WRITE)
	clientPtr[0] = tmp[0];
	clientPtr[1] = tmp[1];
	clientPtr[2] = tmp[2];

	//UV���W�̃A�����b�N
	glUnmapBuffer(GL_ARRAY_BUFFER);
#endif
//--------------------VBO�̕`��--------------------------------------��

    // vertex
    glBindBuffer(GL_ARRAY_BUFFER, vid[0]);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(vertexLocation);
 
    // normal
    glBindBuffer(GL_ARRAY_BUFFER, vid[1]);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(normalLocation);
 
    // color
    glBindBuffer(GL_ARRAY_BUFFER, vid[2]);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(colorLocation);
 
    // texcoord
    glBindBuffer(GL_ARRAY_BUFFER, vid[3]);
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(texCoordLocation);
 
    // texture
    glUniform1i(textureLocation, 0); // �e�N�X�`�����j�b�g�ԍ� 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texId[0]);
 
    // index
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vid[4]);
 
    // draw
    glDrawElements(GL_TRIANGLES, array_sizeof(indexAry), GL_UNSIGNED_BYTE, 0);

//--------------------VBO�̕`��--------------------------------------��

	//�V�F�[�_�[�̐ݒ����
	glUseProgram(0);
 
	//�`��R�}���h��S���]��
	::glFinish();

	//�o�b�N�o�b�t�@���t�����g��
	::SwapBuffers( hDC );
}

TEST( opengl , 7 ) 
{
    SKY_LIB_GRAPHIC_INIT();

	//Window�̍쐬
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style			= CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc	= WndProc;
    wcex.cbClsExtra		= 0;
    wcex.cbWndExtra		= 0;
    wcex.hInstance		= hInstance;
    wcex.hIcon			= 0;
    wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName	= window_name;
    wcex.lpszClassName	= window_name;
    wcex.hIconSm		= 0;
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(window_name, window_name, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if ( !hWnd )
    {
		SKY_PANIC_MSG( _T( "Window�쐬�̎��s \n" ) );
    }

   ShowWindow( hWnd , SW_SHOWDEFAULT );
   UpdateWindow( hWnd );

	//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
	RECT clientArea;
	GetClientRect( hWnd, &clientArea );
	int width  = clientArea.right - clientArea.left;
	int height = clientArea.bottom -clientArea.top;

	// �f�o�C�X�R���e�L�X�g�̎擾�A�ݒ�
	hDC = GetDC( hWnd );

	if( NULL == hDC )
	{
		SKY_PANIC_MSG( _T( "�f�o�C�X�R���e�L�X�g�̍쐬�̎��s \n" ) );
	}

	static PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( hDC, &pfd ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	if( TRUE != ::SetPixelFormat( hDC, iPixelFormat, &pfd ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g�̍쐬
	if( 0 == ( hRC = ::wglCreateContext( hDC ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g���J�����g�̃f�o�C�X�R���e�L�X�g�ɐݒ�
	if( TRUE != ::wglMakeCurrent( hDC, hRC ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

   // GLEW������
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        SKY_PANIC_MSG(_T( "Error: %s\n" ), glewGetErrorString( err ) );
    }

    // GL�̃x���_�[�A�y�уo�[�W���� 
    const GLubyte *str = glGetString(GL_VENDOR);
    printf("VND:%s\n", str);
    str = glGetString(GL_RENDERER);
    printf("RND:%s\n",str);
    str = glGetString(GL_VERSION);
    printf("VER:%s\n", str);

	// �V�F�[�_�̓ǂݍ���
	sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(programObject ,_T( "data/test/glsl/TextureMapping.vert"), _T("data/test/glsl/TextureMapping.frag") );

    // Fragment data
    glBindFragDataLocation(programObject, 0, "fragColor");
 	
//--------------------VBO�̍쐬--------------------------------------��

    // in(attribute)�ϐ��ւ̊֘A�t��
    vertexLocation = glGetAttribLocation(programObject, "a_vertex");
    normalLocation = glGetAttribLocation(programObject, "a_normal");
    colorLocation = glGetAttribLocation(programObject, "a_color");
    texCoordLocation = glGetAttribLocation(programObject, "a_texcoord");
 
    // uniform�ϐ��ւ̊֘A�t��
    textureLocation = glGetUniformLocation(programObject, "texture0");
 
    glGenBuffers(5, vid);
    // ���_�f�[�^��GPU���������֓]��
    glBindBuffer(GL_ARRAY_BUFFER, vid[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAry), vertexAry, GL_DYNAMIC_DRAW);
    // �@���f�[�^��GPU���������֓]��
    glBindBuffer(GL_ARRAY_BUFFER, vid[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAry), normalAry, GL_STATIC_DRAW);
    // �F�f�[�^��GPU���������֓]��
    glBindBuffer(GL_ARRAY_BUFFER, vid[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colorAry), colorAry, GL_STATIC_DRAW);
    // �e�N�X�`�����W�f�[�^��GPU���������֓]��
    glBindBuffer(GL_ARRAY_BUFFER, vid[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexAry), uvAry, GL_STATIC_DRAW);
    // ���_�ԍ��f�[�^��GPU���������֓]��
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vid[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexAry), indexAry, GL_STATIC_DRAW);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
 
    ////////////////////////////////////////////////////////////////
    // texture

	IplImage *pTex = cvLoadImage("data/test/texture/OpenGLTexMap.jpg",CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
	if ( pTex == NULL )
	{
		SKY_PANIC_MSG( _T( "�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂����B" ) );
	}

	cvCvtColor(pTex, pTex, CV_BGR2RGB);
	cvFlip(pTex,NULL,0);

	/******* �e�N�X�`���쐬 ****************/
	glGenTextures(1, &texId[0]);
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pTex->width,pTex->height, GL_RGB,GL_UNSIGNED_BYTE,pTex->imageData);

	cvReleaseImage( &pTex );
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 
    glBindTexture(GL_TEXTURE_2D, 0);

    //*******************UBO************************
    globalparamIndex = glGetUniformBlockIndex(programObject, "GlobalParam");
    glGetActiveUniformBlockiv(
            programObject, globalparamIndex,
            GL_UNIFORM_BLOCK_DATA_SIZE, &globalparamBlockSize);
 
    // UBO���g�p
    glGenBuffers(1, globalparamUboId);
    glBindBuffer(GL_UNIFORM_BUFFER, globalparamUboId[0]);
    glBufferData(GL_UNIFORM_BUFFER, 1, NULL, GL_DYNAMIC_DRAW);
 
	enum { UBO_INDEX_1=0 };
    glBindBufferBase(GL_UNIFORM_BUFFER, UBO_INDEX_1, globalparamUboId[0]);
    glUniformBlockBinding(programObject, globalparamIndex, UBO_INDEX_1);

//--------------------VBO�̍쐬--------------------------------------��

	// �N���A�J���[�̐ݒ�
	::glClearColor( 0, 0, 0, 0 );	// ��

	// �f�v�X�o�b�t�@�̃N���A
	::glClearDepth( 1.0f );

	// �f�v�X�e�X�g��L���ɂ���
	::glEnable( GL_DEPTH_TEST );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	::glDepthFunc( GL_LEQUAL );

	//�r���[�|�[�g
	::glViewport(0,0,width,height);

	//�\�����`��
//	glEnable(GL_CULL_FACE); 
//	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE); 

#ifdef LH_
		glFrontFace(GL_CCW); //�����v���肪�\�i�f�t�H���g�j
#else
		glFrontFace(GL_CW);  //���v��肪�\
#endif

    graphic::sl_LoadIdentityf(g_GlobalParam.viewMatrix);

//#define DISP_2D
#ifdef DISP_2D
	graphic::sl_Orthof( viewMatrix , 
		        - width * 0.5 / 10.0,
				  width * 0.5 / 10.0,
				- height * 0.5 / 10.0,
				  height * 0.5 / 10.0,
				  0.1,
				  1000.0 );
#else
    graphic::sl_LookAtf(g_GlobalParam.viewMatrix, 3.0f, 4.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
#endif

	// ���_���W�ϊ�
    graphic::sl_LoadIdentityf(g_GlobalParam.projectionMatrix);
    graphic::sl_Perspectivef(g_GlobalParam.projectionMatrix, 30, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f);

	// FOB
	graphic::CFramebufferObject fob;
	fob.Initialize( width , height );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//���͊Ǘ��X�V
		Ckeyboard_->Update( 0 );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//�v�����g�X�N���[��
		if ( Ckeyboard_->IsKeyPress( KEY_P ) )
		{
			//Window�̃N���C�A���g�̈�̃T�C�Y���擾����
			RECT clientArea;
			GetClientRect( hWnd, &clientArea );
			width  = clientArea.right - clientArea.left;
			height = clientArea.bottom -clientArea.top;

			graphic::GLUtil::ScreenShot( "data/out/GLScreen.bmp" , width , height );
		}

		//FBO�ɕ`���e�N�X�`���}�b�s���O�Ƃ��Ďg�p����
		if ( Ckeyboard_->IsKeyPress( KEY_F ) )
		{
			//FBO�ɕ`��
			fob.Render( Rendering );

			texId[0] = fob.texID;
		}

		Rendering();
	}
	GAME_LOOP_END

	::wglMakeCurrent(hDC,0);
	::wglDeleteContext( hRC );
	ReleaseDC( hWnd, hDC );

	DestroyWindow( hWnd );
	UnregisterClass( window_name , hInstance );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif

#endif
