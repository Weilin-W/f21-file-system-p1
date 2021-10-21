////////////////////////////////////////////////////////////////////////////////
//
//  File           : fs3_driver.c
//  Description    : This is the implementation of the standardized IO functions
//                   for used to access the FS3 storage system.
//
//   Author        : *** Wei Lin Weng ***
//   Last Modified : *** 10/05/2021-10/15/2021 ***
//

// Includes
#include <string.h>
#include <cmpsc311_log.h>

// Project Includes
#include <fs3_driver.h>
#include <fs3_controller.h>
//
// Defines
#define SECTOR_INDEX_NUMBER(x) ((int)(x/FS3_SECTOR_SIZE))

//
// Static Global Variables
typedef struct TFiles{
	char fname;
	int length;
	int trk;
	int sect;
	int fileOpen;
	int handler;
	int position;
}TFiles;
//File array with sector 64 and track 1024
TFiles farray[64][1024];
TFiles tfile;
//
// Implementation



//FS#CmdBlk Implementation: construct fs3
FS3CmdBlk construct_fs3_cmdblock(uint8_t op, uint16_t sec, uint_fast32_t trk, uint8_t ret){
	// create FS3 array opcode from the variable fields
	FS3CmdBlk CmdBlk = (uint64_t)op << 60|(uint64_t)sec << 44|(uint64_t)trk << 12|(uint64_t)ret << 11;
	return CmdBlk;
}
int deconstruct_fs3_cmdblock(FS3CmdBlk cmdblock, uint8_t *op, uint16_t *sec, uint32_t *trk, uint8_t *ret){
	// extract register state from bus values
	*op = cmdblock >> 60;
	*sec = cmdblock >> 44;
	*trk = cmdblock >> 12;
	return *ret;
}
////////////////////////////////////////////////////////////////////////////////
//
// Function     : fs3_mount_disk
// Description  : FS3 interface, mount/initialize filesystem
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int32_t fs3_mount_disk(void) {
	uint16_t sec = 0;
	uint_fast32_t trk = 0;
	uint8_t ret = 0;
	FS3CmdBlk fs3_syscall(FS3CmdBlk cmdblock, void *buf);
	fs3_syscall(construct_fs3_cmdblock(FS3_OP_MOUNT,sec,trk,ret), NULL);
	tfile.fileOpen = 0;

	/*printf("Break line----------");    ATTENTION: Checker for if deconstruct equals 0
	if(deconstruct_fs3_cmdblock(FS3CmdBlk cmdblock,uint8_t FS3_OP_MOUNT,uint16_t sec,uint32_t trk, uint8_t ret) == 0){
		return(0);
	}*/
	return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : fs3_unmount_disk
// Description  : FS3 interface, unmount the disk, close all files
//
// Inputs       : none
// Outputs      : 0 if successful, -1 if failure

int32_t fs3_unmount_disk(void) {
	uint16_t sec = 0;
	uint32_t trk = 0;
	uint8_t ret = 0;
	FS3CmdBlk fs3_syscall(FS3CmdBlk cmdblock, void *buf);
	fs3_syscall(construct_fs3_cmdblock(FS3_OP_UMOUNT,sec,trk,ret), NULL);

	return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : fs3_open
// Description  : This function opens the file and returns a file handle
//
// Inputs       : path - filename of the file to open
// Outputs      : file handle if successful, -1 if failure

int16_t fs3_open(char *path) {
	/*Struc{
		length of the file
		int sectors[][]
		choose sectors and files

		int handle "Use handle to pass info"
	}*/
	/*
	tfile Tfile;
	for(int i = 0; i < FS3_MAX_TRACKS-1; i++){
		for (int j = 0; j < FS3_SECTOR_SIZE; j++){
			if(tfile farray[i][j] == -1){
				//fs3_open("assign2/penn-state.txt");
				Tfile.trk = 0;
				Tfile.sect = 0;
				Tfile.length = 0;
			}
			else{
				Tfile.fileOpen = 1; //Boolean of open
				Tfile.trk = 0;
				Tfile.sect = 0;
			}
		}
	}
	typedef struct TFiles{
	char fname;
	int length;
	int trk;
	int sect;
	int fileOpen;
	int handler;
}
	*/
	/*int filepath = path;
	if(filepath = NULL){
		tfile.length = 0;
	}*/
	if (tfile.fileOpen == 1){
		return(-1);
	}
	else{
		tfile.handler = 1;
		tfile.position = 0;
		tfile.fileOpen = 1;
	}

	return (tfile.handler); // Likely wrong
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : fs3_close
// Description  : This function closes the file
//
// Inputs       : fd - the file descriptor
// Outputs      : 0 if successful, -1 if failure

int16_t fs3_close(int16_t fd) {
	//check if file already closed
	if(tfile.fileOpen == 0){
		return -1;
	}
	else{
		//close the file
		tfile.fileOpen = 0; 
	}
	return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : fs3_read
// Description  : Reads "count" bytes from the file handle "fh" into the 
//                buffer "buf"
//
// Inputs       : fd - filename of the file to read from
//                buf - pointer to buffer to read into
//                count - number of bytes to read
// Outputs      : bytes read if successful, -1 if failure

int32_t fs3_read(int16_t fd, void *buf, int32_t count) {
	uint16_t sec = fd;
	uint32_t trk = 0;
	uint8_t ret = 0;
	FS3CmdBlk fs3_syscall(FS3CmdBlk cmdblock, void *buf);
	fs3_syscall(construct_fs3_cmdblock(FS3_OP_RDSECT,sec,trk,ret), void &buf);
	char buffer[1024];
	memcpy(buffer,&buf,count);
	return(buffer[1]);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : fs3_write
// Description  : Writes "count" bytes to the file handle "fh" from the 
//                buffer  "buf"
//
// Inputs       : fd - filename of the file to write to
//                buf - pointer to buffer to write from
//                count - number of bytes to write
// Outputs      : bytes written if successful, -1 if failure

int32_t fs3_write(int16_t fd, void *buf, int32_t count) {
	//memcpy
	//syscall
	//return # of bytes
	return (0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : fs3_seek
// Description  : Seek to specific point in the file
//
// Inputs       : fd - filename of the file to write to
//                loc - offfset of file in relation to beginning of file
// Outputs      : 0 if successful, -1 if failure

int32_t fs3_seek(int16_t fd, uint32_t loc) {
	//change the position of file
	return (0);
}
