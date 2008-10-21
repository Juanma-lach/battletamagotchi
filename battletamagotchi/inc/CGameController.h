#ifndef __CLASS_GAME_CONTROLLER_H_
#define __CLASS_GAME_CONTROLLER_H_

#include <gles/egl.h>
#include <e32base.h>
#include <w32std.h>

#include "CGame.h"
class RWindow;
class CGameEventReceiver;

class CGameController : public CBase {
	
		CGameController(EGLDisplay aEGLDisplay);		
		void init(void);

	public:

		~CGameController();
		static CGameController* get_new(EGLDisplay aEGLDisplay);
		void start_game(CGame& aGame);
		void HandleWsEvent( const TWsEvent& aEvent );
		
	private:		

		void ProcessBackgroundTasks(TBool aIsBlocking);
		void CreateNativeWindowL();
		
	    /** Handle to the EGL Display */
	    EGLDisplay iEglDisplay;
	 
	    /** Handle to the EGL Surface */
	    EGLSurface iEglSurface;
	    
	    /** Handle to the EGL Context */
	    EGLContext iEglContext;
	    
	    /** Handle to Symbian's Window */
	    RWindow* iWindow;

	    /** The Active Object that waits for Windows Server Events */    
	    CGameEventReceiver* iWsEventReceiver;
	    
	    /** The Game */
	    CGame* iGame;
	    
	    /** Handle to the Windows Server session */
	    RWsSession iWsSession;

	    /** Handle to the Window group */
	    RWindowGroup iWindowGroup;
	    
	    /** The Screen device */
	    CWsScreenDevice* iWsScreenDevice;
	    
	    /** Flag for the applications focus state */
	    TBool iIsAppInFocus;
	    
	    /** Flag to check if the Window is visible */
	    TBool iIsVisible;
};

#endif
