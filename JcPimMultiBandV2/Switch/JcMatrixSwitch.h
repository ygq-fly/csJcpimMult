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

#define MATRIX_CHAN_IDX_INVAID_1				(-10001)//通道1序号不合法
#define MATRIX_CHAN_IDX_INVAID_2				(-10002)//通道2序号不合法
#define MATRIX_CHAN_IDX_INVAID_3				(-10003)//通道3序号不合法
#define MATRIX_CHAN_IDX_INVAID_4				(-10004)//通道4序号不合法
//通信连接方式定义
#define COMM_TYPE_TCP		(2)
#define COMM_TYPE_UDP		(3)
#define COMM_TYPE_COM		(4)
//矩阵开关类型
#define ID_HUAWEI			(1)
#define ID_POI				(2)
#define ID_HUAWEI_EXT12    	(3)
#define ID_HUAWEI_EXT8    	(4)
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
#define ID_CHAN_TX1_1500_1      (14)
#define ID_CHAN_TX1_1500_2      (15)
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
#define ID_CHAN_TX1_1500_1      (14)
#define ID_CHAN_TX1_1500_2      (15)
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
#define ID_CHAN_PIM_1500_1      (14)
#define ID_CHAN_PIM_1500_2      (15)
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
#define ID_CHAN_DET_1500_1      (14)
#define ID_CHAN_DET_1500_2      (15)
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
#define IP_Testmdtdd26      (8)
#define Testmdlte700		(9)
#define Testmdpcs1900       (10)
//POI-TX1 通道
#define POI_IDX_1Cmgsmtx1				(0)
#define POI_IDX_2Cucdmatx1				(1)
#define POI_IDX_3Ctfd18tx1				(2)
#define POI_IDX_4Cufd18tx1				(3)
#define POI_IDX_5Ctfd21tx1				(4)
#define POI_IDX_6Cuw21tx1				(5)
#define POI_IDX_7Cmdcstx1				(6)
#define POI_IDX_8Cmtdftx1				(7)
#define POI_IDX_9null                   (8)
#define POI_IDX_10Cmtdetx1				(9)
#define POI_IDX_11Cttdetx1				(10)
#define POI_IDX_12null                  (11)
#define POI_IDX_13Cmtdftx1				(12)
#define POI_IDX_14lte700tx1             (13)
#define POI_IDX_15pcs1900tx				(14)
//POI-TX2 通道             
#define POI_IDX_1Cmgsmtx2 				(0)
#define POI_IDX_2Cucdmatx2				(1) 
#define POI_IDX_3Ctfd18tx2				(2)
#define POI_IDX_4Cufd18tx2				(3)
#define POI_IDX_5Ctfd21tx2				(4)
#define POI_IDX_6Cuw21tx2				(5)
#define POI_IDX_7Cmdcstx2				(6)
#define POI_IDX_8Cmtdftx2				(7)
#define POI_IDX_9Cmtdatx2				(8)
#define POI_IDX_10Cmtdetx2				(9)
#define POI_IDX_11null                  (10)
#define POI_IDX_12Cutdetx2				(11)
#define POI_IDX_13Cmtdftx2				(12)
#define POI_IDX_14lte700tx2             (13)
#define POI_IDX_15pcs1900t2				(14)
//POI-RX 通道                      
#define POI_IDX_1gsmpim    				(0)
#define POI_IDX_2cdmapim                (1)
#define POI_IDX_3ctfd18pim              (2)
#define POI_IDX_4cufd18pim              (3) 
#define POI_IDX_5ctfd21pim              (4)
#define POI_IDX_6cuw21pim               (5)
#define POI_IDX_7cmdcspim               (6)
#define POI_IDX_8cmtdfpim               (7)
#define POI_IDX_9null                   (8) 
#define POI_IDX_10cmtde23pim			(9) 
#define POI_IDX_11cttde23pim            (10) 
#define POI_IDX_12null                  (11)
#define POI_IDX_13cttdd26pim            (12)
#define POI_IDX_14lte700pim             (13)
#define POI_IDX_15pcs1900pim            (14)
//POI-Coup 通道
#define POI_IDX_1Cdmagsmcp1				(0)
#define POI_IDX_2Cdmagsmcp2				(1)
#define POI_IDX_3fdd18cp1               (2) 
#define POI_IDX_4fdd18cp2               (3)  
#define POI_IDX_5fdd21cp1               (4) 
#define POI_IDX_6fdd21cp2               (5) 
#define POI_IDX_7tdftdacp1              (6)
#define POI_IDX_8tdftdacp2              (7)
#define POI_IDX_9tdftdacp3              (8) 
#define POI_IDX_10tde23cp1              (9) 
#define POI_IDX_11tde23cp2              (10) 
#define POI_IDX_12tdd26cp1              (11)
#define POI_IDX_13tdd26cp2              (12)
#define POI_IDX_14lte700cp1             (13)
#define POI_IDX_14lte700cp2             (14)
#define POI_IDX_15pcs1900cp1            (15)
#define POI_IDX_15pcs1900cp2            (16)
/**********************************************************/
/*HUA WEI EXT12 DEFINE*/
//TX1通道序号
#define ID_1tx1to4501ant1                               ( 0   )
#define ID_2tx1to4501ant2                               ( 1   )
#define ID_3tx1to4502ant1                               ( 2   )
#define ID_4tx1to4502ant2                               ( 3   )
#define ID_5tx1to700lant1                                ( 4   )
#define ID_6tx1to700lant2                                ( 5   )
#define ID_7tx1to700hant1                               ( 6   )
#define ID_8tx1to700hant2                               ( 7   )
#define ID_9tx1to700aptant1                           ( 8   )
#define ID_10tx1to700aptant2                         ( 9   )
#define ID_11tx1todd800ant1                          ( 10 )
#define ID_12tx1todd800ant2                          ( 11 )
#define ID_13tx1tocdma800ant1                     ( 12 )
#define ID_14tx1tocdma800ant2                     ( 13 )
#define ID_15tx1tocdma800ant1                     ( 14 )
#define ID_16tx1tocdma800ant2                     ( 15 )
#define ID_17tx1todcs1800ant1                       ( 16)
#define ID_18tx1todcs1800ant2                       ( 17)
#define ID_19tx1topcs1900ant1                       ( 18)
#define ID_20tx1topcs1900ant2                       ( 19)
#define ID_21tx1towcdma2100ant1                ( 20 )
#define ID_22tx1towcdma2100ant2                ( 21 )
#define ID_23tx1towcdma2600ant1                ( 22 )
#define ID_24tx1towcdma2600ant2					(23 )
//TX2通道序号
#define ID_1tx2to4501ant1								  ( 0   )
#define ID_2tx2to4501ant2                                  ( 1   )
#define ID_3tx2to4502ant1                                  ( 2   )
#define ID_4tx2to4502ant2                                  ( 3   )
#define ID_5tx2to700lant1                                    ( 4   ) 
#define ID_6tx2to700lant2                                    ( 5   ) 
#define ID_7tx2to700hant1                                  ( 6   )
#define ID_8tx2to700hant2                                  ( 7   )
#define ID_9tx2to700aptant1                               ( 8   ) 
#define ID_10tx2to700aptant2                            ( 9   )
#define ID_11tx2todd800ant1                             ( 10 )
#define ID_12tx2todd800ant2                             ( 11 )
#define ID_13tx2tocdma800ant1                        ( 12 )
#define ID_14tx2tocdma800ant2                        ( 13 )
#define ID_15tx2tocdma800ant1                        ( 14 )
#define ID_16tx2tocdma800ant2                        ( 15 )
#define ID_17tx2todcs1800ant1                          ( 16)
#define ID_18tx2todcs1800ant2                          ( 17)
#define ID_19tx2topcs1900ant1                          ( 18)
#define ID_20tx2topcs1900ant2                          ( 19)
#define ID_21tx2towcdma2100ant1                   ( 20 )
#define ID_22tx2towcdma2100ant2                   ( 21 )
#define ID_23tx1towcdma2600ant1                   ( 22 )
#define ID_24tx1towcdma2600ant2				    	(23 )
//PIM通道序号
#define ID_1pimant1of4501								( 0   )
#define ID_2pimant2of4501                               ( 1   )
#define ID_3pimant1of4502                               ( 2   )
#define ID_4pimant2of4502                               ( 3   )
#define ID_5pimant1of700l                                 ( 4   ) 
#define ID_6pimant2of700l                                 ( 5   ) 
#define ID_7pimant1of700h                               ( 6   )
#define ID_8pimant2of700h                               ( 7   )
#define ID_9pimant1of700apt                            ( 8   ) 
#define ID_10pimant2of700apt                         ( 9   )
#define ID_11pimant1ofdd800                          ( 10 )
#define ID_12pimant2ofdd800                          ( 11 )
#define ID_13pimant1ofcdma800                     ( 12 )
#define ID_14pimant2ofcdma800                     ( 13 )
#define ID_15pimant1ofgsm900                       ( 14 )
#define ID_16pimant2ofgsm900                       ( 15 )
#define ID_17pimant1ofdcs1800                       ( 16)
#define ID_18pimant2ofdcs1800                       ( 17)
#define ID_19pimant1ofpcs1900                       ( 18)
#define ID_20pimant2ofpcs1900                       ( 19)
#define ID_21pimant1ofwcdam2100                 ( 20 ) 
#define ID_22pimant2ofwcdma2100                 ( 21 ) 
#define ID_23pimant1oflte2600                         ( 22 )
#define ID_24pimant2oflte2600                         	(23 )
//DET通道序号
#define ID_1couptx1of4501							( 0   )
#define ID_2couptx2of4501                           ( 1   )
#define ID_3couptx1of4502                           ( 2   )
#define ID_4couptx2of4502                           ( 3   )
#define ID_5couptx1of700l                              ( 4   )  
#define ID_6couptx2of700l                              ( 5   )  
#define ID_7couptx1of700h                           ( 6   )
#define ID_8couptx2of700h                           ( 7   )
#define ID_9couptx1of700apt                         ( 8   )  
#define ID_10couptx2of700apt                     ( 9   )
#define ID_11couptx1ofdd800                      ( 10 )
#define ID_12couptx2ofdd800                      ( 11 )
#define ID_13couptx1ofcdma800                 ( 12 )
#define ID_14couptx2ofcdma800                 ( 13 )
#define ID_15couptx1ofgsm900                    ( 14 ) 
#define ID_16couptx2ofgsm900                    ( 15 ) 
#define ID_17couptx1ofdcs1800                    ( 16)
#define ID_18couptx2ofdcs1800                    ( 17)
#define ID_19couptx1ofpcs1900                    ( 18)
#define ID_20couptx2ofpcs1900                    ( 19)
#define ID_21couptx1ofwcdma2100             ( 20 )
#define ID_22couptx2ofwcdma2100             ( 21 )
#define ID_23couptx1oflte2600                     ( 22 )
#define ID_24couptx2oflte2600                     	(23 )
/**********************************************************/
/*HUA WEI EXT8 DEFINE*/
//TX1通道序号
//1tx1to800aant1
//2tx1to800aant2
//3tx1to800bant1
//4tx1to800bant2
//5tx1to1900aant1
//6tx1to1900aant2
//7tx1to1900bant1
//8tx1to1900bant2
//9tx1to2600aant1
//10tx1to2600aant2
//11tx1to2600bant1
//12tx1to2600bant2
//13tx1to2600cant1
//14tx1to2600cant2
//15tx1to2600dant1
//16tx1to2600dant2
//TX2通道序号
//1tx2to800aant1
//2tx2to800aant2
//3tx2to800bant1
//4tx2to800bant2
//5tx2to1900aant1
//6tx2to1900aant2
//7tx2to1900bant1
//8tx2to1900bant2
//9tx2to2600aant1
//10tx2to2600aant2
//11tx2to2600bant1
//12tx2to2600bant2
//13tx2to2600cant1
//14tx2to2600cant2
//15tx2to2600dant1
//16tx2to2600dant2
//PIM通道序号
//1pimant1of800a
//2pimant2of800a
//3pimant1of800b
//4pimant2of800b
//5pimant1of1900a
//6pimant2of1900a
//7pimant1of1900b
//8pimant2of1900b
//9pimant1of2600a
//10pimant2of2600a
//11pimant1of2600b
//12pimant2of2600b
//13pimant1of2600c
//14pimant2of2600c
//15pimant1of2600d
//16pimant2of2600d
//DET通道序号
//1couptx1of800a
//2couptx2of800a
//3couptx1of800b
//4couptx2of800b
//5couptx1of1900a
//6couptx2of1900a
//7couptx1of1900b
//8couptx2of1900b
//9couptx1oflte2600a
//10couptx2oflte2600a
//11couptx1oflte2600b
//12couptx2oflte2600b
//13couptx1oflte2600c
//14couptx2oflte2600c
//15couptx1oflte2600d
//16couptx2oflte2600d
/**********************************************************/
//函数指针类型
typedef int (*pMartrixSwitchInit)(int,char*,int,int);
typedef int (*pMartrixSwitchExcute)(int, int, int);
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
*函数名称：MartrixSwitchBoxReset
*函数功能：矩阵开组合通道操作(零帧序)
*参数说明：int tx1	—— TX1通道序号
*			int tx2 —— TX2通道序号
*			int pim	—— PIM通道序号
*			int det	—— DET通道序号
*返回类型：参照错误返回代码
*/
extern int MartrixSwitchBoxReset(int tx1, int tx2, int pim, int det);
/*******************************************************************************
 *函数名称：MartrixSwitchExcute
 *函数功能：矩阵开关单开关操作
 *参数说明：int addr	——模块地址			(1~n:只要模块支持相应的天关即可)
 *			int swId	——开关类型序号	(ID_SW1_SDT3...)
 *			int swIdx	——开关引脚序号	(1~n)
 *返回类型：参照错误返回代码
 */
