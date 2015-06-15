/*********************Copy-Right****************************
 *														   *
 *														   *
 *														   *
 *														   *
 ***********************************************************/
#ifndef __JC_MATRIX_SWITCH_H__
#define __JC_MATRIX_SWITCH_H__

//#define EXPORT_OUT_DLL

//函数返回错误代码
#define MATRIX_SWITCH_OK						(0 )	//成功
#define MATRIX_SWITCH_ERROR_INIT_EXCEPTION		(-1)    //初始化异常
#define MATRIX_SWITCH_ERROR_INI_FILE_NOEXIST	(-2)    //配置文件不存在
#define MATRIX_SWITCH_ERROR_OBJECT_NULL			(-3)    //对象不存在
#define MATRIX_SWITCH_ERROR_CHAN_NO_EXIST		(-4)    //通道序号不存在
#define MATRIX_SWITCH_EXCUTE_FAILED				(-1)    //开关操作执行失败

#define MATRIX_CHAN_IDX_INVAID_1				(-10001)//通道1不合法
#define MATRIX_CHAN_IDX_INVAID_2				(-10002)//通道2不合法
#define MATRIX_CHAN_IDX_INVAID_3				(-10003)//通道3不合法
#define MATRIX_CHAN_IDX_INVAID_4				(-10004)//通道4不合法
//通信连接方式定义
#define COMM_TYPE_TCP		(2)
#define COMM_TYPE_UDP		(3)
#define COMM_TYPE_COM		(4)
//矩阵开关类型
#define ID_HUAWEI			(1)
#define ID_POI				(2)
//忽略通道设置字
#define ID_CHAN_IGNORE		(-1)
/**********************************************************/
/*HUA WEI DEFINE*/
//TX1通道序号
#define ID_CHAN_TX1_700_1		(0 )
#define ID_CHAN_TX1_700_2       (1 )
#define ID_CHAN_TX1_800_1       (2 )
#define ID_CHAN_TX1_800_2       (3 )
#define ID_CHAN_TX1_900_1       (4 )
#define ID_CHAN_TX1_900_2       (5 )
#define ID_CHAN_TX1_1800_1      (6 )
#define ID_CHAN_TX1_1800_2      (7 )
#define ID_CHAN_TX1_1900_1      (8 )
#define ID_CHAN_TX1_1900_2      (9 )
#define ID_CHAN_TX1_2100_1      (10)
#define ID_CHAN_TX1_2100_2      (11)
#define ID_CHAN_TX1_2600_1      (12)
#define ID_CHAN_TX1_2600_2      (13)
//TX2通道序号
#define ID_CHAN_TX2_700_1		(0 )
#define ID_CHAN_TX2_700_2       (1 )
#define ID_CHAN_TX2_800_1       (2 )
#define ID_CHAN_TX2_800_2       (3 )
#define ID_CHAN_TX2_900_1       (4 )
#define ID_CHAN_TX2_900_2       (5 )
#define ID_CHAN_TX2_1800_1      (6 )
#define ID_CHAN_TX2_1800_2      (7 )
#define ID_CHAN_TX2_1900_1      (8 )
#define ID_CHAN_TX2_1900_2      (9 )
#define ID_CHAN_TX2_2100_1      (10)
#define ID_CHAN_TX2_2100_2      (11)
#define ID_CHAN_TX2_2600_1      (12)
#define ID_CHAN_TX2_2600_2      (13)
//PIM通道序号
#define ID_CHAN_PIM_700_1		(0 )
#define ID_CHAN_PIM_700_2       (1 )
#define ID_CHAN_PIM_800_1       (2 )
#define ID_CHAN_PIM_800_2       (3 )
#define ID_CHAN_PIM_900_1       (4 )
#define ID_CHAN_PIM_900_2       (5 )
#define ID_CHAN_PIM_1800_1      (6 )
#define ID_CHAN_PIM_1800_2      (7 )
#define ID_CHAN_PIM_1900_1      (8 )
#define ID_CHAN_PIM_1900_2      (9 )
#define ID_CHAN_PIM_2100_1      (10)
#define ID_CHAN_PIM_2100_2      (11)
#define ID_CHAN_PIM_2600_1      (12)
#define ID_CHAN_PIM_2600_2      (13)
//DET通道序号
#define ID_CHAN_DET_700_1		(0 )
#define ID_CHAN_DET_700_2       (1 )
#define ID_CHAN_DET_800_1       (2 )
#define ID_CHAN_DET_800_2       (3 )
#define ID_CHAN_DET_900_1       (4 )
#define ID_CHAN_DET_900_2       (5 )
#define ID_CHAN_DET_1800_1      (6 )
#define ID_CHAN_DET_1800_2      (7 )
#define ID_CHAN_DET_1900_1      (8 )
#define ID_CHAN_DET_1900_2      (9 )
#define ID_CHAN_DET_2100_1      (10)
#define ID_CHAN_DET_2100_2      (11)
#define ID_CHAN_DET_2600_1      (12)
#define ID_CHAN_DET_2600_2      (13)
/**********************************************************/
/*POI DEFINE*/
#define ID_SW1_SDT3			(1)   //开关类型1 ：操作范围<1~3>
#define ID_SW2_SDT3         (2)	  //开关类型2 ：操作范围<1~3>
#define ID_SW3_SDT4         (3)   //开关类型3 ：操作范围<1~4>
#define ID_SW4_SDT4         (4)   //开关类型4 ：操作范围<1~4>
#define ID_SW5_SDT7         (5)   //开关类型5 ：操作范围<1~7>
#define ID_SW6_SDT7         (6)   //开关类型6 ：操作范围<1~7>
#define ID_SW7_SDT2         (7)   //开关类型7 ：操作范围<1~2>
#define ID_SW8_SDT2         (8)   //开关类型8 ：操作范围<1~2>
#define ID_SW9_SDT2         (9)   //开关类型9 ：操作范围<1~2>
#define ID_SW10_SDT2        (10)  //开关类型10：操作范围<1~2>
#define ID_SW11_SDT2        (11)  //开关类型11：操作范围<1~2>
#define ID_SW12_SDT2        (12)  //开关类型12：操作范围<1~2>
//POI模块地址
#define IP_Signalswich  	(1)
#define IP_Paspecumpwmt     (2)
#define IP_Testmdcdmagsm    (3)
#define IP_Testmdfdd18      (4)
#define IP_Testmdfdd21      (5)
#define IP_Testmdtdftda     (6)
#define IP_Testmdtdd23      (7)
//POI-TX1 通道
#define ID_1Cmgsmtx1        (0)
#define ID_2Cucdmatx1       (1)
#define ID_3Ctfd18tx1       (2)
#define ID_4Cufd18tx1       (3)
#define ID_5Ctfd21tx1       (4)
#define ID_6Cuw21tx1        (5)
#define ID_7Cmdcstx1        (6)
#define ID_8Cmtdftx1        (7)
#define ID_10Cmtdetx1       (9)
#define ID_11Cttdetx1       (10)
//POI-TX2 通道                     
#define ID_1Cmgsmtx2        (0 )
#define ID_2Cucdmatx2       (1 )
#define ID_3Ctfd18tx2       (2 )
#define ID_4Cufd18tx2       (3 )
#define ID_5Ctfd21tx2       (4 )
#define ID_6Cuw21tx2        (5 )
#define ID_7Cmdcstx2        (6 )
#define ID_8Cmtdftx2        (7 )
#define ID_9Cmtdatx2        (8 )
#define ID_10Cmtdetx2       (9 )
#define ID_12Cutdetx2       (11)
//POI-RX 通道                      
#define ID_1gsmpim 	        (0)
#define ID_2cdmapim         (1)
#define ID_3ctfd18pim       (2)
#define ID_4cufd18pim       (3)
#define ID_5ctfd21pim       (4)
#define ID_6cuw21pim        (5)
#define ID_7cmdcspim        (6)
#define ID_8cmtdfpim        (7)
#define ID_10cmtde23pim     (9)
#define ID_11cttde23pim     (10)
//POI-Coup 通道
#define ID_1Cdmagsmcp1      (0 )
#define ID_2Cdmagsmcp2      (1 )
#define ID_3fdd18cp1        (2 )
#define ID_4fdd18cp2        (3 )
#define ID_5fdd21cp1        (4 )
#define ID_6fdd21cp2        (5 )
#define ID_7tdftdacp1       (6 )
#define ID_8tdftdacp2       (7 )
#define ID_9tdftdacp3       (8 )
#define ID_10tde23cp1	    (9 )
#define ID_11tde23cp2       (10)
/**********************************************************/

