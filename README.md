# Final Library and Usage

## 1. Enable SPI and CS

1. Enable SPI in motorola 8 bits and speed should be nearly 1MBPS.
2. Enable CS pin as GPIO output.

## 2. Copy These following folders into the STM32 workspace

1. LFS
2. LFS_LLD
3. LFS_Wrapper
4. w25qxx

## 3. In the `MAIN.C`

1. Include following header

   ```cpp
   #include "w25qxx.h"
   #include "LFS_wrapper.h"
   #include "w25qxx_littlefs.h"
   ```

2. Declare this variable in the global scope

   ```cpp
   W25QXX_HandleTypeDef W25Q128_Details = { 0 };
   ```

3. Copy the following function

   ```cpp
   bool initLittle_FS(void){
       W25Q128_Details.spiHandle = &hspi1;
       W25Q128_Details.cs_port = SPI1_FLASH_CS_GPIO_Port;
       W25Q128_Details.cs_pin = SPI1_FLASH_CS_Pin;
       W25Q128_Details.manufacturer_id = 0XEF;
       W25Q128_Details.device_id = 0X4018;
       W25Q128_Details.block_size = 4096;
       W25Q128_Details.block_count = 4096;
       W25Q128_Details.sector_size = 4096;
       W25Q128_Details.sectors_in_block = 1;
       W25Q128_Details.page_size = 256;
       W25Q128_Details.pages_in_sector = 16;

       w25qxx_littlefs_init (&W25Q128_Details);
       return true;
   }
   ```

4. Change SPI handle and CS pin name as defined in your IOC.
5. Call the function mentioned below to mount the little FS
   ```cpp
   if(!initLittle_FS ()){
       printf ("[ ERROR ] Little FS init failed \r\n");
       return false;
   }
   readAndPrintStorageDetails ();
   listFiles ();
   ```
