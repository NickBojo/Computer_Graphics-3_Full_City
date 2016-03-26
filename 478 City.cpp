// 478 City

// Nick Bojanowski
// #23947054
// EECE 478

#include "478 City.h"

const int screenWidth = 1280;		// Initial Screen Size
const int screenHeight = 720;
const int videoFramerate = 30;		// 30 FPS for video output

// Bezier Curve Points
float curve1[4][3] = {
	{0.0,3.0,140.0},
	{0.0,8.0,140.0},
	{0.0,-8.0,47.0},
	{0.0,10.0,45.0}
};

float curve2[4][3] = {
	{0.0,10.0,45.0},
	{100.0,10.0,45.0},
	{100.0,20.0,-50.0},
	{0.0,15.0,-50.0}
};

float curve3[4][3] = {
	{0.0,15.0,-50.0},
	{-100.0,20.0,-50.0},
	{-100.0,20.0,45.0},
	{0.0,20.0,45.0}
};

float curve4[4][3] = {
	{0.0,20.0,45.0},
	{0.0,30.0,100.0},
	{0.0,100.0,0.0},
	{0.0,150.0,80.0}
};


// Bezier Curve LOD
int LOD = 240;

///// INITIAL CONDITIONS /////
//////////////////////////////
void init()
{
	glClearColor(0, 0, 0, 255);		// Background to Black
	glColor3ub(255, 255, 255);		// Initial Colour to White

	glEnable(GL_TEXTURE_2D);		// Enable 2D Texturing
	glEnable(GL_DEPTH_TEST);		// Depth Enable
	glFrontFace(GL_CCW);			// Enable Back-Face Culling
	glEnable(GL_LIGHTING);			// Enable Lights
	glEnable(GL_LIGHT0);			// Sunlight
	glEnable(GL_LIGHT1);			// Lightning South
	glEnable(GL_LIGHT2);			// Lightning NorthWest
	glEnable(GL_NORMALIZE);

	gluLookAt (0.0, -2.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	// Initial GLook

	// Set Framecount to 0
	frameCount = 0;
	curveFrameCount = 0;

	// Set Initial Bezier Curve
	currentCurve = 1;
	lookz = 0.0;

	// Initial Lighting Setup
	skyboxRad = 1000.0f;				// Skybox Radius
	intensitySun = 0.3;					// Initial ambient light intensity
	intensityLightningS = 0.0;			// Variables for Lightning
	intensityLightningNW = 0.0;

	// Lighting Animation Setup
	sunLightInc = TRUE;					// Cloud Simulation light change direction
	sunLightDelta = 0.004;				// Speed of Cloud Simulation light change
	sunLightMax = 0.7;					// Cloud Simulation light Max
	sunLightMin = 0.3;					// Cloud Simulation light Min

	// Lightning Timing Setup
	bolt1Time = 4.5;
	bolt2Time = 7;

	// Load the City
	cout << "Attempting to load " << inPath << endl << endl;
	inCity.loadCity(inPath);

	// Load the Skybox
	inSkybox.Load();
}


// Enters a 2D Perspective.
// Called in Init
// Used for 2D Text Printing
void enter2D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);	// Ortho Projection

	glMatrixMode(GL_MODELVIEW);
}


// Enters a 3d Perspective
// Used to return to 3D after 2D operations
void enter3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(59, (GLfloat)screenWidth/(GLfloat)screenHeight, 1, 3000.0);	// Perspective Projection, FOV 90, 1000 distance

	glMatrixMode(GL_MODELVIEW);
}


// Uses glutBitmapCharacter to display an entire String
// Input: x, y, font, string, colour (0-255)
void printBitmapString(float x, float y, void *font, char *string, int red, int green, int blue) 
{  
	glLoadIdentity();
	char *ch;

	glColor3ub(red, green, blue);			// text colour

	glRasterPos2f(x,y);						// x, y co-ordinate position for text

	for (ch=string; *ch != '\0'; ch++)		// iterates through input String
	{
		glutBitmapCharacter(font, *ch);		// print character after character
	}

	glColor3ub(255, 255, 255);				// Colour to White
}


// Exit with Esc key
void keyPress (unsigned char key, int x, int y)
{
	// Exit Program with Esc
	if (key == 27)
	{
		exit(0);
	}
}


// Varies the intensity of the ambient light, to simulate cloudy weather
void cloudLighting()
{
	// Gradient ambient light over time
	// If variable set to false, decrease
	if (sunLightInc == FALSE)
	{
		intensitySun -= sunLightDelta;
	}

	// Otherwise, increase light
	else
	{
		intensitySun += sunLightDelta;
	}

	// Switch Light gradient direction at boundry points
	if (intensitySun <= sunLightMin || intensitySun >= sunLightMax)
	{
		sunLightInc = !sunLightInc;
	}
}


// Lightning Flashes
// Takes the Time Period (in seconds) between flashes, per source
void lightningIntensity(float time1, float time2)
{
	// Calculate Number of Frames between bolts
	bolt1Frames = (videoFramerate * time1);
	bolt2Frames = (videoFramerate * time2);

	// Two Frames per Flash
	if (((frameCount % bolt1Frames) == 0) || ((frameCount % bolt1Frames+1) == 0))
	{
		intensityLightningS = 1.0;
	}
	else
	{
		intensityLightningS = 0.0;
	}

	if (((frameCount % bolt2Frames) == 0) || ((frameCount % bolt2Frames+1) == 0))
	{
		intensityLightningNW = 1.0;
	}
	else
	{
		intensityLightningNW = 0.0;
	}
}


