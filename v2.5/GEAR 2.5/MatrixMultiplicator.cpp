//#include "MatrixMultiplicator.h"
//#include <oclUtils.h>
//#include <CL/opencl.h>
//#include "Logger.h"
//#include "FileResource.h"
//
//#define WA 4
//#define HA 4
//#define WB 4
//#define HB 4
//#define WC 4
//#define HC 4
//
//const char * getErrorString(cl_int err){
//    switch(err){
//        case 0: return "CL_SUCCESS";
//        case -1: return "CL_DEVICE_NOT_FOUND";
//        case -2: return "CL_DEVICE_NOT_AVAILABLE";
//        case -3: return "CL_COMPILER_NOT_AVAILABLE";
//        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
//        case -5: return "CL_OUT_OF_RESOURCES";
//        case -6: return "CL_OUT_OF_HOST_MEMORY";
//        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
//        case -8: return "CL_MEM_COPY_OVERLAP";
//        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
//        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
//        case -11: return "CL_BUILD_PROGRAM_FAILURE";
//        case -12: return "CL_MAP_FAILURE";
//
//        case -30: return "CL_INVALID_VALUE";
//        case -31: return "CL_INVALID_DEVICE_TYPE";
//        case -32: return "CL_INVALID_PLATFORM";
//        case -33: return "CL_INVALID_DEVICE";
//        case -34: return "CL_INVALID_CONTEXT";
//        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
//        case -36: return "CL_INVALID_COMMAND_QUEUE";
//        case -37: return "CL_INVALID_HOST_PTR";
//        case -38: return "CL_INVALID_MEM_OBJECT";
//        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
//        case -40: return "CL_INVALID_IMAGE_SIZE";
//        case -41: return "CL_INVALID_SAMPLER";
//        case -42: return "CL_INVALID_BINARY";
//        case -43: return "CL_INVALID_BUILD_OPTIONS";
//        case -44: return "CL_INVALID_PROGRAM";
//        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
//        case -46: return "CL_INVALID_KERNEL_NAME";
//        case -47: return "CL_INVALID_KERNEL_DEFINITION";
//        case -48: return "CL_INVALID_KERNEL";
//        case -49: return "CL_INVALID_ARG_INDEX";
//        case -50: return "CL_INVALID_ARG_VALUE";
//        case -51: return "CL_INVALID_ARG_SIZE";
//        case -52: return "CL_INVALID_KERNEL_ARGS";
//        case -53: return "CL_INVALID_WORK_DIMENSION";
//        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
//        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
//        case -56: return "CL_INVALID_GLOBAL_OFFSET";
//        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
//        case -58: return "CL_INVALID_EVENT";
//        case -59: return "CL_INVALID_OPERATION";
//        case -60: return "CL_INVALID_GL_OBJECT";
//        case -61: return "CL_INVALID_BUFFER_SIZE";
//        case -62: return "CL_INVALID_MIP_LEVEL";
//        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
//        default: return "Unknown OpenCL error";
//    }
//}
//
//using namespace G2;
//
//MatrixMultiplicator::MatrixMultiplicator() 
//{
// 
//   // 5. Initialize OpenCL
//   // OpenCL specific variables
// 
//   /*****************************************/
//   /* Initialize OpenCL */
//   /*****************************************/
//
//   // query the number of platforms
//   cl_uint numPlatforms;
//   errcode = clGetPlatformIDs(0, NULL, &numPlatforms);
//	logger << "State: " << getErrorString(errcode) << endl;
//
//   // now get all the platform IDs
//   cl_platform_id* platforms = new cl_platform_id[numPlatforms];
//   errcode = clGetPlatformIDs(numPlatforms, platforms, NULL);
//	logger << "State: " << getErrorString(errcode) << endl;
//
//   // set platform property - we just pick the first one
//   cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (int) platforms[0], 0};
//
//   clGPUContext = clCreateContextFromType(properties, 
//                   CL_DEVICE_TYPE_GPU, 
//                   NULL, NULL, &errcode);
//   //shrCheckError(errcode, CL_SUCCESS);
//	logger << "State: " << getErrorString(errcode) << endl;
// 
//   // get the list of GPU devices associated 
//   // with context
//   errcode = clGetContextInfo(clGPUContext, 
//              CL_CONTEXT_DEVICES, 0, NULL, 
//              &dataBytes);
//   cl_device_id *clDevices = (cl_device_id *)
//              malloc(dataBytes);
//   errcode |= clGetContextInfo(clGPUContext, 
//              CL_CONTEXT_DEVICES, dataBytes, 
//              clDevices, NULL);
//   //shrCheckError(errcode, CL_SUCCESS);
//	logger << "State: " << getErrorString(errcode) << endl;
// 
//   //Create a command-queue
//   clCommandQue = clCreateCommandQueue(clGPUContext, 
//              clDevices[0], 0, &errcode);
//   //shrCheckError(errcode, CL_SUCCESS);
//	logger << "State: " << getErrorString(errcode) << endl;
// 
//   // Setup device memory
//   d_C = clCreateBuffer(clGPUContext, 
//          CL_MEM_READ_WRITE, 
//          sizeof(float) * 16, NULL, &errcode);
//   d_A = clCreateBuffer(clGPUContext, 
//          CL_MEM_READ_WRITE, 
//          sizeof(float) * 16, NULL, &errcode);
//   d_B = clCreateBuffer(clGPUContext, 
//          CL_MEM_READ_WRITE, 
//          sizeof(float) * 16, NULL, &errcode);
// 
//   // 6. Load and build OpenCL kernel
//
//   FileResource file("../../Resources/multkernel.cl");
//
//   file.load();
//   std::string kernelContent = file.getContent();
//   
//   char const*clMatrixMul = kernelContent.c_str();
//   kernelLength = kernelContent.length();
//	/*
//   char *clMatrixMul = oclLoadProgSource("../../Resources/multkernel.cl",
//                        "// My comment\n", 
//                        &kernelLength);
//	*/
//   //shrCheckError(clMatrixMul != NULL, shrTRUE);
// 
//   clProgram = clCreateProgramWithSource(clGPUContext, 
//                1, (const char **)&clMatrixMul, 
//                &kernelLength, &errcode); // TAKES LONG!
//   //shrCheckError(errcode, CL_SUCCESS);
// 
//   errcode = clBuildProgram(clProgram, 0, 
//              NULL, NULL, NULL, NULL);
//   //shrCheckError(errcode, CL_SUCCESS);
//	logger << "State: " << getErrorString(errcode) << endl;
// 
//   clKernel = clCreateKernel(clProgram, 
//               "matrixMul", &errcode);
//   //shrCheckError(errcode, CL_SUCCESS);
//	logger << "State: " << getErrorString(errcode) << endl;
// 
//   // 7. Launch OpenCL kernel
//   wA = 4;
//   wC = 4;
//   errcode = clSetKernelArg(clKernel, 0, 
//              sizeof(cl_mem), (void *)&d_C);
//   errcode |= clSetKernelArg(clKernel, 1, 
//              sizeof(cl_mem), (void *)&d_A);
//   errcode |= clSetKernelArg(clKernel, 2, 
//              sizeof(cl_mem), (void *)&d_B);
//   errcode |= clSetKernelArg(clKernel, 3, 
//              sizeof(int), (void *)&wA);
//   errcode |= clSetKernelArg(clKernel, 4, 
//              sizeof(int), (void *)&wC);
//   //shrCheckError(errcode, CL_SUCCESS);
// 
//   localWorkSize[0] = 4;
//   localWorkSize[1] = 4;
//   globalWorkSize[0] = 4;
//   globalWorkSize[1] = 4;
// 
//   // run kernel in 2-dimensional worker mode
//   errcode = clEnqueueNDRangeKernel(clCommandQue, 
//              clKernel, 2, NULL, globalWorkSize, 
//              localWorkSize, 0, NULL, NULL);
//   //shrCheckError(errcode, CL_SUCCESS);
// 
//   // 10. clean up memory
//}
//
//void
//MatrixMultiplicator::mult(glm::mat4 const& m1, glm::mat4 const& m2, glm::mat4& m3) 
//{
//	errcode = clEnqueueWriteBuffer(clCommandQue, d_C, false, 0, sizeof(float) * 16, glm::value_ptr(m3), 0, NULL, NULL);
//	errcode = clEnqueueWriteBuffer(clCommandQue, d_B, false, 0, sizeof(float) * 16, glm::value_ptr(m1), 0, NULL, NULL);
//	errcode = clEnqueueWriteBuffer(clCommandQue, d_A, false, 0, sizeof(float) * 16, glm::value_ptr(m2), 0, NULL, NULL);
//	//logger << "State: " << getErrorString(errcode) << endl;
//
//	// run kernel
//	clEnqueueNDRangeKernel(clCommandQue, 
//              clKernel, 2, NULL, globalWorkSize, 
//              localWorkSize, 0, NULL, NULL);
//
// 
//   // 8. Retrieve result from device
//   //errcode = clEnqueueReadBuffer(clCommandQue, 
//   //           d_C, CL_TRUE, 0, sizeof(float) * 16, 
//   //           glm::value_ptr(m3), 0, NULL, NULL);
//   //shrCheckError(errcode, CL_SUCCESS);
//}