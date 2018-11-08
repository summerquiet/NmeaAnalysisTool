/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_FILEDEF_H
#define ANT_FILEDEF_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#if !defined(_WIN32) && !defined(_WIN32_WCE)
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/file.h>

#if !defined(_FOR_APPLE_)
#   include <sys/statfs.h>
#else
#   include <sys/param.h>
#   include <sys/mount.h>
#endif

#include <dirent.h>

#define ANT_CONFIG_COPY_BUF_KB  32

#define ANT_FEOF                feof
#define ANT_FERROR              ferror
#define ANT_FILENO              fileno
#define ANT_OPENDIR             opendir
#define ANT_CLOSEDIR            closedir
#define ANT_READDIR             readdir
#define ANT_ACCESS              access
#define ANT_STAT                stat
#define ANT_FSTAT               fstat
#define ANT_LSTAT               lstat
#define ANT_CHMOD               chmod
#define ANT_FCHMOD              fchmod
#define ANT_UTIME               utime
#define ANT_RENAME              rename
#define ANT_LSEEK               lseek
#define ANT_FSYNC               fsync
#define ANT_MKDIR               mkdir
#define ANT_RMDIR               rmdir
#define ANT_UNLINK              unlink


extern ANTLIB_API FILE*         Ant_Safe_Fopen(const char* filename, const char* mode);
extern ANTLIB_API int           Ant_Safe_Fclose(FILE* fp);
extern ANTLIB_API size_t        Ant_Safe_Fread(void* ptr, size_t size, FILE* fp);
extern ANTLIB_API size_t        Ant_Safe_Fwrite(const void* ptr, size_t size, FILE* fp);
extern ANTLIB_API int           Ant_Safe_Fseek(FILE* fp, long offset, int whence);
extern ANTLIB_API long          Ant_Safe_Ftell(FILE* fp);
extern ANTLIB_API int           Ant_Safe_Fflush(FILE* fp);
extern ANTLIB_API int           Ant_Safe_Open(const char *filename, int flags, mode_t mode);
extern ANTLIB_API int           Ant_Safe_Open2(const char *filename, int flags);
extern ANTLIB_API int           Ant_Safe_Close(int fd);
extern ANTLIB_API ssize_t       Ant_Safe_Read(int fd, void* buf, size_t count);
extern ANTLIB_API ssize_t       Ant_Safe_Write(int fd, const void* buf, size_t count);
extern ANTLIB_API int           Ant_Safe_Truncate(const char* path, size_t len);
extern ANTLIB_API int           Ant_Safe_Ftruncate(int fd, size_t len);
extern ANTLIB_API int           Ant_Safe_Statfs(const char* path, struct statfs* sbuf);

#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
extern ANTLIB_API int           Ant_Safe_Chown(const char* path, uid_t owner, gid_t group);
extern ANTLIB_API int           Ant_Safe_Fchown(int fd, uid_t owner, gid_t group);
#endif

extern ANTLIB_API int           Ant_Safe_Flock(int fd, int operation);


#define ANT_FOPEN               Ant_Safe_Fopen
#define ANT_FCLOSE              Ant_Safe_Fclose
#define ANT_FREAD               Ant_Safe_Fread
#define ANT_FWRITE              Ant_Safe_Fwrite
#define ANT_FSEEK               Ant_Safe_Fseek
#define ANT_FTELL               Ant_Safe_Ftell
#define ANT_FFLUSH              Ant_Safe_Fflush
#define ANT_OPEN                Ant_Safe_Open
#define ANT_OPEN2               Ant_Safe_Open2
#define ANT_CLOSE               Ant_Safe_Close
#define ANT_READ                Ant_Safe_Read
#define ANT_WRITE               Ant_Safe_Write
#define ANT_TRUNCATE            Ant_Safe_Truncate
#define ANT_FTRUNCATE           Ant_Safe_Ftruncate
#define ANT_STATFS              Ant_Safe_Statfs

#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
#define ANT_CHOWN               Ant_Safe_Chown
#define ANT_FCHOWN              Ant_Safe_Fchown
#endif

#define ANT_FLOCK               Ant_Safe_Flock

#endif  // _LINUX

