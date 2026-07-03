/*	Kernel ASM symbols.
$LK,"KernelB",A="FF:::/Kernel/Kernel.PRJ,KernelB"$ Kernel.PRJ
$LK,"KernelB",A="FF:::/Compiler/Compiler.PRJ,KernelB"$ Compiler.PRJ
$LK,"KernelB",A="FF:::/StartOS.ZC,KernelB"$ StartOS.ZC
*/

#help_index "Bit"
#help_file "::/Doc/Bit"
public _intern IC_BSF				I64	 Bsf(I64 bit_field_val); //Scan forward from lowest for 1st set. -1 if not found.
public _intern IC_BSR				I64  Bsr(I64 bit_field_val); //Scan rev from highest for 1st set. -1 if not found.
public _intern IC_BT				Bool Bt(		  U8 *bit_field, I64 bit); //Bit test.
public _intern IC_BTC				Bool Btc(		  U8 *bit_field, I64 bit); //Bit test and complement (same as xor with 1).
public _intern IC_BTR				Bool Btr(		  U8 *bit_field, I64 bit); //Bit test and reset to zero.
public _intern IC_BTS				Bool Bts(		  U8 *bit_field, I64 bit); //Bit test and set to one.
public _intern IC_LBTC				Bool LBtc(		  U8 *bit_field, I64 bit); //Locked bit test and complement (xor with 1).
public _intern IC_LBTR				Bool LBtr(		  U8 *bit_field, I64 bit); //Locked bit test and reset to zero.
public _intern IC_LBTS				Bool LBts(		  U8 *bit_field, I64 bit); //Locked bit test and set to one.
public _intern IC_POPCNT			I64  PopCount(I64 bit_field_val); //Count number of set bits in number.
public _extern _BEQUAL				Bool BEqual(	  U8 *bit_field, I64 bit, Bool val);//Set bit equal to value.
public _extern _LBEQUAL				Bool LBEqual(	  U8 *bit_field, I64 bit, Bool val); //Locked Set bit equal to value.
public _extern _BIT_FIELD_EXT_U32	U32  BFieldExtU32(U8 *bit_field, I64 bit, I64 size); //Extract U32 from bit field.
public _extern _BIT_FIELD_OR_U32	U0   BFieldOrU32( U8 *bit_field, I64 bit, U32 pattern); //Or U32 into bit field.

#help_index "Boot"
// EZODUS: ring-0 boot linker tags -> plain globals (seed asm cant resolve MEM_BOOT_BASE etc)
_extern _GET_FS CTask *GetFs();
_extern _SET_FS U0 SetFs(CTask *);
_extern _GET_GS CCPU *GetGs();
_extern _SET_GS U0 SetGs(CCPU *);
public U32 mem_boot_base;
public U32 sys_boot_blk;
public U32 sys_boot_patch_table_base;
public U32 sys_boot_src;
public U32 sys_run_level;

#help_index "Call"
public _extern _CALL		I64 Call(U8 *machine_code); //Call address with no args.
public _extern _CALLEXTSTR	I64 CallExtStr(/*argpop*/U8 *name, ...);//Search sym table for function and pass it args.
//Can have any fixed number of arguments.
public _extern _CALL_IND	I64 CallInd(/*argpop*/I64 (*fp_addr)(...), ...); //Call with fixed number of args.

#help_index "Call/System Extern Num"
public /*argpop*/	I64 (**ext)(...); //Array of function pointers.	See $LK,"Extern Num Definitions",A="MN:EXT_EXTS_NUM"$
public _extern _CALLEXTNUM					I64 CallExtNum(I64 num, ...); //Call ext num with fixed number of args.

#help_index "Char/Conversion"
public _intern IC_TOUPPER	I64 ToUpper(U8 ch); //Convert char to upper case.

#help_index "Char/Operations"
public _intern IC_STRLEN	I64 StrLen(U8 *st); //String length.

