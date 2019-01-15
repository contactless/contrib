#ifndef NCHAN_MODULE_H
#define NCHAN_MODULE_H
//#define NCHAN_SUBSCRIBER_LEAK_DEBUG 1
//#define NCHAN_MSG_RESERVE_DEBUG 1
//#define NCHAN_MSG_LEAK_DEBUG 1
//#define NCHAN_BENCHMARK 1

//debugging config
//#define FAKESHARD 1
#if FAKESHARD
//#define PUB_FAKE_WORKER 0
//#define SUB_FAKE_WORKER 1
//#define ONE_FAKE_CHANNEL_OWNER 2
#define MAX_FAKE_WORKERS 5
#endif

#include <ngx_http.h>

//building for old versions
#ifndef NGX_MAX_INT_T_VALUE
#if (NGX_PTR_SIZE == 4)
#define NGX_MAX_INT_T_VALUE  2147483647
#else
#define NGX_MAX_INT_T_VALUE  9223372036854775807
#endif
#endif

#include <nginx.h>
#include <nchan_types.h>
#include <nchan_defs.h>
#include <util/nchan_util.h>
#include <util/nchan_channel_id.h>
#include <util/nchan_channel_info.h>
#include <util/nchan_msgid.h>
#include <util/nchan_output.h>

extern ngx_pool_t *nchan_pool;
extern ngx_int_t nchan_worker_processes;
extern ngx_module_t ngx_nchan_module;
extern nchan_store_t *nchan_store;

extern int nchan_stub_status_enabled;

ngx_int_t nchan_stub_status_handler(ngx_http_request_t *r);
ngx_int_t nchan_pubsub_handler(ngx_http_request_t *r);
ngx_buf_t *nchan_channel_info_buf(ngx_str_t *accept_header, ngx_uint_t messages, ngx_uint_t subscribers, time_t last_seen, nchan_msg_id_t *last_msgid, ngx_str_t **generated_content_type);

time_t nchan_loc_conf_message_timeout(nchan_loc_conf_t *cf);
ngx_int_t nchan_loc_conf_max_messages(nchan_loc_conf_t *cf);

ngx_int_t nchan_maybe_send_channel_event_message(ngx_http_request_t *, channel_event_type_t);

#define nchan_update_stub_status(counter_name, count) __memstore_update_stub_status(offsetof(nchan_stub_status_t, counter_name), count)
void __memstore_update_stub_status(off_t offset, int count);
nchan_stub_status_t *nchan_get_stub_status_stats(void);
size_t nchan_get_used_shmem(void);

#if NCHAN_SUBSCRIBER_LEAK_DEBUG
void subscriber_debug_add(subscriber_t *);
void subscriber_debug_remove(subscriber_t *);
void subscriber_debug_assert_isempty(void);
#endif

#if NCHAN_BENCHMARK
int nchan_timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y);
#endif

#define nchan_log_notice(fmt, args...)  ngx_log_error(NGX_LOG_NOTICE, ngx_cycle->log, 0, "nchan: " fmt, ##args)
#define nchan_log_warning(fmt, args...) ngx_log_error(NGX_LOG_WARN,   ngx_cycle->log, 0, "nchan: " fmt, ##args)
#define nchan_log_error(fmt, args...)   ngx_log_error(NGX_LOG_ERR,    ngx_cycle->log, 0, "nchan: " fmt, ##args)
#define nchan_log_request_warning(request, fmt, args...) ngx_log_error(NGX_LOG_WARN, (request)->connection->log, 0, "nchan: " fmt, ##args)
#define nchan_log_request_error(request, fmt, args...)    ngx_log_error(NGX_LOG_ERR, (request)->connection->log, 0, "nchan: " fmt, ##args)

#define nchan_log(level, log, errno, fmt, args...) ngx_log_error(level, log, errno, "nchan: " fmt, ##args)


#endif /*NCHAN_MODULE_H*/
