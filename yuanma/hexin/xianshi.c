#include <stdio.h>

int csh_mianban(void){
    printf("初始化控制面板\n");
    return 0;
}

void mb_qidong(void){
    char shuru;
    while(1) {
        char shuru;
        printf("\n\n====漏洞平台控制中心===\n");
        printf("[1] web漏洞\n");
        printf("[2] pwn漏洞\n");
        printf("[3] 逆向漏洞\n");
        printf("[0] 退出\n");
        scanf(" %c", &shuru);
    }
}

