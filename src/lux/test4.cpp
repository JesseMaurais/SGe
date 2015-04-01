#include "lux.hpp"
#include <unistd.h>

#define LX(ent) {#ent, ent}

lux_Entry ents[] = 
{
	LX(F_LOCK),
	LX(F_OK),
	LX(F_TEST),
	LX(F_TLOCK),
	LX(F_ULOCK),
	LX(R_OK),
	LX(SEEK_CUR),
	LX(SEEK_END),
	LX(SEEK_SET),	
	LX(STDERR_FILENO),
	LX(STDIN_FILENO),
	LX(STDOUT_FILENO),
	LX(W_OK),
	LX(X_OK),
	LX(_CS_PATH),
	LX(_CS_XBS5_ILP32_OFF32_CFLAGS),
	LX(_CS_XBS5_ILP32_OFF32_LDFLAGS),
	LX(_CS_XBS5_ILP32_OFF32_LIBS),
	LX(_CS_XBS5_ILP32_OFF32_LINTFLAGS),
	LX(_CS_XBS5_ILP32_OFFBIG_CFLAGS),
	LX(_CS_XBS5_ILP32_OFFBIG_LDFLAGS),
	LX(_CS_XBS5_ILP32_OFFBIG_LIBS),
	LX(_CS_XBS5_ILP32_OFFBIG_LINTFLAGS),
	LX(_CS_XBS5_LP64_OFF64_CFLAGS),
	LX(_CS_XBS5_LP64_OFF64_LDFLAGS),
	LX(_CS_XBS5_LP64_OFF64_LIBS),
	LX(_CS_XBS5_LP64_OFF64_LINTFLAGS),
	LX(_CS_XBS5_LPBIG_OFFBIG_CFLAGS),
	LX(_CS_XBS5_LPBIG_OFFBIG_LDFLAGS),
	LX(_CS_XBS5_LPBIG_OFFBIG_LIBS),
	LX(_CS_XBS5_LPBIG_OFFBIG_LINTFLAGS),
	LX(_PC_ASYNC_IO),
	LX(_PC_CHOWN_RESTRICTED),
	LX(_PC_FILESIZEBITS),
	LX(_PC_LINK_MAX),
	LX(_PC_MAX_CANON),
	LX(_PC_MAX_INPUT),
	LX(_PC_NAME_MAX),
	LX(_PC_NO_TRUNC),
	LX(_PC_PATH_MAX),
	LX(_PC_PIPE_BUF),
	LX(_PC_PRIO_IO),
	LX(_PC_SYNC_IO),
	LX(_PC_VDISABLE),
	LX(_POSIX2_VERSION),
	LX(_POSIX_VERSION),
	LX(_SC_2_C_BIND),
	LX(_SC_2_C_DEV),
	LX(_SC_2_C_VERSION),
	LX(_SC_2_FORT_DEV),
	LX(_SC_2_FORT_RUN),
	LX(_SC_2_LOCALEDEF),
	LX(_SC_2_SW_DEV),
	LX(_SC_2_UPE),
	LX(_SC_2_VERSION),
	LX(_SC_AIO_LISTIO_MAX),
	LX(_SC_AIO_MAX),
	LX(_SC_AIO_PRIO_DELTA_MAX),
	LX(_SC_ARG_MAX),
	LX(_SC_ASYNCHRONOUS_IO),
	LX(_SC_ATEXIT_MAX),
	LX(_SC_BC_BASE_MAX),
	LX(_SC_BC_DIM_MAX),
	LX(_SC_BC_SCALE_MAX),
	LX(_SC_BC_STRING_MAX),
	LX(_SC_CHILD_MAX),
	LX(_SC_CLK_TCK),
	LX(_SC_COLL_WEIGHTS_MAX),
	LX(_SC_DELAYTIMER_MAX),
	LX(_SC_EXPR_NEST_MAX),
	LX(_SC_FSYNC),
	LX(_SC_GETGR_R_SIZE_MAX),
	LX(_SC_GETPW_R_SIZE_MAX),
	LX(_SC_IOV_MAX),
	LX(_SC_JOB_CONTROL),
	LX(_SC_LINE_MAX),
	LX(_SC_LOGIN_NAME_MAX),
	LX(_SC_MAPPED_FILES),
	LX(_SC_MEMLOCK),
	LX(_SC_MEMLOCK_RANGE),
	LX(_SC_MEMORY_PROTECTION),
	LX(_SC_MESSAGE_PASSING),
	LX(_SC_MQ_OPEN_MAX),
	LX(_SC_MQ_PRIO_MAX),
	LX(_SC_NGROUPS_MAX),
	LX(_SC_OPEN_MAX),
	LX(_SC_PAGESIZE),
	LX(_SC_PAGE_SIZE),
	LX(_SC_PASS_MAX),
	LX(_SC_PRIORITIZED_IO),
	LX(_SC_PRIORITY_SCHEDULING),
	LX(_SC_REALTIME_SIGNALS),
	LX(_SC_RE_DUP_MAX),
	LX(_SC_RTSIG_MAX),
	LX(_SC_SAVED_IDS),
	LX(_SC_SEMAPHORES),
	LX(_SC_SEM_NSEMS_MAX),
	LX(_SC_SEM_VALUE_MAX),
	LX(_SC_SHARED_MEMORY_OBJECTS),
	LX(_SC_SIGQUEUE_MAX),
	LX(_SC_STREAM_MAX),
	LX(_SC_SYNCHRONIZED_IO),
	LX(_SC_THREADS),
	LX(_SC_THREAD_ATTR_STACKADDR),
	LX(_SC_THREAD_ATTR_STACKSIZE),
	LX(_SC_THREAD_DESTRUCTOR_ITERATIONS),
	LX(_SC_THREAD_KEYS_MAX),
	LX(_SC_THREAD_PRIORITY_SCHEDULING),
	LX(_SC_THREAD_PRIO_INHERIT),
	LX(_SC_THREAD_PRIO_PROTECT),
	LX(_SC_THREAD_PROCESS_SHARED),
	LX(_SC_THREAD_SAFE_FUNCTIONS),
	LX(_SC_THREAD_STACK_MIN),
	LX(_SC_THREAD_THREADS_MAX),
	LX(_SC_TIMERS),
	LX(_SC_TIMER_MAX),
	LX(_SC_TTY_NAME_MAX),
	LX(_SC_TZNAME_MAX),
	LX(_SC_VERSION),
	LX(_SC_XBS5_ILP32_OFF32),
	LX(_SC_XBS5_ILP32_OFFBIG),
	LX(_SC_XBS5_LP64_OFF64),
	LX(_SC_XBS5_LPBIG_OFFBIG),
	LX(_SC_XOPEN_CRYPT),
	LX(_SC_XOPEN_ENH_I18N),
	LX(_SC_XOPEN_LEGACY),
	LX(_SC_XOPEN_REALTIME),
	LX(_SC_XOPEN_REALTIME_THREADS),
	LX(_SC_XOPEN_SHM),
	LX(_SC_XOPEN_UNIX),
	LX(_SC_XOPEN_VERSION),
	LX(_SC_XOPEN_XCU_VERSION),
	#if _XOPEN_CRYPT
	LX(_XOPEN_CRYPT),
	#endif
	#if _XOPEN_ENH_I18N
	LX(_XOPEN_ENH_I18N),
	#endif
	#if _XOPEN_LEGACY
	LX(_XOPEN_LEGACY),
	#endif
	#if _XOPEN_REALTIME
	LX(_XOPEN_REALTIME),
	#endif
	#if _XOPEN_REALTIME_THREADS
	LX(_XOPEN_REALTIME_THREADS),
	#endif
	#if _XOPEN_SHM
	LX(_XOPEN_SHM),
	#endif
	#if _XOPEN_STREAMS
	LX(_XOPEN_STREAMS),
	#endif
	#if _XOPEN_UNIX
	LX(_XOPEN_UNIX),
	#endif
	LX(_XOPEN_VERSION)
};

