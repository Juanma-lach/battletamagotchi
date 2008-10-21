#include "CGameController.h"
#include <coemain.h>

#include "CGameEventReceiver.h"

CGameController::CGameController(EGLDisplay aEGLDisplay)
				:iEglDisplay( aEGLDisplay ),
				 iEglSurface( NULL ),
				 iEglContext( NULL ),
				 iWindow( NULL ),
				 //iWsEventReceiver( NULL ),
				 iGame( NULL ),
				 iWsScreenDevice( NULL ),
				 iIsAppInFocus( EFalse ),
				 iIsVisible( ETrue )
{
}

void CGameController::init(void){

	CCoeEnv* env = CCoeEnv::Static();
	if( !env )
	{
	    User::Leave( KErrCoeEnvNotInitialized );
	}
	
	// Get the Windows Server Session, screen device and window group.
	iWsSession = env->WsSession();
	iWsScreenDevice = env->ScreenDevice();
	iWindowGroup = env->RootWin();
	if( !iWsScreenDevice )
	{
	    User::Leave( KErrCoeEnvNotInitialized );
	}
	
	// Intializes iWindow to a full screen window.
	CreateNativeWindowL();
	
	// Choose the buffer size based on the Window's display mode.
	TDisplayMode displayMode = iWindow->DisplayMode();
	TInt bufferSize = 0;
	
	switch( displayMode )
	{
	    case(EColor4K):
	        bufferSize = 12;
	        break;
	    case(EColor64K):
	        bufferSize = 16;
	        break;
	    case(EColor16M):
	        bufferSize = 24;
	        break;
	    case(EColor16MU):
	    case(EColor16MA):
	        bufferSize = 32;
	        break;
	    default:
	        break;
	}
	         
	// Set the desired properties for the EGLSurface
	const EGLint attributeList[] = { 
	                                 EGL_SURFACE_TYPE, EGL_WINDOW_BIT, 
	                                 EGL_BUFFER_SIZE, bufferSize, 
	                                 EGL_NONE 
	                               };
	EGLConfig bestConfig;
	EGLint numConfigs = 0;
	
	// Choose the best EGLConfig that matches the desired properties. 
	if( eglChooseConfig( iEglDisplay, attributeList, &bestConfig,
	                      1, &numConfigs
	                    ) == EGL_FALSE
	  )
	{
	    User::Leave( KErrConfigFailed );
	}
	
	if( numConfigs == 0 )
	{
		User::Leave( KErrConfigFailed );
	}
	    
	// Create a window surface where the graphics are blitted.
	// Pass the Native Window handle to egl.
	iEglSurface = eglCreateWindowSurface( iEglDisplay, bestConfig, 
	                                      (void*)iWindow, NULL 
	                                    );
	if( iEglSurface == NULL )
	{
	    User::Leave( KErrCreateWindowSurfaceFailed );
	}
	    
	// Create a rendering context
	iEglContext = eglCreateContext( iEglDisplay, bestConfig,
	                                EGL_NO_CONTEXT, NULL 
	                              );
	if( iEglContext == NULL )
	{
	    User::Leave( KErrCreateContextFailed );    
	}
	
	// Make it the current context.
	if( eglMakeCurrent( iEglDisplay, iEglSurface, 
	                     iEglSurface, iEglContext 
	                  ) == EGL_FALSE
	  )
	{
	    User::Leave( KErrSetContextFailed );        
	}
	
	// Creates the Active Object that waits for Windows Server events.
	iWsEventReceiver = CGameEventReceiver::get_new(*this);
}

void CGameController::start_game(CGame& aGame)
{    
    iGame = &aGame;   
    
    aGame.init_screen( iWindow->Size().iWidth, iWindow->Size().iHeight );
    
    TTime currentTime;
    TTime lastTimeVisited;
    lastTimeVisited.HomeTime();    
        
    while( 1 )
    {       
        // Process any pending tasks.
        // This runs any Active objects that are waiting for 
        // some processing. 
        // The CWsEventReceiver Active Object gets a chance to
        // run here (on a key event for example).
        ProcessBackgroundTasks( EFalse );
                
        // If the application is not in focus or is not visible.
        // Block until it regains focus.
        while( EFalse == iIsAppInFocus || EFalse == iIsVisible )
            ProcessBackgroundTasks( ETrue );
                
        // Get the current time.
        currentTime.HomeTime();       
        TTimeIntervalMicroSeconds dur 
                    = currentTime.MicroSecondsFrom( lastTimeVisited );                        
                
        // The game renders itself using opengl es apis.
        // A return value of EFalse signifies an exit from the game.
        // Pass in the time (in micro secs) elapsed since last call. 
        if( EFalse == aGame.render( dur.Int64() ) )
        {
            break;
        }                                

        // Call eglSwapBuffers, which blits the graphics to the window.
        eglSwapBuffers( iEglDisplay, iEglSurface );    

        // To keep the background light on.
        if( !( ( iGame->get_frame() )%100 ) )
        {
            User::ResetInactivityTime();
        }
        
        // Store the last time the Game was rendered.
        lastTimeVisited = currentTime;
    }
    
    // Cleanup.
    aGame.deinit();    
}

CGameController* CGameController::get_new(EGLDisplay aEGLDisplay){
	
	CGameController* self = new CGameController(aEGLDisplay);
	stack_push(self);
	self->init();
	
	return self;
}

CGameController::~CGameController(void)
{
    eglMakeCurrent( iEglDisplay, EGL_NO_SURFACE, 
                    EGL_NO_SURFACE, EGL_NO_CONTEXT 
                  );
    eglDestroySurface( iEglDisplay, iEglSurface );
    eglDestroyContext( iEglDisplay, iEglContext );

    if( iWindow != NULL )
    {
        iWindow->SetOrdinalPosition( KOrdinalPositionSwitchToOwningWindow );
        iWindow->Close();
        delete iWindow;
        iWindow = NULL;
    }
    
    delete iWsEventReceiver;
}

