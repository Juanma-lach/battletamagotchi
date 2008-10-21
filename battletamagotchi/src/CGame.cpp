#include "CGame.h"
#include <e32base.h>
#include <gles/gl.h> // for opengl es functions.
#include <unistd.h>  // for usleep.

CGameSprite* c;

#define FRUSTUM_LEFT   -1.f     //left vertical clipping plane
#define FRUSTUM_RIGHT   1.f     //right vertical clipping plane
#define FRUSTUM_BOTTOM -1.f     //bottom horizontal clipping plane
#define FRUSTUM_TOP     1.f     //top horizontal clipping plane
#define FRUSTUM_NEAR    3.f     //near depth clipping plane
#define FRUSTUM_FAR  1000.f     //far depth clipping plane

// Vertice coordinates for the cube. 
static const GLbyte vertices[4 * 3] =
     {
     -1,  1,  1,
      1,  1,  1,
      1, -1,  1,
     -1, -1,  1,

     /*-1,  1, -1,
      1,  1, -1,
      1, -1, -1,
     -1, -1, -1*/
     };

static const GLfloat tex_coord[4 * 2] =
     {
      0,  1,
      1,  1,
      1,  0,
      0,  0,

     /*-1,  1, -1,
      1,  1, -1,
      1, -1, -1,
     -1, -1, -1*/
     };

// Colors for vertices (Red, Green, Blue, Alpha). 
static const GLubyte colors[4 * 4] =
    {
    0  ,255,  0,255,
    0  ,  0,255,255,
    0  ,255,  0,255,
    255,  0,  0,255,

    /*0  ,  0,255,255,
    255,  0,  0,255,
    0  ,  0,255,255,
    0  ,255,  0,255*/
    };

// Indices for drawing the triangles.
static const GLubyte triangles[2 * 3] =
    {
    // front 
    1,0,3,
    1,3,2,

    // right 
    /*2,6,5,
    2,5,1,
    
    // back 
    7,4,5,
    7,5,6,
    
    // left 
    0,4,7,
    0,7,3,
    
    // top 
    5,4,0,
    5,0,1,
    
    // bottom 
    3,7,6,
    3,6,2*/
    };

CGame* CGame::get_new()
{
    CGame* self = new (ELeave) CGame;
    
    stack_push(self);    
    self->init();

    return self;    
}

CGame::CGame()
      :iScreenWidth( 0 ),
       iScreenHeight( 0 ),
       iGameOver( EFalse ),
       iZPos( -100 ),
       iIsUpPressed( EFalse ),
       iIsDownPressed( EFalse ),
       iFrame( 0 )
{

}

CGame::~CGame()
{
    deinit();
}

void CGame::deinit()
{
	delete c;
}

void CGame::init()
{

	TUint8 key[] = {0,0,0};
	c = new CGameSprite(_L("lena.mbm"), &key[0], NULL, NULL, 0);
}

void CGame::init_screen( TUint aScreenWidth, TUint aScreenHeight )
{
iScreenWidth = aScreenWidth;
    iScreenHeight = aScreenHeight;

    // Set the screen background color. 
    glClearColor( 0.f, 0.f, 0.f, 1.f );

    // Enable back face culling. 
    glEnable( GL_CULL_FACE  );

    // Initialize viewport and projection. 
    glViewport( 0, 0, iScreenWidth, iScreenHeight );
    glMatrixMode( GL_PROJECTION );

    // Calculate the view frustrum
    GLfloat aspectRatio = (GLfloat)(iScreenWidth) / (GLfloat)(iScreenHeight);
    glFrustumf( FRUSTUM_LEFT * aspectRatio, FRUSTUM_RIGHT * aspectRatio,
                FRUSTUM_BOTTOM, FRUSTUM_TOP,
                FRUSTUM_NEAR, FRUSTUM_FAR );

    glMatrixMode( GL_MODELVIEW );

    // Enable vertex arrays. 
    glEnableClientState( GL_VERTEX_ARRAY );

    // Set array pointers. 
    glVertexPointer( 3, GL_BYTE, 0, vertices );

    // Enable color arrays. 
    //glEnableClientState( GL_COLOR_ARRAY );    

    // Set color pointers. 
    glColorPointer( 4, GL_UNSIGNED_BYTE, 0, colors );
    
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
    glTexCoordPointer(2,GL_FLOAT,0,tex_coord);

    // Set the initial shading mode 
    glShadeModel( GL_FLAT ); 

    // Do not use perspective correction 
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );

unsigned int id = 0;
glGenTextures(1, &id); 

//Dizemos ao OpenGL que iremos trabalhar com o objeto.
glBindTexture(GL_TEXTURE_2D, id);

//Filtro. Veremos isso nos próximos artigos.
glTexParameteri(GL_TEXTURE_2D,
GL_TEXTURE_MIN_FILTER, GL_NEAREST);

//Descobrimos o formato a partir da imagem
GLint format = GL_RGBA;

//Carregamos a imagem do disco
glTexImage2D(GL_TEXTURE_2D, 0, format, SIZE, SIZE, 0,
format, GL_UNSIGNED_BYTE, c->getData());
	
glEnable(GL_TEXTURE_2D);

}

TBool CGame::render( TInt64 aMicrosSinceLastVisit )
{      
if( iGameOver ) // If the game has ended, return EFalse to exit.
        return EFalse;
    
    // Ensure that the desired fps is maintained by sleeping 
    // for the rest of the time.   
    if( aMicrosSinceLastVisit < timeForOneFrame )
    {
        usleep( timeForOneFrame - aMicrosSinceLastVisit );    
    }
    
    // Clear the buffer.
    glClear( GL_COLOR_BUFFER_BIT );
        
    glLoadIdentity();
    
    // Move the Cube.
    glTranslatex( 0 , 0 , iZPos << 16 );
    
    // Rotate the cube.
    /*glRotatex( iFrame << 16, 1 << 16,    0   ,    0    );
    glRotatex( iFrame << 15,    0   , 1 << 16,    0    );
    glRotatex( iFrame << 14,    0   ,    0   , 1 << 16 );*/

    glScalef( 15.0f, 15.0f, 15.0f );
    
    glDrawElements( GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE, triangles );
    
    ++iFrame;
    
    // Move the cube if the up/down keys are pressed, but not released.
    // If the up/down keys are not released, then continuously move
    // cube.
    if( iIsUpPressed )
        --iZPos;
    
    if( iIsDownPressed )
        ++iZPos;
    
    return ETrue;
}

void CGame::handle_key( TUint32 aKey, TBool aIsKeyUp )
{
   // Handle key events
   if( EFalse == aIsKeyUp )
   {
        switch( aKey )
        {
            case( EStdKeyUpArrow ):
            {
                // Move the cube farther from the screen.
                --iZPos;
                iIsUpPressed = ETrue;
                break;
            }
            case( EStdKeyDownArrow ): 
            {
                // Move the cube closer.
                ++iZPos;
                iIsDownPressed = ETrue;
                break;
            }
            case( EStdKeyDevice1 ): 
            {
                // Exit on Right Soft Key press.
                iGameOver = ETrue;
                break;
            }
            default:                
                break;
        }
   }
   else
   {
        // On a key release, reset the flags.
        switch( aKey )
        {
            case( EStdKeyUpArrow ):
            {                
                iIsUpPressed = EFalse;
                break;
            }
            case( EStdKeyDownArrow ):
            {
                iIsDownPressed = EFalse;
                break;
            }
            default:
                break;
        }       
   }
}

void CGame::set_screen_size( TUint aScreenWidth, TUint aScreenHeight )
{    
    iScreenWidth = aScreenWidth;
    iScreenHeight = aScreenHeight;
    
    // Initialize the viewport and projection. 
    glViewport( 0, 0, iScreenWidth, iScreenHeight );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
    // Calculate the view frustrum
    GLfloat aspectRatio = (GLfloat)(iScreenWidth) / (GLfloat)(iScreenHeight);
    /*glFrustumf( FRUSTUM_LEFT * aspectRatio, FRUSTUM_RIGHT * aspectRatio,
                FRUSTUM_BOTTOM, FRUSTUM_TOP,
                FRUSTUM_NEAR, FRUSTUM_FAR );*/

    glMatrixMode( GL_MODELVIEW );    
}