#help_index "Data Types/Circular Queue"
#help_file "::/Doc/Queue"
public _intern IC_QUEUE_INIT		U0 QueueInit(CQueue *head); //Init queue head links.
public _intern IC_QUEUE_INSERT		U0 QueueInsert(   CQueue *entry, CQueue *pred);//Insert item into que after predecessor.
public _intern IC_QUEUE_INSERT_REV	U0 QueueInsertRev(CQueue *entry, CQueue *succ); //Revd insert into que.
																					//Ins item into que before successor.
public _intern IC_QUEUE_REMOVE		U0 QueueRemove(   CQueue *entry); //Remove item from queue.

#help_index "I/O;Processor/IO Port"
public _intern IC_IN_U16	U16 InU16( I64 port); //Read U16 from I/O port.
public _intern IC_IN_U32	U32 InU32( I64 port); //Read U32 from I/O port.
public _intern IC_IN_U8		U8  InU8(  I64 port); //Read U8 from I/O port.
public _intern IC_OUT_U16	U0  OutU16(I64 port,  I64 val); //Write U16 to I/O port.
public _intern IC_OUT_U32	U0  OutU32(I64 port,  I64 val); //Write U32 to I/O port.
public _intern IC_OUT_U8	U0  OutU8( I64 port,  I64 val); //Write U8 to I/O port.
public _extern _REP_IN_U16	U0  RepInU16( U8 *buf, I64 count, I64 port); //Repeated read U16 from I/O port.
public _extern _REP_IN_U32	U0  RepInU32( U8 *buf, I64 count, I64 port); //Repeated read U32 from I/O port.
public _extern _REP_IN_U8	U0  RepInU8(  U8 *buf, I64 count, I64 port); //Repeated read U8 from I/O port.
public _extern _REP_OUT_U16 U0  RepOutU16(U8 *buf, I64 count, I64 port); //Repeated write U16 to I/O port.
public _extern _REP_OUT_U32 U0  RepOutU32(U8 *buf, I64 count, I64 port); //Repeated write U32 to I/O port.
public _extern _REP_OUT_U8	U0  RepOutU8( U8 *buf, I64 count, I64 port); //Repeated write U8 to I/O port.

