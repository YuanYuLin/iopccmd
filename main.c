#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <stdlib.h>

#include "iopcdefine.h"
#include "iopc_vminfo.h"
#include "iopcops_mq.h"
#include "iopccmd_vmadd.h"

#include "iopccmd_vmcount.h"
#include "iopccmd_vmget.h"
#include "iopccmd_vmset.h"
#include "client_vm.h"

#include "iopccmd_mntbasecount.h"
#include "iopccmd_mntbaseget.h"
#include "iopccmd_mntbaseset.h"
#include "client_mntbase.h"

#include "iopccmd_raiddevcount.h"
#include "iopccmd_raiddevget.h"
#include "iopccmd_raiddevset.h"
#include "client_raiddev.h"

#include "client_db.h"

#define TASK_IOPCCMD		"IOPCCMD"
#define STR_EQUAL(str1, str2) if(strcmp(str1, str2) == 0)

static uint8_t cmd_raw(int argc, char** argv)
{
    struct msg_t req;
    struct msg_t res;
    uint8_t* req_data;
    uint8_t* res_data;
    int i = 0;
    uint16_t req_data_len = 0;

    req_data = (uint8_t*)&req.data;
    res_data = (uint8_t*)&res.data;

    strcpy(req.hdr.src, TASK_IOPCCMD);
    strcpy(req.hdr.dst, TASK_IOPCLAUNCHER);

    req.hdr.fn = strtoul(argv[1], NULL, 16);
    req.hdr.cmd = strtoul(argv[2], NULL, 16);
    //printf("%s, %s\n", argv[1], argv[2]);
    //printf("fn:%x, cmd:%x\n", req.hdr.fn, req.hdr.cmd);

    //printf("show data:\n");
    for(i=0;i<(argc-3);i++) {
        req_data[i] = strtoul(argv[i+3], NULL, 16);
    //    printf("%x,", req_data[i]);
    }
    //printf("\n%d\n", i);

    req_data_len = i;
    req.hdr.data_size = req_data_len;

    GET_INSTANCE_MQ_OBJ()->set_to(TASK_IOPCLAUNCHER, &req);
    GET_INSTANCE_MQ_OBJ()->get_from(TASK_IOPCCMD, &res);
    GET_INSTANCE_MQ_OBJ()->destroy(TASK_IOPCCMD);

    printf("magic:%x\n", res.hdr.magic);
    printf("src:%s\n", res.hdr.src);
    printf("dst:%s\n", res.hdr.dst);
    printf("data size:%d\n", res.hdr.data_size);
    printf("group:0x%x\n", res.hdr.fn);
    printf("cmd:0x%x\n", res.hdr.cmd);
    printf("crc:%x\n", res.hdr.crc16);
    printf("data:\n");

    for(i=0;i<res.hdr.data_size;i++) {
        printf("%x,", res_data[i]);
    }
    printf("\n");

    return 0;
}

static uint8_t cmd_vmadd(int argc, char** argv)
{
    struct msg_t req;
    struct msg_t res;
    struct req_vmadd_t* req_data;
    struct res_vmadd_t* res_data;
    uint8_t* p_argv = NULL;

    printf("argc %d\n", argc);

    req_data = (struct req_vmadd_t*)&req.data;
    res_data = (struct res_vmadd_t*)&res.data;

    strcpy(req.hdr.src, TASK_IOPCCMD);
    strcpy(req.hdr.dst, TASK_IOPCLAUNCHER);

    req.hdr.fn = 0x05;//strtoul(argv[1], NULL, 16);
    req.hdr.cmd = 0x01;//strtoul(argv[2], NULL, 16);
    printf("fn:%x, cmd:%x\n", req.hdr.fn, req.hdr.cmd);

    memset(req_data, 0, MAX_MSG_DATA_SIZE);
    memset(res_data, 0, MAX_MSG_DATA_SIZE);

    p_argv = argv[1];
    req_data->auto_start = strtoul(p_argv, NULL, 16);
    printf("1.%d\n", req_data->auto_start);

    p_argv = argv[2];
    memcpy(req_data->name, p_argv, strlen(p_argv));
    printf("2.%s\n", req_data->name);
    
    p_argv = argv[3];
    memcpy(req_data->nettype, p_argv, strlen(p_argv));
    printf("3.%s\n", req_data->nettype);

    p_argv = argv[4];
    memcpy(req_data->nethwlink, p_argv, strlen(p_argv));
    printf("4.%s\n", req_data->nethwlink);

    p_argv = argv[5];
    memcpy(req_data->nethwaddr, p_argv, strlen(p_argv));
    printf("5.%s\n", req_data->nethwaddr);

    p_argv = argv[6];
    memcpy(req_data->base_path, p_argv, strlen(p_argv));
    printf("6.%s\n", req_data->base_path);

    req.hdr.data_size = sizeof(struct req_vmadd_t);
    printf("size: %d, %d\n", sizeof(struct req_vmadd_t), sizeof(req_data));

    GET_INSTANCE_MQ_OBJ()->set_to(TASK_IOPCLAUNCHER, &req);
    GET_INSTANCE_MQ_OBJ()->get_from(TASK_IOPCCMD, &res);
    GET_INSTANCE_MQ_OBJ()->destroy(TASK_IOPCCMD);

    printf("magic:%x\n", res.hdr.magic);
    printf("src:%s\n", res.hdr.src);
    printf("dst:%s\n", res.hdr.dst);
    printf("data size:%d\n", res.hdr.data_size);
    printf("group:0x%x\n", res.hdr.fn);
    printf("cmd:0x%x\n", res.hdr.cmd);
    printf("crc:%x\n", res.hdr.crc16);
    printf("data:\n");
    printf("status:%x\n", res_data->status);
    printf("index:%d\n", res_data->index);

    return 0;
}

