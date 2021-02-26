#pragma once

#define MULTI_WALK

#include <unistd.h>

#include<pzcl/pzcl_ocl_wrapper.h>
#include<PZSDKHelper.h>
#include "./pezy/class_device.hpp"
#include "./pezy/cl_pointer.h"
#include "parameter.hpp"

const int N_WALK_LIMIT = 256;
const int N_THREAD_MAX = 15872;
const int NI_LIMIT = N_THREAD_MAX * N_WALK_LIMIT;
const int NJ_LIMIT = 8 * NI_LIMIT;

clPointer<int>    j_disp_dev;
clPointer<EpDev>  epi_dev;
clPointer<EpDev>  epj_dev;

clPointer<int>      id_epj_dev;
clPointer<ForceDev> force_dev;

cl_platform_id platform_id = NULL;
cl_uint ret_num_platforms;
cl_device_id device_id[NPEZY];
cl_uint ret_num_devices;
cl_context context = NULL;
cl_command_queue command_queue = NULL;
cl_program program = NULL;
cl_kernel kernel = NULL;
cl_kernel kernel_clus = NULL;
std::vector<cl_device_id> device_id_lists;
cl_event event_list[10];

///////////////////////////////////////////////////////////
static bool LoadFile( const char* name, size_t size,  char* pData )
{
  FILE* fp = fopen( name, "rb");
  if( fp == NULL ){
    std::cerr << "can not open" << name << "in LoadFile at " << getHostnameAndDeviceIDInString() << std::endl;
    return false;
  }

  if( size == 0 || pData == NULL){
    std::cerr << "invalid params " << __FUNCTION__ << " at " << getHostnameAndDeviceIDInString() << std::endl;
    return false;
  }
  size_t size_ret = fread(pData, sizeof(char), size, fp);
  fclose(fp);

  if( size_ret != size ){
    std::cerr << "can not read requested size at " << getHostnameAndDeviceIDInString() << std::endl;
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////
static size_t GetFileSize(const char* name)
{
  FILE* fp = fopen( name, "rb");
  if( fp == NULL ){
    std::cerr << "can not open " << name << " in GetFileSize at " << getHostnameAndDeviceIDInString() << std::endl;
    return 0;
  }
  fseek(fp, 0, SEEK_END);

  size_t size = ftell(fp);
  fclose(fp);

  return size;
}

/**
 * Crate program object
 */
cl_program CreateProgram(cl_context context,
			 std::vector<cl_device_id> &device_id_lists,
			 const char* bin_name
			 ){
  cl_program program = NULL;
  char* pBin = NULL;
  cl_int result;

  size_t sizeFile = GetFileSize( bin_name );
  if( sizeFile == 0 ){
    goto leaving;
  }

  PZSDK_ALIGNED_ALLOC(pBin, sizeFile, 8 /*8 byte alignment*/);
  if( pBin == NULL){
    std::cerr << "out of host memory\n" << std::endl;
    goto leaving;
  }

  if( !LoadFile(bin_name, sizeFile, pBin)){
    goto leaving;
  }

  {
    const unsigned char* listBin[1];
    listBin[0] = (unsigned char*)pBin;
    cl_int binary_status = CL_SUCCESS;
    size_t length = sizeFile;

    program = clCreateProgramWithBinary( context, (cl_uint)device_id_lists.size(), &device_id_lists[0], &length, listBin, &binary_status, &result);
  }
  if(program == NULL){
    std::cerr << "error: clCreateProgramWithBinary failed at " << getHostnameAndDeviceIDInString() << ", " << result << std::endl;
    goto leaving;
  }
  if(result != CL_SUCCESS){
    std::cerr << "error: failed to create program with binary (" << result << ") at " << getHostnameAndDeviceIDInString() << std::endl;
    exit(-1);
  }
 leaving:
  if(pBin)
    {
      PZSDK_ALIGNED_FREE(pBin);
    }
  return program;
}

void InitializeDEVICE(){
    const PS::S32 my_rank = PS::Comm::getRank();
    //std::cout<<"my_rank="<<my_rank<<std::endl;
    cl_int ret;
    clSafeCall(ret,clGetPlatformIDs(1, &platform_id, &ret_num_platforms));
    //std::cerr<<"rank " << my_rank << ": " << "platform_id="<<platform_id<<" ret_num_platforms="<<ret_num_platforms<<" ret="<<ret<<std::endl;

    clSafeCall(ret,clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, NPEZY, device_id, &ret_num_devices));
    //std::cerr<<"rank " << my_rank << ": " << "device_id[" << my_rank%NPEZY << "]=" << device_id[my_rank%NPEZY] <<" ret_num_devices="<<ret_num_devices<<" ret="<<ret<<std::endl;

    //context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    context = clCreateContext(NULL, 1, &device_id[my_rank%NPEZY], NULL, NULL, &ret);
    if(ret != CL_SUCCESS){
      std::cerr << "error: failed to create context(" << ret << ") at " << getHostnameAndDeviceIDInString() << std::endl;
      exit(-1);
    }

    command_queue = clCreateCommandQueue(context, device_id[my_rank%NPEZY], 0, &ret);
    if(ret != CL_SUCCESS){
      std::cerr << "error: failed to create command queue(" << ret << ") at " << getHostnameAndDeviceIDInString() << std::endl;
      exit(-1);
    }
    clSafeCall(ret, j_disp_dev.allocate(context,N_WALK_LIMIT+2));
    clSafeCall(ret, epi_dev  .allocate(context,NI_LIMIT));
    clSafeCall(ret, epj_dev  .allocate(context,NJ_LIMIT));
    clSafeCall(ret, id_epj_dev.allocate(context,NJ_LIMIT));
    clSafeCall(ret, force_dev .allocate(context,NI_LIMIT));

    //device_id_lists.push_back(device_id);
    device_id_lists.push_back(device_id[my_rank%NPEZY]);
    program = CreateProgram(context, device_id_lists, "./pezy/kernel.sc32/kernel.pz");
    if(program == NULL){
      std::cerr<<"can't create program at "<< getHostnameAndDeviceIDInString() <<std::endl;
      exit(-1);
    }
    kernel = clCreateKernel(program, "ForceKernelIndex", &ret);

    if(kernel == NULL || ret != CL_SUCCESS){
      std::cerr<<"can't create kernel (" << ret << ") at " << getHostnameAndDeviceIDInString() <<std::endl;
      exit(-1);
    }

    // to use local memory
    typedef CL_API_ENTRY cl_int (CL_API_CALL *pfnPezyExtSetPerThreadStackSize) (pzcl_kernel kernel, size_t size);
    pfnPezyExtSetPerThreadStackSize clExtSetPerThreadStackSize = (pfnPezyExtSetPerThreadStackSize)clGetExtensionFunctionAddress("pezy_set_per_thread_stack_size");
    assert(clExtSetPerThreadStackSize);
    size_t per_thread_stack = 0x0400;
    clSafeCall(ret,clExtSetPerThreadStackSize(kernel, per_thread_stack));
}

PS::F64 WTIME_SET_OFFSET = 0.0;
PS::F64 WTIME_SET = 0.0;

PS::S32 DispatchKernelIndex(const PS::S32 tag,
			    const PS::S32 n_walk,
			    const EPI *epi[],
			    const PS::S32 n_epi[],
			    const int    *id_epj[],
			    const PS::S32 n_epj[],
			    const EPJ *epj,
			    const PS::S32 n_epj_tot,
			    const bool    send_flag
			    ){
  //assert(n_walk <= N_WALK_LIMIT);
  cl_int ret = CL_SUCCESS;
  static int nevent = 0;
  if(send_flag){
    assert(0 < n_epj_tot && n_epj_tot < NJ_LIMIT);
    for(int j=0; j<n_epj_tot; j++){
      epj_dev[j].x    = epj[j].pos.x;
      epj_dev[j].y    = epj[j].pos.y;
      epj_dev[j].z    = epj[j].pos.z;
      epj_dev[j].vx   = epj[j].vel.x;
      epj_dev[j].vy   = epj[j].vel.y;
      epj_dev[j].vz   = epj[j].vel.z;
      epj_dev[j].walk = epj[j].type;
    }
    epj_dev.htod_async(command_queue,&event_list[nevent++],n_epj_tot);
    return 0;
  }

  j_disp_dev[0] = 0;
  for(int k=0; k<n_walk; k++){
    j_disp_dev[k+1] = j_disp_dev[k] + n_epj[k];
  }
  j_disp_dev[n_walk+1] = j_disp_dev[n_walk];
  assert(j_disp_dev[n_walk] < NJ_LIMIT);
  j_disp_dev.htod_async(command_queue,&event_list[nevent++],n_walk+2);

  PS::U32 ni_tot = 0;
  for(int iw=0; iw<n_walk; iw++){
    for(int i=0; i<n_epi[iw]; i++){
      epi_dev[ni_tot].x     = epi[iw][i].pos.x;
      epi_dev[ni_tot].y     = epi[iw][i].pos.y;
      epi_dev[ni_tot].z     = epi[iw][i].pos.z;
      epi_dev[ni_tot].vx    = epi[iw][i].vel.x;
      epi_dev[ni_tot].vy    = epi[iw][i].vel.y;
      epi_dev[ni_tot].vz    = epi[iw][i].vel.z;
      epi_dev[ni_tot].walk  =(epi[iw][i].type << 30) | (iw & 0x3fffffff); // msb+1 bit is type
      //std::cerr << std::hex << epi_dev[ni_tot].walk << " " << iw << " " << epi[iw][i].type << std::endl;
      ni_tot++;
    }
  }
  assert(ni_tot < NI_LIMIT);
  epi_dev.htod_async(command_queue,&event_list[nevent++],ni_tot);

  int nj_tot = 0;
  for(int iw=0; iw<n_walk; iw++){
    for(int j=0; j<n_epj[iw]; j++){
      id_epj_dev[nj_tot++] = id_epj[iw][j];
    }
  }
  assert(nj_tot < NJ_LIMIT);
  id_epj_dev.htod_async(command_queue,&event_list[nevent++],nj_tot);

  const float dt        = CONFIG.dt;
  const float temp      = CONFIG.temp;
  const float temp_rand = CONFIG.temp_rand;
  clSafeCall(ret,clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)j_disp_dev));
  clSafeCall(ret,clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)epi_dev));
  clSafeCall(ret,clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)epj_dev));
  clSafeCall(ret,clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)id_epj_dev));
  clSafeCall(ret,clSetKernelArg(kernel, 4, sizeof(cl_mem), (void*)force_dev));
  clSafeCall(ret,clSetKernelArg(kernel, 5, sizeof(float),  (void*)&dt));
  clSafeCall(ret,clSetKernelArg(kernel, 6, sizeof(float),  (void*)&temp));
  clSafeCall(ret,clSetKernelArg(kernel, 7, sizeof(float),  (void*)&temp_rand));
  clSafeCall(ret,clSetKernelArg(kernel, 8, sizeof(int),    (void*)&ni_tot));

  clWaitForEvents(nevent,event_list);
  nevent = 0;

  size_t work_size = N_THREAD_MAX;
  clSafeCall(ret,clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &work_size, NULL, 0, NULL, &event_list[0]));
  force_dev.dtoh_async_wait(command_queue,1,event_list,&event_list[1],ni_tot);
  return 0;
}

PS::S32 RetrieveKernel(const PS::S32 tag,
                       const PS::S32 n_walk,
                       const PS::S32 ni[],
                       Force        *force[]
		       ){
  clWaitForEvents(2,event_list);
  int ni_tot = 0;
  for(int k=0; k<n_walk; k++){
    ni_tot += ni[k];
  }
  int n_cnt = 0;
  for(int iw=0; iw<n_walk; iw++){
    for(int i=0; i<ni[iw]; i++){
      force[iw][i].force.x = force_dev[n_cnt].x;
      force[iw][i].force.y = force_dev[n_cnt].y;
      force[iw][i].force.z = force_dev[n_cnt].z;
      force[iw][i].pot     = force_dev[n_cnt].u;
      n_cnt++;
    }
  }
  return 0;
}
