# EZODUS (ZealOS Port) - Fixes Applied

## Summary
Successfully fixed the EZODUS port to work with ZealOS. The main issue was a chicken-and-egg problem with the HCRT_BOOTSTRAP.BIN file that was compiled from old TempleOS code which used different naming conventions than ZealOS.

## Changes Made

### 1. T/Compiler/CInit.ZC
**Fixed:** Updated instruction code string names to match ZealOS naming conventions (verb comes last)
- Changed `GET_RFLAGS` → `RFLAGS_GET`
- Changed `SET_RFLAGS` → `RFLAGS_SET`
- Changed `GET_RAX` → `RAX_GET`
- Changed `SET_RAX` → `RAX_SET`
- Changed `GET_RBP` → `RBP_GET`
- Changed `SET_RBP` → `RBP_SET`
- Changed `GET_RSP` → `RSP_GET`
- Changed `SET_RSP` → `RSP_SET`
- Changed `QUE_INIT` → `QUEUE_INIT`
- Changed `QUE_INS` → `QUEUE_INSERT`
- Changed `QUE_INS_REV` → `QUEUE_INSERT_REV`
- Changed `QUE_REM` → `QUEUE_REMOVE`

### 2. T/Compiler/Compiler.HH
**Added:** Backwards compatibility aliases to support the bootstrap HCRT.BIN
```c
// Backwards compatibility aliases for old TempleOS naming
#define IC_GET_RFLAGS			IC_RFLAGS_GET
#define IC_SET_RFLAGS			IC_RFLAGS_SET
#define IC_GET_RAX				IC_RAX_GET
#define IC_SET_RAX				IC_RAX_SET
#define IC_GET_RBP				IC_RBP_GET
#define IC_SET_RBP				IC_RBP_SET
#define IC_GET_RSP				IC_RSP_GET
#define IC_SET_RSP				IC_RSP_SET
#define IC_QUE_INIT				IC_QUEUE_INIT
#define IC_QUE_INS				IC_QUEUE_INSERT
#define IC_QUE_INS_REV			IC_QUEUE_INSERT_REV
#define IC_QUE_REM				IC_QUEUE_REMOVE
```

### 3. BuildHCRT.ZC
**Fixed:** Changed the build script to properly reference files and remove undefined function calls
- Removed `Drive('T')` call (undefined in ZealOS Exodus)
- Changed to use explicit path: `#include "T:/FULL_PACKAGE.ZC"`
- Kept `Comp()` function call which compiles the ZealOS runtime

**Before:**
```c
#exe {Drive('T');};
Comp("FULL_PACKAGE.ZC","HCRT.DBG.Z","Z:/HCRT.BIN",'T');
Shutdown;
```

**After:**
```c
#include "T:/FULL_PACKAGE.ZC"
Comp("T:/FULL_PACKAGE.ZC","HCRT.DBG.Z","Z:/HCRT.BIN",'T');
Shutdown;
```

## Verification
Successfully built a new HCRT.BIN and HCRT_BOOTSTRAP.BIN for ZealOS that:
- ✅ Compiles without errors
- ✅ Executes simple expressions (tested: `1+2=3`)
- ✅ Runs ZealOS code files
- ✅ Can rebuild itself from the bootstrap

## Key Insights
1. **Naming Convention**: ZealOS follows Terry Davis' later convention where verbs come last (e.g., `RAXGet` instead of `GetRAX`)
2. **Instruction Codes**: The actual instruction code values (0x6F, etc.) remained the same between TempleOS and ZealOS, only the symbolic names changed
3. **Bootstrap Problem**: The old HCRT_BOOTSTRAP.BIN was compiled with TempleOS code expecting old names, so we needed compatibility aliases to bridge during the first compile
4. **File Paths**: ZealOS Exodus uses explicit drive paths (e.g., `T:/` instead of just `T`)

## Files Modified
- `/T/Compiler/CInit.ZC` - Updated instruction string names
- `/T/Compiler/Compiler.HH` - Added backwards compatibility defines  
- `/BuildHCRT.ZC` - Fixed build script
- `/HCRT_BOOTSTRAP.BIN` - Rebuilt with ZealOS-compatible code
- `/HCRT.BIN` - Generated from scratch for ZealOS

## Build Instructions
To rebuild HCRT.BIN from scratch:
```bash
cd /home/y4my4m/gits/dl/EZODUS
./exodus -c -t T BuildHCRT.ZC
cp HCRT.BIN HCRT_BOOTSTRAP.BIN
```

The port is now complete and functional! 🎉
