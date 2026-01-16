#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#define zd_pwn_shu 50
#define zd_mingzi 100
#define zd_lujing 256
typedef struct{
    char pwn_mingzi[zd_mingzi];
    char pwn_lujing[zd_lujing];
} mk_xinxi;

static mk_xinxi pwn_shuzu[zd_pwn_shu];
int pwn_shu = 0;

static int pwn_saomiao(){
    pwn_shu = 0;
    DIR *mulu = opendir("./yuanma/gongneng/pwn_mod/");
    if (mulu == NULL){
        perror("无法打开目录");
        return 0;
    }

    struct dirent *jinru;
    while ((jinru = readdir(mulu)) != NULL && pwn_shu < zd_pwn_shu){
        if (jinru->d_type != DT_DIR) {
            continue;
        }
        if (strcmp(jinru->d_name, ".") == 0 ||
            strcmp(jinru->d_name, "..") == 0 ||
            jinru->d_name[0] == '.'){
            continue;
        }

        char jiancha[zd_lujing];
        snprintf(jiancha, sizeof(jiancha), "./yuanma/gongneng/pwn_mod/%s/docker-compose.yml", jinru->d_name);

        if (access(jiancha, F_OK) == 0){
            snprintf(pwn_shuzu[pwn_shu].pwn_mingzi, zd_mingzi, "%s", jinru->d_name);
            snprintf(pwn_shuzu[pwn_shu].pwn_lujing, zd_lujing, "./yuanma/gongneng/pwn_mod/%s", jinru->d_name);
            pwn_shu++;
        }else{
            printf("模块 %s 缺少 docker-compose.yml 文件，跳过。\n", jinru->d_name);
        }
    }

    closedir(mulu);

    if (pwn_shu == 0){
        printf("未发现任何pwn漏洞模块，请添加模块到 ./yuanma/gongneng/pwn_mod/ 目录下。\n");
        return 0;
    }else{
        return pwn_shu;
    }
}

void pwn_liebiao(void){
    pwn_saomiao();
    printf("发现以下pwn漏洞模块：\n");
    for (int i = 0; i < pwn_shu; i++){
        printf("%d. 模块名称: %s, 模块路径: %s\n", i + 1, pwn_shuzu[i].pwn_mingzi, pwn_shuzu[i].pwn_lujing);
    }
}
void pwn_qidong(int xuhao){
    pwn_saomiao();
    if (xuhao < 1 || xuhao > pwn_shu){
        printf("无效的选择，请选择1到%d之间的数字。\n", pwn_shu);
        return;
    }

    int suoyin = xuhao - 1;
    char mingling[512];

    snprintf(mingling,sizeof(mingling),"cd %s && docker compose up -d",pwn_shuzu[suoyin].pwn_lujing);
    printf("正在启动模块 %s...\n",pwn_shuzu[suoyin].pwn_mingzi);
    system(mingling);
    printf("模块 %s 已启动完成。\n",pwn_shuzu[suoyin].pwn_mingzi);

    char compose_wenjian[zd_lujing];
    snprintf (compose_wenjian, sizeof(compose_wenjian), "%sdocker-compose.yml", pwn_shuzu[suoyin].pwn_lujing);

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
void pwn_tingzhi(int xuhao){
    pwn_saomiao();
    if (xuhao < 1 || xuhao > pwn_shu){
        printf("无效的选择，请选择1到%d之间的数字。\n", pwn_shu);
        return;
    }

    int suoyin = xuhao - 1;
    char mingling[512];
    snprintf(mingling,sizeof(mingling),"cd %s && docker compose down",pwn_shuzu[suoyin].pwn_lujing);
    printf("正在停止模块 %s...\n",pwn_shuzu[suoyin].pwn_mingzi);
    int jieguo = system(mingling);
    if(jieguo == 0){
        printf("模块 %s 已成功停止。\n",pwn_shuzu[suoyin].pwn_mingzi);
    }else{
        printf("停止模块失败代码 %d 。\n",jieguo);
    }
}
void pwn_zhuangtai(int xuhao){
    pwn_saomiao();
    if (xuhao < 1 || xuhao > pwn_shu){
        printf("无效的选择，请选择1到%d之间的数字。\n", pwn_shu);
        return;
    }
    int suoyin = xuhao - 1;
    char mingling[512];
    snprintf(mingling,sizeof(mingling),"cd %s && docker compose ps | grep -c 'Up'",pwn_shuzu[suoyin].pwn_lujing);
    printf("正在检查模块 %s 的状态...\n",pwn_shuzu[suoyin].pwn_mingzi);
    FILE *gd = popen(mingling,"r");
    char shuchu[100];
    if(fgets(shuchu,sizeof(shuchu),gd)){
        int jilu = atoi(shuchu);
        if(jilu > 0){
            printf("模块 %s 正在运行。\n",pwn_shuzu[suoyin].pwn_mingzi);
        }else{
        printf("模块 %s 未运行。\n",pwn_shuzu[suoyin].pwn_mingzi);
        }
    }
}
void pwn_guanli(void){
    int xuanze;
    int mk_xuhao;

    while(1){
        printf("\n=== PWN漏洞管理器 ===\n");
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
            pwn_liebiao();
            break;
        case 2:
            pwn_liebiao();
            printf("请输入要启动的模块编号: ");
            scanf("%d",&mk_xuhao);
            pwn_qidong(mk_xuhao);
            break;
        case 3:
            pwn_liebiao();
            printf("请输入要停止的模块编号: ");
            scanf("%d",&mk_xuhao);
            pwn_tingzhi(mk_xuhao);
            break;
        case 4:
            pwn_liebiao();
            printf("请输入要检查的模块编号: ");
            scanf("%d",&mk_xuhao);
            pwn_zhuangtai(mk_xuhao);
            break;
        case 0:
            return;
        default:
            printf("无效选择，请重新输入。\n");
        }
        // 按任意键继续
        printf("\n按回车键继续...");
        while(getchar() != '\n')  // 清除缓冲区
            ;
        getchar();
    }
}

