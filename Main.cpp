#include<iostream>
#include <fstream>
#include <string>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include"shader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"


const unsigned int width = 800;
const unsigned int height = 800;
int Height = 257; // 513
int Width = 257;
int Size_vertices = Height * Width * 6;
float vertices[257*257* 6];
float vertices_diamond[257][257];
unsigned int indices[(257 - 1) * 257 * 2];

void find_max_min(float* max, float* min, float vertices[][257]);
void make_water_on_sides(float vertices[][257], float max, float  min);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void print_diamond_square (float vertices[][257]) {

	for (int i = 0; i < Height; i += 1) {
		for (int j = 0; j < Width; j += 1) {
			printf("%f  ", vertices[i][j]);
		}
		printf("\n\n");
	}
	return;

}



void smoothen_terrain(float vertices_diamond[][257], int smooth) {
	float left, right, top, bottom;
	left = 0;
	right = 0;
	bottom = 0;
	top = 0;
	for (int s = 0; s < smooth; s++) {
		for (int i = 0; i < Height; i += 1) {
			for (int j = 0; j < Width; j += 1) {
				(j - 1 >= 0) ? left = vertices_diamond[i][j - 1] : 0;
				(j + 1 < Width) ? right = vertices_diamond[i][j + 1] : 0;
				(i - 1 >= 0) ? bottom = vertices_diamond[i - 1][j] : 0;
				(i + 1 < Height) ? top = vertices_diamond[i + 1][j] : 0;

				vertices_diamond[i][j] = (left + right + bottom + top) / 4.0f;

			}
		}
	}


}

void find_max_min(float* max, float* min, float vertices[][257]) {
	float max2;
	float min2 = max2 = vertices[0][0];
	for (int i = 0; i < Height; i += 1) {
		for (int j = 0; j < Width; j += 1) {
			if (max2 < vertices[i][j]) {
				 max2 = vertices[i][j];
			}
			if (min2 > vertices[i][j]) {
				min2 = vertices[i][j];
			}
		}
	}
	*max = max2;
	*min = min2;
	
}

void make_diamond_vertices(float vertices[][257], float MAX_HEIGHT, float roughness, int smooth,int terrain) {
	
	// start step
	// random initializer
	float min;
	float max = 0;

	srand(time(NULL));
	float seed1 = (float)(rand() % (int)MAX_HEIGHT);
	float seed2 = (float)(rand() % (int)MAX_HEIGHT);
	float seed3 = (float)(rand() % (int)MAX_HEIGHT);
	float seed4 = (float)(rand() % (int)MAX_HEIGHT);

	vertices[0][0] = seed1;
	vertices[(Height - 1)][0] = seed2;
	vertices[0][(Width - 1)] = seed3;
	vertices[(Height - 1)][Width - 1] = seed4;

	int step_size = Width - 1;

	while (step_size > 1) {
		int half_step_size = step_size / 2;

		// half step
		
		for (int y = 0; y < Height - 1; y += step_size) {
			for (int x = 0; x < Width - 1; x += step_size) {

				float topleft = vertices[x][y];
				float topright = vertices[x + step_size][y];
				float botleft = vertices[x][y + step_size];
				float botright = vertices[x + step_size][y + step_size];
				float avg = (topleft + topright + botleft + botright) / 4.0f;
				
				// float random genertaor
				float random_dis = ((((float)(rand() % 100)) / 100.0f ) * 2* roughness) - roughness;
				vertices[y + half_step_size][x + half_step_size] = avg + random_dis;		
				
			}
		}
		 
		// square step

		
		for (int y = 0; y < Height ; y += half_step_size) {			
			for (int x = (y + half_step_size)%step_size ; x < Width; x += step_size) {			
				float left = x - half_step_size < 0 ? 0 : vertices[x - half_step_size][y];
				float right = x + half_step_size >= Width ? 0 : vertices[x + half_step_size][y];
				float top = y + half_step_size  >= Height ? 0 : vertices[x][y + half_step_size] ;
				float bottom = y - half_step_size < 0 ? 0: vertices[x][y - half_step_size];

				vertices[y][x] =(top + bottom+ right + left) / 4.0;
				float random_dis = ((((float)(rand() % 100)) / 100.0f) * 2* roughness) - roughness;
				vertices[y][x] += random_dis;
	
			}
			
		}

		step_size = step_size / 2;
		roughness /= 2.0f;

	}
	
	find_max_min( &max, &min, vertices_diamond);
	if (terrain == 1) {
		make_water_on_sides(vertices_diamond, max, min);
	}
	smoothen_terrain(vertices_diamond, smooth);


	
}

void make_water_on_sides(float vertices[][257],float max,float  min) {
	for (unsigned int i = 0; i < Height; i++)
	{
		for (unsigned int j = 0; j < Width; j++)
		{
			if ((i >= 0 && i < 5) || (i >= 251 && i < 257) || (j >= 0 && j < 5) || (j >= 251 && j < 257)) {
				vertices[i][j] = ((((float)(rand() % 100)) / 100.0f) * 0.3 * (max - min));
			}
		}
	}
}


