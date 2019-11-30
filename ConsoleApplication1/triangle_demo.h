#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include <random>

#include "demo_base.h"

#include "bitmap.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#define TEXTURE_COUNT 3
#define SPECTRUM_SIZE 128 //Rec = 128 or 512
//#define 

//FMOD ERROR CHECKING
void ERRORCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error! : " << FMOD_ErrorString(result) << std::endl;
	}
}

class TriangleDemo : public DemoBase
{
private:
	GLuint mTextureID[TEXTURE_COUNT]; //GLu = GL Unsigned = Only Positive

	//FMOD
	FMOD::System* m_fmodSystem;
	FMOD::Sound* m_music;
	FMOD::Channel* m_musicChannel;
	float m_Spectrum_Left[SPECTRUM_SIZE];
	float m_Spectrum_Right[SPECTRUM_SIZE];

	void LoadTexture(const char* path, GLuint textureID)
	{
		CBitmap bitmap(path);

		//Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//apply Texture wrapping along horizontal axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//apply Texture wrapping along vertical axis
		/*
		//old school (Minecraft) filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//near filtering, (For when texture needs to scale up) 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//far filtering, (For when texture needs to scale down)
		*/
		//bilinear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//near filtering, (For when texture needs to scale up)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//far filtering, (For when texture needs to scale down)
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());

	}

