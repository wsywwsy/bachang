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

int web_saomiao(void)
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
        if (jinru->d_type != DT_DIR ||
            strcmp(jinru->d_name, ".") == 0 ||
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
            snprintf(mk_shuzu[mk_shu].lujing, zd_lujing, "./yuanma/gongneng/web_mod/%s/", jinru->d_name);
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