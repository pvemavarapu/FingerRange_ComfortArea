//|___________________________________________________________________
//!
//! \file exceptionHandler.h
//!
//! \brief Handles various exceptions.
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

#ifndef _EXCEPTION_HANDLER_H_
#define _EXCEPTION_HANDLER_H_

//|___________________
//|
//| Local Includes
//|___________________

#include <stdlib.h>
#include <iostream>

//|___________________________________________________________________
//|
//! Handles various exceptions.
//|___________________________________________________________________

class exceptionHandler
{
public :
	exceptionHandler();
	~exceptionHandler();

	void MemoryAllocationError(char *errorMsg);
	void FileInputError(char *filename);
	void ProcessStarted(char *status);
	void ProcessComplete();
	void ProcessReadFile(char *filename);
	void errMsg(char *message);
};

#endif