public:
	//FMOD
	void initFMOD()
	{
		FMOD_RESULT		results;
		unsigned int	version;

		results = FMOD::System_Create(&m_fmodSystem);
		ERRORCHECK(results);

		results = m_fmodSystem->getVersion(&version);
		ERRORCHECK(results);

		if (version < FMOD_VERSION)
		{
			std::cout << "FMOD Error! You are using old version of FMOD" << std::endl;
		}

		//Initialize
		results = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
		ERRORCHECK(results);

		//Load
		results = m_fmodSystem->createStream("../media/TOTEM - From The Earth.mp3", FMOD_SOFTWARE, 0, &m_music);
		ERRORCHECK(results);

		//Play
		results = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music, false, &m_musicChannel);
		ERRORCHECK(results);
	}

	void init()
	{
		//Start FMOD Music
		initFMOD();

		glGenTextures(TEXTURE_COUNT, mTextureID);
		LoadTexture("../media/rocks.bmp", mTextureID[0]);
		LoadTexture("../media/glass.bmp", mTextureID[1]);
		//LoadTexture("../media/water.bmp", mTextureID[2]);
	}

	void deinit()
	{
	}

	void UpdateFMOD()
	{
		m_fmodSystem->update();
		//Get Spectrum for Left and Right Stereo Channels
		m_musicChannel->getSpectrum(m_Spectrum_Left, SPECTRUM_SIZE, 0, FMOD_DSP_FFT_WINDOW_RECT);
		m_musicChannel->getSpectrum(m_Spectrum_Right, SPECTRUM_SIZE, 1, FMOD_DSP_FFT_WINDOW_RECT);

		//Test and Print First Frequency Range
		//std::cout << "Left: " << m_Spectrum_Left[0] <<", Right:"<< m_Spectrum_Right[0] << std::endl;
	}

	void drawAxis(const Matrix& viewMatrix)
	{
	// ============draw axis.
		glLoadMatrixf((GLfloat*)viewMatrix.mVal);
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.3f, 0.3f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);

			glColor3f(0.3f, 1.0f, 0.3f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 1.0f, 0.0f);

			glColor3f(0.3f, 0.3f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 1.0f);
		glEnd();
		// ============================
	}

	float m_rotation1 = 0.0f;
	float m_value1 = 0.0f;

	void DrawFmodCube(float height)
	{
		//Better Draw
		//Create Vertex Data
		GLfloat CubeVertices[] =
		{

			-1.0f	, -1.0f	,			1.0f,
			1.0f	, -1.0f	,			1.0f,
			1.0f	, 1.0f * height ,	1.0f,
			1.0f	, 1.0f * height	,	1.0f,
			-1.0f	, 1.0f * height	,	1.0f,
			-1.0f	, -1.0f	, 1.0f	,

			-1.0f	, -1.0f	, -1.0f	,
			1.0f	, -1.0f	, -1.0f	,
			1.0f	, 1.0f * height,	-1.0f	,
			1.0f	, 1.0f * height,	-1.0f	,
			-1.0f	, 1.0f * height,	-1.0f	,
			-1.0f	, -1.0f	, -1.0f	,

			-1.0f	, -1.0f	, 1.0f	,
			-1.0f	, -1.0f	, -1.0f	,
			-1.0f	, 1.0f * height, -1.0f	,
			-1.0f	, 1.0f * height, -1.0f	,
			-1.0f	, 1.0f * height, 1.0f	,
			-1.0f	, -1.0f	, 1.0f	,

			1.0f	, -1.0f	, 1.0f	,
			1.0f	, -1.0f	, -1.0f	,
			1.0f	, 1.0f * height, -1.0f	,
			1.0f	, 1.0f * height, -1.0f	,
			1.0f	, 1.0f * height, 1.0f	,
			1.0f	, -1.0f, 1.0f	,

			-1.0f	, 1.0f * height, 1.0f	,
			-1.0f	, 1.0f * height, -1.0f	,
			1.0f	, 1.0f * height, -1.0f	,
			1.0f	, 1.0f * height, -1.0f	,
			1.0f	, 1.0f * height, 1.0f	,
			-1.0f	, 1.0f * height, 1.0f	,

			-1.0f	, -1.0f	, 1.0f	,
			-1.0f	, -1.0f	, -1.0f	,
			1.0f	, -1.0f	, -1.0f	,
			1.0f	, -1.0f	, -1.0f	,
			1.0f	, -1.0f	, 1.0f	,
			-1.0f	, -1.0f	, 1.0f

		};
		
		//Create Color Data
		static GLfloat CubeColor[] =
		{
			1.0f	,0.0f	,0.0f	,
			0.0f	,1.0f	,0.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,1.0f	,0.0f	,
			1.0f	,0.0f	,0.0f	,

			1.0f	,0.0f	,0.0f	,
			0.0f	,1.0f	,0.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,1.0f	,0.0f	,
			1.0f	,0.0f	,0.0f	,

			1.0f	,0.0f	,0.0f	,
			0.0f	,1.0f	,0.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,1.0f	,0.0f	,
			1.0f	,0.0f	,0.0f	,

			1.0f	,0.0f	,0.0f	,
			0.0f	,1.0f	,0.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,1.0f	,0.0f	,
			1.0f	,0.0f	,0.0f	,

			1.0f	,0.0f	,0.0f	,
			0.0f	,1.0f	,0.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,1.0f	,0.0f	,
			1.0f	,0.0f	,0.0f	,

			1.0f	,0.0f	,0.0f	,
			0.0f	,1.0f	,0.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,0.0f	,1.0f	,
			0.0f	,1.0f	,0.0f	,
			1.0f	,0.0f	,0.0f	,
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, CubeVertices);
		glColorPointer(3, GL_FLOAT, 0, CubeColor);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void DrawNormalFmodCube()
	{
		//Better Draw
		//Create Vertex Data
		GLfloat CubeVertices[] =
		{

			-1.0f	, -1.0f	,			1.0f,
			1.0f	, -1.0f	,			1.0f,
			1.0f	, 1.0f ,	1.0f,
			1.0f	, 1.0f,	1.0f,
			-1.0f	, 1.0f,	1.0f,
			-1.0f	, -1.0f	, 1.0f	,

			-1.0f	, -1.0f	, -1.0f	,
			1.0f	, -1.0f	, -1.0f	,
			1.0f	, 1.0f ,	-1.0f	,
			1.0f	, 1.0f ,	-1.0f	,
			-1.0f	, 1.0f ,	-1.0f	,
			-1.0f	, -1.0f	, -1.0f	,

			-1.0f	, -1.0f	, 1.0f	,
			-1.0f	, -1.0f	, -1.0f	,
			-1.0f	, 1.0f , -1.0f	,
			-1.0f	, 1.0f , -1.0f	,
			-1.0f	, 1.0f, 1.0f	,
			-1.0f	, -1.0f	, 1.0f	,

			1.0f	, -1.0f	, 1.0f	,
			1.0f	, -1.0f	, -1.0f	,
			1.0f	, 1.0f, -1.0f	,
			1.0f	, 1.0f, -1.0f	,
			1.0f	, 1.0f, 1.0f	,
			1.0f	, -1.0f, 1.0f	,

			-1.0f	, 1.0f, 1.0f	,
			-1.0f	, 1.0f, -1.0f	,
			1.0f	, 1.0f, -1.0f	,
			1.0f	, 1.0f, -1.0f	,
			1.0f	, 1.0f, 1.0f	,
			-1.0f	, 1.0f, 1.0f	,

			-1.0f	, -1.0f	, 1.0f	,
			-1.0f	, -1.0f	, -1.0f	,
			1.0f	, -1.0f	, -1.0f	,
			1.0f	, -1.0f	, -1.0f	,
			1.0f	, -1.0f	, 1.0f	,
			-1.0f	, -1.0f	, 1.0f
		};

		glEnableClientState(GL_VERTEX_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, CubeVertices);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDisableClientState(GL_VERTEX_ARRAY);
	}

	void DrawFmodBlueCircle()
	{
		//int Squares = 380; //Squares
		//int verts = Squares * 6;
		//int points = verts * 3;

		//Size = points
		GLfloat CircleVertex[3240];
		GLfloat CircleColor[3240];

		GLfloat radius = 2.0f;
		GLfloat DPi = 2.0f*M_PI;
		GLfloat CirclePoint = 0.0f;

		//Vertex
		for (int i = 0; i <= (3240 - 9); i += 9)
		{
			if (CirclePoint < M_2PI)
			{
				CircleVertex[i] = 0.0f;
				CircleVertex[i + 1] = 0.0f;
				CircleVertex[i + 2] = 0.0f;

				CircleVertex[i + 3] = cos(CirclePoint)*(radius);// + (m_Spectrum_Left[i/180] * 2));
				CircleVertex[i + 4] = 0.0f;
				CircleVertex[i + 5] = sin(CirclePoint)*(radius); // +(m_Spectrum_Left[i / 180] * 2));

				CircleVertex[i + 6] = cos(CirclePoint + 0.1f)*(radius);// +(m_Spectrum_Left[i / 180] * 2));
				CircleVertex[i + 7] = 0.0f;
				CircleVertex[i + 8] = sin(CirclePoint + 0.1f)*(radius);// +(m_Spectrum_Left[i / 180] * 2));

				CirclePoint += 0.1f;
			}
			else
			{
				CirclePoint = 0.0f;
			}
		}

		GLfloat specLeftColor;
		int colorNum = 0;
		int countNum = 0;
		//Color
		for (int i = 0; i <= (3240 - 9); i += 9)
		{
			specLeftColor = m_Spectrum_Left[colorNum] * 10.0f;

			CircleColor[i] = 0.0f;
			CircleColor[i + 1] = specLeftColor;
			CircleColor[i + 2] = 1.0f;

			CircleColor[i + 3] = 0.0f;
			CircleColor[i + 4] = 1.0f;
			CircleColor[i + 5] = 1.0f;

			CircleColor[i + 6] = 0.0f;
			CircleColor[i + 7] = 1.0f;
			CircleColor[i + 8] = 1.0f;

			colorNum++;

			if (colorNum == 8)
			{
				colorNum = 0;
			}

			/*
			countNum++;

			colorNum = countNum / 18;
			*/
			/*
			colorNum++;

			if (colorNum == 8)
			{
			colorNum = 0;
			}
			*/
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, CircleVertex);
		glColorPointer(3, GL_FLOAT, 0, CircleColor);

		glDrawArrays(GL_TRIANGLES, 0, 1080);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	void DrawFmodRedCircle()
	{
		//int Squares = 380; //Squares
		//int verts = Squares * 6;
		//int points = verts * 3;

		//Size = points
		GLfloat CircleVertex[3240];
		GLfloat CircleColor[3240];

		GLfloat radius = 2.0f;
		GLfloat DPi = 2.0f*M_PI;
		GLfloat CirclePoint = 0.0f;

		//Vertex
		for (int i = 0; i <= (3240 - 9); i += 9)
		{
			if (CirclePoint < M_2PI)
			{
				CircleVertex[i] = 0.0f;
				CircleVertex[i + 1] = 0.0f;
				CircleVertex[i + 2] = 0.0f;

				CircleVertex[i + 3] = cos(CirclePoint)*(radius);// + (m_Spectrum_Left[i/180] * 2));
				CircleVertex[i + 4] = 0.0f;
				CircleVertex[i + 5] = sin(CirclePoint)*(radius); // +(m_Spectrum_Left[i / 180] * 2));

				CircleVertex[i + 6] = cos(CirclePoint + 0.1f)*(radius);// +(m_Spectrum_Left[i / 180] * 2));
				CircleVertex[i + 7] = 0.0f;
				CircleVertex[i + 8] = sin(CirclePoint + 0.1f)*(radius);// +(m_Spectrum_Left[i / 180] * 2));

				CirclePoint += 0.1f;
			}
			else
			{
				CirclePoint = 0.0f;
			}
		}

		GLfloat specLeftColor;
		int colorNum = 0;
		int countNum = 0;
		//Color
		for (int i = 0; i <= (3240 - 9); i += 9)
		{
			specLeftColor = m_Spectrum_Right[colorNum] * 10.0f;

			CircleColor[i] = specLeftColor;
			CircleColor[i + 1] = 0.0f;
			CircleColor[i + 2] = 0.0f;

			CircleColor[i + 3] = 1.0f;
			CircleColor[i + 4] = 0.0f;
			CircleColor[i + 5] = 0.0f;

			CircleColor[i + 6] = 1.0f;
			CircleColor[i + 7] = 0.0f;
			CircleColor[i + 8] = 0.0f;

			colorNum++;

			if (colorNum == 8)
			{
				colorNum = 0;
			}
		}
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, CircleVertex);
		glColorPointer(3, GL_FLOAT, 0, CircleColor);

		glDrawArrays(GL_TRIANGLES, 0, 1080);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

	}

	void DrawFmodSoundCircleCube(const Matrix& viewMatrix, GLfloat radius, int cubeAmount)
	{
		GLfloat CubePosition[3];
		//GLfloat CircleColor[3240];

		GLfloat DPi = 2.0f*M_PI;
		GLfloat CirclePoint = 0.0f;

		//Vertex
		/*
		for (int i = 0; i <= (24-3); i += 3)
		{
			CubePosition[i] = cos(CirclePoint) * (radius);
			CubePosition[i + 1] = 0.0f;
			CubePosition[i + 2] = sin(CirclePoint) * (radius);
			CirclePoint += 1.0f;
		}
		*/
		for (int i = 0; i < cubeAmount; i ++)
		{
			if (CirclePoint < M_2PI)
			{
				CubePosition[0] = cos(CirclePoint)*(radius);// + (m_Spectrum_Left[i/180] * 2));
				CubePosition[1] = 0.0f;
				CubePosition[2] = sin(CirclePoint)*(radius); // +(m_Spectrum_Left[i / 180] * 2));

				Matrix translate = Matrix::makeTranslationMatrix(CubePosition[0], CubePosition[1], CubePosition[2]);
				//Matrix rotate = Matrix::makeRotateMatrix(-CirclePoint * 90, Vector(0.0f, 1.0f, 0.0f));
				Matrix scale = Matrix::makeScaleMatrix(0.2f, 0.2f, 0.2f); //(m_Spectrum_Left[18 - i] * 10.0f)
				Matrix modelMatrix = translate * scale; // *rotate;
				Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
				glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
				DrawFmodCube(1.0f);

				CirclePoint += M_2PI/cubeAmount;
			}
			else
			{
				CirclePoint = 0.0f;
			}
		}
	}

	void DrawFmodSoundRectangleCube(const Matrix& viewMatrix, int cubeWidth, int cubeLength)
	{
		float widthOffSet = 0.0f;
		float lengthOffSet = 0.0f;
		int height = 0;
		for (int i = 0; i < cubeWidth; i++)
		{
			for (int j = 0; j < cubeLength; j++)
			{
				Matrix translate = Matrix::makeTranslationMatrix(widthOffSet, 0.0f, lengthOffSet);
				Matrix scale = Matrix::makeScaleMatrix(0.2f, 0.2f, 0.2f); //(m_Spectrum_Left[18 - i] * 10.0f)
				Matrix modelMatrix = translate * scale; // *rotate;
				Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
				glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);

				DrawFmodCube(m_Spectrum_Left[height] * 10.0f);
				height++;
				lengthOffSet += 1.0f;
			}
			lengthOffSet = 0.0f;
			widthOffSet += 1.0f;
		}
	}

	void DrawFmodDoubleCube(const Matrix& viewMatrix)
	{

		Matrix translate = Matrix::makeTranslationMatrix(0.0f, -2.0f, -5.0f);
		Matrix scale = Matrix::makeScaleMatrix(2.0f, 2.0f, 2.0f);
		Matrix rotate = Matrix::makeRotateMatrix(sin(m_rotation1), Vector(1.0f, 1.0f, 1.0f));
		Matrix modelMatrix = translate * scale; // *rotate;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;

		translate = Matrix::makeTranslationMatrix(0.0f, 2.0f, -5.0f);
		scale = Matrix::makeScaleMatrix(1.0f + (m_Spectrum_Left[0]), 1.0f + (m_Spectrum_Left[0]), 1.0f + (m_Spectrum_Left[0]));
		rotate = Matrix::makeRotateMatrix(m_rotation1 + 90.0f, Vector(1.0f, 1.0f, 1.0f));
		modelMatrix = translate * scale * rotate;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(0.0f, 1.0f, 1.0f);
		DrawNormalFmodCube();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_rotation1++;

		translate = Matrix::makeTranslationMatrix(0.0f, 2.0f, -5.0f);
		scale = Matrix::makeScaleMatrix(2.0f + (m_Spectrum_Right[0]), 2.0f + (m_Spectrum_Right[0]), 2.0f + (m_Spectrum_Right[0] * 2));
		rotate = Matrix::makeRotateMatrix(- m_rotation1 + 90.0f, Vector(1.0f, 1.0f, 1.0f));
		modelMatrix = translate * scale * rotate;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		glColor3f(1.0f, 0.0f, 0.0f);
		DrawNormalFmodCube();

		glDisable(GL_BLEND);

		
	}

	void SpectrumTest()
	{
		GLfloat specLeftColor;
		int colorNum = 0;

		for (int i = 0; i < 360; i ++)
		{
			specLeftColor = m_Spectrum_Left[colorNum]*10.0f;
			std::cout << colorNum << ": " << (specLeftColor*1.0f) << std::endl;
			
			colorNum++;
			if (colorNum == 18)
			{
				colorNum = 0;
			}
		}
	}

	void FmodTest(const Matrix& viewMatrix)
	{
		int offset = 1;
		for (int i = 0; i < 36; i++)
		{
			if (i < 18)
			{
				Matrix translate = Matrix::makeTranslationMatrix(0.0f + i - 18, 0.0f, 0.0f);
				Matrix scale = Matrix::makeScaleMatrix(0.2f, (m_Spectrum_Left[18 - i] * 10.0f), 0.2f);
				Matrix modelMatrix = translate * scale;
				Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
				glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
				float color[3] = { 0.0f, 0.0f, 1.0f };
				DrawFmodCube(1.0f);
			}
			else
			{
				Matrix translate = Matrix::makeTranslationMatrix(0.0f + i - 18, 0.0f, 0.0f);
				Matrix scale = Matrix::makeScaleMatrix(0.2f, (m_Spectrum_Right[i - 18] * 10.0f), 0.2f);
				Matrix modelMatrix = translate * scale;
				Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
				glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
				float color[3] = { 0.0f, 1.0f, 1.0f };
				DrawFmodCube(1.0f);
			}
		}
	}

	void draw(const Matrix& viewMatrix)
	{
		UpdateFMOD();
		
		drawAxis(viewMatrix);

		//enable wireframe mode
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		
		glLoadMatrixf((GLfloat*)viewMatrix.mVal); //Get Matrix from viewMatrix(Camera Matrix)

		DrawFmodDoubleCube(viewMatrix);

		Matrix translate = Matrix::makeTranslationMatrix(0.0f, -5.0f, -5.0f);
		Matrix scale = Matrix::makeScaleMatrix(3.0f, 3.0f, 3.0f);
		Matrix modelMatrix = translate * scale;
		Matrix viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		DrawFmodBlueCircle();

		translate = Matrix::makeTranslationMatrix(0.0f, -4.99f, -5.0f);
		scale = Matrix::makeScaleMatrix(2.0f, 2.0f, 2.0f);
		modelMatrix = translate * scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		DrawFmodRedCircle();
		
		translate = Matrix::makeTranslationMatrix(-2.5f, -4.00f, -7.5f);
		//scale = Matrix::makeScaleMatrix(2.0f, 2.0f, 2.0f);
		modelMatrix = translate; // *scale;
		viewSpaceMatrix = viewMatrix * modelMatrix;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix.mVal);
		DrawFmodSoundRectangleCube(viewSpaceMatrix, 6, 6);
	}
};

#endif