#help_index "Math"
public _intern IC_ATAN		F64		ATan(F64 d); //Arc Tan (Inverse Tan).
public _intern IC_ABS		F64		Abs(F64 d); //Absolute F64.
public _intern IC_ABS_I64	I64		AbsI64(I64 i); //Absolute I64.
public _intern IC_COS		F64		Cos(F64 d); //Cosine.
public _intern IC_MAX_I64	I64		MaxI64(I64 n1, I64 n2); //Max of two I64s.
public _intern IC_MAX_U64	U64		MaxU64(U64 n1, U64 n2); //Max of two U64s.
public _intern IC_MIN_I64	I64		MinI64(I64 n1, I64 n2); //Min of two I64s.
public _intern IC_MIN_U64	U64		MinU64(U64 n1, U64 n2); //Min of two U64s.
public _intern IC_MOD_U64	U64		ModU64(U64 *q, U64 d); //Div and return U64 Modulo.
public _intern IC_SIGN_I64	I64		SignI64(I64 i); //Sign of I64: -1,0,1.
public _intern IC_SIN		F64		Sin(F64 d); //Sine.
public _intern IC_SQR		F64		Sqr(F64 d); //Square of F64.
public _intern IC_SQR_I64	I64		SqrI64(I64 i); //Square of I64.
public _intern IC_SQR_U64	U64		SqrU64(U64 i); //Square of U64.
public _intern IC_SQRT		F64		Sqrt(F64 d); //Square head of F64.
public _intern IC_SWAP_I64	U0		SwapI64(I64 *n1, I64 *n2); //Swap two I64s. Not atomic.
public _intern IC_SWAP_U16	U0		SwapU16(U16 *n1, U16 *n2); //Swap two U16s. Not atomic.
public _intern IC_SWAP_U32	U0		SwapU32(U32 *n1, U32 *n2); //Swap two U32s. Not atomic.
public _intern IC_SWAP_U8	U0		SwapU8(U8 *n1, U8 *n2); //Swap two U8s. Not atomic.
public _intern IC_TAN		F64		Tan(F64 d); //Tangent.
public _intern IC_TO_BOOL	Bool	ToBool(I64 i); //Convert to Boolean.
public _intern IC_TO_F64	F64		ToF64(I64 i); //Convert to F64.
public _intern IC_TO_I64	I64		ToI64(F64 d);	//Convert to I64. Truncates.
public _extern _ARG			F64		Arg(F64 x, F64 y); //Polar coordinate angle.
public _extern _CEIL		F64		Ceil(F64 d); //Ceiling of F64.
public _extern _CLAMP_I64	I64		ClampI64(I64 num, I64 lo, I64 hi); //Clamp to I64 [] range.
public _extern _CLAMP_U64	U64		ClampU64(U64 num, U64 lo, U64 hi); //Clamp to U64 [] range.
public _extern _EXP			F64		Exp(F64 d); //Exponential function.
_extern _FCLEX				U0		Fclex();
_extern _FLDCW				U0		Fldcw(U16 w);
public _extern _FLOOR		F64		Floor(F64 d); //Floor of F64.
_extern _FSTCW				U16		Fstcw();
_extern _FSTSW				U16		Fstsw();
public _extern _LN			F64		Ln(F64 d); //Logarithm.
public _extern _LOG10		F64		Log10(F64 d); //Log base 10.
public _extern _LOG2		F64		Log2(F64 d); //Log base 2.
public _extern _POW			F64		Pow(F64 base, F64 power); //F64 base to a power.
public _extern _POW10		F64		Pow10(F64 d); //Ten to the dth power.
public _extern _ROUND		F64		Round(F64 d); //Round F64 to whole number.
public _extern _SIGN		F64		Sign(F64 d); //Sign of F64: -1,0,1
public _extern _TRUNC		F64		Trunc(F64 d); //Truncate F64.

#help_index "Math/CD3;Data Types/CD3"
#help_file "::/Doc/D3"
public _extern _D3_ADD		CD3 *D3Add(   CD3 *sum, CD3 *d1, CD3 *d2); //sum=d1+d2
public _extern _D3_ADD_EQU	CD3 *D3AddEqu(CD3 *sum, CD3 *d); //sum+=d
public _extern _D3_COPY		CD3 *D3Copy(CD3 *dst, CD3 *src); //dst=src
public _extern _D3_CROSS	CD3 *D3Cross(CD3 *prod, CD3 *d1, CD3 *d2); //prod=d1xd2
public _extern _D3_DIST		F64  D3Dist(   CD3 *d1, CD3 *d2); //Distance
public _extern _D3_DIST_SQR	F64  D3DistSqr(CD3 *d1, CD3 *d2); //Distance Squared
public _extern _D3_DIV		CD3 *D3Div(   CD3 *quot, CD3 *d, F64 s); //quot=d/s
public _extern _D3_DIV_EQU	CD3 *D3DivEqu(CD3 *quot, F64 s); //quot/=s
public _extern _D3_DOT		F64  D3Dot(CD3 *d1, CD3 *d2); //d1 dot d2
public _extern _D3_EQU		CD3 *D3Equ(CD3 *dst, F64 x=0, F64 y=0, F64 z=0); //dst=(x, y,z)
public _extern _D3_MUL		CD3 *D3Mul(   CD3 *prod, F64 s, CD3 *d); //prod=s*d
public _extern _D3_MUL_EQU	CD3 *D3MulEqu(CD3 *prod, F64 s); //prod*=s
public _extern _D3_NORM		F64  D3Norm(CD3 *d); //Norm
public _extern _D3_NORM_SQR	F64  D3NormSqr(CD3 *d); //Norm Squared
public _extern _D3_SUB		CD3 *D3Sub(   CD3 *diff, CD3 *d1, CD3 *d2); //diff=d1-d2
public _extern _D3_SUB_EQU	CD3 *D3SubEqu(CD3 *diff, CD3 *d); //diff-=d
public _extern _D3_UNIT		CD3 *D3Unit(CD3 *d); //To unit vect
public _extern _D3_ZERO		CD3 *D3Zero(CD3 *dst); //To zero

