//|___________________________________________________________________
//!
//! \file exceptionHandler.cpp
//!
//! \brief Handles various exceptions.
//!
//! Author: Phillip (modified by Mark).
//|___________________________________________________________________

//|___________________
//|
//| Includes
//|___________________

#include "exceptionHandler.h"

//|____________________________________________________________________
//|
//| Function: exceptionHandler
//|
//! \param None.
//! \return None.
//!
//! Constructor. 
//|____________________________________________________________________

exceptionHandler::exceptionHandler()
{
	//Default Constructor
}

//|____________________________________________________________________
//|
//| Function: ~exceptionHandler
//|
//! \param None.
//! \return None.
//!
//! Destructor. 
//|____________________________________________________________________

exceptionHandler::~exceptionHandler()
{
	//Default Destructor
}

//|____________________________________________________________________
//|
//| Function: MemoryAllocationError 
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void exceptionHandler::MemoryAllocationError (char *errorMsg)
{
	std::cout<<"Cannot Allocate Memory for "<<errorMsg;
	exit(0);
}

//|____________________________________________________________________
//|
//| Function: ProcessStarted 
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void exceptionHandler::ProcessStarted (char *status)
{
	std::cout<<std::endl<<status<<"..."<<std::endl;
}

//|____________________________________________________________________
//|
//| Function: ProcessComplete 
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void exceptionHandler::ProcessComplete ()
{
	std::cout<<"... Successfully Completed the Process";
}

//|____________________________________________________________________
//|
//| Function: ProcessReadFile 
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void exceptionHandler::ProcessReadFile (char *filename)
{
	std::cout<<"\nStarted Reading from file "<<filename<<" ..."<<std::endl;
}

//|____________________________________________________________________
//|
//| Function: errMsg
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void exceptionHandler::errMsg (char *message)
{
	std::cout<<std::endl<<message;
	exit(0);
}

//|____________________________________________________________________
//|
//| Function: FileInputError 
//|
//! \param None.
//! \return None.
//!
//! ... 
//|____________________________________________________________________

void exceptionHandler::FileInputError (char *filename)
{
	std::cout<<std::endl<<"Cannot Read from File : "<<filename<<std::endl ;
	exit(0);
}