// Opens an empty file for writing and reding. If the file does not exist, create it.
CONST INT ANT_FILE_OPEN_MODE_RW                 = 0x00000001;
// Opens for reading. If the file does not exist or cannot be found, the fopen call fails.
CONST INT ANT_FILE_OPEN_MODE_R                  = 0x00000002;
// Opens for both reading and writing. (The file must exist.)
CONST INT ANT_FILE_OPEN_MODE_RP                 = 0x00000004;
// Opens an empty file for writing. If the file exists, its contents are destroyed.
CONST INT ANT_FILE_OPEN_MODE_W                  = 0x00000008;
// Opens an empty file for writing. If the file exists, its contents are destroyed.
CONST INT ANT_FILE_OPEN_MODE_WP                 = 0x00000010;
// Opens for writing at the end of the file (appending) without removing the EOF marker before writing new data to the file; creates the file first if it doesn't exist.
CONST INT ANT_FILE_OPEN_MODE_A                  = 0x00000020;
// Opens for writing at the end of the file (appending) without removing the EOF marker before writing new data to the file and for reading; creates the file first if it doesn't exist.
CONST INT ANT_FILE_OPEN_MODE_AP                 = 0x00000040;
// The file can be shared access.
CONST INT ANT_FILE_OPEN_MODE_SH                 = 0x00000080;

// Common
const ANT_ERROR  ANT_NOERROR                     = 0x00000000;
const ANT_ERROR  ANT_TRANSMIT_FAILED             = 0x00000001;
const ANT_ERROR  ANT_TIMEOUT                     = 0x00000002;
const ANT_ERROR  ANT_ERROR_PARAMETER             = 0x00000003;
const ANT_ERROR  ANT_INVALID                     = 0x0fffffff;

//  MAX 0x20004fff
const ANT_ERROR ANT_ERROR_FILE_NOERROR           = 0x00000000;
const ANT_ERROR ANT_ERROR_FILE_DEVICE_ERR        = 0x20005600;
const ANT_ERROR ANT_ERROR_FILE_CRC_ERR           = 0x20005601;
const ANT_ERROR ANT_ERROR_FILE_NOTFOUND          = 0x20005602;
const ANT_ERROR ANT_ERROR_FILE_OTHER             = 0x20005603;
const ANT_ERROR ANT_ERROR_FILE_ATA_NOTOPEN       = 0x20005604;
const ANT_ERROR ANT_ERROR_FILE_DEV_NOTREADY      = 0x20005605;
const ANT_ERROR ANT_ERROR_FILE_ALREADY_EXISTS    = 0x20005606;
const ANT_ERROR ANT_ERROR_FILE_NO_MORE_FILES     = 0x20005607;
const ANT_ERROR ANT_ERROR_FILE_DISK_FULL         = 0x20005608;

const ANT_ERROR ANT_ERROR_FILE_IO_ERR            = 0x20005609;   // An low-level I/O error occurred.
const ANT_ERROR ANT_ERROR_FILE_IS_DIR            = 0x2000560A;   // Pathname refers to a directory.
const ANT_ERROR ANT_ERROR_FILE_ACCESS            = 0x2000560B;   // Permission denied.
const ANT_ERROR ANT_ERROR_FILE_NOT_DIR           = 0x2000560C;   // A component of the path prefix is not a directory.
const ANT_ERROR ANT_ERROR_FILE_NO_MEM            = 0x2000560D;   // Out of memory.
const ANT_ERROR ANT_ERROR_FILE_BUSY              = 0x2000560E;   // The file or directory is in use.

const DWORD ANT_FILE_WIN32API_NAME_SIZE         = 64;

struct Ant_FileErrInfo {

    DWORD   dwErrCode;
    TCHAR   szPath[MAX_PATH +1];
    CHAR    szFunctionName[ANT_FILE_WIN32API_NAME_SIZE];
    DWORD   dwLine;
};

#define ANT_FILE_WRITE_FLUSH                    (0x00000000)    // Write & Flush
#define ANT_FILE_WRITE_ONLY                     (0x00000001)    // Write Only (no flush)
#define ANT_FILE_NO_CHECKSUM                    (0x00000002)    // No CheckSum (NFileSafe)

#if defined(_WIN32) || defined(_WIN32_WCE)
#define ANT_INVALID_FILE_HANDLE                 INVALID_HANDLE_VALUE
#else
#define ANT_INVALID_FILE_HANDLE                 ((INT)-1)
#endif

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_FILEDEF_H

/*---------------------------------------------------------------------------*/
/* EOF */