#help_index "Memory"
public _extern _MEMCOMPARE	I64  MemCompare(U8 *ptr1, U8 *ptr2, I64 count); //Compare chunk of memory.
public _extern _MEMCOPY		U8  *MemCopy(U8 *dst, U8 *src, I64 count); //Copy chunk of memory. Only goes forward.
public _extern _MEMSET		U8  *MemSet(    U8 *dst, I64 val, I64 count); //Set chunk of U8s to value.
public _extern _MEMSET_I64	I64 *MemSetI64(I64 *dst, I64 val, I64 I64count); //Set chunk of I64s to value.
public _extern _MEMSET_U16	U16 *MemSetU16(U16 *dst, I64 val, I64 U16count); //Set chunk of U16s to value.
public _extern _MEMSET_U32	U32 *MemSetU32(U32 *dst, I64 val, I64 U32count); //Set chunk of U32s to value.
public U8 sys_heap_debug;
public U8 sys_heap_init_flag;
public U8 sys_heap_init_val;
public U8 sys_mem_init_flag;
public U8 sys_mem_init_val;
public U8 sys_var_init_flag;
public U8 sys_var_init_val;

#help_index "Memory/BlkPool"
public CBlkPool *sys_code_bp; //System's BlkPool for code.
public CBlkPool *sys_data_bp; //System's BlkPool for data.

#help_index "Memory/Info"
public U8 *mem_heap_base;
public U8 *mem_heap_limit;
public U8 *mem_mapped_space;
public U8 *mem_physical_space;

#help_index "Messages;Windows"
public CTask *sys_focus_task; //Current focus task.

#help_index "Misc"
public CDate sys_compile_time; //Date and time kernel was compiled.
public I64   sys_ctrl_alt_flags[1]; //Should be semiphores?

#help_index "Misc/Progress Bars"
public I64 progress1; //Current progress 1.
public U8  progress1_desc[PROGRESS_DESC_LEN]; //Progress 1 description.
public I64 progress1_max; //Progress 1 max.
public F64 progress1_t0; //Progress 1 start time.
public F64 progress1_tf; //Progress 1 end time.
public I64 progress2; //Current progress 2.
public U8	progress2_desc[PROGRESS_DESC_LEN]; //Progress 2 description.
public I64 progress2_max; //Progress 2 max.
public F64 progress2_t0; //Progress 2 start time.
public F64 progress2_tf; //Progress 2 end time.
public I64		progress3; //Current progress 3.
public U8	progress3_desc[PROGRESS_DESC_LEN]; //Progress 3 description.
public I64 progress3_max; //Progress 3 max.
public F64 progress3_t0; //Progress 3 start time.
public F64 progress3_tf; //Progress 3 end time.
public I64 progress4; //Current progress 4.
public U8	progress4_desc[PROGRESS_DESC_LEN]; //Progress 4 description.
public I64 progress4_max; //Progress 4 max.
public F64 progress4_t0; //Progress 4 start time.
public F64 progress4_tf; //Progress 4 end time.
public CProgress sys_progresses[PROGRESS_BARS_NUM]; //All progress bars.

#help_index "MultiCore"
#help_file "::/Doc/MultiCore"
// cpu_structs[] defined in HCRT_TOS.ZC (not a boot-linker pointer).
public I64   mp_count; //Count of cores
public I64   mp_count_initial; //only used during $LK,"Core0StartMP",A="MN:Core0StartMP"$
public I64   mp_count_lock;
public CSema sys_semas[SEMA_SEMAS_NUM];