static uint8_t cmd_vmcount(int argc, char** argv)
{
    uint32_t count = 0;
    count = GET_INSTANCE_CLIENT_VM()->count();
    printf("count:%d\n", count);

    return 0;
}

static uint8_t cmd_mntbasecount(int argc, char** argv)
{
    uint32_t count = 0;
    count = GET_INSTANCE_CLIENT_MNTBASE()->count();
    printf("count:%d\n", count);

    return 0;
}

static uint8_t cmd_raiddevcount(int argc, char** argv)
{
    uint32_t count = 0;
    count = GET_INSTANCE_CLIENT_RAIDDEV()->count();
    printf("count:%d\n", count);

    return 0;
}

static uint8_t cmd_vmget(int argc, char** argv)
{
    struct vm_info_t info;
    int index = 0;
    index = strtoul(argv[1], NULL, 10);

    memset(&info, 0, sizeof(struct vm_info_t));
    GET_INSTANCE_CLIENT_VM()->get(index, &info);

    printf("index:%d\n", index);
    printf("auto_start:%d\n", info.auto_start);
    printf("name:%s\n", info.name);
    printf("base_path:%s\n", info.base_path);
    printf("nettype:%s\n", info.nettype);
    printf("nethwlink:%s\n", info.nethwlink);
    printf("nethwaddr:%s\n", info.nethwaddr);

    return 0;
}

static uint8_t cmd_mntbaseget(int argc, char** argv)
{
    struct mntbase_info_t info;
    int index = 0;
    index = strtoul(argv[1], NULL, 10);

    memset(&info, 0, sizeof(struct mntbase_info_t));
    GET_INSTANCE_CLIENT_MNTBASE()->get(index, &info);

    printf("index:%d\n", index);
    printf("enabled:%d\n", info.enabled);
    printf("fixed:%d\n", info.fixed);
    printf("name:%s\n", info.name);
    printf("src:%s\n", info.src);
    printf("dst:%s\n", info.dst);
    printf("type:%s\n", info.type);

    return 0;
}

