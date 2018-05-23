#include <stdio.h>
#include <assert.h>
//#include <omp.h>
#include <inttypes.h>
#include "utils.h"
//#include <OpenCL/OpenCL.h>
#include <CL/cl.h>

#define MAXGPU 1
#define MAXN 16777216
#define GPULOCAL 256

char clSrc[1024] = "";
char k_name[1024] = "vecdot";
uint32_t hostC[MAXN/GPULOCAL],keyA,keyB;
int N;
cl_context              clCtx;
cl_program              clPrg;
cl_kernel               clKrn;
cl_command_queue        clQue;
cl_mem                  clMemOut;

void destroyGPU(cl_context *clCtx,cl_program *clPrg, cl_kernel *clKrn, cl_command_queue *clQue, cl_mem *clMemOut  ) {
    fprintf(stderr, "Starting Cleanup ...\n\n");
    if (*clMemOut)    clReleaseMemObject(*clMemOut);
    if (*clKrn) clReleaseKernel(*clKrn);
    if (*clPrg) clReleaseProgram(*clPrg);
    if (*clQue) clReleaseCommandQueue(*clQue);
    if (*clCtx) clReleaseContext(*clCtx);
    exit(0);
}

int main( int argc, char *argv[]){

    //openfile
    FILE *f = fopen("vecdot.cl","r");
    assert(f != NULL);
    size_t len_src = fread(clSrc,1,1024,f); //length of source
    fclose(f);

    //init platform => device =>context => command => program =>kernel
    cl_platform_id plat_id;
    cl_int Status;
    cl_uint plat_num,gpu_num;
    cl_device_id gpuid[MAXGPU];
    const char *pointer_src = clSrc; //used to create program
    clGetPlatformIDs(1,&plat_id,&plat_num);
    clGetDeviceIDs(plat_id,CL_DEVICE_TYPE_GPU,MAXGPU,gpuid,&gpu_num);
    clCtx = clCreateContext(NULL, 1, gpuid, NULL, NULL, &Status);
    if ( Status != CL_SUCCESS) { 
        fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
        destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
    }
    clQue = clCreateCommandQueue(clCtx, gpuid[0], 0, &Status);
    if ( Status != CL_SUCCESS) { 
        fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
        destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
    }
    clPrg = clCreateProgramWithSource(clCtx, 1, &pointer_src, &len_src, &Status);
    if ( Status != CL_SUCCESS) { 
        fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
        destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
    }
    Status = clBuildProgram( clPrg, 1, gpuid, NULL, NULL, NULL);
    if (Status != CL_SUCCESS) {
        fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
        size_t log_size;
        clGetProgramBuildInfo( clPrg, gpuid[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *program_log = (char *) calloc(log_size+1, sizeof(char));
        clGetProgramBuildInfo( clPrg, gpuid[0], CL_PROGRAM_BUILD_LOG, log_size+1, program_log, NULL);
        printf("%s", program_log);
        free(program_log);
        destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut);
    }
    //kernel
    clKrn = clCreateKernel( clPrg, k_name, &Status);
    if ( Status != CL_SUCCESS) { 
        fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
        destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
    }

    //buffer
    clMemOut = clCreateBuffer( clCtx, CL_MEM_WRITE_ONLY, sizeof(uint32_t)*MAXN/GPULOCAL, hostC, &Status);
    if ( Status != CL_SUCCESS) { 
        fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
        destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
    }




    //read input
    while( scanf("%d %u %u", &N, &keyA, &keyB) == 3){
        uint32_t padding = 0;
        
        size_t globalOffset[] = {0}; //set all to 0
        size_t globalSize[] = {((N/GPULOCAL) + 1)*GPULOCAL};  //set all  to  N
        size_t localSize[] = {GPULOCAL}; //set all to 256
        //set A to kernel
        Status = clSetKernelArg(clKrn, 0, sizeof(cl_uint), (void *) &keyA);
        if ( Status != CL_SUCCESS) { 
            fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
            destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
        }
        //B
        Status = clSetKernelArg(clKrn, 1, sizeof(cl_uint), (void *) &keyB);
        if ( Status != CL_SUCCESS) { 
            fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
            destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
        }
        //ans
        Status = clSetKernelArg(clKrn, 2, sizeof(cl_mem), (void *) &clMemOut);
        if ( Status != CL_SUCCESS) { 
            fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
            destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
        }
        // run
        Status = clEnqueueNDRangeKernel(clQue, clKrn, 1, globalOffset,globalSize, NULL, 0, NULL, NULL);
        if ( Status != CL_SUCCESS) { 
            fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
            destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
        }
        
        // -- read back
        clEnqueueReadBuffer(clQue, clMemOut, CL_TRUE, 0, sizeof(uint32_t)*N/GPULOCAL, hostC, 0, NULL, NULL);
        //uint32_t padding = 0;
        uint32_t sum = 0;
        for (int i = 0; i < N/GPULOCAL; i++)
            sum += hostC[i];
        while( N%GPULOCAL ){
            padding += encrypt( N-1,keyA)*encrypt(N-1,keyB);
            N--;
        } 
        sum += padding;
        printf("%u\n", sum ); //need to deal with padding
    }
    destroyGPU(&clCtx, &clPrg, &clKrn, &clQue , &clMemOut); 
    return 0;























    return 0;
}