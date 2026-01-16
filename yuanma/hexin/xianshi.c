#include <stdio.h>
#include <stdlib.h>  // 添加 system() 需要
#include <string.h>  // 添加字符串函数
#include "../gongneng/nx_guanli.h"
#include "../gongneng/pwn_guanli.h"
#include "../gongneng/web_guanli.h"

int csh_mianban(void){
    printf("初始化控制面板\n");
    return 0;
}
static void docker_anzhuang(void){
    printf("Docker一键安装功能\n");
    char mingling[256];
    char xg_yuan[2048];
    char jiancha[256];
    snprintf(mingling,sizeof(mingling),"sudo curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun");

    int result = system(mingling);
    if (result != 0) {
        printf("❌ 命令执行失败: %s\n", mingling);
        return;
    }

    snprintf(xg_yuan, sizeof(xg_yuan), 
    "sudo tee /etc/docker/daemon.json > /dev/null << 'EOF'\n"
    "{\n"
    "  \"registry-mirrors\": [\n"
    "    \"https://docker.m.daocloud.io\",\n"
    "    \"https://docker.imgdb.de\",\n"
    "    \"https://docker-0.unsee.tech\",\n"
    "    \"https://docker.hlmirror.com\",\n"
    "    \"https://docker.1ms.run\",\n"
    "    \"https://func.ink\",\n"
    "    \"https://lispy.org\",\n"
    "    \"https://docker.xiaogenban1993.com\"\n"
    "  ]\n"
    "}\n"
    "EOF\n");

    system(xg_yuan);
    system("sudo systemctl daemon-reload && sudo systemctl restart docker");
   
    snprintf(jiancha,sizeof(jiancha),"docker --version");
    int jieguo = system (jiancha);
    if(jieguo == 0){
        printf("Docker安装成功！\n");
    }else{
        printf("Docker安装失败，请检查安装过程中的错误信息。\n");
    }
}

static void hj_jiance(){
    printf("环境监测功能\n");
    char jc_mingling[256];
    char xz;
    snprintf(jc_mingling,sizeof(jc_mingling),"docker --version");
    printf("正在检测Docker环境...\n");
    int jieguo = system (jc_mingling);
    if(jieguo == 0){
        printf("Docker环境检测通过。\n");
        system("docker --version");
    }else{
        printf("Docker环境检测未通过，请确保Docker已正确安装并配置。\n");
        printf("是否一键安装Docker？(y/n): ");
        scanf(" %c", &xz);
        if(xz == 'y' || xz == 'Y'){
            printf("正在安装Docker...\n");
            docker_anzhuang();
        }else{
            printf("已取消Docker安装。\n");
        }
    }
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
        case '4':
            printf("选择了自建漏洞\n");
            break;
        case '5':
            printf("选择了环境监测\n");
            hj_jiance();
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
        printf("[4] 自建漏洞\n");
        printf("[5] 环境监测\n");
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
    printf("清理并且退出程序\n");
}
