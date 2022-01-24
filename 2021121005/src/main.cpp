#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "shader.h"
#include <bits/stdc++.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "camera.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, glm::mat4 *view);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

float first;
float second=0.0f;
int vs=0;
int flag=0;
float strt;
glm::vec3 dupf=glm::vec3(1.0f);
glm::vec3 dupp=glm::vec3(1.0f);

//amera camera(glm::vec3(0.0f, 0.0f,2.0f));
glm::vec3 pos = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
float mx = 0.0f, my = 0.0f, mz = 0.0f;
float spin = 0.0f;
float angle = 0.0f;
int main(int rgc, char *argv[])
{
	//glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow *window =
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "shiridi", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

	//----------------------------------------vertices top------------------------------------------------------------------------------
	//glm::mat4 myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	int sides = atoi(argv[1]);
	float vertices[2 * (3 + 3) * (sides + 1)];
	double cp = 1.0f / (sides);
	double ini = cp;
	float r = cp, g = cp, b = cp, alpha = 1.0f;
	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = -0.5f;
	vertices[3] = 0;
	vertices[4] = 0;
	vertices[5] = 0; //;vertices[3]=1;
	ini += cp;
	vertices[6] = 0.5f;
	vertices[7] = 0.0f;
	vertices[8] = -0.5f;
	vertices[9] = 0;
	vertices[10] = 0;
	vertices[11] = 1; //;vertices[7]=1;
	ini += cp;
	float vertices_bottom[3 * (sides + 1)];
	int coun = 12;
	glm::mat4 myMatrix = glm::mat4(1.0f);
	glm::vec4 myVector(0.5f, 0.0f, -0.5f, 0.0f);
	myMatrix = glm::rotate(myMatrix, glm::radians((float)360 / sides), glm::vec3(0.0f, 0.0f, 1.0f));
	for (int i = 1; i < sides; i++)
	{
		myVector = myMatrix * myVector;
		double dArray[4] = {0.0};
		const float *pSource = (const float *)glm::value_ptr(myVector);
		cout << "(";
		float x = pSource[0], y = pSource[1], z = pSource[2];
		for (int j = 0; j < 3; ++j)
		{
			vertices[coun++] = pSource[j];
			cout << pSource[j] << ", ";
		}
		int mul = (i % 2 == 0) ? 0 : 1;
		vertices[coun++] = 0;
		vertices[coun++] = mul;
		vertices[coun++] = 1 - mul;
		ini += cp;
		cout << ")";
		cout << endl;
	}
	//------------------------------vertices bottom--------------------------------------------------------------
	cout << "-----------------------" << endl
		 << endl;

	glm::mat4 transMatrix = glm::mat4(1.0f);
	transMatrix = glm::translate(transMatrix, glm::vec3(0.0f, 0.0f, 1.0f));
	int ind = coun;
	int l = 0;
	for (int i = 0; i < (sides + 1) * 6; i += 6)
	{
		l++;
		float x = vertices[i], y = vertices[i + 1], z = vertices[i + 2], w = 1.0f;
		glm::vec4 oldvec(x, y, z, w);
		glm::vec4 newvec = transMatrix * oldvec;
		const float *pSource1 = (const float *)glm::value_ptr(newvec);
		cout << "(";
		for (int j = 0; j < 3; ++j)
		{
			vertices[ind++] = pSource1[j];

			cout << pSource1[j] << ",";
		}
		int mu = (l % 2 == 0) ? 1 : 0;
		//cout<<mu<<"*************************"<<endl;
		if (l == 0)
		{
			vertices[ind++] = 0;
			vertices[ind++] = 0;
			vertices[ind++] = 0;
		}
		else
		{
			vertices[ind++] = 0;
			vertices[ind++] = mu;
			vertices[ind++] = 1 - mu;
		}
		cout << ")";
		cout << " "
			 << "(" << x << " ," << y << " ," << z << ")" << endl;
		cout << endl;
	}
	unsigned int indices[2 * 3 * sides + 6 * sides];
	coun = 0;
	int count = 1;
	for (int i = 0; i < sides; i++)
	{
		indices[coun] = 0;
		cout << indices[coun] << " ";
		coun++;
		indices[coun] = count;
		cout << indices[coun] << " ";
		coun++;
		indices[coun] = count + 1;
		cout << indices[coun] << " ";
		coun++;
		count++;
		cout << endl;
	}

	int pad = sides + 1;
	count = 1;
	indices[3 * sides - 1] = 1;
	cout << coun << "*************" << endl;
	for (int i = 0; i < sides; i++)
	{
		indices[coun] = pad;
		cout << indices[coun] << " ";
		coun++;
		indices[coun] = pad + count;
		cout << indices[coun] << " ";
		coun++;
		indices[coun] = pad + count + 1;
		cout << indices[coun] << " ";
		count++;
		coun++;
		cout << endl;
	}

	cout << endl;
	int top_p = 1;
	int bottom_p = pad + 1;
	for (int i = 0; i < sides; i++)
	{
		indices[coun] = top_p;
		coun++;
		indices[coun] = bottom_p;
		coun++;
		indices[coun] = bottom_p + 1;
		coun++;
		top_p++;
		bottom_p++;
	}
	indices[coun - 1] = pad + 1;

	top_p = 1;
	bottom_p = pad + 1;
	for (int i = 0; i < sides; i++)
	{
		indices[coun] = bottom_p + 1;
		coun++;
		indices[coun] = top_p;
		coun++;
		indices[coun] = top_p + 1;
		coun++;
		top_p++;
		bottom_p++;
	}
	cout << "*********" << endl;
	indices[coun - 1] = 1;
	indices[coun - 3] = pad + 1;
	for (int i = 0; i < coun; i++)
	{
		if (i % 3 == 0)
		{
			cout << endl;
		}
		cout << indices[i] << ", ";
	}
	cout << endl;

	cout << endl;
	for (int i = 0; i < 2 * (3 + 3) * (sides + 1); i += 6)
	{
		cout << vertices[i] << " ," << vertices[i + 1] << " ," << vertices[i + 2] << endl;
	}
	cout << endl;
	vector<float> v;
	int low = 0;
	int high = +1;
	int i = 0;
	cout << endl;
	float a = 0.0f, c = 1.0f;
	b = 0.0f;
	for (i = 1; i < (sides); i += 1)
	{
		v.push_back(vertices[0]);
		v.push_back(vertices[1]);
		v.push_back(vertices[2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(vertices[i * 6]);
		v.push_back(vertices[i * 6 + 1]);
		v.push_back(vertices[i * 6 + 2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(vertices[(i + 1) * 6]);
		v.push_back(vertices[(i + 1) * 6 + 1]);
		v.push_back(vertices[(i + 1) * 6 + 2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
	}
	v.push_back(vertices[0]);
	v.push_back(vertices[1]);
	v.push_back(vertices[2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(vertices[i * 6]);
	v.push_back(vertices[i * 6 + 1]);
	v.push_back(vertices[i * 6 + 2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(vertices[1 * 6]);
	v.push_back(vertices[(1) * 6 + 1]);
	v.push_back(vertices[(1) * 6 + 2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	cout << "*****************" << v.size() << endl;

	cout << endl;
	int pad2 = 6 * (sides + 1);

	for (i = 1; i < (sides); i += 1)
	{
		v.push_back(vertices[0 + pad2]);
		v.push_back(vertices[1 + pad2]);
		v.push_back(vertices[2 + pad2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(vertices[i * 6 + pad2]);
		v.push_back(vertices[i * 6 + 1 + pad2]);
		v.push_back(vertices[i * 6 + 2 + pad2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(vertices[(i + 1) * 6 + pad2]);
		v.push_back(vertices[(i + 1) * 6 + 1 + pad2]);
		v.push_back(vertices[(i + 1) * 6 + 2 + pad2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
	}
	v.push_back(vertices[0 + pad2]);
	v.push_back(vertices[1 + pad2]);
	v.push_back(vertices[2 + pad2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(vertices[i * 6 + pad2]);
	v.push_back(vertices[i * 6 + 1 + pad2]);
	v.push_back(vertices[i * 6 + 2 + pad2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(vertices[1 * 6 + pad2]);
	v.push_back(vertices[(1) * 6 + 1 + pad2]);
	v.push_back(vertices[(1) * 6 + 2 + pad2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);

	for (i = 1; i < sides; i++)
	{
		if (i % 2 == 0)
		{
			a = 1.0f, b = 0.0f, c = 0.0f;
		}
		else
		{
			a = 0.0f, b = 1.0f, c = 0.0f;
		}
		v.push_back(vertices[i * 6]);
		v.push_back(vertices[i * 6 + 1]);
		v.push_back(vertices[i * 6 + 2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(vertices[i * 6 + pad2]);
		v.push_back(vertices[i * 6 + 1 + pad2]);
		v.push_back(vertices[i * 6 + 2 + pad2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(vertices[(i + 1) * 6 + pad2]);
		v.push_back(vertices[(i + 1) * 6 + 1 + pad2]);
		v.push_back(vertices[(i + 1) * 6 + 2 + pad2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
	}
	if (i % 2 == 0)
	{
		a = 1.0f, b = 0.0f, c = 0.0f;
	}
	else
	{
		a = 1.0f, b = 1.0f, c = 0.0f;
	}
	v.push_back(vertices[i * 6]);
	v.push_back(vertices[i * 6 + 1]);
	v.push_back(vertices[2 + i * 6]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(vertices[i * 6 + pad2]);
	v.push_back(vertices[i * 6 + 1 + pad2]);
	v.push_back(vertices[i * 6 + 2 + pad2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(vertices[1 * 6 + pad2]);
	v.push_back(vertices[(1) * 6 + 1 + pad2]);
	v.push_back(vertices[(1) * 6 + 2 + pad2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);

	for (i = 1; i < sides; i++)
	{
		if (i % 2 == 0)
		{
			a = 1.0f, b = 0.0f, c = 0.0f;
		}
		else
		{
			a = 0.0f, b = 1.0f, c = 0.0f;
		}
		v.push_back(vertices[(i + 1) * 6 + pad2]);
		v.push_back(vertices[(i + 1) * 6 + 1 + pad2]);
		v.push_back(vertices[(i + 1) * 6 + 2 + pad2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(vertices[i * 6]);
		v.push_back(vertices[i * 6 + 1]);
		v.push_back(vertices[i * 6 + 2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
		v.push_back(vertices[(i + 1) * 6]);
		v.push_back(vertices[(i + 1) * 6 + 1]);
		v.push_back(vertices[(i + 1) * 6 + 2]);
		v.push_back(a);
		v.push_back(b);
		v.push_back(c);
	}
	if (i % 2 == 0)
	{
		a = 1.0f, b = 0.0f, c = 0.0f;
	}
	else
	{
		a = 1.0f, b = 1.0f, c = 0.0f;
	}
	v.push_back(vertices[(1) * 6 + pad2]);
	v.push_back(vertices[(1) * 6 + 1 + pad2]);
	v.push_back(vertices[2 + (1) * 6 + pad2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(vertices[i * 6]);
	v.push_back(vertices[i * 6 + 1]);
	v.push_back(vertices[i * 6 + 2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);
	v.push_back(vertices[1 * 6]);
	v.push_back(vertices[(1) * 6 + 1]);
	v.push_back(vertices[(1) * 6 + 2]);
	v.push_back(a);
	v.push_back(b);
	v.push_back(c);

	float vert[36 * sides * 2];
	cout << "*****************" << v.size() << endl;
	for (int i = 0; i < v.size(); i++)
	{
		vert[i] = v[i];
	}
	cout << endl;
	unsigned int EBO, VAO, VBO, VAO2, VBO2, EBO2, *VAOs;
	glGenVertexArrays(1, &VAO);

	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(0);

	//*******************************************************

	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 model = glm::mat4(1.0f);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::rotate(transform, glm::radians(-90.0f), //(float)glfwGetTime(),
								glm::vec3(1.0f, 0.0f, 0.0f));

		//model = glm::rotate(model, (float)glfwGetTime()*1.0f, glm::vec3(0.0f, 1.0f, 0.0f));


		model = glm::rotate(model, (spin==1.0f)?(float)glfwGetTime()*1:angle, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(mx, my, mz));

		ourShader.use();
		unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
						   glm::value_ptr(transform));

		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		// // note that we're translating the scene in the reverse direction of where we want to move
		if(vs==1){
			pos-= glm::normalize(glm::cross(up,front))*((float)glfwGetTime()-strt)*0.005f;
			front=(glm::normalize(pos-glm::vec3(mx,my,mz)))*((float)glfwGetTime()-strt)*(-0.005f);
			float *po=(float *)glm::value_ptr(pos);
			//out<<pos[0]<<"&&"<<po[1]<<"::"<<po[2]<<endl;

		}
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(pos, pos + front, up);


		//projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 10.0f);

		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT,
			0.1f, 100.0f);
		int modelLoc = glGetUniformLocation(ourShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int modelLoc2 = glGetUniformLocation(ourShader.ID, "view");
		glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(view));
		int modelLoc3 = glGetUniformLocation(ourShader.ID, "projection");
		glUniformMatrix4fv(modelLoc3, 1, GL_FALSE, glm::value_ptr(projection));
		processInput(window, &view);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36 * sides);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window, glm::mat4 *view)
{
	const float c = 0.05f;
	glm::mat4 transz=glm::mat4(1.0f);
	transz = glm::translate(transz, glm::vec3(0.0f, 0.0f, 0.05f));

	glm::mat4 transzl = glm::mat4(1.0f);
	transzl = glm::translate(transzl, glm::vec3(0.0f, 0.0f, -0.05f));


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		front=glm::vec3(mx,my,mz)-pos;
		flag=0;
		pos = transzl*glm::vec4(pos,1.0f);
		front = -1.0f * glm::normalize(pos-glm::vec3(mx,my,mz));
	}
	glm::mat4 transr = glm::mat4(1.0f);
	glm::mat4 transx = glm::mat4(1.0f);
	glm::mat4 transxl = glm::mat4(1.0f);
	glm::mat4 transyl=glm::mat4(1.0f);
	glm::mat4 transy=glm::mat4(1.0f);
	transx = glm::translate(transx, glm::vec3(0.05f, 0.0f, 0.0f));
	transxl = glm::translate(transxl, glm::vec3(-0.05f, 0.0f, 0.0f));
	transyl = glm::translate(transyl, glm::vec3(0.0f, 0.05f, 0.0f));
	transy = glm::translate(transy, glm::vec3(0.0f, -0.05f, 0.0f));
	//transr = glm::rotate(transr, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		front=glm::vec3(mx,my,mz)-pos;
		flag=0;
		pos = transz*glm::vec4(pos,1.0f);
		front = -1.0f * glm::normalize(pos-glm::vec3(mx,my,mz));

	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		front=glm::vec3(mx,my,mz)-pos;
		flag=0;
		pos = transx * glm::vec4(pos, 1.0f);
		front = -1.0f * glm::normalize(pos-glm::vec3(mx,my,mz));
		//front=glm::normalize(pos);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		front=glm::vec3(mx,my,mz)-pos;
		flag=0;
		pos = transxl * glm::vec4(pos, 1.0f);
		front = -1.0f * glm::normalize(pos-glm::vec3(mx,my,mz));
		//front=glm::normalize(pos);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		front=glm::vec3(mx,my,mz)-pos;
		flag=0;
		pos = transyl*glm::vec4(pos,1.0f);
		front = -1.0f*glm::normalize(pos-glm::vec3(mx,my,mz));
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		front=glm::vec3(mx,my,mz)-pos;
		flag=0;
		pos = transy*glm::vec4(pos,1.0f);
		front = -1.0f*glm::normalize(pos-glm::vec3(mx,my,mz));
	}
	//left
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	{
		flag=1;
		glm::vec3 ne = glm::vec3(mx, my, mz);
		ne -= glm::normalize(glm::cross(front, up)) * c;
		float *ps = (float *)glm::value_ptr(ne);
		mx = ps[0];
		my = ps[1];
		mz = ps[2];
		cout << ps[0] << "    " << ps[1] << "   " << ps[2] << endl;
	}

	//right
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		flag=1;
		glm::vec3 ne = glm::vec3(mx, my, mz);
		ne += glm::normalize(glm::cross(front, up)) * c;
		float *ps = (float *)glm::value_ptr(ne);
		mx = ps[0];
		my = ps[1];
		mz = ps[2];
		cout << ps[0] << "    " << ps[1] << "   " << ps[2] << endl;
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		flag=1;
		glm::vec3 u=glm::vec3(mx,my,mz);
		glm::vec3 ne=glm::normalize(glm::cross(front,up));
		u-=glm::normalize(glm::cross(ne,front))*c;
		float *ps = (float *)glm::value_ptr(u);
		mx = ps[0];
		my = ps[1];
		mz = ps[2];
	}

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		flag=1;
		glm::vec3 u=glm::vec3(mx,my,mz);
		glm::vec3 ne=glm::cross(front,up);
		u+=glm::normalize(glm::cross(ne,front))*c;
		float *ps = (float *)glm::value_ptr(u);
		mx = ps[0];
		my = ps[1];
		mz = ps[2];
		cout<<mx<<" "<<my<<" "<<mz<<endl;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		flag=1;
		glm::vec3 dir=glm::normalize(front);
		glm::vec3 u(1.0f);
		u=glm::vec3(mx,my,mz);
		u+=dir*c;
		float *ps = (float *)glm::value_ptr(u);
		mx = ps[0];
		my = ps[1];
		mz = ps[2];

	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		flag=1;
		glm::vec3 dir=glm::normalize(front);
		glm::vec3 u(1.0f);
		u=glm::vec3(mx,my,mz);
		u-=dir*c;
		float *ps = (float *)glm::value_ptr(u);
		mx = ps[0];
		my = ps[1];
		mz = ps[2];

	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		flag=0;
		pos = glm::vec3(0.0f, 0.0f, 2.0f);
		front = glm::vec3(glm::normalize(glm::vec3(mx, my, mz - 2.0f)));
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		front=glm::vec3(mx,my,mz)-pos;
		if ((float)glfwGetTime() - first > 0.5f)
		{

			if (spin == 1.0f)
			{
				spin = 0.0f;
				angle=(float)glfwGetTime() * 1.0f;
				cout<<"set"<<endl;
			}
			else
			{
				spin=1.0f;
				cout<<"reset"<<endl;
			}
		}
		first=(float)glfwGetTime();
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS){
		flag=0;
		if((float)glfwGetTime()-second>0.5f){
			if(vs==0){
				vs=1;
				strt=(float)glfwGetTime();
			}
			else{
				vs=0;
			}
		}
		second=(float)glfwGetTime();
	}

}


void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}