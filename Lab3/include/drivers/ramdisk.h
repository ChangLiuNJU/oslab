#ifndef __RAMDISK_H__
#define __RAMDISK_H__

#define NR_MAX_FILE 8
#define NR_FILE_SIZE (128 * 1024)
#define MSG_DEV_READ 3

extern const char *ramdiskname;
extern pid_t RAMDISK;

extern uint8_t file[NR_MAX_FILE][NR_FILE_SIZE];
extern uint8_t *disk;

void init_ramdisk(void);
void ramdiskd(void);
void ram_read(DevMessage* m);

#endif 