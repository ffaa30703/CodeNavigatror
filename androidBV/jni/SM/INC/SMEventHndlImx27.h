/**
 *@file 	SMEventHndl.h
 *@brief 	南向接口模块对外相关接口
 *@author 	cx
 *@version 	1.0
 *@date 	2012-06-10
 */

#ifdef _YK_IMX27_AV_

#ifndef SM_EVENT_HNDL_IMX27_H_
#define SM_EVENT_HNDL_IMX27_H_

#include <YKSystem.h>
#include <YKTimer.h>
#include <YKMsgQue.h>

#define _SM_EVENT_DEBUG_

//SM消息队列长度
#define SM_MSG_QUE_LEN          1024

//串口接收缓冲区大小
#define COMM_DATA_RCV_SIZE      1024

//串口数据接收间隔（毫秒）
#define RCV_COM_DATA_INTERVAL	 1000*10

//设备类型定义
#define DEV_TYPE_TTL       0x30
#define DEV_TYPE_R232      0x31
#define DEV_TYPE_SELFPROC  0x32
#define DEV_TYPE_N1        0x33
#define DEV_TYPE_N3        0x34
#define DEV_TYPE_N4        0x35
#define DEV_NO_TYPE        0x38

//智慧门铃终端请求事件定义
#define SMIB_CALLEE_PICK_UP               0x30
#define SMIB_CALLEE_HANG_OFF              0x31
#define SMIB_CALL_AUDIO_CONNECT           0x32
#define SMIB_DTMF_SIGNAL                  0x33
#define SMIB_HEART_BEAT                   0x34
#define SMIB_INTERFACE_BOARD_REBOOT       0x35
#define SMIB_SET_REQUSET                  0x36
#define SMIB_MONITOR_REQUSET              0x37
#define SMIB_MONITOR_CANCEL               0x38

//终端南向接口模块请求事件定义
#define IBSM_DOOR_MACHINE_CALL          0x30
#define IBSM_INDOOR_MACHINE_PICK_UP     0x31
#define IBSM_DOOR_MACHINE_HANG_OFF      0x32
#define IBSM_RESERVED                   0x33
#define IBSM_READ_CARD                  0x34
#define IBSM_OPEN_DOOR                  0x35
#define IBSM_MONITOR_CANCEL             0x36
#define IBSM_ALARM                      0x37

/*消息类型定义*/
//串口接收的数据
#define SM_SERIAL_DATA_RCV         0x6001
//定时器事件
#define SM_TIMER_EVENT             0x6002
//退出消息处理线程
#define SM_HNDL_MSG_CANCEL         0x6003
// SM处理XT发过来的事件
#define SM_HDNL_XT_EVENT           0x6004

//定时器发送心跳包触发
#define TIMER_ID_HEARTBEAT              1
//应答包响应超时
#define TIMER_ID_PACKET_RSP_TIMEOUT     2

//时间定义
#define HEARTBEAT_INTERVAL              20*1000		//心跳间隔
#define CLIENT_RSP_TIMROUT              80*1000	    //心跳超时

//串口接收数据结构体
typedef struct
{
	char acBuf[COMM_DATA_RCV_SIZE];
	char *pcHead;
	unsigned char ucCounter;
	unsigned char ucState;
	int iLen;
}__attribute__((packed)) COM_DATA_RCV_ST;

/* 串口通讯包 (佳乐设备)*/
//终端请求包
typedef struct
{
	char acEventType[1];
	char acMachineType[1];
}__attribute__((packed)) HOST_REQ_PACKET_ST;

//终端发送备注信息
typedef struct
{
	char acEventType[1];
	char acRemark[8];
}__attribute__((packed)) HOST_REMARK_PACKET_ST;

//智慧门铃终端应答包
typedef struct
{
	char cCmdContent;
}__attribute__((packed)) HOST_RSP_PACKET_ST;

//接口板请求包
typedef struct
{
	char acEventType[1];
	char acCallerId[8];
	char acCalleeId[8];
	char acRemark[16];
}__attribute__((packed)) CLIENT_REQ_PACKET_ST;

//接口板应答包
typedef struct
{
	char acCmdContent[1];
}__attribute__((packed)) CLIENT_RSP_PACKET_ST;

//串口接收数据
typedef struct
{
	unsigned int 	uiPrmvType;				//原语类型
	unsigned int uiLen; 					//数据长度
	char *pcData;	    					//接收的串口数据
}__attribute__((packed)) SM_RCV_DATA_ST;

//SM定时器
typedef struct
{
	YK_TIMER_ST     *pstTimer;
	unsigned long	 ulMagicNum;
}SM_TIMER_ST;

//定时器事件结构体
typedef struct
{
	unsigned int uiPrmvType;
	unsigned int uiTimerId;
}SM_TIMER_EVENT_ST;

//退出SM事件处理结构体
typedef struct
{
	unsigned int 	uiPrmvType;            //原语类型
}SM_EXIT_MSG_ST;


extern YK_MSG_QUE_ST *g_pstSMMsgQ;

/**
 *@brief 		发送智慧门铃终端应答
 *@param[in] 	操作结果（P-成功 E-非法参数 R-拒绝请求 C-CRC校验错误）
 *@return 		TRUE-成功	FALSE-失败
 */
BOOL SMSendHostRsq(char cContent);

/**
 *@brief 		重启接口板
 *@param[in] 	无
 *@return 		TRUE-成功	FALSE-失败
 */
BOOL SMNMDevReboot(void);

/**
 *@brief 		开门
 *@param[in] 	无
 *@return 		TRUE-成功	FALSE-失败
 */
BOOL SMOpenDoor(void);

/**
 *@brief 		南向接口模块初始化
 *@param[in] 	无
 *@return 		RST_OK-成功	RST_ERR-失败
 */
int SMInit(void);

/**
 *@brief 		释放南向接口模块占用资源
 *@param[in] 	无
 *@return 		无
 */
void SMFini(void);

/**
 *@brief 		SM命令行处理
 *@param[in] 	pcCmd 待处理命令
 *@return 		无
 */
int SMProcessCommand(const char *pcCmd);


#endif /* SM_EVENT_HNDL_IMX27_H_ */

#endif /* _YK_IMX27_AV_ */
