// yuanma/gongneng/web_guanli.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include "web_guanli.h"

#define genmulu "./yuanma/cunfang/web"

yx_zhuangtai yx_mk_shu[zd_mk_shu];
int jl_shu = 0;

int web_panduan(const char *mulu){
    char compose_wj[zd_lujing];
    snprintf(compose_wj,sizeof(compose_wj),"%s/docker-compose.yml",mulu);

    return (access(compose_wj, F_OK) == 0);
}

int web_saomiao(const char *mulu,jg *mk_shuzu,int zd_shu){
    int jl_jb_shu = 0;

    DIR *dir = opendir(mulu);
    if (dir == NULL){
        perror("文件夹下没有内容");
        return 0;
    }

    struct dirent *jinru;
    while ((jinru = readdir(dir)) != NULL && jl_jb_shu < zd_shu){
        if (jinru->d_name[0] == '.'){
            continue;
        }

        if(jinru->d_type != DT_DIR){
            continue;
        }
        char compose_mulu[zd_lujing];
        snprintf(compose_mulu,sizeof(compose_mulu),"%s/%s",mulu,jinru->d_name);

        if(web_panduan(compose_mulu)){
            mk_shuzu[jl_jb_shu].lx = mk;
        }else{
            mk_shuzu[jl_jb_shu].lx = ml;
        }

        strncpy(mk_shuzu[jl_jb_shu].ming, jinru->d_name, zd_mingzi - 1);
        mk_shuzu[jl_jb_shu].ming[zd_mingzi - 1] = '\0';

        strncpy(mk_shuzu[jl_jb_shu].lujing,compose_mulu,zd_lujing - 1);
        mk_shuzu[jl_jb_shu].lujing[zd_lujing - 1] = '\0';
        jl_jb_shu++;
    }
    closedir(dir);
    return jl_jb_shu;
}

void web_mk_yunxing(const char *ming,const char *mulu){
    if(!web_panduan(mulu)){
        printf("目录 %s 不是有效的Web项目，无法启动。\n", mulu);
        return;
    }

    char mingling[512];
    snprintf(mingling, sizeof(mingling), "cd %s && docker compose up -d", mulu);
    printf("正在启动模块...");
    int jieguo = system(mingling);

    if(jieguo==0){
        printf("模块 %s 启动成功！\n", ming);
    }
    else{
        printf("模块 %s 启动失败，请检查错误信息。\n", ming);
    }

    snprintf(mingling,sizeof(mingling),"cd %s && docker compose ps -q",mulu);
    FILE *pipe = popen(mingling, "r");
    char docker_id[dokcer_id];
    if(pipe){
        if(fgets(docker_id, sizeof(docker_id), pipe)){
            docker_id[strcspn(docker_id, "\n")] = 0;
        }
        pclose(pipe);
    }

    if(strlen(docker_id) > 0){
        web_wj_chuangjian(mulu,docker_id);
    }
    
    if(jl_shu<zd_mk_shu){
        strncpy(yx_mk_shu[jl_shu].ming, ming ,zd_mingzi-1);
        strncpy(yx_mk_shu[jl_shu].lujing, mulu ,zd_lujing-1);
        snprintf(yx_mk_shu[jl_shu].docker_id, sizeof(yx_mk_shu[jl_shu].docker_id), "%s", docker_id);
        strcpy(yx_mk_shu[jl_shu].zt, "运行中");
        yx_mk_shu[jl_shu].sj = time(NULL);
        jl_shu++;
    }

    char compose_wj[zd_lujing];
    snprintf(compose_wj,sizeof(compose_wj),"%s/docker-compose.yml",mulu);
    
    FILE *fp = fopen(compose_wj, "r");
    if(fp){
        char dk[512];
        int zhaodao = 0;
        while (fgets(dk, sizeof(dk), fp)){
            if(strstr(dk, "ports:") || strstr(dk, "-\"")){
                printf("%s", dk);
                zhaodao = 1;
            }
        }
        if(!zhaodao){
            printf("未找到端口映射信息，请检查docker-compose.yml文件。\n");
        }
        fclose(fp);
    }else{
        printf("无法打开文件 %s 以读取端口信息。\n", compose_wj);
    }
}

void web_mk_tingzhi(int suoyin){
    if(suoyin < 0 || suoyin >= jl_shu){
        printf("无效的索引，无法停止模块。\n");
        return;
    }
    char mingling[512];
    snprintf(mingling,sizeof(mingling),"cd %s && docker compose down",yx_mk_shu[suoyin].lujing);
    printf("正在停止模块...");
    int jieguo = system(mingling);
    if(jieguo == 0){
        printf("模块 %s 停止成功！\n", yx_mk_shu[suoyin].ming);
        web_wj_shanchu(yx_mk_shu[suoyin].lujing);
        for(int i=suoyin; i<jl_shu-1; i++){
            yx_mk_shu[i] = yx_mk_shu[i+1];
        }
        jl_shu--;
    }else{
        printf("模块 %s 停止失败，请检查错误信息。\n", yx_mk_shu[suoyin].ming);
    }
}