// Using Formula and the current time, calculates the next point on a Bezier Curve
// Returns the X Y Z Point in vector form
vector<float> nextBezier(float curve[4][3], float currentTime)
{
	vector<float> returnVec (3);

	float time = (float)(currentTime)/(LOD);
	float inverseTime = (1.0 - time);

	// Coefficients
	float B3 = time*time*time;
	float B2 = 3*time*time*inverseTime;
	float B1 = 3*time*inverseTime*inverseTime;
	float B0 =  inverseTime*inverseTime*inverseTime;

	// Using Coefficients and Bezier Formula
	returnVec[0] = B0*curve[0][0] + B1*curve[1][0] + B2*curve[2][0] +  B3*curve[3][0] ;

	returnVec[1] = B0*curve[0][1] + B1*curve[1][1] + B2*curve[2][1] + B3*curve[3][1] ;

	returnVec[2] = B0*curve[0][2] + B1*curve[1][2] + B2*curve[2][2] + B3*curve[3][2] ;

	return returnVec;
}


// Updates the Next XYZ camera position using the Current Bezier Curve
// Switches to the next curve at the end of each curve
// Essentially following a B-spline
void camera()
{
	if (currentCurve == 1)
	{
	nextBezVec = nextBezier(curve1, curveFrameCount);
	}

	else if (currentCurve == 2)
	{
	nextBezVec = nextBezier(curve2, curveFrameCount);
	}

	else if (currentCurve == 3)
	{
	nextBezVec = nextBezier(curve3, curveFrameCount);
	}

	else if (currentCurve == 4)
	{
	nextBezVec = nextBezier(curve4, curveFrameCount);
	lookz += 0.2;
	}

	if (curveFrameCount >= (LOD))
	{
		currentCurve++;
		curveFrameCount = 0;
	}
}


void imageCapture()
{
	// Create Storage for ReadPixels
	unsigned char * imageData = new unsigned char [screenWidth * screenHeight * 3];

	std::string filename;
	std:: stringstream out;
	out << frameCount;
	filename = out.str();;

	glReadPixels(0, 0, screenWidth, screenHeight, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	PPM::Write(filename, imageData, screenWidth, screenHeight);

	delete [] imageData;
}


///// RESHAPE METHOD /////
//////////////////////////

void reshape(int width, int height)
{
	if (height == 0) height = 1;	// divide by 0 elimination

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);	// Set viewport to window specifications

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(59, (GLfloat)width/(GLfloat)height, 1, 3000.0);	// Perspective Projection, VFOV 59 = HFOV 90, 3000 distance

	glMatrixMode(GL_MODELVIEW);
}


////////////// IDLE METHOD //////////////
// Contains Lighting and Camera Timing //
/////////////////////////////////////////

void idle()
{
	// Increase Framecount
	frameCount++;
	curveFrameCount++;

	// Light, Lightning, and Camera
	camera();
	cloudLighting();
	lightningIntensity(bolt1Time, bolt2Time);
	
	// Record Frames for Video Creation
	imageCapture();

	glutPostRedisplay();

}


///// DISPLAY METHOD /////
//////////////////////////

void display()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt (nextBezVec[0], nextBezVec[1], nextBezVec[2], 0.0, 0.0, lookz, 0.0, 1.0, 0.0);

	// Sun Position and Intensity
	GLfloat sunAmbientColour[] = {intensitySun, intensitySun, intensitySun, 1.0f};
	GLfloat sunDiffuseColour[] = {(intensitySun+0.2), (intensitySun+0.2), (intensitySun+0.2), 1.0f};
	GLfloat sunSpecColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat sunPosition[] = {(skyboxRad*0.8), (skyboxRad*0.8), 200.0f, 1.0f};

	// Lightning Position and Intensity
	GLfloat lightningSColour[] = {intensityLightningS, intensityLightningS, intensityLightningS, 1.0f};
	GLfloat lightningSPosition[] = {0.0f, (skyboxRad*0.8), (skyboxRad*0.8), 1.0f};
	GLfloat lightningNWColour[] = {intensityLightningNW, intensityLightningNW, intensityLightningNW, 1.0f};
	GLfloat lightningNWPosition[] = {-(skyboxRad*0.8), (skyboxRad*0.8), -(skyboxRad*0.8), 1.0f};

	// AmbientLighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, sunAmbientColour);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuseColour);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecColour);
    glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightningSColour);
	glLightfv(GL_LIGHT1, GL_POSITION, lightningSPosition);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightningNWColour);
	glLightfv(GL_LIGHT2, GL_POSITION, lightningNWPosition);

	// Draw the City
	inCity.createCity();

	// Draw the Skybox
	inSkybox.Create(skyboxRad);

	// Print Words on the Screen
	enter2D();
		printBitmapString(0.70, -0.94, GLUT_BITMAP_TIMES_ROMAN_24, "Nick Bojanowski", 0, 0, 0);
	enter3D();

	glutSwapBuffers();							// Refresh
	glFlush();
}


///// MAIN METHOD ////////
//////////////////////////

int main(int argc, char** argv)
{
	// Take the First Argument for use as a file path for the imported model
	if (argc >= 2)
	{
		inPath = argv[1];
	}

	// If there are no arguments
	// This will allow the environment to load without a model
	else
	{
		inPath = "StormTown.city";
	}

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Setup Viewing Window
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("478 City");

	init();

	glutDisplayFunc(display);				// display callbacks
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyPress);				// normal keyboard callback

	glutMainLoop();
}

