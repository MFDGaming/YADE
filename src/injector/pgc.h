#ifndef YADE_PGC_H
#define YADE_PGC_H

#include <stdint.h>

#define PGC_TM_FR_25 1
#define PGC_TM_FR_30 3

typedef struct {
    uint8_t stream_num_pf;
    uint8_t reserved;
} pgc_ast_ctl_t;

typedef struct {
    uint8_t stream_num_4_3_pf;
    uint8_t stream_num_16_9;
    uint8_t stream_num_letterbox;
    uint8_t stream_num_panscan;
} pgc_spst_ctl_t;

typedef struct {
    uint16_t next_pgcn;
    uint16_t prev_pgcn;
    uint16_t goup_pgcn;
    uint8_t pg_pb_mode;
    uint8_t still_time;
} pgc_nv_ctl_t;

typedef struct {
    uint32_t pgc_cnt;
#define unset_pgc_cnt_pn() pgc_cnt &= 0xffff00ff
#define unset_pgc_cnt_cn() pgc_cnt &= 0xffffff00
#define set_pgc_cnt_pn(x) pgc_cnt |= (x & 255) << 8
#define set_pgc_cnt_cn(x) pgc_cnt |= (x & 255)
#define get_pgc_cnt_pn() pgc_cnt >> 8 & 255
#define get_pgc_cnt_cn() pgc_cnt & 255
    uint32_t pgc_pb_tm;
#define unset_pgc_pb_tm_hours() pgc_pb_tm &= 0xffffff
#define unset_pgc_pb_tm_minutes() pgc_pb_tm &= 0xff00ffff
#define unset_pgc_pb_tm_seconds() pgc_pb_tm &= 0xffff00ff
#define unset_pgc_pb_tm_frames() pgc_pb_tm &= 0xffffffc0
#define unset_pgc_pb_tm_frame_rate() pgc_pb_tm &= 0xffffff3f
#define set_pgc_pb_tm_hours(x) pgc_pb_tm |= (x & 255) << 24
#define set_pgc_pb_tm_minutes(x) pgc_pb_tm |= (x & 255) << 16
#define set_pgc_pb_tm_seconds(x) pgc_pb_tm |= (x & 255) << 8
#define set_pgc_pb_tm_frames(x) pgc_pb_tm |= (x & 63)
#define set_pgc_pb_tm_frame_rate(x) pgc_pb_tm |= (x & 3) << 6
#define get_pgc_pb_tm_hours() pgc_pb_tm >> 24 & 255
#define get_pgc_pb_tm_minutes() pgc_pb_tm >> 16 & 255
#define get_pgc_pb_tm_seconds() pgc_pb_tm >> 8 & 255
#define get_pgc_pb_tm_frames() pgc_pb_tm & 63
#define get_pgc_pb_tm_frame_rate(x) pgc_pb_tm >> 6 & 3
    uint32_t pgc_uop_ctl;
    pgc_ast_ctl_t pgc_ast_ctlt[8];
    pgc_spst_ctl_t pgc_spst_ctlt[32];
    pgc_nv_ctl_t pgc_nv_ctl;
    uint32_t pgc_sp_plt[16];
    uint16_t pgc_cmdt_sa;
    uint16_t pgc_pgmap_sa;
    uint16_t c_pbit_sa;
    uint16_t c_posit_sa;
} pgc_gi_t;

typedef struct {
    uint16_t pre_cmd_n;
    uint16_t post_cmd_n;
    uint16_t c_cmd_n;
    uint16_t pgc_cmdt_ea;
} pgc_cmdti_t;

typedef struct {
    pgc_cmdti_t pgc_cmdti;
    uint8_t cmds[1572840];
} pgc_cmdt_t;

typedef struct {
    uint8_t en_cn[255];
} pgc_pgmap_t;

