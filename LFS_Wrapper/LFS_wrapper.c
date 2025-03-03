/*
 * LFS_wrapper.c
 *
 *  Created on: Feb 28, 2025
 *      Author: sunil
 */

#include "LFS_wrapper.h"
#include <stdio.h>
#include <string.h>

// Save data into a file in LittleFS
bool saveFileIntoFlash(const char *fileName, const void *data, size_t dataSize,
                       size_t *ret_BytesWritten)
{
    lfs_file_t file;
    int err = lfs_file_open(&littlefs, &file, fileName,
                            LFS_O_WRONLY | LFS_O_CREAT);
    if (err < 0)
    {
        printf("Failed to open file for writing: %d\n", err);
        return false;
    }

    lfs_ssize_t bytes_written = lfs_file_write(&littlefs, &file, data,
                                               dataSize);
    if (bytes_written < 0)
    {
        printf("Failed to write to file: %d\n", (int)bytes_written);
        lfs_file_close(&littlefs, &file);
        return false;
    }

    lfs_file_close(&littlefs, &file);
    if (ret_BytesWritten)
    {
        *ret_BytesWritten = (size_t)bytes_written;
    }
    //    printf("Successfully wrote %u bytes to %s\n", (size_t)bytes_written, fileName);
    return true;
}

// Get the size of a file in LittleFS
bool getFileSize(const char *fileName, size_t *ret_FileSize)
{
    lfs_file_t file;
    int err = lfs_file_open(&littlefs, &file, fileName, LFS_O_RDONLY);
    if (err < 0)
    {
        printf("Failed to open file for reading: %d\n", err);
        return false;
    }

    lfs_soff_t file_size = lfs_file_size(&littlefs, &file);
    if (file_size < 0)
    {
        printf("Failed to get file size: %d\n", (int)file_size);
        lfs_file_close(&littlefs, &file);
        return false;
    }

    lfs_file_close(&littlefs, &file);
    if (ret_FileSize)
    {
        *ret_FileSize = (size_t)file_size;
    }
    //    printf("File size of %s: %u bytes\n", fileName, (size_t)file_size);
    return true;
}

// Read data from a file in LittleFS
bool readFilefromFlash(const char *fileName, size_t bytesToRead,
                       char *ret_DataBuffer, size_t *ret_DataSize)
{
    lfs_file_t file;
    int err = lfs_file_open(&littlefs, &file, fileName, LFS_O_RDONLY);
    if (err < 0)
    {
        printf("Failed to open file for reading: %d\n", err);
        return false;
    }

    lfs_ssize_t bytes_read = lfs_file_read(&littlefs, &file, ret_DataBuffer,
                                           bytesToRead);
    if (bytes_read < 0)
    {
        printf("Failed to read from file: %d\n", (int)bytes_read);
        lfs_file_close(&littlefs, &file);
        return false;
    }

    lfs_file_close(&littlefs, &file);
    if (ret_DataSize)
    {
        *ret_DataSize = (size_t)bytes_read;
    }
    //    printf("Successfully read %u bytes from %s\n", (size_t)bytes_read, fileName);
    return true;
}

// Append data to the end of a file in LittleFS
bool appendDataAtTheEndOfFile(const char *fileName, const char *dataBuffer,
                              size_t fileSizeToWrite, size_t *ret_bytesWritten)
{
    lfs_file_t file;
    int err = lfs_file_open(&littlefs, &file, fileName,
                            LFS_O_WRONLY | LFS_O_APPEND | LFS_O_CREAT);
    if (err < 0)
    {
        printf("Failed to open file for appending: %d\n", err);
        return false;
    }

    /*if the length of the file of 0, then normall write, else write in new line*/
    char *bufferToWrite = calloc((fileSizeToWrite + 1), sizeof(char));
    size_t fileSize = 0;
    bool status = getFileSize(fileName, &fileSize);
    if (!status)
    {
        printf("[ ERROR ] reading file size of %s, While appending file",
               fileName);
        free(bufferToWrite);
        return false;
    }
    if (fileSize == 0)
    {
        sprintf(bufferToWrite, "%s", dataBuffer);
    }
    else
    {
        sprintf(bufferToWrite, "\n%s", dataBuffer);
        fileSizeToWrite++; // added with \n
    }

    lfs_ssize_t bytes_written = lfs_file_write(&littlefs, &file, bufferToWrite,
                                               fileSizeToWrite);
    if (bytes_written < 0)
    {
        printf("Failed to append to file: %d\n", (int)bytes_written);
        lfs_file_close(&littlefs, &file);
        free(bufferToWrite);
        return false;
    }
    free(bufferToWrite);

    lfs_file_close(&littlefs, &file);
    if (lfs_file_close)
    {
        *ret_bytesWritten = (size_t)bytes_written;
    }
    //    printf("Successfully appended %u bytes to %s\n", (size_t)bytes_written, fileName);
    return true;
}

