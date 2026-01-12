#include <stdio.h>
#include "../gongneng/nx_guanli.h"
#include "../gongneng/pwn_guanli.h"
#include "../gongneng/web_guanli.h"

int csh_mianban(void){
    printf("初始化控制面板\n");
    return 0;
}

static void sr_chuli(char shuru){
    switch(shuru){
        case '1':
            printf("选择了web漏洞\n");
            web_guanli();
            break;
        case '2':
            printf("选择了pwn漏洞\n");
            pwn_guanli();
            break;
        case '3':
            printf("选择了逆向漏洞\n");
            nx_guanli();
            break;
        case '0':
            //退出选项在主循环中处理
            break;
        default:
            printf("无效输入，请重新选择\n");
    }
}

void qd_mianban(void){
    int bj=0;
    char shuru;
    while(!bj) {
        printf("\n\n====漏洞平台控制中心===\n");
        printf("[1] web漏洞\n");
        printf("[2] pwn漏洞\n");
        printf("[3] 逆向漏洞\n");
        printf("[0] 退出\n");
        scanf(" %c", &shuru);
        
        sr_chuli(shuru);

        if(shuru == '0'){
            printf("退出控制中心\n");
            bj=1;
        }
    }
}

void ql_mianban(void){
    printf("清理并且推出程序\n");
}
