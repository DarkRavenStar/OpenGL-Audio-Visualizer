#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include "demo_base.h"
//#include "cmath>
#include <math.h>
#include "bitmap.h"
#include <fmod.hpp>
#include <fmod_errors.h>

#define TEXTURE_COUNT 2
#define SPECTRUM_SIZE 128


void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD error : " << FMOD_ErrorString(result) << std::endl;
	}
}

class TriangleDemo : public DemoBase
{
private:
	FMOD::System* m_fmodSystem;
	FMOD::Sound* m_music;
	FMOD::Channel *m_musicChannel;
	float m_spectrumLeft[SPECTRUM_SIZE];
	float m_spectrumRight[SPECTRUM_SIZE];


	GLuint mTextureID[TEXTURE_COUNT];

	void loadTexture(const char* path, GLuint textureID)
	{
		CBitmap bitmap(path);

		//Create linear flitered texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //APPLY TEXTURE WRAPPING ALONG HORIZONTAL PART
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //APPLY TEXTURE WRAPPING ALONG VERTICAL PART

		//Old school (minecraft) filtering
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //NEAR FILTERING (FORWHEN TEXTURE NEED TO SCALE UP)
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //FAR FILTERING (FORWHEN TEXTURE NEED TO SCALE DOWN)
		
		//Bilinear Filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //NEAR FILTERING (FORWHEN TEXTURE NEED TO SCALE UP)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //FAR FILTERING (FORWHEN TEXTURE NEED TO SCALE DOWN)
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(),
		bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());
	}

