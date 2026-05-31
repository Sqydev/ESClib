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

#include "../include/esclib.h"

#include "./private/coredata.h"

#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

void InitOpenCl(void) {
	if(DATA.System.computeBackend.backend == COMPUTE_OPENCL) { return; }

#if defined(unix) || defined(__unix) || defined(__unix__)

	DATA.OpenCl.dlHandle = dlopen("libOpenCL.so", RTLD_NOW);

	if(!DATA.OpenCl.dlHandle) {
		char *err = dlerror();
		TraceLog("[ESCLIB.CompileKernel]: WARNING: Couldn't load OpenCL, fallback to COMPUTE_ESC, error code: %s", err ? err : "unknown");
		CleanUpOpenCl();
		return;
	}

	DATA.OpenCl.clGetDeviceIDs = dlsym(DATA.OpenCl.dlHandle, "clGetDeviceIDs");
	DATA.OpenCl.clGetPlatformIDs = dlsym(DATA.OpenCl.dlHandle, "clGetPlatformIDs");

	DATA.OpenCl.clCreateContext = dlsym(DATA.OpenCl.dlHandle, "clCreateContext");
	DATA.OpenCl.clReleaseContext = dlsym(DATA.OpenCl.dlHandle, "clReleaseContext");

	DATA.OpenCl.clCreateCommandQueue = dlsym(DATA.OpenCl.dlHandle, "clCreateCommandQueue");
	DATA.OpenCl.clReleaseCommandQueue = dlsym(DATA.OpenCl.dlHandle, "clReleaseCommandQueue");

	DATA.OpenCl.clCreateProgramWithSource = dlsym(DATA.OpenCl.dlHandle, "clCreateProgramWithSource");

	DATA.OpenCl.clBuildProgram = dlsym(DATA.OpenCl.dlHandle, "clBuildProgram");
	DATA.OpenCl.clReleaseProgram = dlsym(DATA.OpenCl.dlHandle, "clReleaseProgram");

	DATA.OpenCl.clCreateKernel = dlsym(DATA.OpenCl.dlHandle, "clCreateKernel");
	DATA.OpenCl.clReleaseKernel = dlsym(DATA.OpenCl.dlHandle, "clReleaseKernel");

	DATA.OpenCl.clGetProgramBuildInfo = dlsym(DATA.OpenCl.dlHandle, "clGetProgramBuildInfo");

	DATA.OpenCl.clSetKernelArg = dlsym(DATA.OpenCl.dlHandle, "clSetKernelArg");

	DATA.OpenCl.clEnqueueNDRangeKernel = dlsym(DATA.OpenCl.dlHandle, "clEnqueueNDRangeKernel");

	DATA.OpenCl.clFinish = dlsym(DATA.OpenCl.dlHandle, "clFinish");

	DATA.OpenCl.clCreateBuffer = dlsym(DATA.OpenCl.dlHandle, "clCreateBuffer");
	DATA.OpenCl.clReleaseMemObject = dlsym(DATA.OpenCl.dlHandle, "clReleaseMemObject");

	DATA.OpenCl.clEnqueueReadBuffer = dlsym(DATA.OpenCl.dlHandle, "clEnqueueReadBuffer");

	DATA.OpenCl.clEnqueueWriteBuffer = dlsym(DATA.OpenCl.dlHandle, "clEnqueueWriteBuffer");

	if(!DATA.OpenCl.clGetDeviceIDs || !DATA.OpenCl.clGetPlatformIDs || !DATA.OpenCl.clCreateContext || !DATA.OpenCl.clReleaseContext || !DATA.OpenCl.clCreateCommandQueue || !DATA.OpenCl.clReleaseCommandQueue || !DATA.OpenCl.clCreateProgramWithSource || !DATA.OpenCl.clBuildProgram || !DATA.OpenCl.clReleaseProgram || !DATA.OpenCl.clCreateKernel || !DATA.OpenCl.clReleaseKernel || !DATA.OpenCl.clGetProgramBuildInfo || !DATA.OpenCl.clSetKernelArg || !DATA.OpenCl.clEnqueueNDRangeKernel || !DATA.OpenCl.clFinish || !DATA.OpenCl.clCreateBuffer || !DATA.OpenCl.clReleaseMemObject || !DATA.OpenCl.clEnqueueReadBuffer || !DATA.OpenCl.clEnqueueWriteBuffer) {
		TraceLog("[ESCLIB.CompileKernel]: WARNING: Missing OpenCL symbols, fallback to COMPUTE_ESC");
		dlclose(DATA.OpenCl.dlHandle);
		CleanUpOpenCl();
		return;
	}

#elif defined(_WIN32) || defined(_WIN64)

#endif

	cl_int erri;

	erri = DATA.OpenCl.clGetPlatformIDs(1, &DATA.OpenCl.platform, &DATA.OpenCl.platformCount);
	if(erri != CL_SUCCESS || DATA.OpenCl.platformCount == 0) {
		TraceLog("[ESCLIB.CompileKernel]: WARNING: No OpenCL platforms, fallback to COMPUTE_ESC, OpenCL error code: %d", erri);
		CleanUpOpenCl();
		return;
	}

	erri = DATA.OpenCl.clGetDeviceIDs(DATA.OpenCl.platform, CL_DEVICE_TYPE_GPU, 1, &DATA.OpenCl.device, NULL);
	if(erri != CL_SUCCESS) {
		TraceLog("[ESCLIB.CompileKernel]: WARNING: No GPUs, fallback to CPU, OpenCL error code: %d", erri);
		DATA.System.computeBackend.device = DEVICE_CPU;

		erri = DATA.OpenCl.clGetDeviceIDs(DATA.OpenCl.platform, CL_DEVICE_TYPE_CPU, 1, &DATA.OpenCl.device, NULL);
		if(erri != CL_SUCCESS) {
			TraceLog("[ESCLIB.CompileKernel]: WARNING: No CPUs found, fallback to COMPUTE_ESC, OpenCL error code: %d", erri);
			CleanUpOpenCl();
			return;
		}
	}
	else {
		DATA.System.computeBackend.device = DEVICE_GPU;
	}

	DATA.OpenCl.context = DATA.OpenCl.clCreateContext(NULL, 1, &DATA.OpenCl.device, NULL, NULL, &erri);
	if(erri != CL_SUCCESS) {
		TraceLog("[ESCLIB.CompileKernel]: WARNING: Failed to create OpenCL context, fallback to COMPUTE_ESC, OpenCL error code: %d", erri);
		CleanUpOpenCl();
		return;
	}

	DATA.OpenCl.queue = DATA.OpenCl.clCreateCommandQueue(DATA.OpenCl.context, DATA.OpenCl.device, 0, &erri);
	if(erri != CL_SUCCESS) {
		TraceLog("[ESCLIB.CompileKernel]: WARNING: Failed to create OpenCL queue, fallback to COMPUTE_ESC, OpenCL error code: %d", erri);
		CleanUpOpenCl();
		return;
	}

	DATA.System.computeBackend.backend = COMPUTE_OPENCL;

	TraceLog("[ESCLIB.CompileKernel]: INFO: OpenCL initialized successfully, device: %s", DATA.System.computeBackend.device == DEVICE_GPU ? "GPU" : "CPU");
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

Kernel* CompileKernel(const char* path, const char* kernelName) {
	if(DATA.System.computeBackend.backend != COMPUTE_OPENCL) { TraceLog("[ESCLIB.CompileKernel]: WARNING: OpenCL not inited, returninig NULL"); return NULL; }

	Kernel* kernel = malloc(sizeof(Kernel));
	memset(kernel, 0, sizeof(Kernel));

	kernel->argBufs = NULL;
	kernel->argSizes = NULL;
	kernel->argCount = 0;

	FILE* file = fopen(path, "rb");
	if(!file) {
		free(kernel);
		TraceLog("[ESCLIB.CompileKernel]: ERROR: Couldn't open file at: %s, returninig NULL", path);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	rewind(file);

	char* src = malloc(fileSize + 1);
	if(!src) {
		fclose(file);
		free(kernel);
		TraceLog("[ESCLIB.CompileKernel]: ERROR: Couldn't malloc src");
		return NULL;
	}
	fread(src, 1, fileSize, file);
	src[fileSize] = '\0';
	fclose(file);

	cl_int erri;

	const char* sources[] = { src };
	size_t lengths[] = { fileSize };

	cl_program program = DATA.OpenCl.clCreateProgramWithSource(DATA.OpenCl.context, 1, sources, lengths, &erri);
	free(src);
	if(erri != CL_SUCCESS) {
		free(kernel);
		TraceLog("[ESCLIB.CompileKernel]: ERROR: clCreateProgramWithSource return value is not CL_SUCCESS, OpenCL error code: %d", erri);
		return NULL;
	}

	erri = DATA.OpenCl.clBuildProgram(program, 1, &DATA.OpenCl.device, NULL, NULL, NULL);
	if(erri != CL_SUCCESS) {
		DATA.OpenCl.clReleaseProgram(program);
		free(kernel);
		TraceLog("[ESCLIB.CompileKernel]: ERROR: clBuildProgram return value is not CL_SUCCESS, OpenCL error code: %d", erri);
		size_t logSize = 0;

		cl_int logErr;
		logErr = DATA.OpenCl.clGetProgramBuildInfo(program, DATA.OpenCl.device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		if(logErr != CL_SUCCESS) {
			TraceLog("[ESCLIB.CompileKernel]: Failed to get build log: %d", logErr);
		}

		char* log = malloc(logSize + 1);
		logErr = DATA.OpenCl.clGetProgramBuildInfo(program, DATA.OpenCl.device, CL_PROGRAM_BUILD_LOG, logSize, log, NULL);
		if(logErr != CL_SUCCESS) {
			TraceLog("[ESCLIB.CompileKernel]: Failed to get build log: %d", logErr);
		}

		log[logSize] = '\0';
		TraceLog("[ESCLIB.CompileKernel]: BUILD LOG:\n%s", log);
		free(log);
		return NULL;
	}

	cl_kernel Tkernel = DATA.OpenCl.clCreateKernel(program, kernelName, &erri);
	if(erri != CL_SUCCESS) {
		DATA.OpenCl.clReleaseProgram(program);
		free(kernel);
		TraceLog("[ESCLIB.CompileKernel]: ERROR: clCreateKernel return value is not CL_SUCCESS, OpenCL error code: %d", erri);
		return NULL;
	}

	kernel->program = program;
	kernel->kernel = Tkernel;

	return kernel;
}

int AddKernelArgValue(Kernel* kernel, uint32_t index, size_t size, const void* data) {
	if(DATA.System.computeBackend.backend != COMPUTE_OPENCL) {
		TraceLog("[ESCLIB.AddKernelArgValue]: WARNING: OpenCL not inited");
		return -1;
	}
	if(!kernel || !kernel->kernel) {
		TraceLog("[ESCLIB.AddKernelArgValue]: ERROR: invalid kernel");
		return -2;
	}
	cl_int erri = DATA.OpenCl.clSetKernelArg(kernel->kernel, index, size, data);
	if(erri != CL_SUCCESS) {
		TraceLog("[ESCLIB.AddKernelArgValue]: ERROR: clSetKernelArg failed: %d", erri);
		return -3;
	}
	return 0;
}

int AddKernelArgBuffer(Kernel* kernel, uint32_t index, size_t size, const void* data) {
	if(DATA.System.computeBackend.backend != COMPUTE_OPENCL) {
		TraceLog("[ESCLIB.AddKernelArgBuffer]: WARNING: OpenCL not inited");
		return -1;
	}
	if(!kernel || !kernel->kernel) {
		TraceLog("[ESCLIB.AddKernelArgBuffer]: ERROR: invalid kernel");
		return -2;
	}

	if(index >= kernel->argCount) {
		uint32_t newCount = index + 1;
		cl_mem* newBufs = realloc(kernel->argBufs, newCount * sizeof(cl_mem));
		size_t* newSizes = realloc(kernel->argSizes, newCount * sizeof(size_t));
		if(!newBufs || !newSizes) {
			TraceLog("[ESCLIB.AddKernelArgBuffer]: ERROR: realloc failed");
			free(newBufs); free(newSizes);
			return -4;
		}
		for(uint32_t i = kernel->argCount; i < newCount; i++) {
			newBufs[i] = NULL;
			newSizes[i] = 0;
		}
		kernel->argBufs = newBufs;
		kernel->argSizes = newSizes;
		kernel->argCount = newCount;
	}

	if(kernel->argBufs[index] && kernel->argSizes[index] == size) {
		cl_int erri = DATA.OpenCl.clEnqueueWriteBuffer(DATA.OpenCl.queue, kernel->argBufs[index], CL_TRUE, 0, size, data, 0, NULL, NULL);
		if(erri != CL_SUCCESS) {
			TraceLog("[ESCLIB.AddKernelArgBuffer]: ERROR: clEnqueueWriteBuffer failed: %d", erri);
			return -3;
		}
		return 0;
	}

	if(kernel->argBufs[index]) {
		DATA.OpenCl.clReleaseMemObject(kernel->argBufs[index]);
		kernel->argBufs[index] = NULL;
	}
	cl_int erri;
	cl_mem buf = DATA.OpenCl.clCreateBuffer(DATA.OpenCl.context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, size, (void*)data, &erri);
	if(erri != CL_SUCCESS) {
		TraceLog("[ESCLIB.AddKernelArgBuffer]: ERROR: clCreateBuffer failed: %d", erri);
		return -3;
	}
	erri = DATA.OpenCl.clSetKernelArg(kernel->kernel, index, sizeof(cl_mem), &buf);
	if(erri != CL_SUCCESS) {
		DATA.OpenCl.clReleaseMemObject(buf);
		TraceLog("[ESCLIB.AddKernelArgBuffer]: ERROR: clSetKernelArg (buf) failed: %d", erri);
		return -3;
	}
	kernel->argBufs[index] = buf;
	kernel->argSizes[index] = size;
	return 0;
}

int ReadKernelArg(Kernel* kernel, uint32_t index, size_t size, void* out) {
	if(index >= kernel->argCount || !kernel->argBufs[index]) {
		TraceLog("[ESCLIB.ReadKernelArg]: ERROR: no buffer at index %u", index);
		return -1;
	}
	cl_int erri = DATA.OpenCl.clEnqueueReadBuffer(DATA.OpenCl.queue, kernel->argBufs[index], CL_TRUE, 0, size, out, 0, NULL, NULL);
	return erri == CL_SUCCESS ? 0 : -2;
}

int RunKernel(Kernel* kernel, size_t instances, size_t workgroupSize) {
	if(DATA.System.computeBackend.backend != COMPUTE_OPENCL) {
		TraceLog("[ESCLIB.RunKernel]: WARNING: OpenCL not inited, returninig NULL");
		return -1;
	}

	if(!kernel || !kernel->kernel) {
		TraceLog("[ESCLIB.RunKernel]: ERROR: invalid kernel");
		return -2;
	}

	size_t global = instances;
	size_t* local = (workgroupSize > 0) ? &workgroupSize : NULL;

	cl_int erri = DATA.OpenCl.clEnqueueNDRangeKernel(DATA.OpenCl.queue, kernel->kernel, 1, NULL, &global, local, 0, NULL, NULL);
	if(erri != CL_SUCCESS) {
		TraceLog("[ESCLIB.RunKernel]: ERROR: clEnqueueNDRangeKernel failed: %d", erri);
		return -3;
	}

	return 0;
}

int WaitForKernel(void) {
	if(DATA.System.computeBackend.backend != COMPUTE_OPENCL) {
		TraceLog("[ESCLIB.WaitForKernel]: WARNING: OpenCL not inited, returninig NULL");
		return -1;
	}

	cl_int erri = DATA.OpenCl.clFinish(DATA.OpenCl.queue);

	if(erri != CL_SUCCESS) {
		TraceLog("[ESCLIB.WaitForKernel]: ERROR: clFinish failed: %d", erri);
		return -2;
	}

	return 0;
}

void DestroyKernel(Kernel* kernel) {
	if(!kernel) { return; }

	if(DATA.System.computeBackend.backend == COMPUTE_OPENCL) {
		for(uint32_t i = 0; i < kernel->argCount; i++) {
			if(kernel->argBufs[i]) {
				DATA.OpenCl.clReleaseMemObject(kernel->argBufs[i]);
			}
		}
		free(kernel->argBufs);
		free(kernel->argSizes);
		kernel->argBufs = NULL;
		kernel->argSizes = NULL;
		kernel->argCount = 0;

		if(kernel->kernel) {
			DATA.OpenCl.clReleaseKernel(kernel->kernel);
			kernel->kernel = NULL;
		}
		if(kernel->program) {
			DATA.OpenCl.clReleaseProgram(kernel->program);
			kernel->program = NULL;
		}
	}

	free(kernel);
}
