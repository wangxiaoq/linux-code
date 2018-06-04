#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define KEYFILE "./mykey"

#define ENCRYPT_CKEY "encrypt"
#define ENCRYPT_KEY "encrypt-key"

/* key */
static AES_KEY keyEn;

char encrypt_char(char ch)
{
    int num = 0;
    unsigned char ivec[AES_BLOCK_SIZE] = ENCRYPT_KEY;

    AES_cfb8_encrypt(&ch, &ch, 1, &keyEn, ivec, &num, AES_ENCRYPT);
    return ch;
}

char decrypt_char(char ch)
{
    int num = 0;
    unsigned char ivec[AES_BLOCK_SIZE] = ENCRYPT_KEY;
    AES_cfb8_encrypt(&ch, &ch, 1, &keyEn, ivec, &num, AES_DECRYPT);

    return ch;
}

int init_key()
{
    unsigned char ckey[] =  ENCRYPT_CKEY;
    int ret = 0;
    int fd = open(KEYFILE, O_RDONLY);
    if (!(fd < 0)) {
        ret = read(fd, &keyEn, sizeof(keyEn));
        if (ret != sizeof(keyEn)) {
            close(fd);
            return -1;
        }
        close(fd);
        return 0;
    }

    AES_set_encrypt_key(ckey, 128, &keyEn);
    fd = open(KEYFILE, O_RDWR | O_CREAT, 0600);
    if (fd < 0) {
        return -1;
    } 
    ret = write(fd, &keyEn, sizeof(keyEn));
    if (ret != sizeof(keyEn)) {
        close(fd);
        return -1;
    }
    close(fd);

    return 0;
}

int main()
{
    char indata;
    int ret = 0;

    ret = init_key();
    if (ret < 0) {
        return -1;
    }

    indata = 'H';

    indata = encrypt_char(indata);

    printf("encrypt data was: %c\n", indata);
    indata = decrypt_char(indata);
    printf("input data was: %c\n", indata);
    return 0;
}
