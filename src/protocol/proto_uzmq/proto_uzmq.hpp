#ifndef HEYGEARS_PROTO_P2P_H
#define HEYGEARS_PROTO_P2P_H

#include <uzmq_print.pb.h>

namespace heygears{

namespace proto_px{

enum Index
{
    SECTION_LIMIT   = 1000,

    BEGIN_OF_GW     = 0,
    BEGIN_OF_DB     = 1000,
    BEGIN_OF_PRINT  = 2000,
    BEGIN_OF_DEVICE = 2200,
    BEGIN_OF_SLAVE  = 3000,
    BEGIN_OF_CLOUD  = 4000,
    BEGIN_OF_SYS    = 5000,
    BEGIN_OF_PROJ   = 6000,
};

enum Topics
{
    //========================================proxy=============================================

    GW_TOPIC_BEGIN = BEGIN_OF_GW,

    GW_WS_POST, //前端推送后端，无需答复

    GW_WS_REQUEST,  //前端向后端请求， 需要答复

    GW_WS_EVENT,    //后端主动上报前端

    GW_TOPIC_END = BEGIN_OF_GW+SECTION_LIMIT,


    //========================================db=============================================

    DB_TOPIC_BEGIN = BEGIN_OF_DB,

    DB_IMPORT_MODEL,    //传 proto_px::db::ImportTaskInfo    返回 proto_px::db::DB_ERROR

    DB_CANCEL_IMPORT_MODEL,    //传  string  token  路径

    DB_JUDGE_IMPORT_MODEL,     //传  string  token  路径      返回 proto_px::db::DB_ERROR

    DB_GET_MODEL_INFO,    //传  string  modelId    返回  ModelInfo

    DB_GET_MODEL_INFO_BY_TASK_ID,    //传  string  taskId    返回  ModelInfo

    DB_GET_MODEL_PATH,  //传 string  taskId     返回string   path

    DB_GET_ALL_TASK,     //获取所有待打印任务           //传空    返回  proto_px::db::TaskDataList  传taskId 返回 TaskData

    DB_GET_ALL_RECORD,  //获取所有记录  //传空    返回  proto_px::db::RecordList   传 taskId  返回 RecordsData

    DB_MODEL_IMPORT_PROGRESS,      //上报导入进度    proto_px::db::ImportModelProgress
    DB_MODEL_IMPORT_RESULT,        //上报导入结果    proto_px::db::ImportModelResult

    DB_REMOVE_TASK,  //删除任务      传 string taskId
    DB_REMOVE_ALL_TASK,  //删除所有任务         无传惨
    DB_REMOVE_RECORD,  //删除记录    传 string taskId
    DB_REMOVE_ALL_RECORD,//删除所有记录    无传参
    DB_START_PRINT,  //数据库 开始打印   传 string taskId
    DB_END_PRINT,   // 数据库 结束打印   传proto_p2p::db::EndPrintData

    DB_RESTART_PRINT, // 重新打印 传历史id  返回新id

    DB_GET_MODEL_PREVIEW, // 获取模型预览图  传 string taskId    返回 ModelImage

    DB_GET_CONFIGS,  //传 string 表名    返回   ConfigsData
    DB_SAVE_CONFIGS, // 传ConfigsData   返回  int

    DB_UPGRADE_STATE,
    DB_UPGRADE_NOTICE_CLOUD,
    DB_UPGRADE_NOTIFY,
    DB_UPGRADE_NOTIFY_RESULT,
    DB_UPGRADE_QUREY,

    DB_RECORD_FORCE_DATA, //传 proto_px::db::ForceDataList

    DB_CLEAN_FORCE_DATA,

    DB_GET_FORCE_DATA,

    DB_GET_MATERIAL_ENUM,

    DB_GET_MATERIAL_CONFIG,

    DB_UPDATE_MATERIAL_CONFIG,

    DB_DEL_MATERIAL_CONFIG,

    DB_TOPIC_END = BEGIN_OF_DB+SECTION_LIMIT,

    //=====================================print================================================


    PRINT_TOPIC_BEGIN=BEGIN_OF_PRINT,

    PRINT_START,

    PRINT_STOP,

    PRINT_REPORT_PROGRESS,

    PRINT_START_PREPARE,

    PRINT_SKIP_PREPARE,

    PRINT_STOP_PREPARE,

    PRINT_STOP_PREPARE_RESET,

    PRINT_CONTINUE_PREPARE,

    PRINT_PREPARE_NOTIFY,

    PRINT_GET_DEVICE_INFO,

    PRINT_SET_AUTO_FILLING,

    PRINT_CHECK_AUTO_FILLING,

    PRINT_TOPIC_END=BEGIN_OF_PRINT+SECTION_LIMIT,

    //=====================================projector================================================

    PROJ_TOPIC_BEGIN=BEGIN_OF_PROJ,

    PROJ_START_DISPLAY,

    PROJ_STOP_DISPLAY,

    PROJ_END_DISPLAY,

    PROJ_GRAY_CALIB_START,

    PROJ_GRAY_CALIB_STOP,

    PROJ_GRAY_CALIB_UPDATE,

    PROJ_AUTO_CALIB_PREPARE,

    PROJ_AUTO_CALIB_START,

    PROJ_AUTO_CALIB_STOP,

    PROJ_AUTO_CALIB_FINISH,

    PROJ_POWER_NOTIFY,