extern int MartrixSwitchExcute(int addr, int swId, int swIdx);
/*******************************************************************************
*函数名称：MartrixSwitchOpenAccpetMask
*函数功能：矩阵开关箱屏蔽
*参数说明：char *hosts[]     开关箱地址
*		  int size			开关箱个数
*			
*返回类型：参照错误返回代码
*/
extern int MartrixSwitchOpenAccpetMask(char*hosts[],int size);
/*******************************************************************************
*函数名称：MartrixSwitchBoxQClear
*函数功能：矩阵开关箱组合清零
*参数说明：int addr	——模块地址			(1~n:只要模块支持相应的天关即可)
*返回类型：参照错误返回代码
*/
extern int MartrixSwitchBoxQClear(int addr);
/*******************************************************************************
*函数名称：MartrixSwitchBoxQAdd
*函数功能：矩阵开关箱组合入队
*参数说明：int addr	——模块地址			(1~n:只要模块支持相应的天关即可)
*			int swId	——开关类型序号	(ID_SW1_SDT3...)
*			int swIdx	——开关引脚序号	(1~n)
*返回类型：参照错误返回代码
*/
extern int MartrixSwitchBoxQAdd(int addr, int swId, int swIdx);
/*******************************************************************************
*函数名称：MartrixSwitchBoxQExcute
*函数功能：矩阵开关箱组合操作
*参数说明：int addr	——模块地址			(1~n:只要模块支持相应的天关即可)
*返回类型：参照错误返回代码
*/
extern int MartrixSwitchBoxQExcute(int addr);
/*******************************************************************************
 *函数名称：MartrixSwitchDispose
 *函数功能：矩阵开关释放资源
 *返回类型：参照错误返回代码
 */
extern int MartrixSwitchDispose();

#endif

#endif