#define lx(fun) lux_reg(fun)

luaL_Reg regs[] =
{
	lx(access),
	lx(alarm),
	lx(brk),
	lx(chdir),
	lx(chroot),
	lx(chown),
	lx(close),
	lx(confstr),
//	lx(crypt),
	lx(ctermid),
	lx(cuserid),
	lx(dup),
	lx(dup2),
//	lx(encrypt),
//	lx(execl),
//	lx(execle),
//	lx(execlp),
//	lx(execv),
//	lx(execve),
//	lx(execvp),
	lx(_exit),
	lx(fchown),
	lx(fchdir),
	lx(fdatasync),
	lx(fork),
	lx(fpathconf),
	lx(fsync),
	lx(ftruncate),
	lx(getcwd),
	lx(getdtablesize),
	lx(getegid),
	lx(geteuid),
	lx(getgid),
	lx(getgroups),
	lx(gethostid),
	lx(getlogin),
	lx(getlogin_r),
//	lx(getopt),
	lx(getpagesize),
	lx(getpass),
	lx(getpgid),
	lx(getpgrp),
	lx(getpid),
	lx(getppid),
	lx(getsid),
	lx(getuid),
//	lx(getwd),
	lx(isatty),
	lx(lchown),
	lx(link),
	lx(lockf),
	lx(lseek),
	lx(nice),
	lx(pathconf),
	lx(pause),
	lx(pipe),
	lx(pread),
	lx(pwrite),
	lx(read),
	lx(readlink),
	lx(rmdir),
	lx(sbrk),
	lx(setgid),
	lx(setpgid),
	lx(setpgrp),
	lx(setregid),
	lx(setreuid),
	lx(setsid),
	lx(setuid),
	lx(sleep),
	lx(swab),
	lx(symlink),
	lx(sync),
	lx(sysconf),
	lx(tcgetpgrp),
	lx(tcsetpgrp),
	lx(truncate),
	lx(ttyname),
	lx(ttyname_r),
	lx(ualarm),
	lx(unlink),
	lx(usleep),
	lx(vfork),
	lx(write),

	{NULL,NULL}
};

static int __index(lua_State *vm)
{
	const char *key = luaL_checkstring(vm, 2);
	try {
		int value = lux_Entry::find(key, ents, lengthof(ents));
		lua_pushinteger(vm, value);
	}
	catch (...) {
		lua_pushnil(vm);
	}
	return 1;
}

static int luxopen_unistd(lua_State *vm)
{
	luaL_newmetatable(vm, "UNI");
	
	lua_pushliteral(vm, "__index");
	lua_pushcfunction(vm, __index);
	lua_settable(vm, -3);

	luaL_newlib(vm, regs);
	luaL_setmetatable(vm, "UNI");
	lua_setglobal(vm, "uni");

	lua_pop(vm, 1);

	return 0;
}

int main(int argc, char **argv)
{
	if (!lux_Entry::check(ents, lengthof(ents)))
		fputs("FIXME!!!", stderr);

	lua_State *vm = luaL_newstate();
	luaL_openlibs(vm);
	luxopen_array(vm);
	luxopen_unistd(vm);
	luaL_dofile(vm, "test4.lua");
	lux_stackdump(vm);
	lua_close(vm);
}


