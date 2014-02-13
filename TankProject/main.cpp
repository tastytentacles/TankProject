

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// OpenGL without using GLUT - 2013 by Neil Dansey, Tim Dykes and Ian Cant, and using excerpts from here:
// http://bobobobo.wordpress.com/2008/02/11/opengl-in-a-proper-windows-app-no-glut/
// Feel free to adapt this for what you need, but please leave these comments in.

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


#pragma once

#include <windows.h>	// need to be able to create windows etc
#include <gl/gl.h>		// opengl header file
#include <gl/glu.h>		// glu header file - glu helps us set up the perspective projection
#include <iostream>
#include <math.h>

#include "vertexHolder.h"
#include "TankClasses.h"

#define PI 3.14159265359

// some basic numbers to hold the position and size of the window
#define WIDTH		800
#define HEIGHT		600
#define TOPLEFTX	0
#define TOPLEFTY	0


// some player constants
#define ROTATION_SPEED	1.3f
#define NUM_PLAYERS		0


#define TOTAL_PLAYERS	2
#define TANK_SPEED		0.1f
#define TANK_MAX_SPEED	2
#define TANK_ROT_SPEED	0.1f

// some constants to help normalise the frame rate between different machines:

// IDEAL_FPS is a frame rate we can REASONABLY require from the user's PC, but is also high enough to provide smooth animation.
#define IDEAL_FPS		30

// IDEAL_DELTA is how long each frame would take in milliseconds if the game was running at the ideal frame rate.
#define IDEAL_DELTA		(1000 / IDEAL_FPS)

// later in the code, all animation updates will be normalised so the game will appear to run at the IDEAL_FPS, regardless of actual frame rate.
// this is why the IDEAL_FPS needs to be decent enough for smooth animation.
// *BUT* the downside to this is that if the PC cannot achieve a frame rate of IDEAL_FPS, the animation may appear jerky,
// because it will be updated by larger amounts in order to appear to be running at correct speed. 
// this is why the IDEAL_FPS also has to be easily-achievable by the user's pc, to minimise the chance of jerkiness.



// include the opengl and glu libraries
#pragma comment(lib, "opengl32.lib")	
#pragma comment(lib, "glu32.lib")



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// classes:

// this class holds all of the information about a player
class playerInfo
{
	public:
		playerInfo();		// the constructor
		
		bool upPressed;		// whether or not they are pressing their "up" key
		bool downPressed;	// whether or not they are pressing their "down" key
		bool leftPressed;	// and so on
		bool rightPressed;
		float rotation;		// the rotation of their avatar

		float xPos;			// position in world
		float yPos;
	
};

// this class holds all of the information about a game state
class gameInfo
{
	public:
		gameInfo();			// the constructor
		~gameInfo();		// the destructor

		
		// variables required for the frame delta (DWORD is basically a long integer)
		DWORD frameStartTime;	// what the system time (in millisecs) was at the start of the last frame
		DWORD frameEndTime;		// what the system time (in millisecs) was at the end of the last frame 
		DWORD frameDelta;		// difference between the two (in millisecs), i.e. how long the last frame took

		
		// the data for each player will be stored using an instance of the playerInfo class
		// but we will use dynamically-allocated memory, so it needs to be initialised using a pointer
		// we will use an array to hold the pointers to the players, in case we want to have 1000 players or something.
		// however, the array itself will also be dynamically allocated, so that needs to use a pointer too!
		// hence two stars rather than just one
		playerInfo** playerArray;

		// ----- tank code
		_Tank** _lowOrbit;
		_Shell** _hightOrbit;
		// ----- tank code
		
};


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


// function prototypes:

// these were already here as part of the windows application template...
LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );	
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow );

