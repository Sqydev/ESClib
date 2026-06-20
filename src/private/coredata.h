/*
* Copyright (c) 2025-present Wojciech Kaptur ( _Sqyd_ / Sqydev )
* Github: https://github.com/Sqydev
* GPG Fingerprint: 6DC2516B0DFDA9C59661650722F7B8A777F33B56
* 
* This software is provided "as-is", without any express or implied warranty. In no event
* will the authors be held liable for any damages arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose, including commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
* 
* 1. Non-Misrepresentation: The origin of this software must not be misrepresented; 
*    you must not claim that you wrote the original software. An acknowledgment in 
*    product documentation is appreciated but not required.
* 
* 2. Source-Level Copyleft: Any altered versions (forks) of this software's source code, 
*    or files containing significant portions of this code, must be distributed under 
*    these same license terms. Such modified source code must be made publicly available 
*    to any recipient, even if used over a network (SaaS).
* 
* 3. Proprietary Integration: This software may be integrated into, linked with, or 
*    used as a component of proprietary and closed-source products. In such cases, 
*    the surrounding proprietary application code does not need to be disclosed, 
*    provided that the original or modified source code of THIS software remains 
*    available under the terms of Section 2.
* 
* 4. Persistent Metadata: All original credits, including those in the source code headers 
*    and binary metadata (e.g., ELF .comment section, PE StringFileInfo, or equivalent), 
*    must not be removed. You may add your own credits to forks, provided the original 
*    authorship remains clearly identified.
* 
* 5. Notice Retention: This license notice may not be removed or altered from any 
*    source or binary distribution.
*/

#ifndef ESCLIB_PRIVATE_COREDATA_H
#define ESCLIB_PRIVATE_COREDATA_H

#include "../../include/esclib.h"

#include <stdbool.h>
#include <signal.h>
#include <stdio.h>

#include <CL/cl.h>

#if defined(unix) || defined(__unix) || defined(__unix__)

	#include <termios.h>

#elif defined(_WIN32) || defined(__WIN64)
#endif

typedef enum {
	WAYLAND,
	X11,
	NONE,
	WINDOWS
} Compositor;

typedef enum {
	COMP_WAYLAND,
	COMP_X11,
	EVDEV,
	TERMIOS
} InputBackend;

typedef struct {
	enum {
		COMPUTE_ESC,
		COMPUTE_OPENCL,
	} backend;
	enum {
		DEVICE_CPU,
		DEVICE_GPU,
	} device;
} ComputeBackend;

typedef struct Kernel {
    cl_program program;
    cl_kernel kernel;
    cl_mem* argBufs;
    size_t* argSizes;
    uint32_t argCount;
} Kernel;

typedef struct {
	struct {
		SBCell* frontbuff;
		size_t frontbuffOffset;
		
		SBCell* backbuff;
		size_t backbuffOffset;

		char* charbuffer;
		size_t charbufferOffset;
	} Buffers;

	struct {
		Compositor compositor;
		ComputeBackend computeBackend;
	} System;

	struct {
		TuiType type;

		Vector2i lastTuiIndex;
		Vector2i tuidimm;
		Vector2i tuidimmInPixels;

		Vector2i lastTermIndex;
		Vector2i termdimm;
		Vector2i termdimmInPixels;

		Vector2i cellsDimms;
		Vector2i cellsProp;
		
		bool initiated; // TUI MUST DIE
		
#if defined(unix) || defined(__unix) || defined(__unix__)

		struct termios oldTerminalConfig;
		struct termios escTerminalConfig;

#elif defined(_WIN32) || defined(_WIN64)

		DWORD oldConsoleMode;
		HANDLE hStdin;

#endif
	} TuiData;

	struct {
		int targetFps;
		double target;

		double current;
		double previous;
		double delta;
	} Time;

	struct {
		struct {
			bool keyStates[ESC_KEYMAX];
		} Keyboard;
	} Input;

	struct {
		Vector2i pos;
		bool hidden;
		bool preHidden;
		bool oob;
	} Cursor;

	struct {
		struct {
#if defined(unix) || defined(__unix) || defined(__unix__)

			struct sigaction old;

#elif defined(_WIN32) || defined(_WIN64)
#endif
			volatile sig_atomic_t triggered;
			bool enabled;
			bool enabledESClibTasks;
			bool enabledCustomTasks;
			bool enabledBuildInTasks;
			void (**customTasks)(void);
			int customTasksNumber;
		} SIG_INT;

		struct {
#if defined(unix) || defined(__unix) || defined(__unix__)

			struct sigaction old;

#elif defined(_WIN32) || defined(_WIN64)
#endif
			volatile sig_atomic_t triggered;
			bool enabled;
			bool enabledESClibTasks;
			bool enabledCustomTasks;
			bool enabledBuildInTasks;
			void (**customTasks)(void);
			int customTasksNumber;
		} SIG_WINCH;
	} SignalData;

	struct {
		bool enabled;

		FILE* file;
		LogLevel logLevel;

		char* path;
	} Logging;

	struct {
		cl_platform_id platform;
		cl_uint platformCount;

		cl_device_id device;

		cl_context context;
		cl_command_queue queue;
		cl_program program;
		cl_kernel kernel;

		void* dlHandle;
		
		cl_int (*clGetPlatformIDs)(cl_uint, cl_platform_id*, cl_uint*);
		cl_int (*clGetDeviceIDs)(cl_platform_id, cl_device_type, cl_uint, cl_device_id*, cl_uint*);

		cl_context (*clCreateContext)(const cl_context_properties*, cl_uint, const cl_device_id*, void (CL_CALLBACK *)(const char*, const void*, size_t, void*), void*, cl_int*);
		cl_int (*clReleaseContext)(cl_context context);

		cl_command_queue (*clCreateCommandQueue)(cl_context, cl_device_id, cl_command_queue_properties, cl_int*);
		cl_int (*clReleaseCommandQueue)(cl_command_queue);

		cl_program (*clCreateProgramWithSource)(cl_context, cl_uint, const char**, const size_t*, cl_int*);
		
		cl_int (*clBuildProgram)(cl_program, cl_uint, const cl_device_id*, const char*, void (*)(cl_program, void*), void*);
		cl_int (*clReleaseProgram)(cl_program);

    	cl_kernel (*clCreateKernel)(cl_program, const char*, cl_int*);
		cl_int (*clReleaseKernel)(cl_kernel);

		cl_int (*clGetProgramBuildInfo)(cl_program, cl_device_id, cl_program_info, size_t, void*, size_t*);

		cl_int (*clSetKernelArg)(cl_kernel, cl_uint, size_t, const void*);

		cl_int (*clEnqueueNDRangeKernel)(cl_command_queue, cl_kernel, cl_uint, const size_t*, const size_t*, const size_t*, cl_uint, const cl_event*, cl_event*);

		cl_int (*clFinish)(cl_command_queue);

		cl_mem (*clCreateBuffer)(cl_context, cl_mem_flags, size_t, void*, cl_int*);
		cl_int (*clReleaseMemObject)(cl_mem);

		cl_int (*clEnqueueReadBuffer)(cl_command_queue, cl_mem, cl_bool, size_t, size_t, void*, cl_uint, const cl_event*, cl_event*);

		cl_int (*clEnqueueWriteBuffer)(cl_command_queue, cl_mem, cl_bool, size_t, size_t, const void*, cl_uint, const cl_event*, cl_event*);
	} OpenCl;
} CoreData;

extern CoreData DATA;

#endif
