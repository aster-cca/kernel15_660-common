#ifndef __ASM_CCANDROID_H
#define __ASM_CCANDROID_H

#define STR(s) #s
#define FLUSH __asm__ volatile("ISB");

#define BENCH_EV(marker) __asm__ volatile("MOV XZR, " STR(marker))

#define BENCH_START(marker) \
FLUSH; BENCH_EV(marker)

#define BENCH_STOP(marker) \
__asm__ volatile("MOV XZR, " STR(marker)); FLUSH


/*
 * flow:
 * bl1_start ... stop
 * bl2_start ... stop
 * bl31_start ... rmm init start ... rmm init stop ... bl31_stop
 * bl31 loads kernel: ns_kernel start ... somewhere in kernel stop
 * 
 * total tfa: bl1_start ... bl31_end
*/

/*
markers: 
"__asm__ volatile (
        ""movz xzr, #0x1010\n""
    );"
"__asm__ volatile (
        ""movz xzr, #0x1011\n""
    );"

start markers: 0x2000 - 0x2FFF
stop markers: 0x3000 - 0x3FFF    

bl1 start/stop: 0x100
bl2 start/stop: 0x101
bl31 start/ rmm init: 0x102
rmm init start/ stop: 0x103
rmm init stop/ bl31 end: 0x104
ns kernel start/ somewhere in kernel: 0x105 
*/


#define CCA_TFA_BL1_START               BENCH_START(0x2100)
#define CCA_TFA_BL1_END                 BENCH_STOP (0x3100)

#define CCA_TFA_BL2_START               BENCH_START(0x2101)
#define CCA_TFA_BL2_END                 BENCH_STOP (0x3101)

#define CCA_TFA_BL31_START              BENCH_START(0x2102)
#define CCA_TFA_BL31_UNTIL_RMM_INIT     BENCH_STOP (0x3102); BENCH_START(0x2103)

#define CCA_TFA_BL31_UNTIL_RMM_INIT_STOP BENCH_STOP(0x3103); BENCH_START(0x2104)

#define CCA_TFA_BL31_END                BENCH_STOP(0x3104)

#define CCA_NS_KERNEL_START             BENCH_START (0x2105)
#define CCA_NS_KERNEL_STOP              BENCH_START (0x3105)




// ioctl in kvm_main
#define CCA_MARKER_KVM_VCPU_RUN BENCH_EV (0x1100) 

// run loop in kvm main arch specific
#define CCA_MARKER_KVM_VCPU_RUN_LOOP_ITER BENCH_EV(0x1101)

// call into world switch
#define CCA_MARKER_KVM_VCPU_ENTER_EXIT BENCH_EV(0x1102)



#define CCA_MARKER_HYP_SWITCH_VCPU_RUN_NVHE BENCH_EV(0x1105)
#define CCA_MARKER_HYP_SWITCH_VCPU_RUN_VHE BENCH_EV(0x1106)

// world switch run loop iteration
#define CCA_MARKER_GUEST_ENTER_VHE BENCH_EV(0x1103)
#define CCA_MARKER_GUEST_ENTER_NVHE BENCH_EV(0x1104)


/*
// exit handlers for non protected run loop:
	[ESR_ELx_EC_CP15_32]		= kvm_hyp_handle_cp15_32,
	[ESR_ELx_EC_HVC64]		= kvm_hyp_handle_hvc64,
	[ESR_ELx_EC_SYS64]		= kvm_hyp_handle_sysreg,
	[ESR_ELx_EC_SVE]		= kvm_hyp_handle_fpsimd,
	[ESR_ELx_EC_FP_ASIMD]		= kvm_hyp_handle_fpsimd, -> ESR_ELx_EC_SVE
	[ESR_ELx_EC_IABT_LOW]		= kvm_hyp_handle_iabt_low, -> same as kvm_hyp_handle_watchpt_low
	[ESR_ELx_EC_DABT_LOW]		= kvm_hyp_handle_dabt_low,
	[ESR_ELx_EC_WATCHPT_LOW]	= kvm_hyp_handle_watchpt_low,
	[ESR_ELx_EC_PAC]		= kvm_hyp_handle_ptrauth,
*/

#define CCA_MARKER_HYP_EXIT_ESR_ELx_EC_CP15_32 BENCH_EV(0x1200)
#define CCA_MARKER_HYP_EXIT_ESR_ELx_EC_HVC64  BENCH_EV(0x1201)
#define CCA_MARKER_HYP_EXIT_ESR_ELx_EC_SYS64 BENCH_EV(0x1202)
#define CCA_MARKER_HYP_EXIT_ESR_ELx_EC_SVE BENCH_EV(0x1203)

#define CCA_MARKER_HYP_EXIT_ESR_ELx_EC_IABT_LOW BENCH_EV(0x1205)
#define CCA_MARKER_HYP_EXIT_ESR_ELx_EC_DABT_LOW BENCH_EV(0x1206)
#define CCA_MARKER_HYP_EXIT_ESR_ELx_EC_PAC BENCH_EV(0x1208)

#define CCA_MARKER_HYP_UNDELEGATE BENCH_EV(0x1930)
#define CCA_MARKER_HYP_DELEGATE BENCH_EV(0x1931)
#define CCA_MARKER_HYP_FEATURES BENCH_EV(0x1932)
#define CCA_MARKER_HYP_DATA_DESTROY BENCH_EV(0x1933)
#define CCA_MARKER_HYP_CREATE_UNKNOWN BENCH_EV(0x1934)
#define CCA_MARKER_HYP_DATA_CREATE BENCH_EV(0x1935)
#define CCA_MARKER_HYP_PSCI_COMPLETE BENCH_EV(0x1936)
#define CCA_MARKER_HYP_REALM_ACTIVATE BENCH_EV(0x1937)
#define CCA_MARKER_HYP_REALM_CREATE BENCH_EV(0x1938)
#define CCA_MARKER_HYP_REALM_DESTROY BENCH_EV(0x1939)
#define CCA_MARKER_HYP_REC_AUX_CNT BENCH_EV(0x1940)
#define CCA_MARKER_HYP_REC_CREATE BENCH_EV(0x1941)
#define CCA_MARKER_HYP_REC_DESTROY BENCH_EV(0x1942)
#define CCA_MARKER_HYP_REC_ENTER BENCH_EV(0x1943)
#define CCA_MARKER_HYP_RTT_CREATE BENCH_EV(0x1944)
#define CCA_MARKER_HYP_RTT_DESTROY BENCH_EV(0x1945)
#define CCA_MARKER_HYP_RTT_FOLD BENCH_EV(0x1946)
#define CCA_MARKER_HYP_RTT_INIT_RIPAS BENCH_EV(0x1947)
#define CCA_MARKER_HYP_RTT_MAP_UNPROTECTED BENCH_EV(0x1948)
#define CCA_MARKER_HYP_RTT_READ_ENTRY BENCH_EV(0x1949)
#define CCA_MARKER_HYP_RTT_SET_RIPAS BENCH_EV(0x1950)
#define CCA_MARKER_HYP_RTT_UNMAP_UNPROTECTED BENCH_EV(0x1951)



#endif