static uint8_t cmd_raiddevget(int argc, char** argv)
{
    struct raiddev_info_t info;
    int index = 0;
    int i = 0;
    index = strtoul(argv[1], NULL, 10);

    memset(&info, 0, sizeof(struct raiddev_info_t));
    GET_INSTANCE_CLIENT_RAIDDEV()->get(index, &info);

    printf("index:%d\n", index);
    printf("enabled:%d\n", info.enabled);
    printf("name:%s\n", info.name);
    printf("type:%s\n", info.type);
    printf("dev_size:%d\n", info.dev_size);
    for(i=0;i<info.dev_size;i++) {
        printf("dev[%d]enabled:%d\n", i, info.dev[i].enabled);
	printf("dev[%d]path:%s\n", i, info.dev[i].path);
    }

    return 0;
}
/*
static uint8_t cmd_dbgetbool(int argc, char** argv)
{
    uint8_t* media = argv[1];
    uint8_t* key = argv[2];
    uint8_t val = 0;

    if(strcmp(media, "flash") == 0) {
        val = GET_INSTANCE(client_db)->get_bool_flash(key);
    }
    if(strcmp(media, "ram") == 0) {
        val = GET_INSTANCE(client_db)->get_bool_ram(key);
    }
    printf("get boolean [%s:%s:%d]\n", media, key, val);
    return 0;
}

static uint8_t cmd_dbsetbool(int argc, char** argv)
{
    uint8_t* media = argv[1];
    uint8_t* key = argv[2];
    uint8_t val = strtoul(argv[3], NULL, 10);
    uint8_t status = 0;

    if(strcmp(media, "flash") == 0) {
        status = GET_INSTANCE(client_db)->set_bool_flash(key, val);
    }
    if(strcmp(media, "ram") == 0) {
        status = GET_INSTANCE(client_db)->set_bool_ram(key, val);
    }
    printf("set boolean [%s:%s:%d]%d\n", media, key, val, status);
    return 0;
}

static uint8_t cmd_dbgetuint32(int argc, char** argv)
{
    uint8_t* media = argv[1];
    uint8_t* key = argv[2];
    uint32_t val = 0;

    if(strcmp(media, "flash") == 0) {
        val = GET_INSTANCE(client_db)->get_uint32_flash(key);
    }
    if(strcmp(media, "ram") == 0) {
        val = GET_INSTANCE(client_db)->get_uint32_ram(key);
    }
    printf("get uint32 [%s:%s:%d]\n", media, key, val);
    return 0;
}

static uint8_t cmd_dbsetuint32(int argc, char** argv)
{
    uint8_t* media = argv[1];
    uint8_t* key = argv[2];
    uint32_t val = strtoul(argv[3], NULL, 10);
    uint8_t status = 0;

    if(strcmp(media, "flash") == 0) {
        status = GET_INSTANCE(client_db)->set_uint32_flash(key, val);
    }
    if(strcmp(media, "ram") == 0) {
        status = GET_INSTANCE(client_db)->set_uint32_ram(key, val);
    }
    printf("set uint32 [%s:%s:%d]%d\n", media, key, val, status);
    return 0;
}

static uint8_t cmd_dbgetstring(int argc, char** argv)
{
    uint8_t* media = argv[1];
    uint8_t* key = argv[2];
    uint8_t val[STR_LEN];
    uint16_t len = 0;
    memset(val, 0, STR_LEN);

    if(strcmp(media, "flash") == 0) {
        len = GET_INSTANCE(client_db)->get_string_flash(key, &val[0]);
    }
    if(strcmp(media, "ram") == 0) {
        len = GET_INSTANCE(client_db)->get_string_ram(key, &val[0]);
    }
    printf("get string [%s:%s:%d:%d]\n", media, key, val, len);
    return 0;
}

static uint8_t cmd_dbsetstring(int argc, char** argv)
{
    uint8_t* media = argv[1];
    uint8_t* key = argv[2];
    uint8_t* val = argv[3];
    uint16_t len = strlen(val);
    uint8_t status = 0;
    if(strcmp(media, "flash") == 0) {
        status = GET_INSTANCE(client_db)->set_string_flash(key, val);
    }
    if(strcmp(media, "ram") == 0) {
        status = GET_INSTANCE(client_db)->set_string_ram(key, val);
    }
    printf("set string [%s:%s:%d:%d]%d\n", media, key, val, len, status);
    return 0;
}

static uint8_t cmd_dbsave2persist(int argc, char** argv)
{
    GET_INSTANCE(client_db)->save2persist();
    return 0;
}

static uint8_t cmd_dbrestore2default(int argc, char** argv)
{
    GET_INSTANCE(client_db)->restore2default();
    return 0;
}
*/
static uint8_t cmd_vmset(int argc, char** argv)
{
    uint8_t *p_argv = NULL;
    uint32_t index = 0;
    struct vm_info_t info;
    memset(&info, 0, sizeof(struct vm_info_t));

    p_argv = argv[1];
    index = strtoul(p_argv, NULL, 10);
    printf("1.%d\n", index);

    p_argv = argv[2];
    info.auto_start = strtoul(p_argv, NULL, 16);
    printf("2.%d\n", info.auto_start);

    p_argv = argv[3];
    memcpy(info.name, p_argv, strlen(p_argv));
    printf("3.%s\n", info.name);
    
    p_argv = argv[4];
    memcpy(info.nettype, p_argv, strlen(p_argv));
    printf("4.%s\n", info.nettype);

    p_argv = argv[5];
    memcpy(info.nethwlink, p_argv, strlen(p_argv));
    printf("5.%s\n", info.nethwlink);

    p_argv = argv[6];
    memcpy(info.nethwaddr, p_argv, strlen(p_argv));
    printf("6.%s\n", info.nethwaddr);

    p_argv = argv[7];
    memcpy(info.base_path, p_argv, strlen(p_argv));
    printf("7.%s\n", info.base_path);

    GET_INSTANCE_CLIENT_VM()->set(index, &info);

    return 0;
}