// these are ones we have added...
void updateGame(HDC deviceContext, gameInfo* currentGame);				// update the game here
void draw(HDC deviceContext, gameInfo* currentGame);        			// drawing function containing OpenGL function calls



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// In a C++ Windows app, the starting point is WinMain() rather than _tmain() or main().
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow )
{


	// some variables need for our game...
    MSG msg;								// this will be used to store messages from the operating system
	bool needToQuit = false;				// whether or not the player has quit the app
	gameInfo* currentGame = new gameInfo;	// this creates our game instance, and calls it currentGame



	// this bit creates a window class, basically a template for the window we will make later, so we can do more windows the same.
    WNDCLASS myWindowClass;
    myWindowClass.cbClsExtra = 0;											// no idea
    myWindowClass.cbWndExtra = 0;											// no idea
    myWindowClass.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );	// background fill black
    myWindowClass.hCursor = LoadCursor( NULL, IDC_ARROW );					// arrow cursor       
    myWindowClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );				// type of icon to use (default app icon)
    myWindowClass.hInstance = hInstance;									// window instance name (given by the OS when the window is created)   
    myWindowClass.lpfnWndProc = WndProc;									// window callback function - this is our function below that is called whenever something happens
    myWindowClass.lpszClassName = TEXT("GameScreen");				// our new window class name
    myWindowClass.lpszMenuName = 0;											// window menu name (0 = default menu?) 
    myWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;				// redraw if the window is resized horizontally or vertically, allow different context for each window instance

    // Register that class with the Windows OS..
	RegisterClass(&myWindowClass);
    
   
	// create a rect structure to hold the dimensions of our window
    RECT rect;
    SetRect( &rect, TOPLEFTX,				// the top-left corner x-coordinate
                    TOPLEFTY,				// the top-left corner y-coordinate
                    TOPLEFTX + WIDTH,		// far right
                    TOPLEFTY + HEIGHT );	// far left
    

    
    // adjust the window, no idea why.
    AdjustWindowRect( &rect, WS_OVERLAPPEDWINDOW, false );

  
    
    // call CreateWindow to create the window
   HWND myWindow = CreateWindow(TEXT("GameScreen"),		// window class to use - in this case the one we created a minute ago
                          TEXT("Tank Game (OpenGL)"),				// window title
                          WS_OVERLAPPEDWINDOW,						// ??
                          TOPLEFTX, TOPLEFTY,						// x, y
                          TOPLEFTX + WIDTH, TOPLEFTY + HEIGHT,		// width and height
                          NULL, NULL,								// ??
                          hInstance, NULL);							// ??

    
   // check to see that the window created okay
    if( myWindow == NULL )
    {
        FatalAppExit( NULL, TEXT("CreateWindow() failed!") );
    }

    // if so, show it
    ShowWindow( myWindow, iCmdShow );
   

    // get a device context from the window
    HDC myDeviceContext = GetDC( myWindow );


	// we create a pixel format descriptor, to describe our desired pixel format. 
	// we set all of the fields to 0 before we do anything else
	// this is because PIXELFORMATDESCRIPTOR has loads of fields that we won't use
    PIXELFORMATDESCRIPTOR myPfd = { 0 };
 

    // now set only the fields of the pfd we care about:
    myPfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );		// size of the pfd in memory
    myPfd.nVersion = 1;									// always 1

    myPfd.dwFlags = PFD_SUPPORT_OPENGL |				// OpenGL support - not DirectDraw
					PFD_DOUBLEBUFFER   |				// double buffering support
					PFD_DRAW_TO_WINDOW;					// draw to the app window, not to a bitmap image

    myPfd.iPixelType = PFD_TYPE_RGBA ;					// red, green, blue, alpha for each pixel
    myPfd.cColorBits = 24;								// 24 bit == 8 bits for red, 8 for green, 8 for blue.
														// This count of color bits EXCLUDES alpha.

    myPfd.cDepthBits = 32;								// 32 bits to measure pixel depth.



	// now we need to choose the closest pixel format to the one we wanted...	
    int chosenPixelFormat = ChoosePixelFormat( myDeviceContext, &myPfd );
 
	// if windows didnt have a suitable format, 0 would have been returned...
    if( chosenPixelFormat == 0 )
    {
        FatalAppExit( NULL, TEXT("ChoosePixelFormat() failed!") );
    }

	// if we get this far it means we've got a valid pixel format
	// so now we need to set the device context up with that format...
    int result = SetPixelFormat( myDeviceContext, chosenPixelFormat, &myPfd );

	// if it failed...
    if (result == NULL)
    {
        FatalAppExit( NULL, TEXT("SetPixelFormat() failed!") );
    }

  
    // we now need to set up a render context (for opengl) that is compatible with the device context (from windows)...
    HGLRC myRenderContext = wglCreateContext( myDeviceContext );

	// then we connect the two together
    wglMakeCurrent( myDeviceContext, myRenderContext );


    // opengl display setup
	glMatrixMode(GL_PROJECTION);	// select the projection matrix, i.e. the one that controls the "camera"
    glLoadIdentity();				// reset it
    gluPerspective(45.0,(float)WIDTH/(float)HEIGHT, 1, 1000);	// set up fov, and near / far clipping planes
	glViewport(0, 0, WIDTH, HEIGHT);							// make the viewport cover the whole window
	glClearColor( 0.05f, 0.05f, 0.05f, 1.0f );						// set the colour used for clearing the screen




	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// main game loop starts here!
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// as long as the player hasnt exited the app: 
    while( !needToQuit )
    {
		// this is the start of the main game loop, so we need to record the time...
		currentGame->frameStartTime = GetTickCount();


		// we need to listen out for OS messages.
		// if there is a windows message to process...
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
			
			// if the message was a "quit" message...
			if( msg.message == WM_QUIT )
			{
				needToQuit = true;	// we want to quit asap
			}

			// otherwise, if it was a "key pressed" message...
			else if (msg.message == WM_KEYDOWN)
			{
				WPARAM _param = msg.wParam;
				char _KeyBox = MapVirtualKey(_param,MAPVK_VK_TO_CHAR);

				for (int n = 0; n < TOTAL_PLAYERS; n++)
				{
					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._UP_ID)
					{
						currentGame->_lowOrbit[n]->_Key_Up = true;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._DOWN_ID)
					{
						currentGame->_lowOrbit[n]->_Key_Down = true;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._LEFT_ID)
					{
						currentGame->_lowOrbit[n]->_Key_Left = true;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._RIGHT_ID)
					{
						currentGame->_lowOrbit[n]->_Key_Right = true;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._TRot_Right_ID)
					{
						currentGame->_lowOrbit[n]->_Key_TRot_Right = true;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._TRot_Left_ID)
					{
						currentGame->_lowOrbit[n]->_Key_TRot_Left = true;
					}
				}
			}
			// otherwise, if they let go of a key (you get the idea)
			else if (msg.message == WM_KEYUP)
			{
				WPARAM _param = msg.wParam;
				char _KeyBox = MapVirtualKey(_param, MAPVK_VK_TO_CHAR);

				for (int n = 0; n < TOTAL_PLAYERS; n++)
				{
					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._UP_ID)
					{
						currentGame->_lowOrbit[n]->_Key_Up = false;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._DOWN_ID)
					{
						currentGame->_lowOrbit[n]->_Key_Down = false;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._LEFT_ID)
					{
						currentGame->_lowOrbit[n]->_Key_Left = false;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._RIGHT_ID)
					{
						currentGame->_lowOrbit[n]->_Key_Right = false;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._TRot_Right_ID)
					{
						currentGame->_lowOrbit[n]->_Key_TRot_Right = false;
					}

					if (_KeyBox == currentGame->_lowOrbit[n]->_Tank_Key_ID._TRot_Left_ID)
					{
						currentGame->_lowOrbit[n]->_Key_TRot_Left = false;
					}
				}
			}
            else 
            {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
            }
        }
		
		// now update the gameplay...
		updateGame(myDeviceContext, currentGame);

		// now draw the scene...
		draw(myDeviceContext, currentGame);
		
		// this is the end of the main game loop, so we need to record the time again and then work out how long this frame took...
		currentGame->frameEndTime = GetTickCount();
		currentGame->frameDelta = (currentGame->frameEndTime - currentGame->frameStartTime);

    }
	// end of "while" loop


	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


    // the next bit will therefore happen when the player quits the app,
	// because they are trapped in the game loop as long as (needToQuit == false).


    // free up the memory used by the game state and set its pointer to NULL
    delete currentGame;
	currentGame = NULL;

    // UNmake our rendering context (make it 'uncurrent')
    wglMakeCurrent( NULL, NULL );

    // delete the rendering context, we no longer need it.
    wglDeleteContext( myRenderContext );

    // release our window's DC from the window
    ReleaseDC( myWindow, myDeviceContext );
  
	// end the program
    return msg.wParam;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



