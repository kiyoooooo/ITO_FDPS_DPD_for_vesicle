#ifndef H_CL_POINTER
#define H_CL_POINTER

#include<pzcl/pzcl_ocl_wrapper.h>
#include<PZSDKHelper.h>

#include <cassert>
#include <string>

#define NPEZY 8

std::string getHostnameAndDeviceIDInString(){
  std::string ret;
  const PS::S32 rank   = PS::Comm::getRank();
  const PS::S32 id_dev = rank % NPEZY;
  char hostname[128];
  if(gethostname(hostname,sizeof(hostname))!=NULL){
    std::cerr << "failed to get hostname" << std::endl;
    ret = "rank" + std::to_string(rank) + ":" + std::to_string(id_dev);
  }else{
    ret = std::string(hostname);// + ":" + std::to_string(id_dev);
  }
  return ret;
}

#define clSafeCall(err,command)						\
  err = command;							\
  if(err != CL_SUCCESS){						\
    std::cerr <<"error: "#command" failed(" << err << ") at " << getHostnameAndDeviceIDInString() << std::endl; \
    exit(-1);								\
  }

template <class T>
class clPointer{
 public:
  cl_mem dev;
  T *host;
  size_t size;
  cl_int err;

  clPointer(){
    dev = NULL;
    host = nullptr;
    size = 0;
  }
  ~clPointer(){
    if(host != nullptr) delete[] host;
  }

  cl_int allocate(cl_context& context,size_t _size){
    size = _size;
    assert(host == nullptr && dev == NULL);
    host = new T[size];
    dev = clCreateBuffer(context, CL_MEM_READ_WRITE, size*sizeof(T), NULL, &err);
    if(err != CL_SUCCESS){
      fprintf(stderr,"error: failed to create device buffer(%d)\n",err);
      exit(-1);
    }
    return err;
  }
  void htod(cl_command_queue& command_queue){
    clSafeCall(err,clEnqueueWriteBuffer(command_queue, dev, CL_TRUE, 0, size*sizeof(T),  host, 0, NULL, NULL));
  }
  void htod(cl_command_queue& command_queue, size_t count, size_t offset=0){
    if(offset + count > size){
      fprintf(stderr,"offset(=%lu) + count(=%lu) > size(=%lu)\n",offset,count,size);
      exit(-1);
    }
    clSafeCall(err,clEnqueueWriteBuffer(command_queue, dev, CL_TRUE, offset*sizeof(T), count*sizeof(T),  host, 0, NULL, NULL));
  }
  void htod_async(cl_command_queue& command_queue,cl_event* event){
    clSafeCall(err,clEnqueueWriteBuffer(command_queue, dev, CL_FALSE, 0, size*sizeof(T),  host, 0, NULL, event));
  }
  void htod_async(cl_command_queue& command_queue,cl_event* event,  size_t count, size_t offset=0){
    if(offset + count > size){
      fprintf(stderr,"offset(=%lu) + count(=%lu) > size(=%lu)\n",offset,count,size);
      exit(-1);
    }
    clSafeCall(err,clEnqueueWriteBuffer(command_queue, dev, CL_FALSE, offset*sizeof(T), count*sizeof(T),  host, 0, NULL, event));
  }
  void dtoh(cl_command_queue& command_queue){
    clSafeCall(err,clEnqueueReadBuffer(command_queue, dev, CL_TRUE, 0, size*sizeof(T),  host, 0, NULL, NULL));
  }
  void dtoh(cl_command_queue& command_queue, size_t count, size_t offset = 0){
    assert(offset + count <= size);
    clSafeCall(err,clEnqueueReadBuffer(command_queue, dev, CL_TRUE, offset*sizeof(T), count*sizeof(T),  host, 0, NULL, NULL));
  }
  void dtoh_async(cl_command_queue& command_queue,cl_event* event){
    clSafeCall(err,clEnqueueReadBuffer(command_queue, dev, CL_FALSE, 0, size*sizeof(T),  host, 0, NULL, event));
  }
  void dtoh_async(cl_command_queue& command_queue, cl_event* event, size_t count, size_t offset = 0){
    assert(offset + count <= size);
    clSafeCall(err,clEnqueueReadBuffer(command_queue, dev, CL_FALSE, offset*sizeof(T), count*sizeof(T),  host, 0, NULL, event));
  }
  void dtoh_async_wait(cl_command_queue& command_queue,cl_uint nwait,const cl_event* event_list,cl_event* event, size_t count, size_t offset = 0){
    assert(offset + count <= size);
    clSafeCall(err,clEnqueueReadBuffer(command_queue, dev, CL_FALSE, offset*sizeof(T), count*sizeof(T),  host, nwait, event_list, event));
  }

        T &operator[](const int i)       { return host[i]; }
  const T &operator[](const int i) const { return host[i]; }
  operator cl_mem* (){return &dev;};
};

#endif