static uint8_t cmd_mntbaseset(int argc, char** argv)
{
    uint8_t *p_argv = NULL;
    uint32_t index = 0;
    struct mntbase_info_t info;
    memset(&info, 0, sizeof(struct mntbase_info_t));

    p_argv = argv[1];
    index = strtoul(p_argv, NULL, 10);
    printf("1.index:%d\n", index);

    p_argv = argv[2];
    info.enabled = strtoul(p_argv, NULL, 16);
    printf("2.enabled:%d\n", info.enabled);

    p_argv = argv[3];
    memcpy(info.name, p_argv, strlen(p_argv));
    printf("3.name:%s\n", info.name);

    p_argv = argv[4];
    memcpy(info.src, p_argv, strlen(p_argv));
    printf("4.src:%s\n", info.src);

    p_argv = argv[5];
    memcpy(info.dst, p_argv, strlen(p_argv));
    printf("5.dst:%s\n", info.dst);

    p_argv = argv[6];
    memcpy(info.type, p_argv, strlen(p_argv));
    printf("6.type:%s\n", info.type);

    GET_INSTANCE_CLIENT_MNTBASE()->set(index, &info);

    return 0;
}

static uint8_t cmd_raiddevset(int argc, char** argv)
{
    uint8_t *p_argv = NULL;
    uint32_t index = 0;
    struct raiddev_info_t info;
    int i = 0;
    int idx = 0;
    memset(&info, 0, sizeof(struct raiddev_info_t));

    p_argv = argv[1];
    index = strtoul(p_argv, NULL, 10);
    printf("1.index:%d\n", index);

    p_argv = argv[2];
    info.enabled = strtoul(p_argv, NULL, 16);
    printf("2.enabled:%d\n", info.enabled);

    p_argv = argv[3];
    memcpy(info.name, p_argv, strlen(p_argv));
    printf("3.name:%s\n", info.name);

    p_argv = argv[4];
    memcpy(info.type, p_argv, strlen(p_argv));
    printf("4.type:%s\n", info.type);

    p_argv = argv[5];
    info.dev_size = strtoul(p_argv, NULL, 10);
    printf("5.dev_size:%d\n", info.dev_size);

    for(i=0; i<info.dev_size; i++) {
        idx = 6 + (i * 2);
        p_argv = argv[idx];
        info.dev[i].enabled = strtoul(p_argv, NULL, 16);
	printf("%d.dev[%d]enabled:%d\n", idx, i, info.dev[i].enabled);
	idx = 7 + (i * 2);
	p_argv = argv[idx];
	memcpy(info.dev[i].path, p_argv, strlen(p_argv));
	printf("%d.dev[%d]path:%s\n", idx, i, info.dev[i].path);
    }

    GET_INSTANCE_CLIENT_RAIDDEV(client_raiddev)->set(index, &info);

    return 0;
}

static uint8_t cmd_reboot(int argc, char** argv)
{
    reboot(LINUX_REBOOT_CMD_RESTART);
    return 0;
}