typedef struct {
    uint32_t c_cat;
#define unset_c_cat_cell_block_mode() c_cat &= 0x3fffffff
#define unset_c_cat_cell_block_type() c_cat &= 0xcfffffff
#define unset_c_cat_seamless_pb_flag() c_cat &= 0xf7ffffff
#define unset_c_cat_interleaved_alloc_flag() c_cat &= 0xfbffffff
#define unset_c_cat_sys_tm_clk_discontinuity_flag() c_cat &= 0xfdffffff
#define unset_c_cat_seamless_angle_change_flag() c_cat &= 0xfeffffff
#define unset_c_cat_cell_pb_mode() c_cat &= 0xffbfffff
#define unset_c_cat_access_restriction_flag() c_cat &= 0xffdfffff
#define unset_c_cat_cell_type() c_cat &= 0xffe0ffff
#define unset_c_cat_cell_still_time() c_cat &= 0xffff00ff
#define unset_c_cat_cell_cmd_num() c_cat &= 0xffffff00
#define set_c_cat_cell_block_mode(x) c_cat |= (x & 3) << 30
#define set_c_cat_cell_block_type(x) c_cat |= (x & 3) << 28
#define set_c_cat_seamless_pb_flag(x) c_cat |= (x & 1) << 27
#define set_c_cat_interleaved_alloc_flag(x) c_cat |= (x & 1) << 26
#define set_c_cat_sys_tm_clk_discontinuity_flag(x) c_cat |= (x & 1) << 25
#define set_c_cat_seamless_angle_change_flag(x) c_cat |= (x & 1) << 24
#define set_c_cat_cell_pb_mode(x) c_cat |= (x & 1) << 22
#define set_c_cat_access_restriction_flag(x) c_cat |= (x & 1) << 21
#define set_c_cat_cell_type(x) c_cat |= (x & 31) << 16
#define set_c_cat_cell_still_time(x) c_cat |= (x & 255) << 8
#define set_c_cat_cell_cmd_num(x) c_cat |= (x & 255)
#define get_c_cat_cell_block_mode() c_cat >> 30 & 3
#define get_c_cat_cell_block_type() c_cat >> 28 & 3
#define get_c_cat_seamless_pb_flag() c_cat >> 27 & 1
#define get_c_cat_interleaved_alloc_flag() c_cat >> 26 & 1
#define get_c_cat_sys_tm_clk_discontinuity_flag() c_cat >> 25 & 1
#define get_c_cat_seamless_angle_change_flag() c_cat >> 24 & 1
#define get_c_cat_cell_pb_mode() c_cat >> 22 & 1
#define get_c_cat_access_restriction_flag() c_cat >> 21 & 1
#define get_c_cat_cell_type() c_cat >> 16 & 31
#define get_c_cat_cell_still_time() c_cat >> 8 & 255
#define get_c_cat_cell_cmd_num() c_cat & 255
    uint32_t c_pbtm;
#define unset_c_pbtm_hours() c_pbtm &= 0xffffff
#define unset_c_pbtm_minutes() c_pbtm &= 0xff00ffff
#define unset_c_pbtm_seconds() c_pbtm &= 0xffff00ff
#define unset_c_pbtm_frames() c_pbtm &= 0xffffffc0
#define unset_c_pbtm_frame_rate() c_pbtm &= 0xffffff3f
#define set_c_pbtm_hours(x) c_pbtm |= (x & 255) << 24
#define set_c_pbtm_minutes(x) c_pbtm |= (x & 255) << 16
#define set_c_pbtm_seconds(x) c_pbtm |= (x & 255) << 8
#define set_c_pbtm_frames(x) c_pbtm |= (x & 63)
#define set_c_pbtm_frame_rate(x) c_pbtm |= (x & 3) << 6
#define get_c_pbtm_hours() c_pbtm >> 24 & 255
#define get_c_pbtm_minutes() c_pbtm >> 16 & 255
#define get_c_pbtm_seconds() c_pbtm >> 8 & 255
#define get_c_pbtm_frames() c_pbtm & 63
#define get_c_pbtm_frame_rate(x) c_pbtm >> 6 & 3
    uint32_t c_fvobu_sa;
    uint32_t c_filvu_ea;
    uint32_t c_lvobu_sa;
    uint32_t c_lvobu_ea;
} c_pbi_t;

typedef struct {
    c_pbi_t pbi[255];
} c_pbit_t;

typedef struct {
    uint16_t c_vob_idn;
    uint8_t reserved;
    uint8_t c_idn;
} c_posi_t;

typedef struct {
    c_posi_t c_posi[255];
} c_posit_t;

typedef struct {
    pgc_gi_t pgc_gi;
    pgc_cmdt_t pgc_cmdt;
    pgc_pgmap_t pgc_pgmap;
    c_pbit_t c_pbit;
    c_posit_t c_posit;
} pgc_t;

void pgc_build(pgc_t *pgc, uint8_t **buf, uint32_t *len);

#endif