    PROJ_PHO_STATE_NOTIFY,

    PROJ_IMG_RESIZE,

    PROJ_TOPIC_END=BEGIN_OF_PROJ+SECTION_LIMIT,

    //=====================================slave================================================

    SLAVE_TOPIC_BEGIN=BEGIN_OF_SLAVE,

    SLAVE_CLOSE_UART,

    SLAVE_MOTION,

    SLAVE_MOTION_RESULT,

    SLAVE_ZERO,

    SLAVE_RESET,

    SLAVE_PEEL,

    SLAVE_CANCEL_MOVE,

    SLAVE_MOVE_STEP,

    SLAVE_MOVE_STEP_RESULT,

    SLAVE_MAGNET_SWITH,

    SLAVE_CHECK_MAGNET,

    SLAVE_UV_SWITCH,

    SLAVE_HEARTBEAT_NOTIFY,

    SLAVE_UPDATE_SIMULATE,

    SLAVE_RESET_FORCE,

    SLAVE_FILLING_CALIB,

    SLAVE_FILLING_CALIB_NOTIFY,

    SLAVE_DEVINFO_NOTIFY,

    SLAVE_HEATING_STOP,

    SLAVE_VALVE_SWITCH,

    SLAVE_VALVE_SWITCH_NOTIFY,

    SLAVE_RESET_WEIGHT_SENSOR,

    SLAVE_WEIGHT_SENSOR_CALIB,

    SLAVE_QUREY_VERSION,

    SLAVE_TOPIC_END=BEGIN_OF_SLAVE+SECTION_LIMIT,


    //========================================cloud============================================================

    CLOUD_TOPIC_BEGIN = BEGIN_OF_CLOUD,

    CLOUD_LAUNCH_SERVICE,           // Cert
    CLOUD_LAUNCH_SERVICE2,          // string path

    CLOUD_TASK_BEGIN,               // 任务开始  in<< ultracore::cloud::BriefTask
    CLOUD_TASK_UPDATE,              // 任务进度  in<< ultracore::cloud::PrintProgress
    CLOUD_TASK_END,                 // 任务结束  in<< int32_t(errcode)
    CLOUD_TASK_UPLOAD,              // 上传     in<< proto_px::cloud::UploadRequest

    CLOUD_NOTIFY_IMPORT_RESULT,     // 切片导入结果 in<< ultracore::cloud::ImportResult

    CLOUD_QUREY_BIND_INFO,          // 请求绑定信息 in<< null
    CLOUD_QUREY_UPGRADE_INFO,       // 请求app升级信息 in<< null
    CLOUD_NOTIEY_DOWNLOAD,          // 通知下载 in<< proto_px::cloud::DownloadRequest
    CLOUD_REGISTERED_REG,           // 通知注册reg in<< string(sn)


    CLOUD_NOTIFY_REGISTERED_STATE,  //通知注册reg状态 param:   int32_t
    CLOUD_NOTIFY_LOGIN_INFO,        //通知登录信息 param:   proto_px::cloud::LoginInfo
    CLOUD_NOTIFY_CONNECT_INFO,      //通知连接信息 param:   proto_px::cloud::ConnectInfo
    CLOUD_NOTIFY_IMPORT_TASK,       //通知导入切片 param:   proto_px::cloud::Slice
    CLOUD_NOTIFY_CHECK_APP_UPDATE_RESULT,//通知检测app升级结果 param:   proto_px::cloud::Slice
    CLOUD_NOTIFY_DOWNLOAD_APP_RESULT,//通知下载app结果 param:   proto_px::cloud::Slice
    CLOUD_NOTIFY_UPLOAD_LOG,        //日志        param:   proto_px::cloud::LogRequest

    CLOUD_TOPIC_END = BEGIN_OF_CLOUD + SECTION_LIMIT,


    //========================================sys============================================================

    SYS_TOPIC_BEGIN=BEGIN_OF_SYS,

    SYS_UDISK_ADDED, //事件
    SYS_UDISK_REMOVED, //事件
    SYS_UDISK_QUERY_STATE, //请求
    SYS_UDISK_LIST_DIR, //请求

    SYS_WIFI_LIST_CHANGED, //事件
    SYS_WIFI_CONN_STATUS_CHANGED, //事件
    SYS_WIFI_QUERY_LIST, //请求
    SYS_WIFI_CONNECT, //请求
    SYS_WIFI_QUERY_NEED_PWD, //请求
    SYS_WIFI_QUERY_CONN_STATUS, //请求
    SYS_WIFI_OPEN, //请求
    SYS_WIFI_CLOSE, //请求
    SYS_WIFI_STATE, //请求
    SYS_WIFI_ENABLE, //事件

    SYS_LOG_EXPORT, //请求
    SYS_LOG_QEURY_DATES, //请求
    SYS_LOG_EXPORT_BY_DATE, //请求
    SYS_FILE_EXPORT_BY_PATH, //请求
    SYS_LOG_EXPORT_PROGRESS, //事件
    SYS_LOG_EXPORT_RESULT, //事件

    SYS_SYSTEM_RUN_QEURY_INFO, //请求
    SYS_ZONE_GET,//请求
    SYS_ZONE_SET,//请求

    SYS_DISC_CLEAN,//请求
    SYS_WIPE_CACHE,//请求

    SYS_TOPIC_END=BEGIN_OF_SYS+SECTION_LIMIT,
};






}


}

#endif
