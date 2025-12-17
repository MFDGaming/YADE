#include "pgc.h"
#include <stdlib.h>
#include <string.h>

#define MIN(x, y) (x > y ? y : x)
#define MAX(x, y) (x > y ? x : y)

static void write8(uint8_t *buffer, uint32_t *off, uint8_t val) {
    buffer[(*off)++] = val;
}

static void write16(uint8_t *buffer, uint32_t *off, uint16_t val) {
    uint8_t e[2] = {0, 1};
    uint8_t *v = (uint8_t *)&val;
    if (*(uint16_t *)e == 1) {
        buffer[*off] = v[0];
        buffer[*off+1] = v[1];
    } else {
        buffer[*off] = v[1];
        buffer[*off+1] = v[0];
    }
    *off += 2;
}

static void write32(uint8_t *buffer, uint32_t *off, uint32_t val) {
    uint8_t e[2] = {0, 1};
    uint8_t *v = (uint8_t *)&val;
    if (*(uint16_t *)e == 1) {
        buffer[*off] = v[0];
        buffer[*off+1] = v[1];
        buffer[*off+2] = v[2];
        buffer[*off+3] = v[3];
    } else {
        buffer[*off] = v[3];
        buffer[*off+1] = v[2];
        buffer[*off+2] = v[1];
        buffer[*off+3] = v[0];
    }
    *off += 4;
}

static uint32_t calculate_len(pgc_t *pgc) {
    uint32_t cn = pgc->pgc_gi.get_pgc_cnt_cn();
    uint32_t pn = pgc->pgc_gi.get_pgc_cnt_pn();
    uint32_t pgc_cmdt_len = 0, pgc_pgmap_len = 0;
    uint32_t c_pbit_len = 0, c_posit_len = 0;
    uint32_t *len_max_off = &pgc_cmdt_len, max_off = pgc->pgc_gi.pgc_cmdt_sa;
    if (max_off < pgc->pgc_gi.pgc_pgmap_sa) {
        max_off = pgc->pgc_gi.pgc_pgmap_sa;
        len_max_off = &pgc_pgmap_len;
    }
    if (max_off < pgc->pgc_gi.c_pbit_sa) {
        max_off = pgc->pgc_gi.c_pbit_sa;
        len_max_off = &c_pbit_len;
    }
    if (max_off < pgc->pgc_gi.c_posit_sa) {
        max_off = pgc->pgc_gi.c_posit_sa;
        len_max_off = &c_posit_len;
    }
    if (pgc->pgc_gi.pgc_cmdt_sa) {
        pgc_cmdt_len = pgc->pgc_cmdt.pgc_cmdti.pgc_cmdt_ea + 1;
    }
    if (pgc->pgc_gi.pgc_pgmap_sa) {
        pgc_pgmap_len = pn;
    }
    if (pgc->pgc_gi.c_pbit_sa) {
        c_pbit_len = cn * 4;
    }
    if (pgc->pgc_gi.c_posit_sa) {
        c_posit_len = cn * 24;
    }
    if (max_off >= 0xec) {
        return max_off + *len_max_off;
    }
    return 0xec;
}

static void write_pgc_gi(pgc_t *pgc, uint8_t *buf) {
    uint32_t off = 0;
    write32(buf, &off, pgc->pgc_gi.pgc_cnt);
    write32(buf, &off, pgc->pgc_gi.pgc_pb_tm);
    write32(buf, &off, pgc->pgc_gi.pgc_uop_ctl);
    for (int i = 0; i < 8; ++i) {
        write8(buf, &off, pgc->pgc_gi.pgc_ast_ctlt[i].stream_num_pf);
        write8(buf, &off, pgc->pgc_gi.pgc_ast_ctlt[i].reserved);
    }
    for (int i = 0; i < 32; ++i) {
        write8(buf, &off, pgc->pgc_gi.pgc_spst_ctlt[i].stream_num_4_3_pf);
        write8(buf, &off, pgc->pgc_gi.pgc_spst_ctlt[i].stream_num_16_9);
        write8(buf, &off, pgc->pgc_gi.pgc_spst_ctlt[i].stream_num_letterbox);
        write8(buf, &off, pgc->pgc_gi.pgc_spst_ctlt[i].stream_num_panscan);
    }
    write16(buf, &off, pgc->pgc_gi.pgc_nv_ctl.next_pgcn);
    write16(buf, &off, pgc->pgc_gi.pgc_nv_ctl.prev_pgcn);
    write16(buf, &off, pgc->pgc_gi.pgc_nv_ctl.goup_pgcn);
    write8(buf, &off, pgc->pgc_gi.pgc_nv_ctl.pg_pb_mode);
    write8(buf, &off, pgc->pgc_gi.pgc_nv_ctl.still_time);
    for (int i = 0; i < 16; ++i) {
        write32(buf, &off, pgc->pgc_gi.pgc_sp_plt[i]);
    }
    write16(buf, &off, pgc->pgc_gi.pgc_cmdt_sa);
    write16(buf, &off, pgc->pgc_gi.pgc_pgmap_sa);
    write16(buf, &off, pgc->pgc_gi.c_pbit_sa);
    write16(buf, &off, pgc->pgc_gi.c_posit_sa);
}

