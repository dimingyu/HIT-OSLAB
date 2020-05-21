#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/segment.h>
#include <stdarg.h>
#include <stddef.h>

extern int vsprintf(char * buf, const char * fmt, va_list args);

int sprintf(char *buf, const char *fmt, ...){
    va_list args; int i;
    va_start(args, fmt);
    i=vsprintf(buf, fmt, args);
    va_end(args);
    return i;
}

int proc_read(int dev, unsigned long * pos, char * buf, int count){
    struct task_struct ** p;
    int output_count=0;
    char * proc_buf=NULL;
    int file_size=0;
    int offset=*pos;

    struct super_block * sb; 
    struct buffer_head * bh;
    int total_blocks, total_inodes;
    int used_blocks=0, free_blocks=0;
    int i,j,k;
    char * db=NULL;

    sb=get_super(current->root->i_dev);
    total_blocks = sb->s_nzones;
    total_inodes=sb->s_ninodes;

    if(dev==0){  /*psinfo*/
        proc_buf=(char *)malloc(sizeof(char *)*1024);
        file_size=sprintf(proc_buf,"pid\tstate\tfather\tcounter\tstart_time\n");

        for(p = &LAST_TASK ; p >= &FIRST_TASK ; --p)
            if(*p)
                file_size+=sprintf(proc_buf+file_size,"%d\t%d\t%d\t%d\t%d\n",(*p)->pid,(*p)->state,(*p)->father,(*p)->counter,(*p)->start_time);

        *(proc_buf+file_size)='\0';
    }

    if(dev==1) {  /*hdinfo*/
        for(i=0;i<sb->s_zmap_blocks;i++){
            bh=sb->s_zmap[i];
            db=(char*)bh->b_data;
            for(j=0;j<1024;j++){
                for(k=1;k<=8;k++){
                        if((used_blocks+free_blocks)>=total_blocks)
                            break;
                        if( *(db+j) & k)
                            used_blocks++;
                        else
                            free_blocks++;
                }
            }
        }

        proc_buf=(char*)malloc(sizeof(char*)*512);
        file_size=sprintf(proc_buf,"s_imap_blocks:%d\ns_zmap_blocks:%d\n",sb->s_imap_blocks,sb->s_zmap_blocks);
        file_size+=sprintf(proc_buf+file_size,"total_blocks:%d\nfree_blcoks:%d\nused_blocks:%d\ntotal_indoes:%d\n",total_blocks,free_blocks,used_blocks,total_inodes);
    }

    while(count>0){  /*put the data into buffer*/
        if(offset>file_size)
            break;
        put_fs_byte(*(proc_buf+offset),buf++);
        offset++;
        output_count++;
        count--;
    }

    (*pos)+=output_count;  /*change the file->f_pos*/

    free(proc_buf);
    return output_count;
}
