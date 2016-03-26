// Header file for the Main Program
// Constains basic includes, variable, constant, and runction declarations

#include "simpleModel.h"
#include "cityFile.h"
#include "skybox.h"
#include <GL/glut.h>
#include <vector>
#include <sstream>
#include <Windows.h>

// Constant Declaration
#define PI 3.1415926535897932384626433832795	// PI definition for Trig

float skyboxRad;								// Skybox Radius

int frameCount;
int curveFrameCount;

// Bezier Variables
vector<float> nextBezVec (3);
int currentCurve;
float lookz;

// Lighting
float intensitySun;								// Intensity
float intensityLightningS;
float intensityLightningNW;

// Animated Lighting
bool sunLightInc;
float sunLightDelta;
float sunLightMax;
float sunLightMin;

// Lighting Timing
float bolt1Time;
float bolt2Time;
int bolt1Frames;
int bolt2Frames;
 
// Function Declaration
void printBitmapString(float x, float y, void *font, char *string, int red, int green, int blue);
void enter2D();
void enter3D();
void reshape(int width, int height);
void keyPress (unsigned char key, int x, int y);
void cloudLighting();
void lightningIntensity(float time1, float time2);
vector<float> nextBezier(float curve[4][3], float currentTime);
void camera();
void imageCapture();
void init();
void idle();
void display();

// Model Directory, filled with exec argument
char* inPath;

// Imported City Variable
cityFile inCity;

// Imported Skybox Variable
skybox inSkybox;