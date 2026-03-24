
#define __U32_TYPE unsigned int

/*-------------------------------*/

#define __UID_T_TYPE            __U32_TYPE
#define __GID_T_TYPE            __U32_TYPE
#define __MODE_T_TYPE           __U32_TYPE

#define __MY_TIME_T_TYPE           long int

/*-------------------------------*/

/* No need to mark the typedef with __extension__.   */
# define __STD_TYPE             typedef

__STD_TYPE __UID_T_TYPE __uid_t;        /* Type of user identifications.  */
__STD_TYPE __GID_T_TYPE __gid_t;        /* Type of group identifications.  */
__STD_TYPE __MODE_T_TYPE __mode_t;      /* Type of file attribute bitmasks.  */

__STD_TYPE __MY_TIME_T_TYPE __time_t;      /* Seconds since the Epoch.  */

/*-------------------------------*/

#ifndef __time_t_defined
#define __time_t_defined 1

//#include <bits/types.h>

/* Returned by `time'.  */
typedef __time_t time_t;
#endif

#ifndef __gid_t_defined
typedef __gid_t gid_t;
# define __gid_t_defined
#endif


#ifndef __mode_t_defined
typedef __mode_t mode_t;
# define __mode_t_defined
#endif


#ifndef __uid_t_defined
typedef __uid_t uid_t;
# define __uid_t_defined
#endif

