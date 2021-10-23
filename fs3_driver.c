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
	//int mountDisk;
}TFiles;
//File array with sector 64 and track 1024
TFiles farray[64][1024];
TFiles tfile;
//
// Implementation




//FS#CmdBlk Implementation: construct fs3
FS3CmdBlk construct_fs3_cmdblock(uint8_t op, uint16_t sec, uint_fast32_t trk, uint8_t ret){
	// create FS3 array opcode from the variable fields
	//Type cast into 64 bytes for each parameters and then use or to combine them
	//retrun the cmdblock
	FS3CmdBlk cmdblock = (uint64_t)op << 60|(uint64_t)sec << 44|(uint64_t)trk << 12|(uint64_t)ret << 11;
	return cmdblock;
}
int deconstruct_fs3_cmdblock(FS3CmdBlk cmdblock, uint8_t *op, uint16_t *sec, uint32_t *trk, uint8_t *ret){
	// extract register state from bus values
	//masking the values with AND
	*op = 0xF & (cmdblock >> 60); // masking by 1111 and shifts back
	*sec = 0xFFFF & (cmdblock >> 44); //masking by 1111 1111 1111 1111 and shifts back
	*trk = 0xFFFFFFFF & (cmdblock >> 12); // masking by 1111 *8 times and shifts back
	*ret = 0x1 & (cmdblock >> 11); // masking by 1 and shifts back
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
	//calling construct function for a cmdblock and set op code to Mount value with no buffer pointer
	fs3_syscall(construct_fs3_cmdblock(FS3_OP_MOUNT,sec,trk,ret),NULL);
	tfile.fileOpen = 0; //file at closed state
	tfile.position = 0; //file position at 0
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
	//Construct unmount disk based on construct a cmdblock by op code of umount
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
	//check if the file already open
	//0 means not open already
	if (tfile.fileOpen == 1){
		return(-1);
	}
	else{
		//set file handler to any value but consistent, position to 0, set checker for file open to 1(open)
		tfile.handler = 1;
		tfile.position = 0;
		tfile.length = 0;
		tfile.fileOpen = 1;
	}

	return (tfile.handler);
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
		return (0);
	}
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
	//checks if file handler is bad, and file is not close
	if(tfile.handler != 1 || tfile.fileOpen == 0){
		return -1;
	}else{
		uint16_t sec = 0;
		uint32_t trk = 0;
		uint32_t track = 23;
		uint8_t ret = 0;

		//temp buffer array
		char buffer[1024];
		

		//call tseek, no access to buffer
		//read by using buffer pointer
		fs3_syscall(construct_fs3_cmdblock(FS3_OP_TSEEK,sec,track,ret), NULL);
		fs3_syscall(construct_fs3_cmdblock(FS3_OP_RDSECT,sec,trk,ret), buffer);
		
		//memcpy(destination,source,count);
		//put buffer into buf pointer, by # of count
		memcpy(buf,buffer,count);
			
		if(count + tfile.position > tfile.length){
			//count equals the length minus the position
			//position equals the length of file
			count = tfile.length - tfile.position;
			tfile.position = tfile.length;
		}else{
			//position + count 
			tfile.position = tfile.position + count;
		}
		return(count);
	}
	
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
	//Check for the condition of file close and different handler
	if(tfile.handler != 1 || tfile.fileOpen == 0){
		return -1;
	}else{
		//set values in each variables
		uint16_t sec = 0;
		uint32_t trk = 0;
		uint32_t track = 0;
		uint8_t ret = 0;

		//create a temp array for buffer
		char buffer[1024];
		
		//Call tseek, read, write, based on the OP code set to
		fs3_syscall(construct_fs3_cmdblock(FS3_OP_TSEEK,sec,track,ret), NULL);
		fs3_syscall(construct_fs3_cmdblock(FS3_OP_RDSECT,sec,trk,ret), buffer);
		//using memcpy putting buf into buffer array at position by count
		memcpy(&buffer[tfile.position],buf,count);
		fs3_syscall(construct_fs3_cmdblock(FS3_OP_WRSECT,sec,trk,ret), buffer);
		

		
		//update position and length
		// count + position > length
			// pos = pos + count
			//length = position
		if(count + tfile.position > tfile.length){
			tfile.position = tfile.position + count;
			tfile.length = tfile.position;
			//under: position + count < length
			//posit = post + count
		}else if(tfile.position + count < tfile.length){
			tfile.position = tfile.position + count;

		}else{
			// position + count = length
			//position = length
			tfile.position = tfile.length;
		}

		//return # of bytes
		return count;
		
	}	
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
	if((loc > tfile.length)||(tfile.handler != 1)||(tfile.fileOpen == 0)){
		//failed, bc/ mismatch file handler, file closed, length less than offsets
		return -1;
	}
	else{
		//set position to the offset
		tfile.position = loc;
		//Successful
		return(0);
	}
}