// Delete a file from LittleFS
bool deleteFilefromFlash(const char *fileName)
{
    int err = lfs_remove(&littlefs, fileName);
    if (err < 0)
    {
        printf("Failed to delete file: %d\n", err);
        return false;
    }

    //    printf("Successfully deleted %s\n", fileName);
    return true;
}

// Function to read and print storage details
void readAndPrintStorageDetails(void)
{
    // Calculate used bytes in the filesystem
    lfs_ssize_t used_bytes = lfs_fs_size(&littlefs);
    // Calculate total bytes in the filesystem
    lfs_ssize_t total_bytes = littlefs_config.block_count * littlefs_config.block_size;

    total_bytes = total_bytes / 1024;
    used_bytes = used_bytes / 1024;

    // Print storage details
    printf("Storage Details:\n");
    printf("  Used MB: %ld\n", (long)used_bytes);
    printf("  Total MB: %ld\n", (long)total_bytes);
    printf("  Free MB: %ld\n", (long)(total_bytes - used_bytes));
}

// Function to list files with their sizes
void listFiles(void)
{
    lfs_dir_t dir;
    struct lfs_info info;

    // Open the root directory
    int err = lfs_dir_open(&littlefs, &dir, "/");
    if (err)
    {
        printf("Failed to open directory: %d\n", err);
        return;
    }

    printf("Files in LittleFS:\n");
    // Iterate through the directory entries
    while (true)
    {
        int res = lfs_dir_read(&littlefs, &dir, &info);
        if (res < 0)
        {
            printf("Failed to read directory: %d\n", res);
            break;
        }

        // End of directory
        if (res == 0)
        {
            break;
        }

        // Print file name and size
        printf("  %s (size: %ld bytes)\n", info.name, (long)info.size);
    }

    // Close the directory
    lfs_dir_close(&littlefs, &dir);
}

// Function to format the flash
void formatFlash(void)
{
    // Format the LittleFS filesystem
    int err = lfs_format(&littlefs, &littlefs_config);
    if (err)
    {
        printf("Failed to format flash: %d\n", err);
    }
    else
    {
        printf("Flash formatted successfully.\n");
    }
}

// Custom function to read a line from a LittleFS file
char *lfs_gets(char *buf, int size, lfs_file_t *file)
{
    if (size < 1)
    {
        return NULL; // Buffer size must be at least 1
    }

    int i = 0;
    while (i < size - 1)
    {
        // Read one character at a time
        int res = lfs_file_read(&littlefs, file, &buf[i], 1);
        if (res < 1)
        {
            // End of file or read error
            if (i == 0)
            {
                return NULL; // No data read
            }
            break;
        }

        // Check for newline character
        if (buf[i] == '\n')
        {
            i++; // Include the newline in the buffer
            break;
        }

        i++;
    }

    // Null-terminate the string
    buf[i] = '\0';
    return buf;
}

/*check if the file exists*/
bool fileExists(const char *filePath)
{
    struct lfs_info info; // Struct to hold file/directory metadata

    // Use lfs_stat to check if the file exists
    int result = lfs_stat(&littlefs, filePath, &info);

    // If lfs_stat returns 0, the file exists
    if (result == 0)
    {
        return true;
    }

    // If lfs_stat returns LFS_ERR_NOENT, the file does not exist
    if (result == LFS_ERR_NOENT)
    {
        return false;
    }

    // Handle other errors (optional)
    // For example, you could log the error or return false
    return false;
}