// this is the main function in terms of gameplay
// we pass in a pointer to the current game instance,
// so we can read and write information about the players and the world.
void updateGame(HDC deviceContext, gameInfo* currentGame)
{
	// do game logic here

	// work out how close to the ideal frame delta the last frame delta was...
	float deltaAdjustment = (float)currentGame->frameDelta / IDEAL_DELTA;
	// a result higher than 1 suggests the PC couldn't meet the ideal FPS, so the animation will be scaled up to compensate, and therefore could appear jerky.
	// a result of 1 or less means the PC is comfortable at the frame rate and the animation will be scaled down so it doesn't run too fast.


	// this bit updates the rotation of each player if they are holding their key down...
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		if (currentGame->playerArray[i]->rightPressed) 
		{
			// note that the rotation amount is something that depends on frame rate, so we multiply the ideal ROTATION_SPEED by the delta adjustment.
			currentGame->playerArray[i]->rotation -= (ROTATION_SPEED * deltaAdjustment);
		}  

		if (currentGame->playerArray[i]->leftPressed) 
		{
			currentGame->playerArray[i]->rotation += (ROTATION_SPEED * deltaAdjustment);
		}  
	}
	
	for (int n = 0; n < TOTAL_PLAYERS; n++)
	{
		currentGame->_lowOrbit[n]->_Speed += (TANK_SPEED * deltaAdjustment) * (currentGame->_lowOrbit[n]->_Key_Up - currentGame->_lowOrbit[n]->_Key_Down);
		currentGame->_lowOrbit[n]->_Direction += (TANK_ROT_SPEED * deltaAdjustment) * (currentGame->_lowOrbit[n]->_Key_Right - currentGame->_lowOrbit[n]->_Key_Left);

		currentGame->_lowOrbit[n]->_pos._y += (cos(currentGame->_lowOrbit[n]->_Direction) * ((TANK_ROT_SPEED * deltaAdjustment) * currentGame->_lowOrbit[n]->_Speed));
		currentGame->_lowOrbit[n]->_pos._x += (sin(currentGame->_lowOrbit[n]->_Direction) * ((TANK_ROT_SPEED * deltaAdjustment) * currentGame->_lowOrbit[n]->_Speed));

		currentGame->_lowOrbit[n]->_Turret_Direction += (currentGame->_lowOrbit[n]->_Key_TRot_Right - currentGame->_lowOrbit[n]->_Key_TRot_Left) * 0.1f;

		if (currentGame->_lowOrbit[n]->_Speed > TANK_MAX_SPEED)
		{
			currentGame->_lowOrbit[n]->_Speed = TANK_MAX_SPEED;
		}
		else if (currentGame->_lowOrbit[n]->_Speed < -TANK_MAX_SPEED)
		{
			currentGame->_lowOrbit[n]->_Speed = -TANK_MAX_SPEED;
		}

		if (!currentGame->_lowOrbit[n]->_Key_Up && !currentGame->_lowOrbit[n]->_Key_Down)
		{
			currentGame->_lowOrbit[n]->_Speed -= currentGame->_lowOrbit[n]->_Speed / 500;
		}
	}
}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


