#include "skybox.h"

// Preloads the Skybox
// Essentially, just uses loadTexture functions to preload all the texture data
void skybox::Load()
{
	// 6 Textures needed for the Skybox
	skyboxTextures = new GLuint[6];

	// Load the Textures and assign them to locations in the Texture Array
	skyboxTextures[0] =  loadTexture("Skybox/North.ppm", 512, 512);
	skyboxTextures[1] =  loadTexture("Skybox/South.ppm", 512, 512);
	skyboxTextures[2] =  loadTexture("Skybox/West.ppm", 512, 512);
	skyboxTextures[3] =  loadTexture("Skybox/East.ppm", 512, 512);
	skyboxTextures[4] =  loadTexture("Skybox/Top.ppm", 512, 512);
	skyboxTextures[5] =  loadTexture("Skybox/Bottom.ppm", 512, 512);

	groundTexture =  loadGround("Skybox/Bottom.ppm", 512, 512);
}

// Load the Textures for the Skybox
// Textures stored in Skybox folder, same location as executeable
GLuint skybox::loadTexture(string inPath, int width, int height)
{
	// Load .ppm data
	ppmData = PPM::Read(inPath, width, height);

	// Generate Texture Name
	glGenTextures(1, &singleTexture);
	glBindTexture(GL_TEXTURE_2D, singleTexture);

	// Setup Texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ppmData);

	// Clear Data
	free(ppmData);

	return singleTexture;
}

GLuint skybox::loadGround(string inPath, int width, int height)
{
	// Load .ppm data
	ppmData = PPM::Read(inPath, width, height);

	// Generate Texture Name
	glGenTextures(1, &singleTexture);
	glBindTexture(GL_TEXTURE_2D, singleTexture);

	// Setup Texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //set texture environment parameters
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		// Tile Ground Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ppmData);

	// Clear Data
	free(ppmData);

	return singleTexture;
}

void skybox::Create(float skyboxSize)
{
	// Store Current Matrix
	glPushMatrix();

	// Disable Lighting for the Skybox
	glDisable(GL_LIGHTING);

	// Front
	glBindTexture(GL_TEXTURE_2D, skyboxTextures[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1); glVertex3f(skyboxSize, -skyboxSize, -skyboxSize);
		glTexCoord2f(1, 0); glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
		glTexCoord2f(0, 0); glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
		glTexCoord2f(0, 1); glVertex3f(-skyboxSize, -skyboxSize, -skyboxSize);
    glEnd();

	// Back
	glBindTexture(GL_TEXTURE_2D, skyboxTextures[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1); glVertex3f(-skyboxSize, -skyboxSize, skyboxSize);
		glTexCoord2f(1, 0); glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
		glTexCoord2f(0, 0); glVertex3f(skyboxSize, skyboxSize, skyboxSize);
		glTexCoord2f(0, 1); glVertex3f(skyboxSize, -skyboxSize, skyboxSize);
    glEnd();

	// West
	glBindTexture(GL_TEXTURE_2D, skyboxTextures[2]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1); glVertex3f(-skyboxSize, -skyboxSize, -skyboxSize);
		glTexCoord2f(1, 0); glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
		glTexCoord2f(0, 0); glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
		glTexCoord2f(0, 1); glVertex3f(-skyboxSize, -skyboxSize, skyboxSize);
    glEnd();

	// East
	glBindTexture(GL_TEXTURE_2D, skyboxTextures[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1); glVertex3f(skyboxSize, -skyboxSize, skyboxSize);
		glTexCoord2f(1, 0); glVertex3f(skyboxSize, skyboxSize, skyboxSize);
		glTexCoord2f(0, 0); glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
		glTexCoord2f(0, 1); glVertex3f(skyboxSize, -skyboxSize, -skyboxSize);
    glEnd();

	// Top
	glBindTexture(GL_TEXTURE_2D, skyboxTextures[4]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1); glVertex3f(skyboxSize, skyboxSize, -skyboxSize);
		glTexCoord2f(1, 0); glVertex3f(skyboxSize, skyboxSize, skyboxSize);
		glTexCoord2f(0, 0); glVertex3f(-skyboxSize, skyboxSize, skyboxSize);
		glTexCoord2f(0, 1); glVertex3f(-skyboxSize, skyboxSize, -skyboxSize);
    glEnd();

	// Bottom
	glBindTexture(GL_TEXTURE_2D, skyboxTextures[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 1); glVertex3f(skyboxSize, -skyboxSize, skyboxSize);
		glTexCoord2f(1, 0); glVertex3f(skyboxSize, -skyboxSize, -skyboxSize);
		glTexCoord2f(0, 0); glVertex3f(-skyboxSize, -skyboxSize, -skyboxSize);
		glTexCoord2f(0, 1); glVertex3f(-skyboxSize, -skyboxSize, skyboxSize);
    glEnd();

	// Ground, not technically part of the skybox.
	glBindTexture(GL_TEXTURE_2D, groundTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.5, 0.5); glVertex3f(skyboxSize, 0, skyboxSize);
		glTexCoord2f(0.5, 0); glVertex3f(skyboxSize, 0, -skyboxSize);
		glTexCoord2f(0, 0); glVertex3f(-skyboxSize, 0, -skyboxSize);
		glTexCoord2f(0, 0.5); glVertex3f(-skyboxSize, 0, skyboxSize);
    glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();

	// Re-Enable Lighting
	glEnable(GL_LIGHTING);
}