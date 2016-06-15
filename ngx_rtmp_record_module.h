
/*
 * Copyright (C) Roman Arutyunyan
 */


#ifndef _NGX_RTMP_RECORD_H_INCLUDED_
#define _NGX_RTMP_RECORD_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>
#include "ngx_rtmp.h"


#define NGX_RTMP_RECORD_OFF             0x01
#define NGX_RTMP_RECORD_AUDIO           0x02
#define NGX_RTMP_RECORD_VIDEO           0x04
#define NGX_RTMP_RECORD_KEYFRAMES       0x08
#define NGX_RTMP_RECORD_MANUAL          0x10

/*
录制与直播是如何关联的 
直播如何找到对应录制配置信息 
直播如何通知录制 
*/

typedef struct {
    ngx_str_t                           id; /* record name  用于区分多个record  */
    ngx_uint_t                          flags;
    ngx_str_t                           path; /* 录制flv文件目录 */
    size_t                              max_size; /* 设置录制文件的最大值 */
    size_t                              max_frames; /* 设置每个录制文件的视频帧的最大数量 */
    ngx_msec_t                          interval; /* 指定数量毫秒后重启录制 */
    ngx_str_t                           suffix; /* 录制文件后缀 */
    ngx_flag_t                          unique; /* 是否添加时间戳到录制文件,默认关闭 */
    ngx_flag_t                          append; /* 切换文件附加模式 */
    ngx_flag_t                          lock_file; /* 当前录制文件将被 fcntl 调用锁定 */
    ngx_flag_t                          notify;/* 录制启动或停止文件时发送 NetStream.Record.Start 和 NetStream.Record.Stop 状态信息(onStatus)到发布者 */
    ngx_url_t                          *url;   /* */

    void                              **rec_conf;
    ngx_array_t                         rec; /* ngx_rtmp_record_app_conf_t * */
} ngx_rtmp_record_app_conf_t;


/*
rtmp 录制控制信息 
*/
typedef struct {
    ngx_rtmp_record_app_conf_t         *conf; 
    ngx_file_t                          file;
    ngx_uint_t                          nframes;  /* 帧计数 */
    uint32_t                            epoch;
    uint32_t 							time_shift;
    ngx_time_t                          last;
    time_t                              timestamp;
    unsigned                            failed:1;
    unsigned                            initialized:1; /* 是否初始化文件 */
    unsigned                            aac_header_sent:1; /* 是否录制aac头 */
    unsigned                            avc_header_sent:1; /* 是否录制avc头 */
    unsigned                            video_key_sent:1; 
    unsigned                            audio:1; /* 音频 */
    unsigned                            video:1; /* 视频 */
} ngx_rtmp_record_rec_ctx_t;


typedef struct {
    ngx_array_t                         rec; /*  数组成员为 ngx_rtmp_record_rec_ctx_t  */
    u_char                              name[NGX_RTMP_MAX_NAME];
    u_char                              args[NGX_RTMP_MAX_ARGS];
} ngx_rtmp_record_ctx_t;


ngx_uint_t ngx_rtmp_record_find(ngx_rtmp_record_app_conf_t *racf,
           ngx_str_t *id);


/* Manual recording control,
 * 'n' is record node index in config array.
 * Note: these functions allocate path in static buffer */

ngx_int_t ngx_rtmp_record_open(ngx_rtmp_session_t *s, ngx_uint_t n,
          ngx_str_t *path);
ngx_int_t ngx_rtmp_record_close(ngx_rtmp_session_t *s, ngx_uint_t n,
          ngx_str_t *path);


typedef struct {
    ngx_str_t                           recorder;
    ngx_str_t                           path;
} ngx_rtmp_record_done_t;


typedef ngx_int_t (*ngx_rtmp_record_done_pt)(ngx_rtmp_session_t *s,
        ngx_rtmp_record_done_t *v);


extern ngx_rtmp_record_done_pt          ngx_rtmp_record_done;


extern ngx_module_t                     ngx_rtmp_record_module;


#endif /* _NGX_RTMP_RECORD_H_INCLUDED_ */
