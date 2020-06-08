#include<linux/module.h>
#include<linux/proc_fs.h>
#include<linux/seq_file.h>
#include <linux/ktime.h>
static int proc_read_clock(struct seq_file *m,void *v)
{
	struct timespec time;
	getnstimeofday(&time);
	seq_printf(m,"%ld %ld\n",time.tv_sec,time.tv_nsec/1000);
	return 0;
}

static int clock_proc_open(struct inode *inode,struct file *file)
{
	return single_open(file,proc_read_clock,NULL);
}
static const struct file_operations clock_proc_fops=
{
	.owner=THIS_MODULE,
	.open=clock_proc_open,
	.read=seq_read,
	.release=single_release,
};
static int __init init_clock_module(void)
{
	proc_create("clock",0,NULL,&clock_proc_fops);
	return 0;
}
static void __exit cleanup_clock_module(void)
{
	remove_proc_entry("clock",NULL);
}
module_init(init_clock_module);
module_exit(cleanup_clock_module);
MODULE_LICENSE("GPL");