//函数指针类型
typedef int (*pMartrixSwitchInit)(int,char*,int,int);
typedef int(*pMartrixSwitchExcute)(int, int, int);
typedef int (*pMartrixSwitchBoxExcute)(int, int, int, int);
typedef int (*pMartrixSwitchDispose)();

#ifdef EXPORT_OUT_DLL
//动态库申明
extern "C" __declspec(dllexport) int MartrixSwitchInit(int handle, char*dllName, int swType, int comType);
extern "C" __declspec(dllexport) int MartrixSwitchBoxExcute(int tx1, int tx2, int pim, int det);
extern "C" __declspec(dllexport) int MartrixSwitchExcute(int addr, int swId, int swIdx);
extern "C" __declspec(dllexport) int MartrixSwitchDispose();

#else
//静态库申明
/*******************************************************************************
 *函数名称：MartrixSwitchInit
 *函数功能：矩阵开关初始化
 *参数说明：int handle	——宿主句柄，可以设成NULL
 *			char*dllName——宿主动态库名，用于寻找配置文件的路径
 *			int swType	——矩阵箱类型：华为或POI
 *			int comType	——通信方式
 *返回类型：参照错误返回代码
 */
extern int MartrixSwitchInit(int handle, char*dllName, int swType, int comType);
/*******************************************************************************
 *函数名称：MartrixSwitchBoxExcute
 *函数功能：矩阵开组合通道操作
 *参数说明：int tx1	—— TX1通道序号
 *			int tx2 —— TX2通道序号
 *			int pim	—— PIM通道序号
 *			int det	—— DET通道序号
 *返回类型：参照错误返回代码
 */
extern int MartrixSwitchBoxExcute(int tx1, int tx2, int pim, int det);
/*******************************************************************************
 *函数名称：MartrixSwitchExcute
 *函数功能：矩阵开关单开关操作
 *参数说明：int addr	——模块地址
 *			int swId	——开关类型序号
 *			int swIdx	——开关引脚序号
 *返回类型：参照错误返回代码
 */
extern int MartrixSwitchExcute(int addr, int swId, int swIdx);
/*******************************************************************************
 *函数名称：MartrixSwitchDispose
 *函数功能：矩阵开关释放资源
 *返回类型：参照错误返回代码
 */
extern int MartrixSwitchDispose();

#endif

#endif