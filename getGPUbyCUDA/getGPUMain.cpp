//#include <stdio.h>
//#include <string.h>
//#include <sstream>
//#include <unistd.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <time.h>
//#include <dlfcn.h>
#include <nvml.h>
#include <iostream>
//#define CUDAAPI
//#define LOAD_FUNC(1,s) dlsym(1,s)
//#define DL_CLOSE_FUNC(1) dlclose(1)
/*
typedef enum nvmlReturn_enum 
{
	NVML_SUCCESS = 0,
	NVML_ERROR_UNINITIALIZED = 1,
	NVML_ERROR_INVALID_ARGUMENT = 2
}nvmlReturn_t;
*/
//typedef void *nvmlDevice_t;
/*
typedef struct nvmlMemory_st
{
	unsigned long long total;
	unsigned long long free;
	unsigned long long used;
}nvmlMemory_t;
*/
/*
typedef struct nvmlUtilization_st
{
	unsigned int gpu;
	unsigned int memory;
}nvmlUtilization_t;
*/

/*
typedef nvmlReturn_t(CUDAAPI *NVMLINIT)(void);
typedef nvmlReturn_t(CUDAAPI *NVMLSHUTDOWN)(void);
typedef nvmlReturn_t(CUDAAPI *NVMLDEVICEGETCOUNT)(unsigned int *deviceCount);
typedef nvmlReturn_t(CUDAAPI *NVMLDEVICEGETHANDLEBYINDEX)(unsigned int index,nvmlDevice_t *device);
typedef nvmlReturn_t(CUDAAPI *NVMLDEVICETMEMORYINFO)(nvmlDevice_t device,nvmlMemory_t *memory);
typedef nvmlReturn_t(CUDAAPI *NVMLDEVICEGETUTILIZATIONRATES)(nvmlDevice_t device,nvmlUtilization_t *utilization);
typedef nvmlReturn_t(CUDAAPI *NVMLDEVICEGETTEMPERATURE)(nvmlDevice_t device,int sensorType,unsigned int *temp);
*/

//#define GPU_MAX_SIZE 128
#define RETURN_SUCCESS 0
//#define RETURN_ERROR_LOAD_LIB (-1)
//#define RETURN_ERROR_LOAD_FUNC (-2)
//#define RETURN_ERROR_LIB_FUNC (-3)
//#define RETURN_ERROR_NULL_POINTER (-4)

//#define CHECK_LOAD_NVML_FUNC(t,f,s)
static int check_nvml_error(int err,const char *func)
{
	if(err != NVML_SUCCESS){
		printf("%s -failed with error code:%d\n", func, err);
		return 0;
	}
	return 1;
}

//#define check_nvml_errors(f)

void getdata()
{
//	int retCode = RETURN_SUCCESS;
//	void* nvml_lib;
//	NVMLINIT nvml_init;
//	NVMLSHUTDOWN nvml_shutdown;
//	NVMLDEVICEGETCOUNT nvml_device_get_count;
//	NVMLDEVICEGETHANDLEBYINDEX nvml_device_get_handle_by_index;
//	NVMLDEVICETMEMORYINFO nvml_device_get_memory_info;
//	NVMLDEVICEGETUTILIZATIONRATES nvml_device_get_utilization_rates;
//	NVMLDEVICEGETTEMPERATURE nvml_device_get_temperature;

//	unsigned int utilization_value = 0;
//	unsigned int utilization_sample = 0;
//	int best_gpu = 0;
//	unsigned int decode_userd = 100;
/*	
	nvml_lib = NULL;
	nvml_lib = dlopen("libnvidia-ml.so",RTLD_NOW);
	if(nvml_lib == NULL){
		return;
	}
*/
	nvmlDevice_t device_handle;
	unsigned int device_count = 0;
	nvmlMemory_t memory_info;
	nvmlUtilization_t gpu_utilization;
	nvmlProcessInfo_t process_info;
	int i= 0;
	
	std::cout << "1111111111Enter the getdata()" << std::endl;
//	CHECK_LOAD_NVML_FUNC(NVMLINIT,nvml_init,"nvmlInit");
//	CHECK_LOAD_NVML_FUNC(NVMLSHUTDOWN,nvml_shutdown,"nvmlShutdown");
//	CHECK_LOAD_NVML_FUNC(NVMLDEVICEGETCOUNT,nvml_device_get_count,"nvmlDeviceGetCount");
//	CHECK_LOAD_NVML_FUNC(NVMLDEVICEGETHANDLEBYINDEX,nvml_device_get_handle_by_index,"nvmlDeviceGetHandleByIndex");
//	CHECK_LOAD_NVML_FUNC(NVMLDEVICEGETMEMORYINFO,nvml_device_get_memory_info,"nvmlDeviceGetMemoryInfo");
//	CHECK_LOAD_NVML_FUNC(NVMLDEVICEGETUTILIZATIONRATES,nvml_device_get_utilization_rates,"nvmlDeviceGetUtilizationRates");
//	CHECK_LOAD_NVML_FUNC(NVMLDEVICEGETTEMPERATURE,nvml_device_get_temperature,"nvmlDeviceGetTemperature");
	//check_nvml_error(nvml_init(),NULL);
	check_nvml_error(nvmlInit(),NULL);
	//nvmlInit();
	std::cout << "22222222222Enter the getdata()" << std::endl;
	//check_nvml_error(nvml_device_get_count(&device_count),NULL);
	check_nvml_error(nvmlDeviceGetCount(&device_count),NULL);

	std::cout << "Enter the getdata()" << std::endl;
	for(i=0;i<device_count;i++){
		//check_nvml_error(nvml_device_get_handle_by_index(i, &device_handle),NULL);
		check_nvml_error(nvmlDeviceGetHandleByIndex(i, &device_handle),NULL);
		//check_nvml_error(nvml_device_get_memory_info(device_handle,&memory_info),NULL);
		check_nvml_error(nvmlDeviceGetMemoryInfo(device_handle,&memory_info),NULL);
		//check_nvml_error(nvml_device_get_utilization_rates(device_handle,&gpu_utilization),NULL);
		check_nvml_error(nvmlDeviceGetUtilizationRates(device_handle,&gpu_utilization),NULL);
		//check_nvml_error(nvmlDeviceGetComputeRunningProcesses(device_handle,&process_info),NULL);
		printf("GPU\t,Utilization:[gpu:%u, memory:%u], Memory:[total:%llu, free:%llu, used:%llu]\n", gpu_utilization.gpu,gpu_utilization.memory,memory_info.total/1024/1024,memory_info.free/1024/1024,memory_info.used/1024/1024);
	}
}	

int main(int argc, char **argv)
{
	std::cout<< "Start from main()"<< std::endl;
	getdata();
	return 0;
}	

		