static uint8_t cmd_help(int argc, char** argv)
{
    uint8_t* cmd;
    cmd = argv[1];
    printf("----------------------------------------\n");
    printf("cmd list:\n");
    printf(" vmcount, vmget, vmset\n");
    printf(" mntbasecount, mntbaseget, mntbaseset\n");
    printf(" raiddevcount, raiddevget, raiddevset\n");
    printf(" dbgetbool, dbgetuint32, dbgetstring\n");
    printf(" dbsetbool, dbsetuint32, dbsetstring\n");
    printf(" save2persist, restore2default\n");
    printf("----------------------------------------\n");
    if(cmd != NULL) {
        printf("cmd:%s\n    ", cmd);
        STR_EQUAL(cmd, "vmadd")	
            printf("arg:\n");
        STR_EQUAL(cmd, "vmcount")	
            printf("arg:\n");
        STR_EQUAL(cmd, "vmget")	
            printf("arg: index\n");
        STR_EQUAL(cmd, "vmset")	
            printf("arg: index autostart name nettype nethwlink nethwaddr base_path\n");
        STR_EQUAL(cmd, "mntbasecount")	
            printf("arg:\n");
        STR_EQUAL(cmd, "mntbaseget")	
            printf("arg: index\n");
        STR_EQUAL(cmd, "mntbaseset")	
            printf("arg: index enabled name src dst type\n");
        STR_EQUAL(cmd, "raiddevcount")	
            printf("arg:\n");
        STR_EQUAL(cmd, "raiddevget")	
            printf("arg: index\n");
        STR_EQUAL(cmd, "raiddevset")	
            printf("arg: index enabled name type dev_size dev_enabled1 dev_path1 dev_enabled2 dev_path2 ...\n");
	STR_EQUAL(cmd, "dbgetbool")
            printf("arg: media key\n");
	STR_EQUAL(cmd, "dbgetuint32")
            printf("arg: media key\n");
	STR_EQUAL(cmd, "dbgetstring")
            printf("arg: media key\n");
	STR_EQUAL(cmd, "dbsetbool")
            printf("arg: media key val\n");
	STR_EQUAL(cmd, "dbsetuint32")
            printf("arg: media key val\n");
	STR_EQUAL(cmd, "dbsetstring")
            printf("arg: media key val\n");
	STR_EQUAL(cmd, "save2persist")
            printf("arg:\n");
	STR_EQUAL(cmd, "restore2default")
            printf("arg:\n");
    printf("========================================\n");
    }
}

int main(int argc, char** argv)
{
    uint8_t* cmd;
    int cnt = argc -1;

    cmd = argv[1];

    STR_EQUAL(cmd, "help")	cmd_help(cnt, &argv[1]);
    STR_EQUAL(cmd, "raw")	cmd_raw(cnt, &argv[1]);
    STR_EQUAL(cmd, "vmadd")	cmd_vmadd(cnt, &argv[1]);
    STR_EQUAL(cmd, "vmcount")	cmd_vmcount(cnt, &argv[1]);
    STR_EQUAL(cmd, "vmget")	cmd_vmget(cnt, &argv[1]);
    STR_EQUAL(cmd, "vmset")	cmd_vmset(cnt, &argv[1]);
    STR_EQUAL(cmd, "mntbasecount")	cmd_mntbasecount(cnt, &argv[1]);
    STR_EQUAL(cmd, "mntbaseget")	cmd_mntbaseget(cnt, &argv[1]);
    STR_EQUAL(cmd, "mntbaseset")	cmd_mntbaseset(cnt, &argv[1]);
    STR_EQUAL(cmd, "raiddevcount")	cmd_raiddevcount(cnt, &argv[1]);
    STR_EQUAL(cmd, "raiddevget")	cmd_raiddevget(cnt, &argv[1]);
    STR_EQUAL(cmd, "raiddevset")	cmd_raiddevset(cnt, &argv[1]);
/*
    STR_EQUAL(cmd, "dbgetbool")		cmd_dbgetbool(cnt, &argv[1]);
    STR_EQUAL(cmd, "dbgetuint32")	cmd_dbgetuint32(cnt, &argv[1]);
    STR_EQUAL(cmd, "dbgetstring")	cmd_dbgetstring(cnt, &argv[1]);
    STR_EQUAL(cmd, "dbsetbool")		cmd_dbsetbool(cnt, &argv[1]);
    STR_EQUAL(cmd, "dbsetuint32")	cmd_dbsetuint32(cnt, &argv[1]);
    STR_EQUAL(cmd, "dbsetstring")	cmd_dbsetstring(cnt, &argv[1]);
    STR_EQUAL(cmd, "save2persist")	cmd_dbsave2persist(cnt, &argv[1]);
    STR_EQUAL(cmd, "restore2default")	cmd_dbrestore2default(cnt, &argv[1]);
*/
    STR_EQUAL(cmd, "reboot")		cmd_reboot(cnt, &argv[1]);

    return 0;
}
