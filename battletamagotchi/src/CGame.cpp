#include "CGame.h"
#include <e32base.h>
#include <unistd.h>  // for usleep.

CGameSprite* c;
CGameSprite* d;

/*
#define FRUSTUM_LEFT   -1.f     //left vertical clipping plane
#define FRUSTUM_RIGHT   1.f     //right vertical clipping plane
#define FRUSTUM_BOTTOM -1.f     //bottom horizontal clipping plane
#define FRUSTUM_TOP     1.f     //top horizontal clipping plane
#define FRUSTUM_NEAR    3.f     //near depth clipping plane
#define FRUSTUM_FAR  1000.f     //far depth clipping plane
*/

// Vertice coordinates for the cube. 

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
	delete d;
}

void CGame::init()
{
		
}

void CGame::init_screen( TUint aScreenWidth, TUint aScreenHeight )
{
	iScreenWidth = aScreenWidth;
    iScreenHeight = aScreenHeight;
    
    GLfloat ratio = iScreenWidth/((float)iScreenHeight);

    glClearColor( 0.f, 0.f, 0.f, 1.f );
 
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
 
    glViewport( 0, 0, iScreenWidth, iScreenHeight );
    glMatrixMode( GL_PROJECTION );
    
    glLoadIdentity();
        
    glOrthof(0, ratio, 0, 1, 1, -1);
  
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    
    glScalef(1.0f/iScreenHeight, 1.0f/iScreenHeight, 1);
     
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 
    glShadeModel( GL_FLAT ); 
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST );	
    glEnable(GL_TEXTURE_2D);
    
    CGameRect size(50,50, 256/*this->iScreenWidth*/, 128/*this->iScreenHeight*/);
    c = new CGameSprite(this, _L("lena.mbm"), size, 1);
    //c->setSize(size);
    	
    size = CGameRect(0,0,128,256);
    d = new CGameSprite(this, _L("lena.mbm"), size, 1);
    size = CGameRect(100,100,100,100);
    d->setSize(size);
    d->setLayer(1);
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
 
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
 
    c->draw();
    d->draw();
    
    ++iFrame;
     
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


