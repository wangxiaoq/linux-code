# USER MODE HELPER

An example of using kernel facility to invoke user program from
kernel space.

NOTE: This repo should be cloned at the directory of user home to
make this program work properly.

And you should change the `HOME' micro at the file usermod-helper.c
to your own home path.

## Usage

```
1. make 
2. make install 
3. ls -l ~ //check whether the `user-log' file is created.
4. make uninstall 
5. ls -l ~ //check whether the `user-log' file is deleted.
```

