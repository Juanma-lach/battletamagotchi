#include <e32base.h> // for Symbian classes.
#include "gambi.h"

static TInt stack_counter = 0;

void stack_push(void* ptr){
	
	CleanupStack::PushL(ptr);
	stack_counter++;
}

void stack_pop(void* ptr){

	CleanupStack::Pop(ptr);
	stack_counter--;
}

void stack_clean(void){

	CleanupStack::PopAndDestroy(stack_counter);
}