static void write_pgc_cmdt(pgc_t *pgc, uint8_t *buf) {
    if (pgc->pgc_gi.pgc_cmdt_sa) {
        uint32_t off = pgc->pgc_gi.pgc_cmdt_sa;
        write16(buf, &off, pgc->pgc_cmdt.pgc_cmdti.pre_cmd_n);
        write16(buf, &off, pgc->pgc_cmdt.pgc_cmdti.post_cmd_n);
        write16(buf, &off, pgc->pgc_cmdt.pgc_cmdti.c_cmd_n);
        write16(buf, &off, pgc->pgc_cmdt.pgc_cmdti.pgc_cmdt_ea);
        memcpy(buf+off, pgc->pgc_cmdt.cmds, pgc->pgc_cmdt.pgc_cmdti.pgc_cmdt_ea - 7);
    }
}

static void write_pgc_pgmap(pgc_t *pgc, uint8_t *buf) {
    if (pgc->pgc_gi.pgc_pgmap_sa) {
        memcpy(buf+pgc->pgc_gi.pgc_pgmap_sa, pgc->pgc_pgmap.en_cn, pgc->pgc_gi.get_pgc_cnt_pn());
    }
}

static void write_c_pbit(pgc_t *pgc, uint8_t *buf) {
    if (pgc->pgc_gi.c_pbit_sa) {
        uint32_t off = pgc->pgc_gi.c_pbit_sa;
        for (int i = 0; i < (pgc->pgc_gi.get_pgc_cnt_cn()); ++i) {
            write32(buf, &off, pgc->c_pbit.pbi[i].c_cat);
            write32(buf, &off, pgc->c_pbit.pbi[i].c_pbtm);
            write32(buf, &off, pgc->c_pbit.pbi[i].c_fvobu_sa);
            write32(buf, &off, pgc->c_pbit.pbi[i].c_filvu_ea);
            write32(buf, &off, pgc->c_pbit.pbi[i].c_lvobu_sa);
            write32(buf, &off, pgc->c_pbit.pbi[i].c_lvobu_ea);
        }
    }
}

static void write_c_posit(pgc_t *pgc, uint8_t *buf) {
    if (pgc->pgc_gi.c_pbit_sa) {
        uint32_t off = pgc->pgc_gi.c_pbit_sa;
        for (int i = 0; i < (pgc->pgc_gi.get_pgc_cnt_cn()); ++i) {
            write16(buf, &off, pgc->c_posit.c_posi[i].c_vob_idn);
            write8(buf, &off, pgc->c_posit.c_posi[i].reserved);
            write8(buf, &off, pgc->c_posit.c_posi[i].c_idn);
        }
    }
}

void pgc_build(pgc_t *pgc, uint8_t **buf, uint32_t *len) {
    if (pgc && buf && len) {
        *len = calculate_len(pgc);
        *buf = (uint8_t *)calloc(1, *len);
        write_pgc_gi(pgc, *buf);
        write_pgc_cmdt(pgc, *buf);
        write_pgc_pgmap(pgc, *buf);
        write_c_pbit(pgc, *buf);
        write_c_posit(pgc, *buf);
    }
}