/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ERRORCORE_H
#define ANT_ERRORCORE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_ERRORDEF_H
#   include "Ant_ErrorDef.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

class Ant_VolatileMemory;

/**
 * @brief Error core class
 *
 */
class Ant_ErrorCore : public Ant_Object
{
public:

    /**
     * @brief Constructor
     *
     */
    Ant_ErrorCore();

    /**
     * @brief Destructor
     *
     */
    virtual ~Ant_ErrorCore();

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
    VOID outputError(const CHAR* pkszFileInfo, DWORD dwLineNo, LONG iErrorCode, DWORD dwOption, AntErrorType eErrType);

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
    VOID outputError(const WCHAR* pkszFileInfo, DWORD dwLineNo, LONG iErrorCode, DWORD dwOption, AntErrorType eErrType);

    /**
     * @brief Start Ant_ErrorCore
     *
     * @param[IN] CHAR*  szMemoryNameArray[] 
     - szMemoryNameArray[0]: "ANT_ERR_RECOVERY"
     - szMemoryNameArray[1]: "ANT_ERR_DEBUG"
     - szMemoryNameArray[2]: "ANT_ERR_FATAL"
     * @param[IN] INT iArraySize:  Array szMemoryNameArray's size
     * @param[IN] DWORD dwRecNum:    Max record num for each error kind
     * @return VOID
     */
    VOID start(const XCHAR* szMemName[], INT iMemNum, DWORD dwRecNum);

    /**
     * @brief Stop Ant_ErrorCore
     *
     * @param null
     * @return VOID
     */
    VOID stop();

    /**
    * @brief Get error information from memory
    *
    * @param[IN] AntErrorType eErrType: Error type
    - ANT_ERROR_ERROR
    - ANT_ERROR_DEBUG
    - ANT_ERROR_FATAL
    * @param[OUT] ErrorHeader& sErrHeader: Error data header
    * @param[OUT] ErrorRecord*  pErrRecord: Error record data
    * @param[IN] DWORD dwRecNum: Get error record num
    * @return BOOL: Whether getting error information is successful?
    * @retval TRUE: Successful
    * @retval FALSE: Failure
    */
    BOOL getErrorInfo(AntErrorType eType, ErrorHeader& sErrorHeader, ErrorRecord* pErrorRecord, DWORD dwRecNum);


    /**
    * @brief Clear all error information from memory
    *
    * @param null
    * @return BOOL: Whether clear successfully error information from memory?
    * @retval TRUE: Successful
    * @retval FALSE:Failure
    */
    BOOL clearAllErrorInfo();

    /**
     * InitMemory
     *
     * @param eErrType
     * @param *pbyErrMem
     * @return BOOL
     */
    BOOL initMemory(AntErrorType eErrType , BYTE *pbyErrMem);

protected:

    /**
     * @brief Write error information to memory
     *
     * @param[IN] const CHAR*  pkszFileInfo: File information
     * @param[IN] DWORD dwLineNo: Line no
     * @param[IN] LONG iErrorCode: Error code
     * @param[IN] DWORD dwOption: Option data
     * @param[OUT] BYTE* pbyErrBuf: The pointer to memory which is used to save error information
     * @return VOID
     */
    VOID writeError(const CHAR * pkszFileInfo, DWORD dwLineNo, LONG iErrorCode, DWORD dwOption, BYTE* pbyErrBuf);

    /**
    * @brief Write error information to memory
    *
    * @param[IN] const CHAR*  pkszFileInfo: File information
    * @param[IN] DWORD dwLineNo: Line no
    * @param[IN] LONG iErrorCode: Error code
    * @param[IN] DWORD dwOption: Option data
    * @param[OUT] BYTE* pbyErrBuf: The pointer to memory which is used to save error information
    * @return VOID
    */
    VOID writeError(const WCHAR * pkszFileInfo, DWORD dwLineNo, LONG iErrorCode, DWORD dwOption, BYTE* pbyErrBuf);

private:

    Ant_VolatileMemory* m_pcErrMemory[ANT_ERROR_TYPE_NUM];  // Memory list
    DWORD               m_dwMaxRecNum;                      // Max record number

private:

    // Disable the copy constructor and operator =
    Ant_ErrorCore(const Ant_ErrorCore& src);
    Ant_ErrorCore& operator = (const Ant_ErrorCore& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_ERRORCORE_H

/*---------------------------------------------------------------------------*/
/* EOF */
