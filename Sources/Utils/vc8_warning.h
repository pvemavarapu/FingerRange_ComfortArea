/*____________________________________________________________________
|
| File: vc8_warning.h
|
| Description: Contains things EVERY source file needs and always should
|		be the first header file included.
|
| (C) Copyright 2006 Timothy E. Roden.
|___________________________________________________________________*/

#ifndef VC8WARNING_H
#define VC8WARNING_H

// Enable older CRT functions (such as strcpy) without warnings from vc8 (vc 2005 .net)
#if _MSC_VER >= 1400
 #define _CRT_SECURE_NO_DEPRECATE				// shut up the vc8 compiler
 #define _CRT_NONSTDC_NO_DEPRECATE
#endif

#endif
