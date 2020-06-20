#include "Utils.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "TextureManager.h"


#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 480

#define nvbos 2


/* ======================= Data Function ======================= */

GLFWwindow* window;
int width;
int height;


bool first_mouse = false;

float delta_time = 0.0f;
float last_frame = 0.0f;

GLuint program;
TextureManager textures;

GLuint vbo[nvbos];

glm::vec3 cube_pos;
float pitch, yaw;

glm::mat4 proj_mat, view_mat, model_mat;
float angle_view = 60, near = 0.1f, far = 1000.f;


/* ======================= Setup Function ======================= */


void setupCube(){
	float vertexPositions[108] = {
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f , 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};

	float cubeTexCoords[] = {
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	};

	/*
		- glGenBuffers đăng ký id cho mạng có không gian trong GPU
		- glBindBuffer cài đặt rằng id loại dữ liệu này là GL_ARRAY_BUFFER hoặc là active vbo 
		- glBufferData lưu dư liệu array vào một buffer id nơi lưu có thể là khác nhau phụ thuộc vào Static , draw 
	
	*/
	glGenBuffers(nvbos, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeTexCoords), &cubeTexCoords, GL_STATIC_DRAW);
	

}

void setup(){

	
	cube_pos = glm::vec3(0.0f, -2.0f, 0.0f);
	proj_mat = Utils::getMatrixProj(window, angle_view, near, far);
	program = Utils::createShaderProgram("Vertex.glsl", "Frag.glsl");
	Camera::setUp(glm::vec3(0.0, 0.0, 8.0));
	textures.loadTexture( "brick" , "brick/brick.jpg");
	setupCube();
}


/* ======================= Loop Function ======================== */

void display(){
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.1f, 0.1f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// active progam
	glUseProgram(program);

	// get value glsl 
	GLuint id_mv_mat = glGetUniformLocation(program, "mv_mat");
	GLuint id_proj_mat = glGetUniformLocation(program, "proj_mat");

	model_mat = glm::translate(glm::mat4(1.0f), cube_pos);
	view_mat = Camera::getView();

	glUniformMatrix4fv(id_proj_mat, 1, GL_FALSE, glm::value_ptr(proj_mat));
	glUniformMatrix4fv(id_mv_mat, 1, GL_FALSE, glm::value_ptr(view_mat * model_mat));


	// associate VBO with the corresponding vertex attribute in the vertex shader
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);


	// least 80 texture unit (texture unit can access texture from a shader ) 
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures.get("brick"));


	// adjust OpenGL settings and draw model
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}
void update(){


}
void process(){
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){

		Camera::moveLeft(5.f* delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		Camera::moveDown(5.f* delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		Camera::moveRight(5.f*delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		Camera::moveUp(5.f* delta_time);
	}
}

/* ======================= Event Function ======================= */

void funcResize(GLFWwindow* window, int widht, int height){
	glViewport(0, 0, widht, height);
	proj_mat = Utils::getMatrixProj(window, angle_view, near, far);
}

void funcKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, true);
	}
}
void funcMouse(GLFWwindow* window, int button, int action, int mods){

}
void funcScroll(GLFWwindow* window, double xoffset, double yoffset){

}
void funcCurrsorPos(GLFWwindow* window, double xpos, double ypos){

	float xoffset = float (xpos) - SCREEN_WIDTH / 2;
	float yoffset = SCREEN_HEIGHT / 2 - float(ypos);

	const float sensitivity = 10.f*2*delta_time;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Camera::rotate(xoffset, yoffset);
	glfwSetCursorPos(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}



int main(){

	WindowSetting setting;
	setting.antialiaLevel = 8;
	setting.cursor = CUR_HIDDEN;
	setting.displayErrors = true;

	window = Utils::createWindow("OpenGL", SCREEN_WIDTH, SCREEN_HEIGHT ,setting);
	if (window == NULL) exit(EXIT_FAILURE);


	Utils::setKeyboardFunc(window, funcKeyboard);
	Utils::setMouseFunc(window, funcMouse);
	Utils::setScrollFunc(window, funcScroll);
	Utils::setCurrsorPosFunc(window, funcCurrsorPos);
	Utils::setResizeFunc(window, funcResize);

	Utils::setActiveWindow(window);

	setup();
	while (!glfwWindowShouldClose(window)){
		float current_time = glfwGetTime();
		delta_time = current_time - last_frame;
		last_frame = current_time;
		process();
		update();
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	Utils::destroyWindow(window);

	return EXIT_SUCCESS;
}