void web_mk_zt_gengxin(void){
    for (int i = 0; i < jl_shu;i++){
        char mingling[512];
        snprintf(mingling,sizeof(mingling),"docker ps -q --filter \"id=%s\" | grep -c",yx_mk_shu[i].docker_id);
        FILE *guan = popen(mingling, "r");

        if(guan){
            char jieguo[16];
            if(fgets(jieguo,sizeof(jieguo),guan)){
                if(atoi(jieguo) == 0){
                    web_wj_shanchu(yx_mk_shu[i].lujing);

                    for(int j=i; j<jl_shu-1; j++){
                        yx_mk_shu[j] = yx_mk_shu[j+1];
                    }
                    jl_shu--;
                    i--;
                }
            }
        }
        pclose(guan);
    }
}

void web_wj_chuangjian(const char *mulu,const char *docker_id){
    char wj_lujing[zd_lujing];
    snprintf(wj_lujing,sizeof(wj_lujing),"%s/.running",mulu);
    FILE *fp = fopen(wj_lujing, "w");
    if(fp){
        fprintf(fp, "%s|%ld",docker_id,time(NULL));
        fclose(fp);
        printf(" 标记文件创建成功");
    }
}

void web_wj_shanchu(const char *mulu){
    char mingling[zd_lujing];
    snprintf(mingling,sizeof(mingling),"%s/.running",mulu);
    unlink(mingling);
}

char* web_wj_duqu(const char *mulu){
    static char docker_id[dokcer_id];
    docker_id[0] = '\0';

    char wj_lujing[zd_lujing];
    snprintf(wj_lujing,sizeof(wj_lujing),"%s/.running",mulu);

    FILE *fp = fopen(wj_lujing, "r");
    if(fp){
        char hang[256];
        if(fgets(hang,sizeof(hang),fp)){
            char *token = strtok(hang, "|");
            if(token){
                strncpy(docker_id, token, dokcer_id - 1);
                docker_id[dokcer_id - 1] = '\0';
            }           
        }
        fclose(fp);
    }
    return docker_id;
}

void web_mk_yx_zonglan(void){
    web_mk_zt_gengxin();

    printf("\n=== 运行模块总览 ===\n");
    if(jl_shu == 0){
        printf("当前没有运行中的模块。\n");
        return;
    }

    for(int i=0; i<jl_shu; i++){
        char jian_id[13];
        strncpy(jian_id, yx_mk_shu[i].docker_id, 12);
        jian_id[12] = '\0';

        char shijian[50];
        struct tm *tm_info = localtime(&yx_mk_shu[i].sj);
        strftime(shijian, sizeof(shijian), "%H:%M:%S", tm_info);

        printf("模块名称: %s | 状态: %s | Docker ID: %s... | 启动时间: %s\n",
               yx_mk_shu[i].ming,
               yx_mk_shu[i].zt,
               jian_id,
               shijian);
    }
}

static void dg_saomiao(const char *mulu){
    DIR *dir = opendir(mulu);
    if (dir == NULL){
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL){
        if(entry->d_name[0]=='.'){
            if(strcmp(entry->d_name,".running") == 0){
                char *docker_id = web_wj_duqu(mulu);
                if(strlen(docker_id)>0 && jl_shu < zd_mk_shu){
                    char *lujing = strrchr(mulu,'/');
                    char ming[zd_mingzi];
                    if(lujing){
                        strncpy(ming,lujing + 1,zd_mingzi - 1);
                    }else{
                        strncpy(ming,mulu,zd_mingzi - 1);
                    }
                    ming[zd_mingzi - 1] = '\0';

                    strncpy(yx_mk_shu[jl_shu].ming, ming, zd_mingzi - 1);
                    strncpy(yx_mk_shu[jl_shu].lujing, mulu,zd_lujing -1);
                    strncpy(yx_mk_shu[jl_shu].docker_id, docker_id, docker_id - 1);
                    strcpy(yx_mk_shu[jl_shu].zt,"运行中");

                    char wj_lujing[zd_lujing];
                    snprintf(wj_lujing,sizeof(wj_lujing),"%s/.running",mulu);
                    FILE *fp = fopen(wj_lujing,"r");
                    if(fp){
                        char hang[256];
                        if(fgets(hang,sizeof(hang),fp)){
                            char *token = strtok(hang,"|");
                            if(token) token = strtok(NULL,"|");
                            if(token) yx_mk_shu[jl_shu].sj = atol(token);
                        }
                        fclose(fp);
                    }
                    jl_shu++;
            }
        }
    continue;
    }
        if(entry->d_type == DT_DIR){
            char xin_lujing[zd_lujing];
            snprintf(xin_lujing, sizeof(xin_lujing), "%s/%s", mulu, entry->d_name);
            dg_saomiao(xin_lujing);
        }
    }
    closedir(dir);
}

void web_mk_zhuangtai_jiazai(void){
    jl_shu = 0;
    dg_saomiao(genmulu);
}

void web_guanli(void){

}