public:

	void initFmod()
	{
		FMOD_RESULT result;
		unsigned int version;

		result = FMOD::System_Create(&m_fmodSystem);
		ERRCHECK(result);

		result = m_fmodSystem->getVersion(&version);

		if (version < FMOD_VERSION)
		{
			std::cout << "FMOD_ERROR: You are using older version of FMOD." << std::endl;
		}

		//intialize fmod system
		result = m_fmodSystem->init(32, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);

		//load and set up music
		result = m_fmodSystem->createStream("../media/ironfortress.mp3", FMOD_SOFTWARE, 0, &m_music);
		ERRCHECK(result);

		//Play the loaded mp3 music
		result = m_fmodSystem->playSound(FMOD_CHANNEL_FREE, m_music, false, &m_musicChannel);
		ERRCHECK(result);
	}

	void init()
	{
		glGenTextures(TEXTURE_COUNT, mTextureID);
		loadTexture("../media/glass.bmp", mTextureID[0]);
		loadTexture("../media/rocks.bmp", mTextureID[1]);
		initFmod();
	}

	void deinit()
	{
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


	void drawTriangle(void)
	{
		static GLfloat vertices[] = {
			-1.0f, -1.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
		};

		static GLubyte colors[] = {
			255, 255, 255,
			255, 255, 255,
			255, 255, 255,
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);


		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	void drawSquare(void)
	{
		static GLfloat vertices[] = {
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};

		static GLubyte colors[] = {
			255, 255, 255,
			255, 255, 255,
			255, 255, 255,
			255, 255, 255,
			255, 255, 255,
			255, 255, 255
		};

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);


		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	void drawCircle(float xRadius, float zRadius, float yHeight)
	{
		static GLfloat vertices[] = {
			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};
		
		float DEF_D = 1.0f;
		float k = 0.0f;

		glColor3b(255, 0, k);
		glBegin(GL_TRIANGLES);
		for (k = 0; k <= 360; k += DEF_D) {
			glVertex3f(0, yHeight, 0);
			glVertex3f(cos(k) * xRadius, yHeight, sin(k) * zRadius);
			glVertex3f(cos(k + DEF_D * 0.1) * xRadius, yHeight, sin(k + DEF_D * 0.1) * zRadius);
		}
		glEnd();
	}

	void drawCube()
	{
		//create vertices data
		static GLfloat vertices[] = {
			//Front Face
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			//Back Face
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,

			//Back Face
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
		};

		static GLubyte colors[] = {
			255, 255, 255,
			255, 255, 255,
			255, 255, 255,
			255, 255, 255,
			255, 255, 255,
			255, 255, 255
		};

	}

	void drawPlaneWave(void)
	{
		int squares = 50;
		int hSquareLimit = 5;
		int verts = squares * 6;
		int points = verts * 3;

		GLfloat vertex[900];
		int horizontalSquareLimit = hSquareLimit - 1;
		
		int squareCount = 0;
		float up = 0.0f;
		float down = 2.0f;
		float left = 0.0f;
		float right = 2.0f;

		for (int i = 0; i <= (points - 18); i += 18)
		{
			vertex[i] = left; vertex[i + 1] = 0.0f; vertex[i + 2] = up;
			vertex[i+3] = right; vertex[i + 4] = 0.0f; vertex[i + 5] = up;
			vertex[i+6] = right; vertex[i + 7] = 0.0f; vertex[i + 8] = down;

			vertex[i + 9] = right; vertex[i + 10] = 0.0f; vertex[i + 11] = down;
			vertex[i + 12] = left; vertex[i + 13] = 0.0f; vertex[i + 14] = down;
			vertex[i + 15] = left; vertex[i + 16] = 0.0f; vertex[i + 17] = up;

			left += 2.0f;
			right += 2.0f;

			if (squareCount == horizontalSquareLimit)
			{
				squareCount = 0;
				left = 0.0f;
				right = 0.0f;

				up += 2.0f;
				down += 2.0f;
			}
			else
			{
				squareCount++;
			}
		}

		//glEnable(GL_BLEND)
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, vertex);
		//glTexCoordPointer(2, GL_FLOAT, 0, squareTexCoords);
		//glColorPointer(3, GL_UNSIGNED_BYTE, 0, colors);

		glDrawArrays(GL_TRIANGLES, 0, 18);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
	}

	void drawPyramid(float color[3])
	{
		glBegin(GL_TRIANGLES);
		glColor3f(color[0], color[1], color[2]);
		//Square Base
		glVertex3f(-0.25f, -0.25f, 0.25f);
		glVertex3f(0.25f, -0.25f, 0.25f);
		glVertex3f(0.25f, -0.25f, -0.25f);

		glVertex3f(-0.25f, -0.25f, -0.25f);
		glVertex3f(0.25f, -0.25f, -0.25f);
		glVertex3f(-0.25f, -0.25f, 0.25f);

		// Triangle Side 1
		glVertex3f(-0.25f, -0.25f, 0.25f);
		glVertex3f(0.25f, -0.25f, 0.25f);
		glVertex3f(0.0f, 0.25f, 0.0f);
		// Triangle Side 2
		glVertex3f(-0.25f, -0.25f, -0.25f);
		glVertex3f(0.25f, -0.25f, -0.25f);
		glVertex3f(0.0f, 0.25f, 0.0f);
		// Triangle Side 3
		glVertex3f(-0.25f, -0.25f, -0.25f);
		glVertex3f(-0.25f, -0.25f, 0.25f);
		glVertex3f(0.0f, 0.25f, 0.0f);
		// Triangle Side 4
		glVertex3f(0.25f, -0.25f, -0.25f);
		glVertex3f(0.25f, -0.25f, 0.25f);
		glVertex3f(0.0f, 0.25f, 0.0f);

		glEnd();
	}

	void drawCylinder(float x1Radius, float z1Radius, float x2Radius, float z2Radius, float height, float color[3])
	{
		float DEF_D = 1.0f;
		float k = 0.0f;
		glColor3f(color[0], color[1], color[2]);
		glBegin(GL_TRIANGLES);
		for (k = 0; k <= 360; k += DEF_D) {
			glVertex3f(cos(k) * x2Radius, height, sin(k) * z2Radius);
			glVertex3f(cos(k) * x1Radius, 0, sin(k) * z1Radius);
			glVertex3f(cos(k + DEF_D * 0.1) * x1Radius, 0, sin(k + DEF_D * 0.1) * z1Radius);

			glVertex3f(cos(k) * x1Radius, 0, sin(k) * z1Radius);
			glVertex3f(cos(k) * x2Radius, height, sin(k) * z2Radius);
			glVertex3f(cos(k + DEF_D * 0.1) * x2Radius, height, sin(k + DEF_D * 0.1) * z2Radius);
		}
		glEnd();
		
		drawCircle(x1Radius, z1Radius, 0);
		drawCircle(x2Radius, z2Radius, height);
	}

	void drawCone()
	{
		int DEF_D = 1;
		int k = 0;

		glBegin(GL_TRIANGLES);
		for (k = 0; k <= 360; k += DEF_D) {
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(cos(k), sin(k), 1);
			glColor3f(0.0, 1.0, 1.0);
			glVertex3f(cos(k) * 0.0, sin(k) * 0.0, 0);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(cos(k + DEF_D * 0.1) * 0.0, sin(k + DEF_D  * 0.1) * 0.0, 0);

			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(cos(k) * 0.0, sin(k) * 0.0, 0);
			glColor3f(0.0, 1.0, 1.0);
			glVertex3f(cos(k), sin(k), 1);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(cos(k + DEF_D * 0.1), sin(k + DEF_D  * 0.1), 1);
		}
		glEnd();
	}

	void normalize(GLfloat *a) {
		GLfloat d = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
		a[0] /= d; a[1] /= d; a[2] /= d;
	}
	void drawTri(GLfloat *a, GLfloat *b, GLfloat *c, int div, float r) {
		if (div <= 0) {
			glNormal3fv(a); glVertex3f(a[0] * r, a[1] * r, a[2] * r);
			glNormal3fv(b); glVertex3f(b[0] * r, b[1] * r, b[2] * r);
			glNormal3fv(c); glVertex3f(c[0] * r, c[1] * r, c[2] * r);
		}
		else {
			GLfloat ab[3], ac[3], bc[3];
			for (int i = 0; i < 3; i++) {
				ab[i] = (a[i] + b[i]) / 2;
				ac[i] = (a[i] + c[i]) / 2;
				bc[i] = (b[i] + c[i]) / 2;
			}
			normalize(ab); normalize(ac); normalize(bc);
			drawTri(a, ab, ac, div - 1, r);
			drawTri(b, bc, ab, div - 1, r);
			drawTri(c, ac, bc, div - 1, r);
			drawTri(ab, bc, ac, div - 1, r);  //<--Comment this line and sphere looks really cool!
		}
	}
	void drawSphere(int ndiv, float radius, float color[3]) {
		float X = .525731112119133606f;
		float Z = .850650808352039932f;

		GLfloat vdata[12][3] = {
			{ -X, 0.0f, Z },{ X, 0.0f, Z },{ -X, 0.0f, -Z },{ X, 0.0f, -Z },
			{ 0.0f, Z, X },{ 0.0f, Z, -X },{ 0.0f, -Z, X },{ 0.0f, -Z, -X },
			{ Z, X, 0.0f },{ -Z, X, 0.0f },{ Z, -X, 0.0f },{ -Z, -X, 0.0f }
		};
		GLuint tindices[20][3] = {
			{ 0,4,1 },{ 0,9,4 },{ 9,5,4 },{ 4,5,8 },{ 4,8,1 },
			{ 8,10,1 },{ 8,3,10 },{ 5,3,8 },{ 5,2,3 },{ 2,7,3 },
			{ 7,10,3 },{ 7,6,10 },{ 7,11,6 },{ 11,0,6 },{ 0,1,6 },
			{ 6,1,10 },{ 9,0,11 },{ 9,11,2 },{ 9,2,5 },{ 7,2,11 } };
		
		glBegin(GL_TRIANGLES);
		glColor3f(color[0], color[1], color[2]);
		for (int i = 0; i < 20; i++)
			drawTri(vdata[tindices[i][0]], vdata[tindices[i][1]], vdata[tindices[i][2]], ndiv, radius);
		glEnd();
	}

	void TestAudio()
	{

	}
	
	void draw(const Matrix& viewMatrix)
	{
		// Modify the camera matrix and wasd
		Matrix viewSpaceMatrix = viewMatrix;// *modelMatrix;
		drawAxis(viewMatrix);
		//glLoadMatrixf((GLfloat*)viewMatrix.mVal);
		//glColor3f(1.0f, 1.0f, 1.0f);
		drawSquare();

		Matrix translate1 = Matrix::makeTranslationMatrix(5.0f, 0.0f, 0.0f);
		Matrix viewSpaceMatrix1 = viewMatrix * translate1;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix1.mVal);
		drawCircle(1, 1, 0);
		
		/*
		//Draw Opaque object first
		glDisable(GL_BLEND);
		Matrix translate1 = Matrix::makeTranslationMatrix(0.0f, 0.0f, 0.0f);
		Matrix scale1 = Matrix::makeScaleMatrix(Vector(2.0f, 2.0f, 2.0f));
		Matrix viewSpaceMatrix1 = viewMatrix * translate1 * scale1;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix1.mVal);
		drawSquare();

		//Draw Opaque object first
		glDisable(GL_BLEND);
		Matrix translate2 = Matrix::makeTranslationMatrix(0.0f, 0.0f, -8.0f);
		Matrix scale2 = Matrix::makeScaleMatrix(Vector(2.0f, 2.0f, 2.0f));
		Matrix viewSpaceMatrix2 = viewMatrix * translate2 * scale2;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix2.mVal);
		drawSquare();

		//Draw transparent objects here
		glBindTexture(GL_TEXTURE_2D, mTextureID[0]);
		glDepthMask(false);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE); //ADD BLEND
										   //glBlendFunc(GL_DST_COLOR, GL_ZERO);
		Matrix translate3 = Matrix::makeTranslationMatrix(0.0f, 0.0f, -4.0f);
		Matrix scale3 = Matrix::makeScaleMatrix(Vector(1.0f, 1.0f, 1.0f));
		Matrix viewSpaceMatrix3 = viewMatrix * translate3 * scale3;
		glLoadMatrixf((GLfloat*)viewSpaceMatrix3.mVal);
		drawCube();
		glDepthMask(true);
		*/
		
	}
};

#endif
