#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "inode.h"
#include "diskimg.h"



int inode_iget(struct unixfilesystem *fs, int inumber, struct inode *inp) {
    if(fs != NULL && inp != NULL)
    {
        int inodesPerSector = DISKIMG_SECTOR_SIZE / sizeof(struct inode);
        int max_inumber = fs->superblock.s_isize * inodesPerSector;

        if(inumber > 0 && inumber <= max_inumber)
        {
            int sectorNum = INODE_START_SECTOR + (inumber - 1) / inodesPerSector;
            int offset = (inumber - 1) % inodesPerSector;

            struct inode inodes[inodesPerSector];
            if(diskimg_readsector(fs->dfd, sectorNum, inodes) >= 0)
            {
                *inp = inodes[offset];
                return 0;
            }
        }
    }
    return -1;
}



int inode_indexlookup(struct unixfilesystem *fs, struct inode *inp, int blockNum) {  
    if(fs != NULL && inp != NULL && blockNum >= 0)
    {
        if ((inp->i_mode & ILARG) == 0) {
            if (blockNum < 8) {
                return inp->i_addr[blockNum];
            } return -1;
        }

        int ptrs_per_block = DISKIMG_SECTOR_SIZE / sizeof(uint16_t);

        if (blockNum < 7 * ptrs_per_block) {
            int indirect_index = blockNum / ptrs_per_block;
            int indirect_offset = blockNum % ptrs_per_block;

            uint16_t block[ptrs_per_block];
            if (diskimg_readsector(fs->dfd, inp->i_addr[indirect_index], block) < 0) {
                return -1;
            }
            return block[indirect_offset];
        }

        int remaining = blockNum - (7 * ptrs_per_block);
        int first_level_index = remaining / ptrs_per_block;
        int second_level_index = remaining % ptrs_per_block;

        if (first_level_index < ptrs_per_block)
        {
            uint16_t indirect_blocks[ptrs_per_block];
            if (diskimg_readsector(fs->dfd, inp->i_addr[7], indirect_blocks) >= 0) {
                uint16_t data_block[ptrs_per_block];
                if (diskimg_readsector(fs->dfd, indirect_blocks[first_level_index], data_block) >= 0) {
                    return data_block[second_level_index];
                }
            }
        }
    } return -1;
}


int inode_getsize(struct inode *inp) {
    return ((inp->i_size0 << 16) | inp->i_size1); 
}