void make_vertices(float vertices[], float diam_vert[][257], int colour, int terrain){
	unsigned index = 0;
	float red= 0.0f, blue = 0.0f, green = 0.0f;
	float max, min;
	find_max_min(&max, &min, diam_vert);



	for(unsigned int i = 0; i < Height; i++)
	{
		for (unsigned int j = 0; j < Width; j++)
		{
			float y  = diam_vert[i][j];

			if (colour == 0) {
				// blue
				green = 0.00f;
				blue = (y - min) / (max - min);
				red = 0.0f;
			}
			else if (colour == 1) {
				if (terrain == 1) {
					if ((y - min) / (max - min) <= 0.3) { // black to blue
						float upper_limit = 0.3 * (max - min) + min;
						blue = (y - min) / (upper_limit - min);
						red = 0.0f;
						green = 0.0f;
					}
					if ((y - min) / (max - min) > 0.3 && (y - min) / (max - min) < 0.8) // brown to green 
					{
						float lower_limit = 0.3 * (max - min) + min;
						float upper_limit = 0.8 * (max - min) + min;
						red = (y - lower_limit) / (upper_limit - lower_limit) * (0.58);
						green = 0.19 + 0.09 * (y - lower_limit) / (upper_limit - lower_limit);
						blue = 0.12 - 0.12 * (y - lower_limit) / (upper_limit - lower_limit);

					}
					if ((y - min) / (max - min) >= 0.8) { // grey to white
						float lower_limit = 0.8 * (max - min) + min;
						blue = (y - lower_limit) / (max - lower_limit);
						red = (y - lower_limit) / (max - lower_limit);
						green = (y - lower_limit) / (max - lower_limit);
					}
				}
				else if (terrain == 2) {
					if ((y - min) / (max - min) <= 0.3) { // black to blue
						float upper_limit = 0.3 * (max - min) + min;
						blue = (y - min) / (upper_limit - min);
						red = 0.0f;
						green = 0.0f;
					}
					if ((y - min) / (max - min) > 0.3) // dark green to green
					{
						float lower_limit = 0.3 * (max - min) + min;
						float upper_limit = (max - min) + min;
						red = (y - lower_limit) / (upper_limit - lower_limit) * (0.58);
						green = 0.19 + 0.8 * (y - lower_limit) / (upper_limit - lower_limit);
						blue = 0;

					}

				}
				else if (terrain == 3) {
					
						if (i >= 0 && i < 257 && j >= 0 && j < 90) {

							red = 0.26 + 0.78 * (y - min) / (max - min);
							green = 0.22 + 0.71 * (y - min) / (max - min);
							blue = 0.14 + 0.70 * (y - min) / (max - min);
						}
						else if (i >= 0 && i < 257 && j >= 60 && j < 85) {
							int random = rand() % 2;
							if (random == 0) {
								red = 0.26 + 0.78 * (y - min) / (max - min);
								green = 0.22 + 0.71 * (y - min) / (max - min);
								blue = 0.14 + 0.70 * (y - min) / (max - min);
							}
							else {
								blue = (y - min) / (max - min);
								red = 0.0f;
								green = 0.0f;
							}
						}
						else if (i >= 0 && i < 257 && j >= 85 && j < 100) {
							int random = rand() % 4;
							if (random == 2) {
								red = 0.26 + 0.78 * (y - min) / (max - min);
								green = 0.22 + 0.71 * (y - min) / (max - min);
								blue = 0.14 + 0.70 * (y - min) / (max - min);
							}
							else {
								blue = (y - min) / (max - min);
								red = 0.0f;
								green = 0.0f;
							}
						}
						else if (i >= 0 && i < 257 && j >= 100 && j < 110 ) {
							int random = rand() % 16;
							if (random == 4) {
								red = 0.26 + 0.78 * (y - min) / (max - min);
								green = 0.22 + 0.71 * (y - min) / (max - min);
								blue = 0.14 + 0.70 * (y - min) / (max - min);
							}
							else {
								blue = (y - min) / (max - min);
								red = 0.0f;
								green = 0.0f;
							}
						}
						else {
							blue = (y - min) / (max - min);
							red = 0.0f;
							green = 0.0f;
						}

				}

	
			}
			else if(colour == 2) { // grey scale
				float colour =  ((y - min) / (max - min)) * 1 + 0.0;
				red = colour;
				blue = colour;
			    green = colour;
				
			}
		
			else if (colour == 3) {
				float colour = ((y - min) / (max - min)) * 1 + 0.0;
				red = colour;
				blue = 0.0f;
				green = 0.0f;
			}
			else if (colour == 4) {
				float colour = ((y - min) / (max - min)) * 1 + 0.0;
				red = 0.0f;
				blue = 0.0f;
				green = colour;
			}
	
			// vertex generation

			vertices[index] = ((float)(-Height / 2.0f + i))/(float)Height*2 ;
			index = index + 1;

			if (terrain == 1) {
				vertices[index] = (((y - min) / (float)(max - min))) - 0.5f;
				index = index + 1;
			}
			if (terrain == 2) {
				vertices[index] = (((y - min) / (float)(max - min)))*0.3 - 0.5f;
				index = index + 1;
			}
			if (terrain == 3) {
				vertices[index] = (((y - min) / (float)(max - min))) * 0.2 - 0.5f;
				index = index + 1;
			}
			
			vertices[index] = ((float)(-Width / 2.0f + j))/ (float)Width*2 ;
			index = index + 1;

			vertices[index] = red;
			index = index + 1;
			
			vertices[index] = green;
			index = index + 1;

			vertices[index] = blue;
			index = index + 1;
			
		}
	}
	
	
}

