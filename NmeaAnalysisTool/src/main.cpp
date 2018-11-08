/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef CXX_NMEADECODEIF_H
#   include "NmeaDecodeIF.h"
#endif

/*---------------------------------------------------------------------------*/
// Value define

static const INT    PATH_MAX = 1024;
static const CHAR*  OUTPUT_FLIE_NAME = "NMEA_used_satellite_SNR.csv";
static const CHAR*  OUTPUT_FILE_HEAD = "Date,Time,No1,SNR,No2,SNR,No3,SNR,No4,SNR,No5,SNR,No6,SNR,No7,SNR,No8,SNR,No9,SNR,No10,SNR,No11,SNR,No12,SNR\n";

/*---------------------------------------------------------------------------*/
// Function declare

VOID OutputSatelliteInfoToCSV(const GPS_DATA& sGpsData, FILE* pOutputFile);

/*---------------------------------------------------------------------------*/
// Main
int main(int argc, char* argv[])
{
    // Check input
    if (argc != 2) {

        printf("Please input NMEA log file name");
        return -1;
    }

    // Get Working folder
    CHAR working_folder[PATH_MAX];
    getcwd(working_folder, sizeof(working_folder));

    // Try to open NMEA log file
    CHAR input_full_path[PATH_MAX];
    sprintf(input_full_path, "%s\\%s", working_folder, argv[1]);

    FILE* pInputFile = fopen(input_full_path, "rt");
    if (pInputFile == NULL) {
        printf("Open NMEA log[%s] error\n", pInputFile);
        return -1;
    }

    // Try to open output file
    CHAR output_full_path[PATH_MAX];
    sprintf(output_full_path, "%s\\%s", working_folder, OUTPUT_FLIE_NAME);

    FILE* pOutputFile = fopen(output_full_path, "wt+");
    if (pOutputFile == NULL) {
        fclose(pInputFile);
        printf("Open output csv[%s] error\n", pOutputFile);
        return -1;
    }

    // Write output csv file head
    fwrite(OUTPUT_FILE_HEAD, strlen(OUTPUT_FILE_HEAD), 1, pOutputFile);

    // Read data from input file
    static const INT READ_BUFFER_SIZE = 1024;
    CHAR buffer[READ_BUFFER_SIZE] = {0};

    GPS_DATA sGpsData;
    memset(&sGpsData, 0, sizeof(sGpsData));

    while (!feof(pInputFile)) {
        // Read lines from file
        fgets(buffer, READ_BUFFER_SIZE, pInputFile);

        // Send NMEA data to decode
        if (CNmeaDecodeIF::Instance()->ReceiveNmeaInfo(NMEA_RECEIVE_SOURCE_OTHER, buffer, strlen(buffer))) {

            // Try to get GPS data
            if (CNmeaDecodeIF::Instance()->GetGpsData(sGpsData)) {
                // Output
                OutputSatelliteInfoToCSV(sGpsData, pOutputFile);
            }
        }
    }

    // Release resource
    CNmeaDecodeIF::Destroy();

    // Close file
    fclose(pInputFile);
    fclose(pOutputFile);

    return 0;
}

/*---------------------------------------------------------------------------*/
// Output satellite information to CSV
VOID OutputSatelliteInfoToCSV(const GPS_DATA& sGpsData, FILE* pOutputFile)
{
    CHAR str[2048];
    memset(&str, 0, sizeof(str));

    INT iPos = 0;

    sprintf(str + iPos, "%04d/%02d/%02d,%02d:%02d:%02d,",
        sGpsData.sUtcTime.wYear,
        sGpsData.sUtcTime.wMonth,
        sGpsData.sUtcTime.wDay,
        sGpsData.sUtcTime.wHour,
        sGpsData.sUtcTime.wMinute,
        sGpsData.sUtcTime.wSecond);
    iPos = strlen(str);

    for (INT i = 0; i < MAX_GPS_SATELLITE; i++) {
        if (sGpsData.sSatelliteInfo[i].bUsed) {
            sprintf(str + iPos, "%d,%4.2f,",sGpsData.sSatelliteInfo[i].bySatelliteID, sGpsData.sSatelliteInfo[i].fSNR);
            iPos = strlen(str);
        }

        // Check finished
        if (!sGpsData.sSatelliteInfo[i].bValid) {
            break;
        }
    }

    str[iPos - 1] = '\n';

    // Try to write data to file
    fwrite(str, iPos, 1, pOutputFile);
}

/*---------------------------------------------------------------------------*/
/* EOF */
