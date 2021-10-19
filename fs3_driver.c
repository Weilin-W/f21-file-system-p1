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
	FS3CmdBlk fs3_sycall(FS3CmdBlk deconstruct_fs3_cmdblock(), void *buf);

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
	if(path == NULL){
		path = new(path);
		int pathlength = 0;
	}
	else{
		fs3_read(path,*buf,1);
		fs3_write(path,*buf,1);
	}
	return (0); // Likely wrong
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : fs3_close
// Description  : This function closes the file
//
// Inputs       : fd - the file descriptor
// Outputs      : 0 if successful, -1 if failure

int16_t fs3_close(int16_t fd) {
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
	return (0);
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
	return (0);
}
