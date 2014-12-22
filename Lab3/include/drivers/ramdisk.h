#ifndef __RAMDISK_H__
#define __RAMDISK_H__

#define NR_MAX_FILE 8
#define NR_FILE_SIZE (128 * 1024)
 
// static uint8_t file[NR_MAX_FILE][NR_FILE_SIZE];
// static uint8_t *disk = (void*)file;

void init_ramdisk(void);

#endif 