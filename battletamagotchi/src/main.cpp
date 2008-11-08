#include <stdio.h>
#include <e32base.h>
#include <coemain.h>

#include "base/gambi.h"
#include "CGame.h"
#include "CGameController.h"

#ifdef __GCCE__
#include <staticlibinit_gcce.h>
#endif

CCoeEnv* environment;
EGLDisplay eglDisplay;

void StartGameL( EGLDisplay aEglDisplay );

int main(void){

	__UHEAP_MARK;
	
	environment = new (ELeave) CCoeEnv();
	TRAPD( err1, environment->ConstructL( ETrue, 0 ) );
	
	if( err1 != KErrNone )
	{
		printf( "Unable to create a CCoeEnv!\n" );
		getchar();
	            
		return(KErrCoeEnvNotInitialized);        
	}
	
	eglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	
	if( eglDisplay == NULL )
	{
			printf( "EGL Get Display failed!\n" );
			getchar();
		        
			return(KErrDisplayNotCreated);        
	}
	
	if( eglInitialize( eglDisplay, NULL, NULL ) == EGL_FALSE )
	{            
		printf( "EGL Initialize failed!\n" );
		getchar();
            
		return(KErrDisplayNotInitialized);
	}
	
	TRAPD( err2, StartGameL( eglDisplay ) );
        
	if( KErrNone != err2 )
	{
		printf( "StartGameL failed with result = %d!\n", err2 );
		getchar();
	}

	eglTerminate( eglDisplay );
	
	printf("Hello World!\n");
	getchar();
	
	fclose( stdout );
	fclose( stdin );
	
	CCoeEnv::Static()->DestroyEnvironment();
	delete CCoeEnv::Static();
	
	stack_clean();
	
	__UHEAP_MARKEND;
	
	return(0);	
}

void StartGameL( EGLDisplay aEglDisplay )
{   
    delete CActiveScheduler::Current();            
    CActiveScheduler* actScheduler = new (ELeave) CActiveScheduler();    
    CActiveScheduler::Install( actScheduler );
    stack_push(actScheduler);

    CGameController* gameController = CGameController::get_new(aEglDisplay);
    CGame* game = CGame::get_new();

    gameController->start_game( *game );
}

