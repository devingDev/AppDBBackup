// INCLUDES
#include <psp2/kernel/processmgr.h>
#include <psp2/io/stat.h>
#include <psp2/io/fcntl.h>
#include <psp2/ctrl.h>
#include <stdlib.h>
#include "debugScreen.h"

// DEFINES

#define printf psvDebugScreenPrintf

// FUNCTION DECLARATIONS
int backupAppDatabase();
int restoreAppDatabase();

// START

int main(int argc, char *argv[]) {
	
	psvDebugScreenInit();
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	SceCtrlData ctrl;
	sceCtrlPeekBufferPositive(0 , &ctrl , 1);
	
	
	
        printf("Hey , this is coderx3 and this is an app.db backup homebrew!\n");
	printf("\r\nTo restore an app.db with this it has to be here : ");
	printf("\r\nux0:data/appdbbackupper/backedup_app.db");
	
	printf("\r\n\r\n Checking for directory.");
	// SETUP	
	const char *  BACKUPDIR = "ux0:data/appdbbackupper";
	struct SceIoStat * dirStat = (SceIoStat*)malloc(sizeof(SceIoStat));
	if(sceIoGetstat(BACKUPDIR, dirStat) < 0){
		printf("\r\n Creating non-existant directory : ux0:data/appdbbackupper");
		sceIoMkdir(BACKUPDIR , 0777);
	}
	
	printf("\r\n\r\n\r\nTo backup the app.db from ur0 to ux0 press X!");
	printf("\r\nTo restore a previously backed up database press O!");
	
	printf("\r\nPress Start to exit.");
	
	while(ctrl.buttons != SCE_CTRL_START){
		sceCtrlPeekBufferPositive(0 , &ctrl , 1);
		if(ctrl.buttons == SCE_CTRL_CROSS){
			printf("\r\n\r\n Backing up app.db from ur0:shell/db/app.db to ux0:data/appdbbackupper/backedup_app.db");
			backupAppDatabase();
			printf("\r\n Backed up!");
			sceKernelDelayThread(1*1000000);
		}else if(ctrl.buttons == SCE_CTRL_CIRCLE){
			printf("\r\n\r\n Restoring app.db from ux0:data/appdbbackupper/backedup_app.db");
			restoreAppDatabase();
			printf("\r\n Restored!");
			sceKernelDelayThread(1*1000000);
		}
		sceKernelDelayThread(5*10000);
	}
	
	
	printf("\r\n Exiting in 10 seconds.");
	sceKernelDelayThread(10*1000000); // Wait 3 sec
	sceKernelExitProcess(0);
	return 0;
}

int restoreAppDatabase(){
        int fhdb = sceIoOpen("ur0:shell/db/app.db", SCE_O_WRONLY | SCE_O_CREAT, 0777);
        int fhdbux0 = sceIoOpen("ux0:data/appdbbackupper/backedup_app.db" , SCE_O_RDONLY , 0777);
	if(fhdb > 0 && fhdbux0 > 0){
                const int BUFFER_SIZE = 4096;
                char buf[BUFFER_SIZE];
                int readbytes = 0;
                int totalreadbytes = 0;
                int filesize = sceIoLseek(fhdbux0 , 0 , SCE_SEEK_END);
                sceIoLseek(fhdbux0 , 0 , SCE_SEEK_SET);
		
		if(filesize <= 0){
			printf("\r\n Error : File ux0:data/appdbbackupper/backedup_app.db size is 0!");
			return -2;
		}

		while(totalreadbytes < filesize){
			
			readbytes = sceIoRead(fhdbux0 ,  buf , BUFFER_SIZE);
                        totalreadbytes += readbytes;

                        // WRITE
                        sceIoWrite(fhdb , buf , BUFFER_SIZE);



                }

                sceIoClose(fhdb);
                sceIoClose(fhdbux0);
			
		
		
		return 1;
	}else{
		printf("\r\nCould not open files!");
		return -1;
	}
}

int backupAppDatabase(){
	int fhdb = sceIoOpen("ur0:shell/db/app.db", SCE_O_RDONLY, 0777);
	int fhdbux0 = sceIoOpen("ux0:data/appdbbackupper/backedup_app.db" , SCE_O_WRONLY | SCE_O_CREAT , 0777);
	if(fhdb > 0 && fhdbux0 > 0){
		const int BUFFER_SIZE = 4096;
		char buf[BUFFER_SIZE];
		int readbytes = 0;
		int totalreadbytes = 0;
		int filesize = sceIoLseek(fhdb , 0 , SCE_SEEK_END);
		sceIoLseek(fhdb , 0 , SCE_SEEK_SET);
		
		while(totalreadbytes < filesize){
			
			// READ
			readbytes = sceIoRead(fhdb ,  buf , BUFFER_SIZE);
			totalreadbytes += readbytes;
			
			// WRITE
			sceIoWrite(fhdbux0 , buf , BUFFER_SIZE);
			
			
			
		}
		
		sceIoClose(fhdb);
		sceIoClose(fhdbux0);
		
		return 1;
	}else{
		return -1;
	}
}




