#include "event/CSymbianEventReceiver.h"
#include <coemain.h>

CSymbianEventReceiver::CSymbianEventReceiver()
                 :CActive( CActive::EPriorityStandard ),
                  iParent( NULL )
{                      
}

CSymbianEventReceiver::~CSymbianEventReceiver()
{
    Cancel();
}

CSymbianEventReceiver* CSymbianEventReceiver::get_new( CGameController& aParent )
{
	CSymbianEventReceiver* self = new (ELeave) CSymbianEventReceiver;
    
    stack_push(self);
    
    self->init(aParent);
    
    stack_pop(self);
    
    return self;    
}

void CSymbianEventReceiver::init( CGameController& aParent )
{
    if( !CCoeEnv::Static() )
        User::Leave( KErrCoeEnvNotCreated );
    
    iParent = &aParent;
     
    iWsSession = CCoeEnv::Static()->WsSession();
    
    iWsSession.EventReady(&iStatus);
    
    CActiveScheduler::Add(this);
    
    SetActive();
}

void CSymbianEventReceiver::RunL()
{
    TWsEvent wsEvent;
    iWsSession.GetEvent( wsEvent );
    
    iParent->HandleWsEvent( wsEvent );
    
    iWsSession.EventReady( &iStatus );
    
    SetActive();
}

void CSymbianEventReceiver::DoCancel()
{
    iWsSession.EventReadyCancel();
}
