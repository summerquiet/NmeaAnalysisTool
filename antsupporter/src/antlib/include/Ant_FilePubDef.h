/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_FILEPUBDEF_H
#define ANT_FILEPUBDEF_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

/**
 * @brief Definition of error code
 */
enum Ant_FileError
{
    ANT_FER_NoError = 0,            // no error
    ANT_FER_InvalidParam,           // parameter is invalid
    ANT_FER_InvalidPathName,        // path name is invalid
    ANT_FER_FileNotOpen,            // file is not opened
    ANT_FER_NoAccess,               // no access right or permission denied
    ANT_FER_NotExists,              // the file or directory does not exist
    ANT_FER_AlreadyExists,          // the file or directory has already existed
    ANT_FER_IsDirectory,            // the path is a directory
    ANT_FER_IsNotDirectory,         // the path is not a directory
    ANT_FER_NoSpace,                // the disk is full, and there's no remaining space
    ANT_FER_NoMoreFile,             // there's no more file
    ANT_FER_NotEmpty,               // the directory is not empty
    ANT_FER_IsUsed,                 // the file or directory is being used by other process
    ANT_FER_NoMemory,               // there's no more memory
    ANT_FER_Unknown                 // other unknown error
};

/**
 * @brief Definition of file's open mode
 *
 * You can use one or a combination of these modes.
 * If you use a combination, they should be combined with bitwise-<i>or</i>(|).
 */
enum Ant_FileOpenMode
{
    ANT_FOM_NotOpen     = 0x0000,   // not open.
    ANT_FOM_ReadOnly    = 0x0001,   // can be used alone.
    ANT_FOM_WriteOnly   = 0x0002,   // can be used alone.
    ANT_FOM_ReadWrite   = ANT_FOM_ReadOnly | ANT_FOM_WriteOnly,
    ANT_FOM_Append      = 0x0004,   // can be used alone.
    ANT_FOM_Truncate    = 0x0008,   // can not be used alone, should have WriteOnly mode, but not Append mode.
    ANT_FOM_Text        = 0x0010,   // can not be used alone, should have ReadOnly, WriteOnly or Append mode.
    ANT_FOM_NoBuffer    = 0x0010    // can not be used alone, should have ReadOnly, WriteOnly or Append mode.
};

/**
 * @brief Type definition of file's user
 */
enum Ant_FileUser
{
    ANT_FOW_Owner,        // owner of the file
    ANT_FOW_Group,        // user's group of the file
    ANT_FOW_Other         // other user of the file
};

/**
 * @brief Definition of file's permission
 */
enum Ant_FilePermission
{
    ANT_FPM_OwnerRead    = 0x0400,    // the owner can read the file
    ANT_FPM_OwnerWrite   = 0x0200,    // the owner can write the file
    ANT_FPM_OwnerExe     = 0x0100,    // the owner can execute the file

    ANT_FPM_GroupRead    = 0x0040,    // the member in owner's group can read the file
    ANT_FPM_GroupWrite   = 0x0020,    // the member in owner's group can write the file
    ANT_FPM_GroupExe     = 0x0010,    // the member in owner's group can execute the file

    ANT_FPM_OtherRead    = 0x0004,    // other user can read the file
    ANT_FPM_OtherWrite   = 0x0002,    // other user can write the file
    ANT_FPM_OtherExe     = 0x0001,    // other user can execute the file

    ANT_FPM_Mask         = 0xFFFF,    // permission mask
};

/**
 * @brief Definition of file's type
 */
enum Ant_FileType
{
    ANT_FTP_File         = 0x010000,        // Normal file
    ANT_FTP_Directory    = 0x020000,        // Directory
    ANT_FTP_Link         = 0x040000,        // Link file
    ANT_FTP_Device       = 0x080000,        // Device
    ANT_FTP_Pipe         = 0x100000,        // Pipe file
    ANT_FTP_Socket       = 0x200000,        // Socket
    ANT_FTP_Mask         = 0xFF0000         // Type mask
};

/**
 * @brief Type definition of file's attribute
 */
enum Ant_FileAttribute
{
    ANT_FAB_Normal        = 0x01000000,    // The file is a normal file
    ANT_FAB_Hidden        = 0x02000000,    // The file is a hidden file
    ANT_FAB_System        = 0x04000000,    // The file is a system file
    ANT_FAB_NotExist      = 0x08000000,    // The file does not exist
    ANT_FAB_Mask          = 0xFF000000     // Attribute mask
};

/**
 * @brief Definition of file's flag
 */
