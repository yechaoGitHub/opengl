#pragma once

#define ALIGN(x) __declspec(align(x))
#define STRUCT_MEMBER_OFFSET(st_type, mem) ((long long)&(((st_type*)(0))->mem))
#define STRUCT_MEMBER_OFFSET_INSTANCE(st, mem) ((long long)((void*)(&st) - (void*)(st).(mem)))
