#pragma once
#include <cell\sysmodule.h>
#include <cell/cell_fs.h>
#include <cell/pad/libpad.h>
#include <initializer_list>
#include <ppu_intrinsics.h>
#include <stdarg.h>
#include <string>
#include <sys/memory.h>
#include <sys/sys_time.h>
#include <sys/ppu_thread.h>
#include <sys/process.h>
#include <sys/prx.h>
#include <sys/syscall.h>
#include <sys/timer.h>
#include <algorithm>
#include <float.h>
#include <math.h>
#include <fastmath.h>
#include <libpsutil.h>

#include <vector>
#include <map>
#include <stack>
#include <csetjmp>
#include <np.h>
#include <cell/http.h>
#include <sys/synchronization.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netex\libnetctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <cell\cell_fs.h>
#include <stdio.h>
#include <cell\mstream.h>
#include <semaphore.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sysutil/sysutil_msgdialog.h>
#include <sysutil/sysutil_oskdialog.h>
#include <sysutil/sysutil_userinfo.h>
#include "vector.h"
//#include "easing.h"
//#include "scheduler.h"



using namespace math;
extern "C"
{
	int	_sys_printf(const char* format, ...);
	int	_sys_sprintf(char* s, const char* format, ...);
	int	_sys_vsprintf(char* s, const char* format, va_list arg);
	int _sys_snprintf(char* buffer, size_t len, const char* fmt, ...);

	void* _sys_malloc(size_t n);
	void _sys_free(void* ptr);
}

#define printf			_sys_printf
#define sprintf			_sys_sprintf
#define vsprintf		_sys_vsprintf
#define snprintf            _sys_snprintf




static int Sys_Milliseconds()
{
	return sys_time_get_system_time() / 1000;
}

#define clamp(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))


typedef unsigned char byte;

#define STATIC_ALWAYS_INLINE    static inline   __attribute__((always_inline))


inline uint32_t hash(const char* str)
{
	uint64_t hash = 0xCBF29CE484222325;

	while (*str)
	{
		hash =
			(hash ^ uint32_t((*str >= 'A' && *str <= 'Z') ? *str - ('A' - 'a') : *str)) * 0x100000001B3;
		str++;
	}

	return hash;
}

namespace Game
{
	template<typename T, typename... Args>
	static inline T invoke(uint32_t address, Args... args)
	{
		volatile uint32_t func_fptr[2]{ address, 0x1C85330 };
		auto func = reinterpret_cast<T(*)(Args...)>(func_fptr);
		return func(args...);
	}
}


#define __declspec(...) __attribute__((__VA_ARGS__))

extern unsigned char const __start__Ztext[];
