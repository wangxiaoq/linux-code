#define pr_fmt(s) KBUILD_MODNAME ": " s

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/anon_inodes.h>
#include <linux/uaccess.h>

#define NAME "anon-file"

#define MAGIC 'A'
#define CREAT_ANON_FILE _IOW(MAGIC, 0, unsigned long)

static ssize_t anon_file_read(struct file *filp, char __user *ubuf, size_t cnt, loff_t *pos)
{
	char buf[100] = {"hello anonymous file\n"};

	if (cnt > 100)
		cnt = 100;
	if (copy_to_user(ubuf, buf, cnt))
		return -EFAULT;

	return cnt;
}

static struct file_operations anon_file_fops = {
	.read = anon_file_read,
};

static long char_dev_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int fd;

	switch (cmd) {
	case CREAT_ANON_FILE:
		fd = anon_inode_getfd("anon-file", &anon_file_fops, (void*)0, O_RDWR|O_CLOEXEC);
		if (fd < 0)
			pr_err("Create anonamous file error\n");
		return fd;
	default:
		return -ENOTTY;
	}
}

static struct file_operations char_dev_fops = {
	.unlocked_ioctl = char_dev_ioctl,
};

static struct miscdevice miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = NAME,
	.fops = &char_dev_fops,
};

static int test_anon_getfd_init(void)
{
	int ret = misc_register(&miscdev);

	pr_info("module init...\n");

	return ret;
}

static void test_anon_getfd_exit(void)
{
	misc_deregister(&miscdev);
	pr_info("module exit...\n");
}

module_init(test_anon_getfd_init);
module_exit(test_anon_getfd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wang Xiaoqiang <wang_xiaoq@126.com>");
