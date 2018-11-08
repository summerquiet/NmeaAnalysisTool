/**
 * Copyright @ 2017 - 2018
 * All Rights Reserved.
 *
 */

#ifndef CXX_NMEADECODEFACTORY_H
#define CXX_NMEADECODEFACTORY_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
//  Include files

/*---------------------------------------------------------------------------*/
// Class define

class CNmeaDecodeObject;

/**
 * CNmeaDecodeFactory class
 *
 * NMEA decode process factory abstraction
 */
class CNmeaDecodeFactory
{
public:

    /**
     * Get instance of the factory abstraction
     *
     * @param  NONE
     *
     * @return CNmeaDecodeFactory* an abstract pointer
     */
    static CNmeaDecodeFactory* Instance(VOID);

    /**
     * Destroy the factory interface instance
     *
     * @param  NONE
     *
     * @return NONE
     */
    static VOID Destroy(VOID);

    /**
     * Create the NMEA recode object
     *
     * @param   pchRecode           [IN]: the NMEA recode in one line
     *
     * @return  CNmeaDecodeObject*  : new pointer to a NMEA decode object
     */
    virtual CNmeaDecodeObject* CreateNmeaDecodeObejct(const CHAR *pStrData) = 0;

    /**
     * Destroy the NMEA recode object
     *
     * @param   pcNmeaDecodeObject  [IN]: need to destroy object
     *
     * @return  NONE
     */
    virtual VOID DestroyNmeaDecodeObejct(CNmeaDecodeObject* pcNmeaDecodeObject) = 0;

protected:

    /**
     * Constructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    CNmeaDecodeFactory();

    /**
    * Destructor
    *
    * @param  NONE
    *
    * @return NONE
    */
    virtual ~CNmeaDecodeFactory();

protected:

    INT     m_iObjectCount;     // object counter
};

/**
 * CNmeaDecodeConcreteFactory class
 *
 * NMEA decode process concrete factory
 */
class CNmeaDecodeConcreteFactory : public CNmeaDecodeFactory
{
public:

    /**
     * Get the factory instance
     *
     * @param  NONE
     *
     * @return CNmeaDecodeConcreteFactory* the pointer
     */
    static CNmeaDecodeConcreteFactory* Instance(VOID);

    /**
     * Destroy the factory instance
     *
     * @param  NONE
     *
     * @return NONE
     */
    static VOID Destroy(VOID);

    /**
     * Create the NMEA recode object
     *
     * @param   pchRecode           [IN]: the NMEA recode in one line
     *
     * @return  CNmeaDecodeObject*  : new pointer to a NMEA decode object
     */
    virtual CNmeaDecodeObject* CreateNmeaDecodeObejct(const CHAR *pStrData);

    /**
     * Destroy the NMEA recode object
     *
     * @param   pcNmeaDecodeObject  [IN]: need to destroy object
     *
     * @return  NONE
     */
    virtual VOID DestroyNmeaDecodeObejct(CNmeaDecodeObject* pcNmeaDecodeObject);

protected:

    /**
     * Constructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    CNmeaDecodeConcreteFactory();

    /**
     * Destructor
     *
     * @param  NONE
     *
     * @return NONE
     */
    virtual ~CNmeaDecodeConcreteFactory();

private:

    // Static member to recode instance pointer
    static CNmeaDecodeConcreteFactory*  s_pcInstance;
};

/*---------------------------------------------------------------------------*/
#endif  // CXX_NMEADECODEFACTORY_H

/*---------------------------------------------------------------------------*/
/* EOF */