// -----------------------------------------------------------------------------
// CGameController::ProcessBackgroundTasks
// Calls the RunL on all Active Objects which have 
// pending requests.
// If aIsBlocking is ETrue, this API will block till an event occurs.
// -----------------------------------------------------------------------------
//
void CGameController::ProcessBackgroundTasks( TBool aIsBlocking )
{
    RThread thread;    
    TInt error = KErrNone;
    
    if ( aIsBlocking && !thread.RequestCount() )
    {
        // Block for a event.
        User::WaitForAnyRequest();

        // Run the Active Object corresponding to the event
        CActiveScheduler::RunIfReady( error, CActive::EPriorityIdle );
    }
    
    // Processes all outstanding requests
    while( thread.RequestCount() ) 
    {
        // Processes an event.        
        CActiveScheduler::RunIfReady( error, CActive::EPriorityIdle );
        
        // Decreases thread.RequestCount
        User::WaitForAnyRequest();
    }
}

// -----------------------------------------------------------------------------
// CGameController::CreateNativeWindowL
// Create a full screen RWindow and enables screen size
// change events.
// -----------------------------------------------------------------------------
//
void CGameController::CreateNativeWindowL()
{      
    iWindow = new (ELeave) RWindow( iWsSession );
    
    // Construct the window.
    TInt err = iWindow->Construct( iWindowGroup,
                                   reinterpret_cast<TUint32>( this ) 
                                 );
    User::LeaveIfError( err );

    // Enable the EEventScreenDeviceChanged event.
    iWindowGroup.EnableScreenChangeEvents();                                      
        
    TPixelsTwipsAndRotation pixnrot; 
    iWsScreenDevice->GetScreenModeSizeAndRotation( 
                                iWsScreenDevice->CurrentScreenMode(),
                                pixnrot );
        
    // Set size of the window (cover the entire screen)
    iWindow->SetExtent( TPoint( 0, 0 ),
                        pixnrot.iPixelSize
                      );
                       
    iWindow->SetRequiredDisplayMode( iWsScreenDevice->DisplayMode() );
    
    // Activate window and bring it to the foreground
    iWindow->Activate();
    iWindow->SetVisible( ETrue );
    iWindow->SetNonFading( ETrue ); 
    iWindow->SetShadowDisabled( ETrue ); 
    iWindow->EnableRedrawStore( EFalse ); 
    iWindow->EnableVisibilityChangeEvents();
    iWindow->SetNonTransparent(); 
    iWindow->SetBackgroundColor(); 
    iWindow->SetOrdinalPosition( 0 );    
}

// -----------------------------------------------------------------------------
// CGameController::HandleWsEvent
// This function is called by the CWsEventReceiver 
// Active Object, whenever it receives a Windows Server
// event.
// -----------------------------------------------------------------------------
//
void CGameController::HandleWsEvent( const TWsEvent& aWsEvent )
{
    TInt eventType = aWsEvent.Type();
     
    // Handle Key and Screen Size change Events.
    switch( eventType )
    {
        case EEventKeyDown:
        {
            if( iGame )
                iGame->handle_key( aWsEvent.Key()->iScanCode, EFalse );
            
            break;            
        }
        case EEventKeyUp:
        {
            if( iGame )
                iGame->handle_key( aWsEvent.Key()->iScanCode, ETrue );
            
            break;            
        }        
        case( EEventScreenDeviceChanged ): // The screen size has changed.
        {
            TPixelsTwipsAndRotation pixnrot; 
            iWsScreenDevice->GetScreenModeSizeAndRotation( 
                                  iWsScreenDevice->CurrentScreenMode(),
                                  pixnrot 
                                                         );       
            if( pixnrot.iPixelSize != iWindow->Size() )
            {
                 
                // Update the window.
                iWindow->SetExtent( TPoint( 0, 0 ),
                                    pixnrot.iPixelSize
                                  );                
                                        
                // If a game is running, notify it about the change.
                if( iGame )
                {
                    iGame->set_screen_size( pixnrot.iPixelSize.iWidth,
                                          pixnrot.iPixelSize.iHeight 
                                        );                    
                    
                    // Call eglSwapBuffers after the window size has changed.
                    // This updates the window size used by egl. 
                    eglSwapBuffers( iEglDisplay, iEglSurface );                                        
                }                
                                
            }                    
            break;            
        }
        case EEventFocusLost: 
        {
            iIsAppInFocus = EFalse;
            break;
        }
        case EEventFocusGained:
        {
            iIsAppInFocus = ETrue;
            break;
        }
        case EEventWindowVisibilityChanged:
        {
            // Check if the event is for the iWindow
            if( aWsEvent.Handle() ==
                        reinterpret_cast<TUint32>( this )
                  
              )
            {
                if( aWsEvent.VisibilityChanged()->iFlags &
                    TWsVisibilityChangedEvent::ECanBeSeen 
                )                
                {
                      iIsVisible = ETrue;
                }
                else
                      iIsVisible = EFalse;
                
            }                            
            break;
        }
        case EEventNull:
        case EEventKey:        
        case EEventUser:                                
        case EEventWindowGroupListChanged:
        case EEventModifiersChanged:
        case EEventSwitchOn:
        case EEventPassword:
        case EEventWindowGroupsChanged:
        case EEventErrorMessage:
        case EEventPointer:
        case EEventPointerEnter:
        case EEventPointerExit:
        case EEventPointerBufferReady:
        case EEventDragDrop:
            break;
        default:
            break;
    }        
}

