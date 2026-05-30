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

#include "./openCl.h"

#include "./coredata.h"

#include <dlfcn.h>
#include <string.h>

void InitOpenCl(void) {
	if(DATA.System.computeBackend.backend == COMPUTE_OPENCL) { return; }

#if defined(unix) || defined(__unix) || defined(__unix__)

	DATA.OpenCl.dlHandle = dlopen("libOpenCL.so", RTLD_NOW);

	if(!DATA.OpenCl.dlHandle) {
		char *err = dlerror();
		TraceLog("[ESCLIB]: WARNING: Couldn't load OpenCL, fallback to COMPUTE_ESC, error code: %s", err ? err : "unknown");
		CleanUpOpenCl();
		return;
	}

	DATA.OpenCl.clGetDeviceIDs = dlsym(DATA.OpenCl.dlHandle, "clGetDeviceIDs");
	DATA.OpenCl.clGetPlatformIDs = dlsym(DATA.OpenCl.dlHandle, "clGetPlatformIDs");

	DATA.OpenCl.clCreateContext = dlsym(DATA.OpenCl.dlHandle, "clCreateContext");
	DATA.OpenCl.clReleaseContext = dlsym(DATA.OpenCl.dlHandle, "clReleaseContext");

	DATA.OpenCl.clCreateCommandQueue = dlsym(DATA.OpenCl.dlHandle, "clCreateCommandQueue");
	DATA.OpenCl.clReleaseCommandQueue = dlsym(DATA.OpenCl.dlHandle, "clReleaseCommandQueue");

	if(!DATA.OpenCl.clGetDeviceIDs || !DATA.OpenCl.clGetPlatformIDs || !DATA.OpenCl.clCreateContext || !DATA.OpenCl.clReleaseContext || !DATA.OpenCl.clCreateCommandQueue || !DATA.OpenCl.clReleaseCommandQueue) {
    	TraceLog("[ESCLIB]: WARNING: Missing OpenCL symbols, fallback to COMPUTE_ESC");
    	dlclose(DATA.OpenCl.dlHandle);
		CleanUpOpenCl();
    	return;
	}

#elif defined(_WIN32) || defined(_WIN64)

#endif

	cl_int erri;

	erri = DATA.OpenCl.clGetPlatformIDs(1, &DATA.OpenCl.platform, &DATA.OpenCl.platformCount);
	if(erri != CL_SUCCESS || DATA.OpenCl.platformCount == 0) {
		TraceLog("[ESCLIB]: WARNING: No OpenCL platforms, fallback to COMPUTE_ESC, OpenCL error code: %d", erri);
		CleanUpOpenCl();
    	return;
	}

	erri = DATA.OpenCl.clGetDeviceIDs(DATA.OpenCl.platform, CL_DEVICE_TYPE_GPU, 1, &DATA.OpenCl.device, NULL);
	if(erri != CL_SUCCESS) {
		TraceLog("[ESCLIB]: WARNING: No GPUs, fallback to CPU, OpenCL error code: %d", erri);
		DATA.System.computeBackend.device = DEVICE_CPU;

		erri = DATA.OpenCl.clGetDeviceIDs(DATA.OpenCl.platform, CL_DEVICE_TYPE_CPU, 1, &DATA.OpenCl.device, NULL);
		if(erri != CL_SUCCESS) {
			TraceLog("[ESCLIB]: WARNING: No CPUs found, fallback to COMPUTE_ESC, OpenCL error code: %d", erri);
			CleanUpOpenCl();
    		return;
		}
	}
	else {
		DATA.System.computeBackend.device = DEVICE_GPU;
	}

	DATA.OpenCl.context = DATA.OpenCl.clCreateContext(NULL, 1, &DATA.OpenCl.device, NULL, NULL, &erri);
    if(erri != CL_SUCCESS) {
        TraceLog("[ESCLIB]: WARNING: Failed to create OpenCL context, fallback to COMPUTE_ESC, OpenCL error code: %d", erri);
		CleanUpOpenCl();
        return;
    }

	DATA.OpenCl.queue = DATA.OpenCl.clCreateCommandQueue(DATA.OpenCl.context, DATA.OpenCl.device, 0, &erri);
    if(erri != CL_SUCCESS) {
        TraceLog("[ESCLIB]: WARNING: Failed to create OpenCL queue, fallback to COMPUTE_ESC, OpenCL error code: %d", erri);
		CleanUpOpenCl();
        return;
    }

	DATA.System.computeBackend.backend = COMPUTE_OPENCL;

	TraceLog("[ESCLIB]: INFO: OpenCL initialized successfully, device: %s", DATA.System.computeBackend.device == DEVICE_GPU ? "GPU" : "CPU");
}

void CleanUpOpenCl(void) {
	if(DATA.OpenCl.context) {
        DATA.OpenCl.clReleaseContext(DATA.OpenCl.context);
        DATA.OpenCl.context = NULL;
    }
	if(DATA.OpenCl.queue) {
    	DATA.OpenCl.clReleaseCommandQueue(DATA.OpenCl.queue);
		DATA.OpenCl.queue = NULL;
	}
	if(DATA.OpenCl.dlHandle) {
		dlclose(DATA.OpenCl.dlHandle);
		DATA.OpenCl.dlHandle = NULL;
	}
	memset(&DATA.OpenCl, 0, sizeof(DATA.OpenCl));

	DATA.System.computeBackend.device = DEVICE_CPU;
	DATA.System.computeBackend.backend = COMPUTE_ESC;
}
