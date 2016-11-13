#define pr_fmt(s) KBUILD_MODNAME ": " s

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/string.h>

/* Please change your home here */
#define HOME "/home/wangxq"
#define HOME_ENV "HOME="HOME

static char user_helper[128];
static char *user_argv[2] = {user_helper, NULL};

char* envp[]={HOME_ENV, "PATH=/sbin:/bin:/usr/bin", NULL};

static int user_helper_init(void)
{
	sprintf(user_helper, "%s/linux-code/kernel/usermod_helper/user-prog", HOME);
	pr_info("user helper: %s\n", user_helper);
	pr_info("calling usermod helper...\n");
	call_usermodehelper(user_helper, user_argv, envp, UMH_WAIT_PROC);
	pr_info("call usermod helper end.\n");

	return 0;
}

static void user_helper_exit(void)
{
	char file_path[128];
	char *user_argv[3];

	sprintf(file_path, "%s/user-log", HOME);
	user_argv[0] = user_helper;
	user_argv[1] = file_path;
	user_argv[2] = NULL;
	sprintf(user_helper, "%s", "/bin/rm");
	call_usermodehelper(user_helper, user_argv, envp, UMH_WAIT_PROC);

	return ;
}

module_init(user_helper_init);
module_exit(user_helper_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Wang Xiaoqiang <wang_xiaoq@126.com>");
