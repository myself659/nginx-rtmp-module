
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

/*  直播流的rtmp 块流信息 */
typedef struct {
    unsigned                            active:1;  /* 是否启动流  */
    uint32_t                            timestamp;  /* 绝对时间戳 */
    uint32_t                            csid;    /* chunk stream id */
    uint32_t                            dropped; /* 丢弃报文的对应时间之和 */
} ngx_rtmp_live_chunk_stream_t;

/*
直播流对应rtmp流ctx 
*/
struct ngx_rtmp_live_ctx_s {
    ngx_rtmp_session_t                 *session; /* 对应rtmp session  */
    ngx_rtmp_live_stream_t             *stream; /* 对应直播流 */
    ngx_rtmp_live_ctx_t                *next;  /* 同一个直播流下一个rtmp流 ctx */
    ngx_uint_t                          ndropped; /* 丢弃报文个数 */
    ngx_rtmp_live_chunk_stream_t        cs[2]; /*  音频流  视频流 */
    ngx_uint_t                          meta_version;
    ngx_event_t                         idle_evt;  /* 空闲检查事件  */
    unsigned                            active:1;  /* 是否正在直播 */
    unsigned                            publishing:1; /* 是否正在发布 */
    unsigned                            silent:1; /* 是否发送onstatus报文 */
    unsigned                            paused:1; /* 是否停止 */
};

/* 直播流信息 */
struct ngx_rtmp_live_stream_s {
    u_char                              name[NGX_RTMP_MAX_NAME];  /* 流名称  */
    ngx_rtmp_live_stream_t             *next; /* 指向下一个hash节点 */
    ngx_rtmp_live_ctx_t                *ctx;  /*  同一个直播流 ctx头节点 */

    /* 流量统计*/
    ngx_rtmp_bandwidth_t                bw_in;
    ngx_rtmp_bandwidth_t                bw_in_audio;
    ngx_rtmp_bandwidth_t                bw_in_video;
    ngx_rtmp_bandwidth_t                bw_out;
    ngx_msec_t                          epoch;
    unsigned                            active:1; /* 表示是否处于live状态(是否启动) */
    unsigned                            publishing:1;/* 是否为正在发布 */
};


typedef struct {
    ngx_int_t                           nbuckets; /* hash桶个数 */
    ngx_rtmp_live_stream_t            **streams;  /* 流信息hash表 */
    ngx_flag_t                          live; /* 是否开启直播  */
    ngx_flag_t                          meta; /* 默认为1，表示发送元数据到客户端 */
    ngx_msec_t                          sync; /* 同步音频和视频流。如果用户带宽不足以接收发布率，服务器会丢弃一些帧。这将导致同步问题。
    当时间戳差超过 sync 指定的值，将会发送一个绝对帧来解决这个问题。默认为 300 ms */
    ngx_msec_t                          idle_timeout; /* 终止指定时间内闲置(没有音频/视频数据)的发布连接  */
    ngx_flag_t                          atc;  /*  未见有使用 */
    ngx_flag_t                          interleave; /* 默认为0，置1表示 音频和视频数据会在同一个 RTMP chunk 流中传输  */
    ngx_flag_t                          wait_key; /* 默认为0，置1表示使视频流从一个关键帧开始 */
    ngx_flag_t                          wait_video;  /* 默认为0，置1表示在第一个视频帧发送之前禁用音频  */
    ngx_flag_t                          publish_notify;  /* 默认为0，置1表示发送 NetStream.Publish.Start 和 NetStream.Publish.Stop 给用户*/
    ngx_flag_t                          play_restart; /* 发布启动或停止时发送 NetStream.Play.Start 和 NetStream.Play.Stop 到每个用户 */
    ngx_flag_t                          idle_streams;
    ngx_msec_t                          buflen;
    ngx_pool_t                         *pool; /* 内存池 对于资源的统一管理  */
    ngx_rtmp_live_stream_t             *free_streams;  /* 释放后直流流链表  */
} ngx_rtmp_live_app_conf_t;


extern ngx_module_t  ngx_rtmp_live_module;


#endif /* _NGX_RTMP_LIVE_H_INCLUDED_ */
