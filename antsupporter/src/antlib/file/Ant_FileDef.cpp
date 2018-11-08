/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_FILEDEF_H
#   include "Ant_FileDef.h"
#endif
#include <errno.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
#if !defined(_WIN32) && !defined(_WIN32_WCE)

ANTLIB_API FILE* Ant_Safe_Fopen(const char* filename, const char* mode)
{
    FILE* fp = NULL;
    do {
        fp = fopen(filename, mode);
    } while ((NULL == fp) && (EINTR == errno));
    return fp;
}

ANTLIB_API int Ant_Safe_Fclose(FILE* fp)
{
    if (NULL == fp) {
        return EOF;
    }

    int ret = EOF;
    do {
        ret = fclose(fp);
    } while ((EOF == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API size_t Ant_Safe_Fread(void* ptr, size_t size, FILE* fp)
{
    size_t ret = 0;
    do {
        ret = fread(ptr, (size_t)1, size, fp);
    } while ((size > ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API size_t Ant_Safe_Fwrite(const void* ptr, size_t size, FILE* fp)
{
    size_t ret = 0;
    do {
        ret = fwrite(ptr, (size_t)1, size, fp);
    } while ((size > ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Fseek(FILE* fp, long offset, int whence)
{
    int ret = -1;
    do {
        ret = fseek(fp, offset, whence);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API long Ant_Safe_Ftell(FILE* fp)
{
    long ret = -1;
    do {
        ret = ftell(fp);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Fflush(FILE* fp)
{
    int ret = -1;
    do {
        ret = fflush(fp);
    } while ((EOF == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Open(const char *filename, int flags, mode_t mode)
{
    int ret = -1;
    do {
        ret = open(filename, flags, mode);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Open2(const char *filename, int flags)
{
    int ret = -1;
    do {
        ret = open(filename, flags);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Close(int fd)
{
    int ret = -1;
    do {
        ret = close(fd);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API ssize_t Ant_Safe_Read(int fd, void* buf, size_t count)
{
    int ret = -1;
    do {
        ret = read(fd, buf, count);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API ssize_t Ant_Safe_Write(int fd, const void* buf, size_t count)
{
    int ret = -1;
    do {
        ret = write(fd, buf, count);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Truncate(const char* path, size_t len)
{
    int ret = -1;
    do {
        ret = truncate(path, len);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Ftruncate(int fd, size_t len)
{
    int ret = -1;
    do {
        ret = ftruncate(fd, len);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Statfs(const char* path, struct statfs* sbuf)
{
    int ret = -1;
    do {
        ret = statfs(path, sbuf);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Chown(const char* path, uid_t owner, gid_t group)
{
    int ret = -1;
    do {
        ret = chown(path, owner, group);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Fchown(int fd, uid_t owner, gid_t group)
{
    int ret = -1;
    do {
        ret = fchown(fd, owner, group);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

ANTLIB_API int Ant_Safe_Flock(int fd, int operation)
{
    int ret = -1;
    do {
        ret = flock(fd, operation);
    } while ((-1 == ret) && (EINTR == errno));
    return ret;
}

#endif  // !defined(_WIN32) && !defined(_WIN32_WCE)

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