unsigned int* make_indices(unsigned int array[]) {
	
	unsigned index = 0;
	for (unsigned int i = 0; i < Height - 1; i++) {
		for (unsigned int j = 0; j < Width; j++)     {
			for (unsigned int k = 0; k < 2; k++)     {
				indices[index] = (j + Width * (i + k));
				index = index + 1;
			}
		}
	}
	return indices;
}





int main() {
	std::string filename = "terrain.txt";
	std::ifstream file(filename);
	if (!file.is_open())
		throw std::runtime_error("failed to open " + filename);
	
		std::string terrain_type;
		getline(file, terrain_type);
		std::cout << terrain_type << "\n";

		std::string terrain_texture;
		getline(file, terrain_texture);
		std::cout << terrain_texture << "\n";

		std::string terrain_colour;
		getline(file, terrain_colour);
		std::cout << terrain_colour << "\n";

		file.close();


		//user inputs
		float MAX_HEIGHT = 10;
		float roughness = 8.0f; // -roughness to roughness
		float rotation = 30.0f;
	
		int smooth = 2;
		int terrain = 1;
		int colour = 1;

		if (terrain_type == "mountain")
			terrain = 1;
		else if (terrain_type == "plain")
			terrain = 2;
		else if (terrain_type == "beach")
			terrain = 3;
		else {
			return 0;
		}
		
		if (terrain_texture == "rough") {
			roughness = 8.0f;
			smooth = 1;
		}	
		else if (terrain_type == "smooth") {
			roughness = 3.0f;
			smooth = 6;
		}

		if (terrain_colour == "blue_scale") {
			colour = 0;
		}
		else if (terrain_colour == "original") {
			colour = 1;
		}
		else if (terrain_colour == "grey_scale") {
			colour = 2;
		}
		else if (terrain_colour == "red_scale") {
			colour = 3;
		}
		else if (terrain_colour == "green_scale") {
			colour = 4;
		}

		if (terrain == 1) {
			rotation = 50.0f;
		}
		if (terrain == 2) {
			rotation = 35.0f;
		}
		bool mesh = true;
		make_diamond_vertices(vertices_diamond, MAX_HEIGHT, roughness, smooth, terrain);

		make_vertices(vertices, vertices_diamond, colour, terrain);
		make_indices(indices);

		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* window = glfwCreateWindow(width, height, "CS334:Project", NULL, NULL);
		if (window == NULL){
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		gladLoadGL();
		glViewport(0, 0, width, height);
		Shader shaderProgram("default.vert", "default.frag");
		VAO VAO1;
		VAO1.Bind();
		VBO VBO1(vertices, sizeof(vertices));
		EBO EBO1(indices, sizeof(indices));
		VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
		VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

		VAO1.Unbind();
		VBO1.Unbind();
		EBO1.Unbind();

		glEnable(GL_DEPTH_TEST);

		Camera camera(width, height, rotation, glm::vec3(0.0f, 0.0f, 3.5f));

		while (!glfwWindowShouldClose(window)){
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shaderProgram.Activate();
			camera.inputs(window);
			camera.matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMat");

			VAO1.Bind();
			const unsigned int NUM_STRIPS = Height - 1;
			const unsigned int NUM_VERTS_PER_STRIP = Width * 2;

			if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
				mesh = false;
			}
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
				mesh = true;
			}

			for (unsigned int strip = 0; strip < NUM_STRIPS; ++strip)
			{
				if (mesh == false) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				glDrawElements(GL_TRIANGLE_STRIP,   
					NUM_VERTS_PER_STRIP, 
					GL_UNSIGNED_INT,     
					(void*)(sizeof(unsigned int)
						* NUM_VERTS_PER_STRIP
						* strip)); 
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		VAO1.Delete();
		VBO1.Delete();
		EBO1.Delete();
		shaderProgram.Delete();
		glfwDestroyWindow(window);
		glfwTerminate();

	return 0;
}