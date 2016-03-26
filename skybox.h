#ifndef SKYBOX_H
#define SKYBOX_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <GL/glut.h>
#include "PPM.hpp"

using namespace std;

class skybox
{
	public:

		void skybox::Create(float skyboxSize);									// Create the Skybox
		void skybox::Load();													// Load Skybox Data
		GLuint skybox::loadTexture(string inPath, int width, int height);		// Load a Texture using PPM
		GLuint skybox::loadGround(string inPath, int width, int height);

		GLuint singleTexture;					// Single Texture
		GLuint *skyboxTextures;					// Array for Skybox Textures
		GLuint groundTexture;					// Ground Texture
		unsigned char * ppmData;				// loadTexture parsed .ppm file data
};

#endif