#pragma once

#include <exodus/types.h>

void BackTrace(u64 _rbp, u64 _rip);
char const *WhichFun(u8 *ptr);