#help_index "MultiCore;Processor"
public _intern IC_GS CCPU *Gs(); //GS points to current $LK,"CCPU",A="MN:CCPU"$.

#help_index "PCI"
public U16 sys_pci_buses; //Number of PCI buses.

#help_index "Processor"
public _intern IC_CARRY I64 Carry(); //See $LK,"::/Demo/Carry.ZC"$
public _intern IC_RAX_GET		I64		RAXGet(); //Get RAX register value.
public _intern IC_RBP_GET		U8	   *RBPGet(); //Get RBP register value.
public _intern IC_RFLAGS_GET	I64		RFlagsGet(); //Get RFlags register value.
public _intern IC_RSP_GET		U8	   *RSPGet(); //Get RSP register value.
public _intern IC_POP			I64		Pop(); //Pop value from stack.
public _intern IC_PUSH			U0		Push(I64 d); //Push value on stack.
public _intern IC_RAX_SET		U0		RAXSet(I64 d); //Set RAX register value.
public _intern IC_RBP_SET		U0		RBPSet(U8 *d); //Set RBP register value.
public _intern IC_RFLAGS_SET	U0		RFlagsSet(I64 d); //Set RFlags register value.
public _intern IC_RSP_SET		U0		RSPSet(U8 *d); //Set RSP register value.
public _extern _CPUID			U0		CPUId(U32 rax, CRAXRBXRCXRDX *res); //Do CPUID instruction.
public _extern _ENDIAN_U16		U16		EndianU16(U16 val); //Swap big<-->little endian U16.
public _extern _ENDIAN_U32		U32		EndianU32(U32 val); //Swap big<-->little endian U32.
public _extern _ENDIAN_U64		U64		EndianU64(U64 val); //Swap big<-->little endian U64.
public _extern _LXCHG_I64		I64		LXchgI64(I64 *dst, I64 d); //Locked eXchange I64s.
public _extern _LXCHG_U16		U16		LXchgU16(U16 *dst, U16 d); //Locked eXchange U16s.
public _extern _LXCHG_U32		U32		LXchgU32(U32 *dst, U32 d); //Locked eXchange U32s.
public _extern _LXCHG_U8		U8		LXchgU8(  U8 *dst,  U8 d); //Locked eXchange U8s.
public _extern _MSR_READ		I64		MSRRead(I64 model_specific_reg); //Model Specific Reg See $LK,"MSRs",A="MN:IA32_EFER"$.
public _extern _MSR_WRITE		U0		MSRWrite(I64 model_specific_reg, I64 val); //Model Specific Reg See $LK,"MSRs",A="MN:IA32_EFER"$.
public _extern _SYS_HLT			U0		SysHlt(); //Loops doing HLT instruction.
public _extern _XCHG_I64		I64		XchgI64(I64 *dst, I64 d); //eXchange I64s.
public _extern _XCHG_U16		U16		XchgU16(U16 *dst, U16 d); //eXchange U16s.
public _extern _XCHG_U32		U32		XchgU32(U32 *dst, U32 d); //eXchange U32s.
public _extern _XCHG_U8			U8		XchgU8(  U8 *dst,  U8 d); //eXchange U8s.
public CGDT	sys_gdt;

#help_index "Processor/Cache;Memory/Cache"
public _intern IC_CLFLUSH			U0  CLFlush(U8 *a); //Flush cache line instruction.
public I64 sys_cache_line_width; //CPU's Cache line width.

#help_index "Processor/Page Tables;Memory/Page Tables"
public _intern IC_INVLPG		U0  InvalidatePage(U8 *a); //Invalidate page at addr instruction.
public U64 mem_page_size;

#help_index "Task;Processor"
public _intern IC_FS CTask *Fs(); //FS segment register points to the current $LK,"CTask",A="MN:CTask"$.

#help_index "Time/CPU Cycles"
#help_file "::/Doc/TimeCycles"
public _intern IC_RDTSC I64 TSCGet(); //Get time stamp counter.

