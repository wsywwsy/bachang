#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#define zd_nx_shu 50
#define zd_mingzi 100
#define zd_lujing 256
typedef struct{
    char nx_mingzi[zd_mingzi];
    char nx_lujing[zd_lujing];
} mk_xinxi;

static mk_xinxi nx_shuzu[zd_nx_shu];
int nx_shu = 0;

static int nx_saomiao(){
    nx_shu = 0;
    DIR *mulu = opendir("./yuanma/gongneng/nx_mokuai");
    if (mulu == NULL){
        perror("无法打开目录");
        return 0;
    }

    struct dirent *jinru;
    while ((jinru = readdir(mulu)) != NULL && nx_shu < zd_nx_shu)
    {
        if(jinru->d_type != DT_DIR){
            continue;
        }
        if(jinru->d_name[0] == '.' ||
           strcmp(jinru->d_name,"..") == 0 ||
           strcmp(jinru->d_name,".") == 0){
            continue;
        }
        char jiancha[zd_lujing];
        snprintf(jiancha,sizeof(jiancha),"./yuanma/gongneng/nx_mokuai/%s/docker-compose.yml",jinru->d_name);

        if(access(jiancha,F_OK) == 0){
            snprintf(nx_shuzu[nx_shu].nx_mingzi,zd_mingzi,"%s",jinru->d_name);
            snprintf(nx_shuzu[nx_shu].nx_lujing,zd_lujing,"./yuanma/gongneng/nx_mokuai/%s",jinru->d_name);
            nx_shu++;
        }else{
            printf("模块 %s 缺少 docker-compose.yml 文件，跳过。\n", jinru->d_name);
        }
    }
        closedir(mulu); 
        
        if (nx_shu == 0)
        {
            printf("未发现任何nx漏洞模块，请添加模块到 ./yuanma/gongneng/nx_mod/ 目录下。\n");
            return 0;
        }
        else
        {
            return nx_shu;
        }
}

void nx_liebiao(void){
    nx_saomiao();
    printf("发现以下nx漏洞模块：\n");
    for(int i = 0; i < nx_shu; i++){
        printf("模块名称: %s, 模块路径: %s\n", nx_shuzu[i].nx_mingzi, nx_shuzu[i].nx_lujing);
    }
}

void nx_qidong(int xuhao){
    nx_saomiao();
    char mingling[512];
    if(xuhao < 1 || xuhao > nx_shu){
        printf("无效的选择，请选择1到%d之间的数字。\n", nx_shu);
        return;
    }

    int suoyin = xuhao - 1;
    snprintf(mingling,sizeof(mingling),"cd %s && docker compose up -d",nx_shuzu[suoyin].nx_lujing);
    int jieguo = system(mingling);
    if(jieguo == 0){
        printf("模块 %s 已成功启动。\n",nx_shuzu[suoyin].nx_mingzi);
    }else{
        printf("启动模块失败代码 %d 。\n",jieguo);
    }
    
    char compose_wenjian[zd_lujing];
    snprintf(compose_wenjian, sizeof(compose_wenjian), "%s/docker-compose.yml", nx_shuzu[suoyin].nx_lujing);

    FILE *duankou = fopen(compose_wenjian,"r");
    if(duankou){
        char hang[512];
        while (fgets(hang,sizeof(hang),duankou)){
            if(strstr(hang,"ports:")){
                printf("模块端口信息 %s",hang);
            }
        }
        fclose(duankou);
    }else{
    printf("无法打开 %s 文件以读取端口信息。\n",compose_wenjian);
    }
}

void nx_tingzhi(int xuhao){
    nx_saomiao();
    char mingling[512];
    if(xuhao < 1 || xuhao > nx_shu){
        printf("无效的选择，请选择1到%d之间的数字。\n", nx_shu);
        return;
    }

    int suoyin = xuhao - 1;
    snprintf(mingling,sizeof(mingling),"cd %s && docker compose down",nx_shuzu[suoyin].nx_lujing);
    int jieguo = system(mingling);
    if(jieguo == 0){
        printf("模块 %s 已成功停止。\n",nx_shuzu[suoyin].nx_mingzi);
    }else{
        printf("停止模块失败代码 %d 。\n",jieguo);
    }
}

void nx_zhuangtai(int xuhao){
    nx_saomiao();
    if(xuhao < 1 || xuhao > nx_shu){
        printf("无效的选择，请选择1到%d之间的数字。\n", nx_shu);
        return;
    }

    int suoyin = xuhao - 1;
    char mingling[512];
    snprintf(mingling,sizeof(mingling),"cd %s && docker compose ps | grep -c 'Up'",nx_shuzu[suoyin].nx_lujing);
    printf("正在检查模块 %s 的状态...\n",nx_shuzu[suoyin].nx_mingzi);
    FILE *gd = popen(mingling,"r");
    char shuchu[100];
    if(fgets(shuchu,sizeof(shuchu),gd)){
        int jilu = atoi(shuchu);
        if(jilu > 0){
            printf("模块 %s 正在运行。\n",nx_shuzu[suoyin].nx_mingzi);
        }else{
            printf("模块 %s 未运行。\n",nx_shuzu[suoyin].nx_mingzi);
        }
    }
    pclose(gd);
}

void nx_guanli(void){
    int mk_xuhao;
    int xuanze;
    while(1){
        printf("\nnx漏洞模块管理菜单：\n");
        printf("1. 列出模块\n");
        printf("2. 启动模块\n");
        printf("3. 停止模块\n");
        printf("4. 检查模块状态\n");
        printf("0. 退出管理\n");
    
        if(scanf("%d",&xuanze) != 1){
        printf("输入无效，请输入数字。\n");
        while(getchar() != '\n');
        continue;
        }
    
         switch(xuanze){
        case 1:
            nx_liebiao();
            break;
        case 2:
            nx_liebiao();
            printf("请输入要启动的模块编号: ");
            scanf("%d",&mk_xuhao);
            nx_qidong(mk_xuhao);
            break;
        case 3:
            nx_liebiao();
            printf("请输入要停止的模块编号: ");
            scanf("%d",&mk_xuhao);
            nx_tingzhi(mk_xuhao);
            break;
        case 4:
            nx_liebiao();
            printf("请输入要检查的模块编号: ");
            scanf("%d",&mk_xuhao);
            nx_zhuangtai(mk_xuhao);
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