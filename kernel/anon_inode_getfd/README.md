# anon_inode_getfd

*Author: Wang Xiaoqiang <wang_xiaoq@126.com> Distributed & Embeded System Lab [DSLab](http://dslab.lzu.edu.cn)*

This module is used to give an example of the anon_inode_getfd function which is used frequently in kvm source code.

The module creates a character device named "/dev/anon-file" first, 
then the userspace program uses ioctl to tell the character device
to create an anonymous file. The ioctl returns the file descriptor
of the anonymous file. Finally, userspace program uses the read function to read the internal information of anonymous file.

## Usage
1. make
2. make install
3. gcc user-space.c
4. sudo ./a.out

## Output
After the all of the command have been finished, the output is:

`hello anonymous file`

## LICENSE
GPL