enum Ant_FileFlag
{
    ANT_FFG_PermOwnerRead    = ANT_FPM_OwnerRead,   // the owner can read the file
    ANT_FFG_PermOwnerWrite   = ANT_FPM_OwnerWrite,  // the owner can write the file
    ANT_FFG_PermOwnerExe     = ANT_FPM_OwnerExe,    // the owner can execute the file
    ANT_FFG_PermGroupRead    = ANT_FPM_GroupRead,   // the member in owner's group can read the file
    ANT_FFG_PermGroupWrite   = ANT_FPM_GroupWrite,  // the member in owner's group can write the file
    ANT_FFG_PermGroupExe     = ANT_FPM_GroupExe,    // the member in owner's group can execute the file
    ANT_FFG_PermOtherRead    = ANT_FPM_OtherRead,   // other user can read the file
    ANT_FFG_PermOtherWrite   = ANT_FPM_OtherWrite,  // other user can write the file
    ANT_FFG_PermOtherExe     = ANT_FPM_OtherExe,    // other user can execute the file

    ANT_FFG_TypeFile         = ANT_FTP_File,        // Normal file
    ANT_FFG_TypeDirectory    = ANT_FTP_Directory,   // Directory
    ANT_FFG_TypeLink         = ANT_FTP_Link,        // Link file
    ANT_FFG_TypeDevice       = ANT_FTP_Device,      // Device
    ANT_FFG_TypePipe         = ANT_FTP_Pipe,        // Pipe file
    ANT_FFG_TypeSocket       = ANT_FTP_Socket,      // Socket

    ANT_FFG_AttrNormal       = ANT_FAB_Normal,      // The file is a normal file
    ANT_FFG_AttrHidden       = ANT_FAB_Hidden,      // The file is a hidden file
    ANT_FFG_AttrSystem       = ANT_FAB_System,      // The file is a system file
    ANT_FFG_AttrNotExist     = ANT_FAB_NotExist,    // The file does not exist

    ANT_FFG_PermsMask        = ANT_FPM_Mask,        // permission mask
    ANT_FFG_TypesMask        = ANT_FTP_Mask,        // Type mask
    ANT_FFG_AttrsMask        = ANT_FAB_Mask,        // Attribute mask

    ANT_FFG_AllInfo          = ANT_FFG_PermsMask | ANT_FFG_TypesMask | ANT_FFG_AttrsMask        // All info mask
};

/**
 * @brief Type definition of file's time
 */
enum Ant_FileTime
{
    ANT_FTM_Creation,       // File's creation time
    ANT_FTM_LastWrite,      // File's last-write time
    ANT_FTM_LastAccess      // File's last-access time
};

/**
 * @brief Definition of file's filter
 */
enum Ant_FileFilter
{
    ANT_FFT_NoFilter       = 0x0000,    // no filter

    ANT_FFT_Dirs           = 0x0001,    // the files should be directories
    ANT_FFT_Files          = 0x0002,    // the files should be normal files
    ANT_FFT_Drives         = 0x0004,    // the files should be drives
    ANT_FFT_NoSymLinks     = 0x0008,    // there should not be symbol links
    ANT_FFT_AllEntries     = ANT_FFT_Dirs | ANT_FFT_Files | ANT_FFT_Drives,     // all entries
    ANT_FFT_TypeMask       = 0x000F,    // file type mask

    ANT_FFT_Readable       = 0x0010,    // the files should be readable
    ANT_FFT_Writable       = 0x0020,    // the files should be writable
    ANT_FFT_Executable     = 0x0040,    // the files should be executable
    ANT_FFT_PermMask       = 0x0070,    // file's permission mask

    ANT_FFT_Modified       = 0x0080,    // the files should be modified
    ANT_FFT_NoHidden       = 0x0100,    // the files should not be hidden files
    ANT_FFT_NoSystem       = 0x0200,    // the files should not be system files
    ANT_FFT_AccessMask     = 0x03F0,    // file's access mask

    ANT_FFT_AllDirs        = 0x0400,    // all directories
    ANT_FFT_CaseSensitive  = 0x0800,    // the file name is case-sensitive
    ANT_FFT_NoDotAndDotDot = 0x1000     // there should not be . and ..
};

/**
 * @brief Definition of file's sort flag
 */
enum Ant_FileSortFlag
{
    ANT_FSF_NoSort         = 0x0000,    // no sort
    ANT_FSF_ByName         = 0x0001,    // sort by name
    ANT_FSF_ByTime         = 0x0002,    // sort by time
    ANT_FSF_BySize         = 0x0004,    // sort by size

    ANT_FSF_DirFirst       = 0x0010,    // the directories should be at first
    ANT_FSF_DirLast        = 0x0020     // the directories should be at last
};

/**
 * @brief Definition of file's seek mode
 */
enum Ant_FileSeekMode
{
    ANT_FSM_Begin = 0,      // the starting point is 0 (zero) or the beginning of the file.
    ANT_FSM_Current,        // the starting point is the current position of the file pointer.
    ANT_FSM_End             // the starting point is the current end-of-file position.
};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_FILEPUBDEF_H

/*---------------------------------------------------------------------------*/
/* EOF */
