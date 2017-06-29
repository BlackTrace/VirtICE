// Copyright (c) 2015-2016, tandasat. All rights reserved.
// Copyright (c) 2016-2017, KelvinChan. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/// @file
/// Declares interfaces to VMM functions.

#ifndef HYPERPLATFORM_VMM_H_
#define HYPERPLATFORM_VMM_H_

#include <fltKernel.h>

////////////////////////////////////////////////////////////////////////////////
//
// macro utilities
//

////////////////////////////////////////////////////////////////////////////////
//
// constants and macros
//
typedef enum {
	ProtectedMode = 0,
	VmxMode = 1,
	Virtual8086 = 2,
	RealMode = 3,
	SmmMode = 4,
}CPU_MODE;

typedef enum {
	RootMode = 0,
	GuestMode,
}VMX_MODE;

////////////////////////////////////////////////////////////////////////////////
//
// types
//

/// Represents VMM related data shared across all processors
struct SharedProcessorData {
	volatile long reference_count;  //!< Number of processors sharing this data
	void* msr_bitmap;               //!< Bitmap to activate MSR I/O VM-exit
	void* io_bitmap_a;              //!< Bitmap to activate IO VM-exit (~ 0x7FFF)
	void* io_bitmap_b;              //!< Bitmap to activate IO VM-exit (~ 0xffff)
};

typedef struct _VCPU_VMX
{
	ULONG64   vmxon_region;
	ULONG64   vmcs02_pa;				///VMCS02 , actual VMCS L1 will runs on
	ULONG64   vmcs12_pa;				///VMCS12 , for L1's VMREAD and VMWRITE, as a shadow VMCS
	ULONG64   vmcs01_pa;				///VMCS01 , Initial VMCS
	ULONG     InitialCpuNumber;				///VCPU number
	BOOLEAN   blockINITsignal;			///NOT USED
	BOOLEAN   blockAndDisableA20M;		///NOT USED
	VMX_MODE  inRoot;					///is it in root mode
	USHORT	  kVirtualProcessorId;		///NOT USED 
	ULONG_PTR   guest_irql;
	ULONG_PTR   guest_cr8;    
}VCPUVMX, *PVCPUVMX;

/// Represents VMM related data associated with each processor
struct ProcessorData {
	SharedProcessorData* shared_data;         //!< Shared data
	void* vmm_stack_limit;                    //!< A head of VA for VMM stack
	struct VmControlStructure* vmxon_region;  //!< VA of a VMXON region
	struct VmControlStructure* vmcs_region;   //!< VA of a VMCS region
	struct EptData* ept_data;                 //!< A pointer to EPT related data , for 0-1
	struct EptData* ept_data_12;              //!< A pointer to EPT related data , for 1-2
	struct EptData* ept_data_02;              //!< A pointer to EPT related data , for 0-2
	void* xsave_area;                         //!< VA to store state components
	ULONG64 xsave_inst_mask;                  //!< A mask to save state components
	UCHAR fxsave_area[512 + 16];              //!< For fxsave (+16 for alignment)
	LARGE_INTEGER Ia32FeatureMsr;			  //!< For Msr Read / Write
	LARGE_INTEGER Ia32VmxProcBasedCtrls;	  //!< For Msr Read / Write   
	LARGE_INTEGER Ia32VmxProcBasedCtrlsForOff;//!< For Msr Read / Write 
	LARGE_INTEGER Ia32VmxTrueProcBasedCtrls;  //!< For Msr Read / Write   
	LARGE_INTEGER Ia32VmxPinsBasedCtrls;	  //!< For Msr Read / Write     
	LARGE_INTEGER Ia32VmxVmExitCtrls;		  //!< For Msr Read / Write   
	LARGE_INTEGER Ia32VmxVmEntryCtrls;		  //!< For Msr Read / Write    
	LARGE_INTEGER VmxBasicMsr;				  //!< For Msr Read / Write
	LARGE_INTEGER VmxEptMsr;				  //!< For Msr Read / Write     
	LARGE_INTEGER HostKernelGsBase;			  ///guest_gs_kernel_base 
	LARGE_INTEGER GuestKernelGsBase;		  ///guest_gs_kernel_base  
	VCPUVMX*		vcpu_vmx;				  //!< For nested vmx context
	CPU_MODE		CpuMode;				  //!< For CPU Mode 
};



////////////////////////////////////////////////////////////////////////////////
//
// prototypes
//

////////////////////////////////////////////////////////////////////////////////
//
// variables
//

////////////////////////////////////////////////////////////////////////////////
//
// implementations
//

#endif  // HYPERPLATFORM_VMM_H_