#help_index "Graphics/Framebuffer"
public U8	*sys_framebuffer_addr;
public U64	sys_framebuffer_width;
public U64	sys_framebuffer_height;
public U64	sys_framebuffer_pitch;
public U8	sys_framebuffer_bpp;
public									U64	sys_framebuffer_pidth;
public CVideoInfo sys_framebuffer_list[VBE_MODES_NUM];

#help_index "Processor/SMBIOS"
public U8	*sys_smbios_entry;

#help_index ""
public U64	sys_disk_uuid[2];
public Bool	sys_is_uefi_booted;
public U8		sys_bootloader_id;

#help_index ""

// EZODUS: runtime symbol binding when KernelB.HH is ExePutS2'd with BOOT_EXODUS
// (see FULL_PACKAGE.ZC LoadImps). Not active during AOT compile of HCRT_TOS.
#ifdef BOOT_EXODUS
_extern _HASH_STR I64 HashStr(U8 *st);
_extern _HASH_FIND CHash *HashFind(U8 *needle_str, CHashTable *haystack_table, I64 mask, I64 instance=1);
_extern _HASH_SINGLE_TABLE_FIND CHash *HashSingleTableFind(U8 *needle_str, CHashTable *haystack_table, I64 mask, I64 instance=1);
_extern _HASH_BUCKET_FIND CHash **HashBucketFind(U8 *needle_str, CHashTable *haystack_table);
_extern _HASH_REM_DEL Bool HashRemDel(CHash *tmph, CHashTable *table, I64 instance=1);
_extern _STRICOMPARE I64 StrICompare(U8 *st1, U8 *st2);
_extern _STRNCOMPARE I64 StrNCompare(U8 *st1, U8 *st2, I64 n);
_extern _STRNICOMPARE I64 StrNICompare(U8 *st1, U8 *st2, I64 n);
_extern _STRMATCH U8 *StrMatch(U8 *needle, U8 *haystack_str);
_extern _STRIMATCH U8 *StrIMatch(U8 *needle, U8 *haystack_str);
_extern _FREE U0 Free(U8 *addr);
_extern _MSIZE I64 MSize(U8 *src);
_extern _MSIZE2 I64 MSize2(U8 *src);
_extern _MHEAP_CTRL CHeapCtrl *MHeapCtrl(U8 *src);
_extern _MALLOC U8 *MAlloc(I64 size, CTask *mem_task=NULL);
_extern _TEXT_CHAR Bool TextChar(CTask *task, Bool allow_border=FALSE, I64 x, I64 y, I64 d);
_extern _TEXT_LEN_STR Bool TextLenStr(CTask *task, I64 x, I64 y, I64 len, I64 attr, U8 *s);
_extern _TEXT_LEN_ATTR_STR Bool TextLenAttrStr(CTask *task, I64 x, I64 y, I64 len, U32 *_attr);
_extern _TEXT_LEN_ATTR Bool TextLenAttr(CTask *task, I64 x, I64 y, I64 len, I64 attr);
_extern _GR_ROP_EQU_U8_NO_CLIPPING U0 GrRopEquU8NoClipping(I64 ch, U8 *dst, I64 width_internal);
_extern _IS_PIX_COVERED0 Bool IsPixCovered0(CTask *task, I64 x, I64 y);
_extern _DC_BLOT_COLOR4 U0 DCBlotColor4(U8 *dst, I64 *img, I64 *img_cache, I64 cnt);
_extern _LAST_FUN I64 LastFun(I64 argc, I64 *argv);
_extern _TEST_EXCEPT U0 TestExcept();
_extern _TAKE_EXCEPT U0 TakeExcept();
_extern _SAVE_EXCEPT_REGS U0 SaveExceptRegs(CExcept *t);
_extern _TASK_CONTEXT_RESTORE U0 TaskContextRestore();
_extern _TASK_END_NOW U0 TaskEndNow();
_extern _YIELD U0 _Yield();
#endif