// this does all the drawing
// again, we pass in a pointer to the current game instance 
// so we know where things are supposed to be drawn
void draw(HDC deviceContext, gameInfo* currentGame)   
{
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );		// clear the screen and the depth buffer

	glEnableClientState(GL_VERTEX_ARRAY);						// we need enable vertices to draw polygons (obviously)
	glEnableClientState(GL_COLOR_ARRAY);						// we want to use colors too
	
	glMatrixMode(GL_MODELVIEW);									// we are going to affect how models are drawn
    
	
	// so, for each player...
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		
		// in this demo we always use the same piece of geometry (i.e. the triangle)...
		glVertexPointer(3, GL_FLOAT, 0, myVertexArray);					// choose the array of vertex positions to use, 3 dimensional (XYZ), floats, 0 gap between elements
		glColorPointer(4, GL_FLOAT, 0, myColorArray);					// choose the array of vertex colors to use, 4 dimensional (RGBA), floats, 0 gap between elements
		
		glPushMatrix();													// save the current matrix state, in case anything important was done before we got here
		
			glLoadIdentity();											// start with the identity matrix (i.e. no transformation)
			glTranslatef(currentGame->playerArray[i]->xPos,0.0,-5.0);	// then move across and back

			// then rotate to the current player's rotation around z axis
			glRotatef(currentGame->playerArray[i]->rotation, 0.0, 0.0, 1.0);

			// then draw the 3 vertices from the array to make the generic triangle.
			// because we are using matrices the same triangle can be rotated differently for each player!
			glDrawArrays(GL_TRIANGLES, 0, 3);							

		glPopMatrix();													// restore the old matrix state, leaving things as we found them!
	}

	
	for (int n = 0; n < TOTAL_PLAYERS; n++)
	{
		// in this demo we always use the same piece of geometry (i.e. the triangle)...
		glVertexPointer(3, GL_FLOAT, 0, CubeVertexArray);						// choose the array of vertex positions to use, 3 dimensional (XYZ), floats, 0 gap between elements
		
		switch (n)
		{
		case 0:
			glColorPointer(4, GL_FLOAT, 0, cubeColorArrayRed);
			break;

		case 1:
			glColorPointer(4, GL_FLOAT, 0, cubeColorArrayBlue);
			break;

		default:

			break;
		}


		
		
		glPushMatrix();														// save the current matrix state, in case anything important was done before we got here

			glLoadIdentity();												// start with the identity matrix (i.e. no transformation)
			glTranslatef(currentGame->_lowOrbit[n]->_pos._x, currentGame->_lowOrbit[n]->_pos._y, -10.0);	// then move across and back

			// then rotate to the current player's rotation around z axis
			glRotatef(-currentGame->_lowOrbit[n]->_Direction * (180 / PI), 0.0f, 0.0f, 1.0f);

			// then draw the 3 vertices from the array to make the generic triangle.
			// because we are using matrices the same triangle can be rotated differently for each player!
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glPopMatrix();


		glVertexPointer(3, GL_FLOAT, 0, turretVertexArray);
		glColorPointer(4, GL_FLOAT, 0, turretColorArray);

		glPushMatrix();
			
		glTranslatef(currentGame->_lowOrbit[n]->_pos._x, currentGame->_lowOrbit[n]->_pos._y, -10.0);
		glRotatef(-currentGame->_lowOrbit[n]->_Turret_Direction, 0.0, 0.0, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glPopMatrix();
	}
	
	glPushMatrix();

	float debugLineVertexArray[] = {
		currentGame->_lowOrbit[0]->_pos._x, currentGame->_lowOrbit[0]->_pos._y, -10,
		currentGame->_lowOrbit[1]->_pos._x, currentGame->_lowOrbit[1]->_pos._y, -10
	};

	glVertexPointer(3, GL_FLOAT, 0, debugLineVertexArray);
	glColorPointer(4, GL_FLOAT, 0, debugLineColorArray);

	glDrawArrays(GL_LINES, 0, 2);

	glPopMatrix();

	glPushMatrix();

	float debugLineVertexArray2[] = {
		(currentGame->_lowOrbit[0]->_pos._x - currentGame->_lowOrbit[1]->_pos._x) / 2, (currentGame->_lowOrbit[0]->_pos._y - currentGame->_lowOrbit[1]->_pos._y) / 2, -10,
		0.0f, 0.0f, -10
	};

	glVertexPointer(3, GL_FLOAT, 0, debugLineVertexArray2);
	glColorPointer(4, GL_FLOAT, 0, debugLineColorArray);

	glDrawArrays(GL_LINES, 0, 2);

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);						// turn off vertex rendering again, just in case
	glDisableClientState(GL_COLOR_ARRAY);						// turn off vertex colors again, just in case
    
	SwapBuffers(deviceContext);									// put our triangles on the screen!

}


// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// this function is called when any events happen to our window
LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam ) 
{

    switch( message )
    {
		// if they exited the window...	
		case WM_DESTROY:
			// post a message "quit" message to the main windows loop
			PostQuitMessage( 0 ) ;
			return 0;
			break;
    }
 
	// must do this as a default case (i.e. if no other event was handled)...
    return DefWindowProc( hwnd, message, wparam, lparam );

}



// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@




// this sets up the default values that are common to all game instances
gameInfo::gameInfo()
{
	// create an array of pointers to players...
	playerArray = new playerInfo* [NUM_PLAYERS];

	// for each player in the array, allocate memory for that player
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		playerArray[i] = new playerInfo;
	}


	_lowOrbit = new _Tank*[TOTAL_PLAYERS];

	for (int n = 0; n < TOTAL_PLAYERS; n++)
	{
		_lowOrbit[n] = new _Tank;

		switch (n)
		{
		case 0:
			_lowOrbit[0]->_pos._x = -1.3f;
			_lowOrbit[0]->_Tank_Key_ID._UP_ID = 'W';
			_lowOrbit[0]->_Tank_Key_ID._DOWN_ID = 'S';
			_lowOrbit[0]->_Tank_Key_ID._LEFT_ID = 'A';
			_lowOrbit[0]->_Tank_Key_ID._RIGHT_ID = 'D';
			_lowOrbit[0]->_Tank_Key_ID._TRot_Right_ID = 'E';
			_lowOrbit[0]->_Tank_Key_ID._TRot_Left_ID = 'Q';
			break;

		case 1:
			_lowOrbit[1]->_pos._x = 1.3f;
			_lowOrbit[1]->_Tank_Key_ID._UP_ID = 'I';
			_lowOrbit[1]->_Tank_Key_ID._DOWN_ID = 'K';
			_lowOrbit[1]->_Tank_Key_ID._LEFT_ID = 'J';
			_lowOrbit[1]->_Tank_Key_ID._RIGHT_ID = 'L';
			_lowOrbit[1]->_Tank_Key_ID._TRot_Right_ID = 'O';
			_lowOrbit[1]->_Tank_Key_ID._TRot_Left_ID = 'U';
			break;

		default:

			break;
		}
	}



	// adjust the xpos of each triangle so they dont overlap...
	//playerArray[0]->xPos = -1.0f;
	//playerArray[1]->xPos = 1.0f;


	// since the updateGame function requires a delta time from the previous frame for its calculations, 
	// this will not work on the first frame of the game as there will be no delta recorded for it to work with.
	// so we will pretend a value was obtained, and that it was "ideal"...
	frameDelta = IDEAL_DELTA;

	// initialise the other frame delta variables...
	frameStartTime = 0;
	frameEndTime = 0;

}


gameInfo::~gameInfo()
{
	// this is called when the game state is deleted (on quitting)
	
	// for each player in our player array, free up its memory and set its pointer to NULL
	for (int i = 0; i < NUM_PLAYERS; i++)
	{
		delete playerArray[i];
		playerArray[i] = NULL;
	}
	
	// then delete the array itself and set its pointer to NULL
	delete[] playerArray;
	playerArray = NULL;


	for (int n = 0; n < TOTAL_PLAYERS; n++)
	{
		delete _lowOrbit[n];
		_lowOrbit[n] = NULL;
	}
	
	delete[] _lowOrbit;
	_lowOrbit = NULL;
}

// this sets up the default values that are common to all players
playerInfo::playerInfo()
{
	upPressed = false;
	downPressed = false;
	leftPressed = false;
	rightPressed = false;
	rotation = 0.0;
	xPos = 0.0;
	yPos = 0.0;
}


