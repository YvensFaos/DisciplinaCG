#include "GLFW\glfw3.h"

#include "Vector3.h"
#include "Polygon2d.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

GLFWwindow* window;

int qtt;
bool splitLine;
Vector3* p;
Vector3* q;
Polygon2D* polygons;

float _width = 640;
float _height = 480;

#define pi 3.14159265359
#define max(a,b) (a > b)?a:b;
#define min(a,b) (a < b)?a:b;

void defineClipWindow(void)
{
	int length = 6;
	Edge2d* edges = new Edge2d[length];

	int i = 0;
	edges[i++] = new Edge2d(new Vector3(0.15f, 0.25f), 
						  new Vector3(0.85f, 0.25f));

	edges[i++] = new Edge2d(new Vector3(0.85f, 0.25f), 
						  new Vector3(0.55f, 0.60f));

	edges[i++] = new Edge2d(new Vector3(0.55f, 0.60f), 
						  new Vector3(0.85f, 0.90f));

	edges[i++] = new Edge2d(new Vector3(0.85f, 0.90f), 
						  new Vector3(0.15f, 0.90f));

	edges[i++] = new Edge2d(new Vector3(0.15f, 0.90f), 
						  new Vector3(0.35f, 0.60f));

	edges[i++] = new Edge2d(new Vector3(0.35f, 0.60f),
						  new Vector3(0.15f, 0.25f));

	polygons = new Polygon2D[1];
	qtt = 1;
	polygons[0] = Polygon2D(length, edges);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	float modifier = 1.f;
	if(mods == GLFW_MOD_SHIFT)
	{
		modifier = .1f;
	}
	if(action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if(key == GLFW_KEY_LEFT_SHIFT)
		{
		}
		if (key == GLFW_KEY_UP) {
			polygons[0].translate(.0f, .05f*modifier);
		}

		if (key == GLFW_KEY_DOWN) {
			polygons[0].translate(.0f, -.05f*modifier);
		}

		if (key == GLFW_KEY_LEFT) {
			polygons[0].translate(-0.05f*modifier, .0f);
		}

		if (key == GLFW_KEY_RIGHT) {
			polygons[0].translate(0.05f*modifier, .0f);
		}
		if (key == GLFW_KEY_A) {
			polygons[0].rotate(5.f*modifier);
		}
		if (key == GLFW_KEY_S) {
			polygons[0].rotate(-5.f*modifier);
		}

		if(key == GLFW_KEY_D)
		{
			Polygon2D* pol = &polygons[0];
			int i = 0;
			int length = pol->length;
			bool solved = false;
			while(!solved)
			{
				Vector3* i1 = &pol->points[i];
				float x = i1->x;
				float y = i1->y;
				pol->translate(-x,-y);
	
				Vector3* i2 = &pol->points[(i+1)%length];

				float magnitude = i2->magnitude();
				float angle = (acos(i2->x/(float)magnitude))*180/pi;
				if(i2->y < 0)
				{
					angle *= -1;
				}
				pol->rotate(-angle);

				Vector3* i3 = &pol->points[(i+2)%length];
				if(i3->y < 0)
				{
					int j = (i+3)%length;
					int counter = 0;
					Vector3* i4 = nullptr;
					while(counter < length)
					{
						i4 = &pol->points[j];
						if(i4->y >= 0)
						{
							break;
						}
						counter++;
					}

					if(counter != length)
					{
						p = new Vector3(i2);
						q = new Vector3(i4);
						splitLine = true;
						solved = true;
					}
				}

				i++;
				if(i == length)
				{
					solved = true;
				}
			}
		}
	}
}

void glfwMouseCallback(GLFWwindow* window, int button, int action, int mods) 
{
	if(action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		x /= 640.f;
		y /= 480.f;
		y -= 1;
		y *= -1;

		printf("Mouse = x[%f] e y[%f]\n",x,y);
	}
}

void display(void)
{
	float ratio;
	int width, height;

	glfwGetFramebufferSize(window, &width, &height);

	ratio = width / (float) height;

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.f, 2.f, -2.f, 2.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Desenhar a �rea de desenho
	glBegin(GL_LINES);
	for(int j = 0; j < qtt; j++)
		{
			Polygon2D* pol = &polygons[j];
			int length = pol->length;
			for (int i = 0; i < length; i++)
			{
				glColor3f(1.f, 1.f, 1.f);
				glVertex3f(pol->edges[i].o.x, pol->edges[i].o.y, 0.f);
				glVertex3f(pol->edges[i].p.x, pol->edges[i].p.y, 0.f);
			}
	}

	if(splitLine)
	{
		glColor3f(.0f, .0f, 1.f);
		glVertex3f(p->x, p->y, 0.f);
		glVertex3f(q->x, q->y, 0.f);
	}

	//Desenhar eixos
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-2.f, .0f, 0.f);
	glVertex3f( 2.f, .0f, 0.f);

	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(.0f, -2.f, 0.f);
	glVertex3f(.0f,  2.f, 0.f);
	glEnd();

	glfwSwapBuffers(window);
	glfwPollEvents();
}

int main(void)
{
	defineClipWindow();

	if (!glfwInit())
	{
		return -1;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	window = glfwCreateWindow(_width, _height, "Splitting Polygons", NULL, NULL);
	splitLine = false;
	qtt = 1;

	if (!window)
    {
        glfwTerminate();
        return 0;
    }

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, 620, 120);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetMouseButtonCallback(window, glfwMouseCallback);

	if(window != 0)
	{
		while (!glfwWindowShouldClose(window))
		{
			display();
		}
		glfwTerminate();
	}
    return 0;
}