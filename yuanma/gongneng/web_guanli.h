// yuanma/gongneng/web_guanli.h
#ifndef WEB_GUANLI_H
#define WEB_GUANLI_H

#define zd_mk_shu 100
#define zd_mingzi 100
#define zd_lujing 256
#define dokcer_id 64


typedef enum{
    mk = 1,
    ml = 0
}leixing;

typedef struct{
    leixing lx;
    char ming[zd_mingzi];
    char lujing[zd_lujing];
}jg;

typedef struct{
    char ming[zd_mingzi];
    char lujing[zd_lujing];
    char docker_id[dokcer_id];
    char zt[20];
    time_t sj;
}yx_zhuangtai;

extern yx_zhuangtai yx_mk_shu[zd_mk_shu];
extern int jl_shu;

int web_saomiao(const char *mulu,jg *mk_shuzu,int zd_shu);
int web_panduan(const char *mulu);
void web_mk_yunxing(const char *ming,const char *mulu);
void web_mk_tingzhi(int suoyin);
void web_mk_zt_gengxin(void);
void web_wj_chuangjian(const char *mulu,const char *docker_id);
void web_wj_shanchu(const char *mulu);
char* web_wj_duqu(const char *mulu);
void web_mk_yx_zonglan(void);
#endif