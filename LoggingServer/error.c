#include "cs.h"
extern int errno;
//return string of last system call error code
char * sys_err_str( void ){
	return strerror(errno);
}
