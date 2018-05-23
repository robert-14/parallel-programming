#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <CL/cl.h>
#define MAXGPU 1
#define MAXN 2048
int N = MAXN;
char clSrc[1024] = "";
char clSrcMain[1024] = "notused";

cl_context  context1;
cl_program  program1;

void destroyGPU(cl_context *c1,cl_program *p1) {
    fprintf(stderr, "Starting Cleanup ...\n\n");
    if (*c1) clReleaseContext(*c1);
    if (*p1) clReleaseProgram(*p1);
    exit(0);
}

int main() {
    char file[128];
    assert(scanf("%s", file) == 1);
    FILE *f = fopen(file,"r");
    assert(f != NULL);
    size_t len_src = fread(clSrc,1,1024,f); //length of source
    fclose(f);

    cl_platform_id plat_id;
    cl_int Stat;
    cl_uint plat_num,gpu_num;
    cl_device_id gpuid[MAXGPU];
    const char *pointer_src = clSrc;
    clGetPlatformIDs(1,&plat_id,&plat_num);
    clGetDeviceIDs(plat_id,CL_DEVICE_TYPE_GPU,MAXGPU,gpuid,&gpu_num);
    //create context
    context1 = clCreateContext(NULL,1,gpuid,NULL,NULL,&Stat);
    if ( Stat != CL_SUCCESS) { 
        fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
        destroyGPU(&context1, &program1); 
    }
    program1 = clCreateProgramWithSource(context1,1,&pointer_src,&len_src,&Stat);
    if ( Stat != CL_SUCCESS) { 
        fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
        destroyGPU(&context1, &program1); 
    }
    Stat = clBuildProgram( program1, 1, gpuid, NULL, NULL, NULL);
    if (Stat != CL_SUCCESS) {
        static char program_log[32767];
        clGetProgramBuildInfo( program1, gpuid[0],CL_PROGRAM_BUILD_LOG, sizeof(program_log), program_log, NULL);
        printf("%s", program_log);
        if ( CL_BUILD_PROGRAM_FAILURE != CL_SUCCESS) { 
            fprintf(stderr, "Error: Line %u in file %s\n\n", __LINE__, __FILE__);
            destroyGPU(&context1, &program1); 
        }
    }
    destroyGPU(&context1,&program1);
    return 0;
}