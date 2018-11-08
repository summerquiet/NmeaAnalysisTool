/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ERROR_H
#define ANT_ERROR_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif
#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif
#ifndef ANT_ERRORDEF_H
#   include "Ant_ErrorDef.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

class Ant_ErrorCore;

/**
 * @brief Write error information to memory class
 *
 */
class ANTLIB_API Ant_Error : public virtual Ant_Object
{
public:

    /**
     * @brief Start Ant_Error
     *
     * @param[IN] CHAR*  szMemoryNameArray[]
     - szMemoryNameArray[0]: "ANT_ERR_RECOVERY"
     - szMemoryNameArray[1]: "ANT_ERR_DEBUG"
     - szMemoryNameArray[2]: "ANT_ERR_FATAL"
     * @param[IN] INT iArraySize:   Array szMemoryNameArray's size
     * @param[IN] DWORD dwRecNum:   Max record num for each error kind
     * @return VOID
     */
    static VOID start(const XCHAR* szMemoryNameArray[], INT iArraySize, DWORD dwRecNum);

    /**
     * @brief Stop Ant_Error
     *
     * @param null
     * @return VOID
     */
    static VOID stop();

    /**
     * @brief Output error information to memory
     *
     * @param[IN] const CHAR*  szFileInfo: File information
     * @param[IN] DWORD dwLineNo: Line no
     * @param[IN] LONG lErrCode: Error code
     * @param[IN] DWORD dwOption: Option data
     * @param[IN] AntErrorType eErrType: Error type
     - ANT_ERROR_ERROR
     - ANT_ERROR_DEBUG
     - ANT_ERROR_FATAL
     * @return VOID
     */
    static VOID outputError(const CHAR* szFileInfo, DWORD dwLineNo, LONG lErrCode, DWORD dwOption, AntErrorType eErrType);

    /**
     * @brief Output error information to memory
     *
     * @param[IN] const CHAR*  szFileInfo: File information
     * @param[IN] DWORD dwLineNo: Line no
     * @param[IN] LONG lErrCode: Error code
     * @param[IN] DWORD dwOption: Option data
     * @param[IN] AntErrorType eErrType: Error type
     - ANT_ERROR_ERROR
     - ANT_ERROR_DEBUG
     - ANT_ERROR_FATAL
     * @return VOID
     */
    static VOID outputError(const WCHAR* szFileInfo, DWORD dwLineNo, LONG lErrCode, DWORD dwOption, AntErrorType eErrType);

    /**
     * @brief Get error information from memory
     *
     * @param[IN] AntErrorType eErrType: Error type
     - ANT_ERROR_ERROR
     - ANT_ERROR_DEBUG
     - ANT_ERROR_FATAL
     * @param[OUT] Ant_ErrorHeader& sErrHeader: Error data header
     * @param[OUT] Ant_ErrorRecord*  pErrRecord: Error record data
     * @param[IN] DWORD dwRecNum: Get error record num
     * @return BOOL: Whether getting error information is successful?
     * @retval TRUE: Successful
     * @retval FALSE: Failure
     */
    static BOOL getErrorInfo(AntErrorType eErrType, ErrorHeader& sErrHeader, ErrorRecord* pErrRecord, DWORD dwRecNum);

    /**
     * @brief Clear all error information from memory
     *
     * @param null
     * @return BOOL: Whether clear successfully error information from memory
     * @retval TRUE: Successful
     * @retval FALSE:Failure
     */
    static BOOL clearAllErrorInfo();

    /**
     * InitMemory
     *
     * @param eErrType
     * @param *pbyErrMem
     * @return static BOOL
     */
    static BOOL initMemory(AntErrorType eErrType, BYTE* pbyErrMem);

private:

    /**
     * constructor
     */
    Ant_Error();

    /**
     * destruct
     */
    ~Ant_Error() {};

    /**
     * Is Start
     */
    static BOOL isStart();

private:

    static Ant_ErrorCore*   s_pcErrorCore;  // The pointer to error core
    static BOOL             s_bStartFlag;   // Ant_Error start flag
    static Ant_SyncObj      s_cSyncOject;   // Synchronization object

};

/**
 * @brief Record recoverable errors macro
 *
 * If defined file info, it can automatically append the file info and line number.\n
 * <b>You should use this macro instead using Ant_Error::outputError function directly.</b>
 */
#define ErrorLog(ErrorCode, dwOption)   Ant_Error::outputError(__FILE__, __LINE__, ErrorCode, dwOption, ANT_ERROR_ERROR)

/**
 * @brief Record fatal errors macro
 *
 * If defined file info, it can automatically append the file info and line number.\n
 * <b>You should use this macro instead using Ant_Error::outputError function directly.</b>
 */
#define FatalLog(ErrorCode, dwOption)   Ant_Error::outputError(__FILE__, __LINE__, ErrorCode, dwOption, ANT_ERROR_FATAL)

/**
 * @brief Record debug logs macro
 *
 * If defined file info, it can automatically append the file info and line number.\n
 * <b>You should use this macro instead using Ant_Error::outputError function directly.</b>
 */
#define DebugLog(ErrorCode, dwOption)   Ant_Error::outputError(__FILE__, __LINE__, ErrorCode, dwOption, ANT_ERROR_DEBUG)

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_ERROR_H

/*---------------------------------------------------------------------------*/
/* EOF */
