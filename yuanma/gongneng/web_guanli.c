// yuanma/gongneng/web_guanli.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "web_guanli.h"

#define zd_mk_shu 50
#define zd_mingzi 100
#define zd_lujing 256

typedef struct
{
    char ming[zd_mingzi];   // 模块名称
    char lujing[zd_lujing]; // 模块路径
} mk_xinxi;

mk_xinxi mk_shuzu[zd_mk_shu]; // 模块数
int mk_shu = 0;               // 记录数量

static int web_saomiao(void)
{
    mk_shu = 0;

    DIR *mulu = opendir("./yuanma/gongneng/web_mod/");
    if (mulu == NULL)
    {
        perror("无法打开目录");
        return 0;
    }

    struct dirent *jinru;
    // 排除.和..文件夹
    while ((jinru = readdir(mulu)) != NULL && mk_shu < zd_mk_shu)
    {
        if (jinru->d_type != DT_DIR) {
            continue;
        }
        if (strcmp(jinru->d_name, ".") == 0 ||
            strcmp(jinru->d_name, "..") == 0 ||
            jinru->d_name[0] == '.')
        {
            continue;
        }

        // 检查是否存在docker compose
        char jiancha[zd_lujing];
        snprintf(jiancha, sizeof(jiancha), "./yuanma/gongneng/web_mod/%s/docker-compose.yml", jinru->d_name);

        if (access(jiancha, F_OK) == 0)
        {
            snprintf(mk_shuzu[mk_shu].ming, zd_mingzi, "%s", jinru->d_name);
            snprintf(mk_shuzu[mk_shu].lujing, zd_lujing, "./yuanma/gongneng/web_mod/%s", jinru->d_name);
            mk_shu++;
        }
        else
        {
            printf("模块 %s 缺少 docker-compose.yml 文件，跳过。\n", jinru->d_name);
        }
    }
    closedir(mulu);

    if (mk_shu == 0)
    {
        printf("未发现任何web漏洞模块，请添加模块到 ./yuanma/gongneng/web_mod/ 目录下。\n");
        return 0;
    }
    else
    {
        return mk_shu;
    }
}

void web_liebiao(void){
    web_saomiao();
    printf("发现以下web漏洞模块：\n");
    for (int i = 0; i < mk_shu; i++)
    {
        printf("%d. 模块名称: %s, 模块路径: %s\n", i + 1, mk_shuzu[i].ming, mk_shuzu[i].lujing);
    }
}

void web_qidong(int xuhao){
    web_saomiao();
    if (xuhao < 1 || xuhao > mk_shu){
        printf("无效的选择，请选择1到%d之间的数字。\n", mk_shu);
        return;
    }

    int suoyin = xuhao - 1;
    char mingling[512];

    snprintf(mingling,sizeof(mingling),"cd %s && docker compose up -d",mk_shuzu[suoyin].lujing);
    printf("正在启动模块 %s...\n",mk_shuzu[suoyin].ming);
    system(mingling);
    printf("模块 %s 已启动完成。\n",mk_shuzu[suoyin].ming);

    char compose_wenjian[zd_lujing];
    snprintf (compose_wenjian, sizeof(compose_wenjian), "%sdocker-compose.yml", mk_shuzu[suoyin].lujing);

    FILE *wenjian = fopen(compose_wenjian,"r");
    if(wenjian){
        char hang[256];
        while(fgets(hang,sizeof(hang),wenjian)){
            if(strstr(hang,"ports:")){
                printf("模块端口信息 %s" ,hang);
            }
        }
        fclose(wenjian);
    }else{
        printf("无法打开 %s 文件以读取端口信息。\n",compose_wenjian);
    }
}

//停止模块
void web_tingzhi(int xuhao){
    web_saomiao();

    int suoyin = xuhao - 1;
    char mingling[512];
    snprintf(mingling,sizeof(mingling),"cd %s && docker compose down",mk_shuzu[suoyin].lujing);
    printf("正在停止模块 %s...\n",mk_shuzu[suoyin].ming);
    int jieguo = system(mingling);
    if(jieguo == 0){
        printf("模块 %s 已成功停止。\n",mk_shuzu[suoyin].ming);
    }else{
        printf("停止模块失败代码 %d 。\n",jieguo);
    }
}

//检查模块
void web_zhuangtai(int xuhao){
    web_saomiao();

    int suoyin = xuhao - 1;

    printf("正在检查模块 %s 的状态...\n",mk_shuzu[suoyin].ming);
    char mingling[512];
    snprintf(mingling,sizeof(mingling),"cd %s && docker compose ps | grep -c 'Up'",mk_shuzu[suoyin].lujing);

    FILE *gd = popen(mingling,"r");
    char shuchu[100];

    if(fgets(shuchu,sizeof(shuchu),gd)){
        int jilu = atoi(shuchu);
        if(jilu > 0){
            printf("有%s个容器正在运行。\n",shuchu);
        }else{
        printf("所有容器均未运行。\n");
        }
    }
    pclose(gd); 
}

//web管理器主菜单
void web_guanli(void){
    int xuanze;
    int mk_xuhao;

    while(1){
        printf("\n=== Web漏洞管理器 ===\n");
        printf("1. 列出所有漏洞模块\n");
        printf("2. 启动漏洞模块\n");
        printf("3. 停止漏洞模块\n");
        printf("4. 查看模块状态\n");
        printf("0. 返回主菜单\n");
        printf("请选择: ");

            
    if(scanf("%d",&xuanze) != 1){
        printf("输入无效，请输入数字。\n");
        while(getchar() != '\n');
        continue;
      }
    
     switch(xuanze){
        case 1:
            web_liebiao();
            break;
        case 2:
            web_liebiao();
            printf("请输入要启动的模块编号: ");
            scanf("%d",&mk_xuhao);
            web_qidong(mk_xuhao);
            break;
        case 3:
            web_liebiao();
            printf("请输入要停止的模块编号: ");
            scanf("%d",&mk_xuhao);
            web_tingzhi(mk_xuhao);
            break;
        case 4:
            web_liebiao();
            printf("请输入要检查的模块编号: ");
            scanf("%d",&mk_xuhao);
            web_zhuangtai(mk_xuhao);
            break;
        case 0:
            return;
        default:
            printf("无效选择，请重新输入。\n");
        }
        // 按任意键继续
        printf("\n按回车键继续...");
        while(getchar() != '\n')  // 清除缓冲区
        getchar();  // 等待回车
    }

}