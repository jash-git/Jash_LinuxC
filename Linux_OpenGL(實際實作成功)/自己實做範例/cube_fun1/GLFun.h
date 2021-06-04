#ifndef GLFun_H
#define GLFun_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
// Define a constant for the value of PI
#define GL_PI 3.1415f

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;

static GLfloat specular[]={0.7,0.7,0.7,1.0};		//燈光性質
static GLfloat specref[]={0.5,0.5,0.2,1.0};		//反射性質

// Flags for effects
#define MODE_SOLID 0
#define MODE_LINE  1
#define MODE_POINT 2

int iMode = MODE_SOLID;
//GLboolean bEdgeFlag = TRUE;

///////////////////////////////////////////////////////////////////////////////
// Reset flags as appropriate in response to menu selections
void ProcessMenu(int value)
	{
	switch(value)
		{
		case 1:
			iMode = MODE_SOLID;
			break;

		case 2:
			iMode = MODE_LINE;
			break;

		case 3:
			iMode = MODE_POINT;
			break;

		/*case 4:
			bEdgeFlag = TRUE;
			break;*/

		/*case 5:
		default:
			bEdgeFlag = FALSE;*/
			break;
		}

	glutPostRedisplay();
	}
//**************************************************
void ReduceToUnit(float vector[3])
{
	float length;
	
	// Calculate the length of the vector		
	length = (float)sqrt((vector[0]*vector[0]) + 
						(vector[1]*vector[1]) +
						(vector[2]*vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if(length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
}
void calcNormal(float v[3][3], float out[3])
{
	float v1[3],v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);
}

void drawRetc(float W,float L,float H)
{

//	if(Line_Side==false)
//	{
	float normal[3];
	glBegin(GL_QUADS);
	{
	float v[4][3] =	{{ W/2, L/2, H/2},
					{ -W/2, L/2, H/2},
					{ -W/2,-L/2, H/2},
					{  W/2,-L/2, H/2}};

	// Calculate the normal for the plane
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ W/2, L/2, H/2},
					{  W/2,-L/2, H/2},
					{  W/2,-L/2,-H/2},
					{  W/2, L/2,-H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ W/2, L/2, H/2},
					{  W/2, L/2,-H/2},
					{ -W/2, L/2,-H/2},
					{ -W/2, L/2, H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{-W/2, L/2, H/2},
					{ -W/2, L/2,-H/2},
					{ -W/2,-L/2,-H/2},
					{ -W/2,-L/2, H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{-W/2,-L/2, H/2},
					{ -W/2,-L/2,-H/2},
					{  W/2,-L/2,-H/2},
					{  W/2,-L/2, H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ W/2, L/2,-H/2},
					{  W/2,-L/2,-H/2},
					{ -W/2,-L/2,-H/2},
					{ -W/2, L/2,-H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	glEnd();

}


void drawCir(float radius, float L, int segment)
{
	float angle[2];
	float normal[3];

//	if(Line_Side==false)
//	{
	glBegin(GL_QUADS);
	{
		for(angle[0]=0.0f, angle[1]=0.0f; angle[0]< (2.0f*GL_PI); angle[0]+=((2.0f*GL_PI)/segment))
		{
			float r[4][3] = {{radius*(float)cos(angle[1]), radius*(float)sin(angle[1]), L/2}, 
							{ radius*(float)cos(angle[1]), radius*(float)sin(angle[1]),-L/2},  
							{ radius*(float)cos(angle[0]), radius*(float)sin(angle[0]),-L/2},
							{ radius*(float)cos(angle[0]), radius*(float)sin(angle[0]), L/2}};
			calcNormal(r,normal);
			glNormal3fv(normal);
			glVertex3fv(r[0]);
			glVertex3fv(r[1]);
			glVertex3fv(r[2]);
			glVertex3fv(r[3]);
			angle[1]=angle[0];
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f(0.0f, 0.0f, L/2);
		for (angle[1]=0.0f, angle[0]=0.0f; angle[1]<=(2.0f*GL_PI); angle[0]+=((2.0f*GL_PI)/segment))
		{
			float x=radius*(float)cos(angle[0]);
			float y=radius*(float)sin(angle[0]);

			float v[3][3] = {{0.0f                , 0.0f                , L/2},
							{ radius*(float)cos(angle[1]), radius*(float)sin(angle[1]), L/2},   
							{ radius*(float)cos(angle[0]), radius*(float)sin(angle[0]), L/2}};
			calcNormal(v,normal);
			glNormal3fv(normal);
			glVertex3f(x, y, L/2);
			angle[1]=angle[0];
		}
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f(0.0f, 0.0f,-L/2);
		for (angle[1]=0.0f, angle[0]=0.0f; angle[1]<= (2.0f*GL_PI); angle[0]+=((2.0f*GL_PI)/segment))
		{
			float x=radius*(float)cos(angle[0]);
			float y=radius*(float)sin(angle[0]);		
			float v[3][3] = {{0.0f                , 0.0f                ,-L/2},
							{ radius*(float)cos(angle[0]), radius*(float)sin(angle[0]),-L/2},   
							{ radius*(float)cos(angle[1]), radius*(float)sin(angle[1]),-L/2}};
			calcNormal(v,normal);
			glNormal3fv(normal);
			glVertex3f(x, y,-L/2);
			angle[1]=angle[0];
		}
	}
	glEnd();

}
void drawtriangle(float W,float L,float H)
{
//	if(Line_Side==false)
//	{
	static float normal[3];
	glBegin(GL_QUADS);
	{
	float v[4][3] =	{{ W/2, L/2, -H/2},
					{ -W/2, L/2, H/2},
					{ -W/2,-L/2, H/2},
					{  W/2,-L/2, -H/2}};

	// Calculate the normal for the plane
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[3][3] = {{ W/2, L/2, -H/2},
					{  -W/2,L/2, -H/2},
					{  -W/2,L/2,H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ -W/2, L/2, H/2},
					{  -W/2, L/2,-H/2},
					{ -W/2, -L/2,-H/2},
					{ -W/2, -L/2, H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[3][3] = {{-W/2, -L/2, H/2},
					{ -W/2, -L/2,-H/2},
					{ W/2,-L/2,-H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
		float v[4][3] = {{ W/2, L/2, -H/2},
					{  W/2, -L/2,-H/2},
					{ -W/2, -L/2,-H/2},
					{ -W/2, L/2, -H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	
	glEnd();


}

void drawtriangle_1(float W,float L,float H)
{
//	if(Line_Side==false)
//	{
	static float normal[3];
	glBegin(GL_QUADS);
	{
	float v[4][3] =	{{ W/2, -L/2, H/2},
					{ W/2, L/2, H/2},
					{ -W/2,L/2, -H/2},
					{  -W/2,-L/2, -H/2}};

	// Calculate the normal for the plane
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[3][3] = {{ W/2, L/2, H/2},
					{  W/2,L/2, -H/2},
					{  -W/2,L/2,-H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ W/2, L/2, H/2},
					{  W/2, -L/2,H/2},
					{ W/2, -L/2,-H/2},
					{ W/2, L/2, -H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[3][3] = {{W/2, -L/2, H/2},
					{ -W/2,-L/2,-H/2},
					{ W/2, -L/2,-H/2}};  
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ -W/2, L/2, -H/2},
					{  W/2, L/2,-H/2},
					{ W/2, -L/2,-H/2},
					{ -W/2, -L/2, -H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	
	glEnd();


}

void drawladder(float U,float L,float H,float W)

{
//	if(Line_Side==false)
//	{
	static float normal[3];
	glBegin(GL_QUADS);
	{
	float v[4][3] =	{{ U/2, W/2, H/2},
					{ -U/2, W/2, H/2},
					{ -U/2,-W/2, H/2},
					{  U/2,-W/2, H/2}};
	// Calculate the normal for the plane
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ U/2, -W/2, H/2},
					{ -U/2,-W/2, H/2},
					{ -L/2,-W/2,-H/2},
					{  L/2,-W/2,-H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ U/2, W/2, H/2},
					{  U/2, -W/2,H/2},
					{  L/2, -W/2,-H/2},
					{  L/2, W/2, -H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{U/2, W/2, H/2},
					{ -U/2, W/2,H/2},
					{ -L/2, W/2,-H/2},
					{  L/2, W/2,-H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{-U/2,W/2, H/2},
					{ -U/2,-W/2,H/2},
					{ -L/2,-W/2,-H/2},
					{ -L/2,W/2,-H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	{
	float v[4][3] = {{ L/2, W/2,-H/2},
					{ -L/2, W/2,-H/2},
					{ -L/2,-W/2,-H/2},
					{  L/2,-W/2,-H/2}};   
	calcNormal(v,normal);
	glNormal3fv(normal);
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]);
	glVertex3fv(v[3]);
	}
	glEnd();


}


//****************************************************
#endif
