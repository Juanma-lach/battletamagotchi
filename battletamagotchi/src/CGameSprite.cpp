#include "CGameSprite.h"
#include "Utils3d.h"
#include "gambi.h"
#include <stdio.h>

// Parse the current drive letter from application location
	//TPtrC16 driveLetter = utilGetAppRootDirectoryL().Left( 2 );
	//fullTexturePath.Append( driveLetter );

    // Append the application private path

	/*RFs iFs;
	TFileName fullTexturePath;
    iFs.Connect();
	
    TPath privateDir;
    User::LeaveIfError( iFs.PrivatePath( privateDir ) );
    fullTexturePath.Append( privateDir );
    fullTexturePath.Append(filename);
    iFs.Close();*/

	/*#ifndef __SERIES60_3X__
	TFileName name(filename);
	CompleteWithAppPath( name );
	// windows build wants to install apps on rom drive ( "z:\" )
	// but we want the data from "c:\" instead
	#ifdef __WINS__
	name[ 0 ] = 'C';
	#endif
	
	#else // 3rd ed
	
	RFs fsSession;
	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL( fsSession );
	TFileName name;
	fsSession.PrivatePath( name );
	name.Append( filename );
	
	#ifndef __WINS__
	// insert the drive to the private path
	TParsePtrC parse(
	    (CEikonEnv::Static()->EikAppUi()->Application())->AppFullName() );
	name.Insert( 0, parse.Drive() );
	#endif
	
	#endif
	
	for(int i=0; i< name.Length();i++) printf("%c", name[i]);
	printf("\n");
	
	CCoeEnv::Static()->FsSession().MkDirAll(name);*/

/*RFs iFs;
iFs.Connect();

const TInt KPathNameLength = 100;
TBuf< KPathNameLength > pathWithoutDrive;
TBuf< KPathNameLength > driveAndPath;
TBuf< 2 > appDrive;

// Get application's private path
//   This is e.g. '\Private\20001978\'
//   Does not contain drive.
iFs.PrivatePath( pathWithoutDrive );
 
// Extract drive letter into appDrive:
appDrive.Copy(CEikonEnv::Static()->EikAppUi()->Application()->AppFullName().Left(2));
 
// Combine drive letter and private path
driveAndPath.Copy(appDrive);
driveAndPath.Append(pathWithoutDrive);
 
// Now driveAndPath contains e.g. 'E:\Private\20001978\'

driveAndPath.Append(filename);
    
    iBitmap = new (ELeave) CFbsBitmap;
    stack_push(iBitmap);
    
    TInt i = iBitmap->Load(driveAndPath, 1, 0);
    
    iFs.Close();
    
    if(i != KErrNone) printf("FUDEU! %d\n",i);*/

CGameSprite::CGameSprite(TPtrC filename,
						TUint8* colorKey,
						CGameRect* clips,
						TUint32* times,
						TUint32 num)
{
	
	RFs fsSession;
	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL( fsSession );
	TFileName name, path;
	name.Append(_L("c:"));
	fsSession.PrivatePath( path );
	name.Append( path );
	name.Append( filename );
		
	//for(int i=0; i< name.Length();i++) printf("%c", name[i]);
	//printf("\n");
	
	CFbsBitmap *iBitmap = new (ELeave) CFbsBitmap;
	    //stack_push(iBitmap);
	    
	    TInt j,i = iBitmap->Load(name, 0, 0);
	    
	    TInt h = iBitmap->SizeInPixels().iHeight;
	    TInt w = iBitmap->SizeInPixels().iWidth;
	    
	    //printf("Tamanho da textura: height %d, width %d\n", h, w);
	    	    
	    if(i != KErrNone) printf("FUDEU! %d\n",i);
	    
	    //TUint32* baw = iBitmap->DataAddress();
	    
	    TRgb color;
	    TPoint point;
	    
	    this->data = 0;
	    this->data = new TUint32[SIZE*SIZE];
	    if(!this->data){
	    	printf("FUDEU NO DATA!!!!!!!\n");
	    	getchar();
	    }
	    
	    for(i=0;i<SIZE;i++){
	    	for(j=0;j<SIZE;j++){
	    		point.SetXY(i,j);
	    		iBitmap->GetPixel(color,point);
	    		//printf("Copying...\n");
	    		this->data[i*SIZE+j] = color.Value();
	    	}
	    }
	    
	    //printf("Copiei ^^\n");
	    
	    delete iBitmap;

}

CGameSprite::~CGameSprite(){

	delete [] this->data;
}
