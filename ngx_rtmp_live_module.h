
/*
 * Copyright (C) Roman Arutyunyan
 */


#ifndef _NGX_RTMP_LIVE_H_INCLUDED_
#define _NGX_RTMP_LIVE_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include "ngx_rtmp.h"
#include "ngx_rtmp_cmd_module.h"
#include "ngx_rtmp_bandwidth.h"
#include "ngx_rtmp_streams.h"


typedef struct ngx_rtmp_live_ctx_s ngx_rtmp_live_ctx_t;
typedef struct ngx_rtmp_live_stream_s ngx_rtmp_live_stream_t;

/*  ֱ������rtmp ������Ϣ */
typedef struct {
    unsigned                            active:1;  /* �Ƿ�������  */
    uint32_t                            timestamp;  /* ����ʱ��� */
    uint32_t                            csid;    /* chunk stream id */
    uint32_t                            dropped; /* �������ĵĶ�Ӧʱ��֮�� */
} ngx_rtmp_live_chunk_stream_t;

/*
ֱ������Ӧrtmp��ctx 
*/
struct ngx_rtmp_live_ctx_s {
    ngx_rtmp_session_t                 *session; /* ��Ӧrtmp session  */
    ngx_rtmp_live_stream_t             *stream; /* ��Ӧֱ���� */
    ngx_rtmp_live_ctx_t                *next;  /* ͬһ��ֱ������һ��rtmp�� ctx */
    ngx_uint_t                          ndropped; /* �������ĸ��� */
    ngx_rtmp_live_chunk_stream_t        cs[2]; /*  ��Ƶ��  ��Ƶ�� */
    ngx_uint_t                          meta_version;
    ngx_event_t                         idle_evt;  /* ���м���¼�  */
    unsigned                            active:1;  /* �Ƿ�����ֱ�� */
    unsigned                            publishing:1; /* �Ƿ����ڷ��� */
    unsigned                            silent:1; /* �Ƿ���onstatus���� */
    unsigned                            paused:1; /* �Ƿ�ֹͣ */
};

/* ֱ������Ϣ */
struct ngx_rtmp_live_stream_s {
    u_char                              name[NGX_RTMP_MAX_NAME];  /* ������  */
    ngx_rtmp_live_stream_t             *next; /* ָ����һ��hash�ڵ� */
    ngx_rtmp_live_ctx_t                *ctx;  /*  ͬһ��ֱ���� ctxͷ�ڵ� */

    /* ����ͳ��*/
    ngx_rtmp_bandwidth_t                bw_in;
    ngx_rtmp_bandwidth_t                bw_in_audio;
    ngx_rtmp_bandwidth_t                bw_in_video;
    ngx_rtmp_bandwidth_t                bw_out;
    ngx_msec_t                          epoch;
    unsigned                            active:1; /* ��ʾ�Ƿ���live״̬(�Ƿ�����) */
    unsigned                            publishing:1;/* �Ƿ�Ϊ���ڷ��� */
};


typedef struct {
    ngx_int_t                           nbuckets; /* hashͰ���� */
    ngx_rtmp_live_stream_t            **streams;  /* ����Ϣhash�� */
    ngx_flag_t                          live; /* �Ƿ���ֱ��  */
    ngx_flag_t                          meta; /* Ĭ��Ϊ1����ʾ����Ԫ���ݵ��ͻ��� */
    ngx_msec_t                          sync; /* ͬ����Ƶ����Ƶ��������û��������Խ��շ����ʣ��������ᶪ��һЩ֡���⽫����ͬ�����⡣
    ��ʱ������ sync ָ����ֵ�����ᷢ��һ������֡�����������⡣Ĭ��Ϊ 300 ms */
    ngx_msec_t                          idle_timeout; /* ��ָֹ��ʱ��������(û����Ƶ/��Ƶ����)�ķ�������  */
    ngx_flag_t                          atc;  /*  δ����ʹ�� */
    ngx_flag_t                          interleave; /* Ĭ��Ϊ0����1��ʾ ��Ƶ����Ƶ���ݻ���ͬһ�� RTMP chunk ���д���  */
    ngx_flag_t                          wait_key; /* Ĭ��Ϊ0����1��ʾʹ��Ƶ����һ���ؼ�֡��ʼ */
    ngx_flag_t                          wait_video;  /* Ĭ��Ϊ0����1��ʾ�ڵ�һ����Ƶ֡����֮ǰ������Ƶ  */
    ngx_flag_t                          publish_notify;  /* Ĭ��Ϊ0����1��ʾ���� NetStream.Publish.Start �� NetStream.Publish.Stop ���û�*/
    ngx_flag_t                          play_restart; /* ����������ֹͣʱ���� NetStream.Play.Start �� NetStream.Play.Stop ��ÿ���û� */
    ngx_flag_t                          idle_streams;
    ngx_msec_t                          buflen;
    ngx_pool_t                         *pool; /* �ڴ�� ������Դ��ͳһ����  */
    ngx_rtmp_live_stream_t             *free_streams;  /* �ͷź�ֱ��������  */
} ngx_rtmp_live_app_conf_t;


extern ngx_module_t  ngx_rtmp_live_module;


#endif /* _NGX_RTMP_LIVE_H_INCLUDED_ */
