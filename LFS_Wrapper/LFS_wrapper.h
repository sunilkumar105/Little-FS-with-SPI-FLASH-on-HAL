/*
 * LFS_wrapper.h
 *
 *  Created on: Feb 28, 2025
 *      Author: sunil
 */

#ifndef LITTLEFS_WRAPPER_H
#define LITTLEFS_WRAPPER_H

#include "lfs.h"
#include <stdbool.h>
#include <stddef.h>

// Assuming `littlefs` is already initialized and mounted
extern lfs_t littlefs;
extern struct lfs_config littlefs_config;

// Function prototypes

/*
 * Read and print storage details. it prints total size, free size and occupied size in MB
 * */
void readAndPrintStorageDetails(void);

/*
 * It lists and print all files availabel in the SPI Flash and its size
 * */
void listFiles(void);

/*
 * Format the flash
 * */
void formatFlash(void);

/**
 * Save data into a file in LittleFS.
 * @param fileName: Name of the file to save data into.
 * @param data: Pointer to the data to be written.
 * @param dataSize: Size of the data to be written.
 * @param ret_BytesWritten: Pointer to store the number of bytes written.
 * @return: true if successful, false otherwise.
 */
bool saveFileIntoFlash(const char*fileName, const void*data, size_t dataSize,
			  size_t*ret_BytesWritten);

/**
 * Get the size of a file in LittleFS.
 * @param fileName: Name of the file.
 * @param ret_FileSize: Pointer to store the file size.
 * @return: true if successful, false otherwise.
 */
bool getFileSize(const char*fileName, size_t*ret_FileSize);

/**
 * Read data from a file in LittleFS.
 * @param fileName: Name of the file to read from.
 * @param bytesToRead: Number of bytes to read.
 * @param ret_DataBuffer: Buffer to store the read data.
 * @param ret_DataSize: Pointer to store the number of bytes read.
 * @return: true if successful, false otherwise.
 */
bool readFilefromFlash(const char*fileName, size_t bytesToRead,
		       char*ret_DataBuffer, size_t*ret_DataSize);

/**
 * Append data to the end of a file in LittleFS.
 * @param fileName: Name of the file to append data to.
 * @param dataBuffer: Pointer to the data to append.
 * @param fileSizeToWrite: Size of the data to append.
 * @param ret_bytesWritten: Pointer to store the number of bytes written.
 * @return: true if successful, false otherwise.
 */
bool appendDataAtTheEndOfFile(const char*fileName, const char*dataBuffer,
			      size_t fileSizeToWrite, size_t*ret_bytesWritten);

/**
 * Delete a file from LittleFS.
 * @param fileName: Name of the file to delete.
 * @return: true if successful, false otherwise.
 */
bool deleteFilefromFlash(const char*fileName);

/*
 * Read single line from the flash
 * */
char* lfs_gets(char*buf, int size, lfs_file_t*file);

/*
 * check if the file exists or not
 * */
bool fileExists(const char*filePath);

/********* EXAMPLE TEST OF FILE SYSTEM *********/
/*
 const char *data = "Hello, LittleFS!";
 size_t bytes_written;
 if (saveFileIntoLittleFS("test.txt", data, strlen(data), &bytes_written)) {
 printf("Data written successfully\n");
 }

 size_t file_size;
 if (getFileSize("test.txt", &file_size)) {
 printf("File size: %u bytes\n", file_size);
 }

 char buffer[128];
 size_t bytes_read;
 if (readFilefromFlash("test.txt", sizeof(buffer), buffer, &bytes_read)) {
 printf("File content: %s\n", buffer);
 }

 const char *append_data = " This is appended text.";
 if (appendDataAtTheEndOfFile("test.txt", append_data, strlen(append_data),
 &bytes_written)) {
 printf("Data appended successfully\n");
 }

 if (deleteFilefromFlash("test.txt")) {
 printf("File deleted successfully\n");
 }
 */
/**********************************************/

#endif // LITTLEFS_WRAPPER_H
