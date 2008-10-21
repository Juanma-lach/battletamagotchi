#include "CGameEventReceiver.h"
#include <coemain.h>

CGameEventReceiver::CGameEventReceiver()
                 :CActive( CActive::EPriorityStandard ),
                  iParent( NULL )
{                      
}

CGameEventReceiver::~CGameEventReceiver()
{
    Cancel();
}

CGameEventReceiver* CGameEventReceiver::get_new( CGameController& aParent )
{
	CGameEventReceiver* self = new (ELeave) CGameEventReceiver;
    
    stack_push(self);
    
    self->init(aParent);
    
    stack_pop(self);
    
    return self;    
}

void CGameEventReceiver::init( CGameController& aParent )
{
    if( !CCoeEnv::Static() )
        User::Leave( KErrCoeEnvNotCreated );
    
    iParent = &aParent;
     
    iWsSession = CCoeEnv::Static()->WsSession();
    
    iWsSession.EventReady(&iStatus);
    
    CActiveScheduler::Add(this);
    
    SetActive();
}

void CGameEventReceiver::RunL()
{
    TWsEvent wsEvent;
    iWsSession.GetEvent( wsEvent );
    
    iParent->HandleWsEvent( wsEvent );
    
    iWsSession.EventReady( &iStatus );
    
    SetActive();
}

void CGameEventReceiver::DoCancel()
{
    iWsSession.EventReadyCancel();
}
