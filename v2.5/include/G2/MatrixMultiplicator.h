//#pragma once
//#include <oclUtils.h>
//#include <CL/opencl.h>
//
//#include <glm/glm.hpp>
//
//namespace G2 
//{
//	/** This class defines...
//	 * @created:	2014/02/20
//	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
//	 */
//	class MatrixMultiplicator 
//	{
//		public:
//			/** This constructs a new MatrixMultiplicator.
//			 */
//			MatrixMultiplicator();
//
//			void mult(glm::mat4 const& m1, glm::mat4 const& m2, glm::mat4& m3);
//
//		protected:
//		private:
//		   cl_context clGPUContext;
//		   cl_command_queue clCommandQue;
//		   cl_program clProgram;
//		   cl_kernel clKernel;
// 
//		   size_t dataBytes;
//		   size_t kernelLength;
//		   cl_int errcode;
// 
//		   // OpenCL device memory for matrices
//		   cl_mem d_A;
//		   cl_mem d_B;
//		   cl_mem d_C;
//		   size_t localWorkSize[2], globalWorkSize[2];
//			int wA;
//			int wC;
//	};
//};