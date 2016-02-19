/*******************************Copyright (c)***********************************
*
*              Copyright (C), 1999-2015, Jointcom . Co., Ltd.
*
*------------------------------------------------------------------------------
* @file	:
* @author	:mashuai
* @version	:v2.0
* @date		:2015.3.1
* @brief	:
*------------------------------------------------------------------------------*/
#ifndef __SWITCH_CONFIG_HW_EXT12_H__
#define __SWITCH_CONFIG_HW_EXT12_H__
#define IO_STRING_HW_EXT12      "[STATE]\n"\
"//�Ե�Դ����Ϊ��ߣ���8о����һ��Ϊ�Ϸ���ÿ��������ÿо����������\n"\
"//����������1��������8о��J15 1-7\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"//����������2��������4о��J3 1-3\n"\
"7=0,0,0,128,0,\n"\
"8=0,0,0,64,0,\n"\
"9=0,0,0,32,0,\n"\
"//����������3��������8о��J16 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"//����������4������J4��4о�� 1-3\n"\
"17=0,0,1024,0,0,\n"\
"18=32768,0,0,0,0,\n"\
"19=16384,0,0,0,0,\n"\
"//����������1������J5��4о�� 1-3\n"\
"20=16,0,0,0,0,\n"\
"21=32,0,0,0,0,\n"\
"22=64,0,0,0,0,\n"\
"//����������2������J6��4о�� 1-3\n"\
"23=128,0,0,0,0,\n"\
"24=0,0,16,0,0,\n"\
"25=0,0,32,0,0,\n"\
"//����������3������J7��4о�� 1-2\n"\
"26=0,1,0,0,0,\n"\
"27=0,2,0,0,0,\n"\
"//����������4������J8��8о�� 1-3\n"\
"28=0,0,0,0,1024,\n"\
"29=0,0,0,0,2048,\n"\
"30=0,0,0,0,4096,\n"\
"//����������5������J9��4о�� 1-3\n"\
"31=0,0,0,0,8192,\n"\
"32=0,0,0,0,16384,\n"\
"33=0,0,0,0,32768,\n"\
"//����������6������J10��4о�� 1-2\n"\
"34=0,1024,0,0,0,\n"\
"35=0,2048,0,0,0,\n"\
"//////////////////////////////////////////////////////////////////////////////////////\n"\
"//Switch1\n"\
"[Signal1swich]\n"\
"//�Ե�Դ����Ϊ��ߣ���8о����һ��Ϊ�Ϸ���ÿ��������ÿо����������\n"\
"//����������1��������8о��J15 1-7\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"//Switch2\n"\
"[Signal2swich]\n"\
"//����������3��������8о��J16 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"//Switch3\n"\
"[Powermeterswich1]\n"\
"//����������3��������8о��J16 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"//Switch4\n"\
"[Spectrumswich1]\n"\
"//�Ե�Դ����Ϊ��ߣ���8о����һ��Ϊ�Ϸ���ÿ��������ÿо����������\n"\
"//����������1��������8о��J15 1-7\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"//Switch5\n"\
"[PA1swich1]\n"\
"//����������1������J5��4о�� 1-3\n"\
"20=16,0,0,0,0,\n"\
"21=32,0,0,0,0,\n"\
"22=64,0,0,0,0,\n"\
"//Switch6\n"\
"[PA1swich2]\n"\
"//����������2������J6��4о�� 1-3\n"\
"23=128,0,0,0,0,\n"\
"24=0,0,16,0,0,\n"\
"25=0,0,32,0,0,\n"\
"//Switch7\n"\
"[PA2swich1]\n"\
"//����������1������J5��6о�� 1-6\n"\
"20=16,0,0,0,0,\n"\
"21=32,0,0,0,0,\n"\
"22=64,0,0,0,0,\n"\
"23=0,1,0,0,0,\n"\
"24=0,0,0,0,1024,\n"\
"25=0,0,0,0,2048,\n"\
"//Switch8\n"\
"[PA2swich2]\n"\
"//����������2������J6��6о�� 1-6\n"\
"23=128,0,0,0,0,\n"\
"24=0,0,16,0,0,\n"\
"25=0,0,32,0,0,\n"\
"26=0,2,0,0,0,\n"\
"27=0,0,0,0,8192,\n"\
"28=0,0,0,0,16384,\n"\
"//Switch9\n"\
"[Powermeterswich2]\n"\
"//����������3��������8о��J16 1-7\n"\
"//Switch10\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"[Spectrumswich2]\n"\
"//�Ե�Դ����Ϊ��ߣ���8о����һ��Ϊ�Ϸ���ÿ��������ÿо����������\n"\
"//����������1��������8о��J15 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"//Switch11\n"\
"[PA3swich1]\n"\
"//����������4������J8��8о�� 1-3\n"\
"28=0,0,0,0,1024,\n"\
"29=0,0,0,0,2048,\n"\
"30=0,0,0,0,4096,\n"\
"//Switch12\n"\
"[PA3swich2]\n"\
"//����������5������J9��4о�� 1-3\n"\
"31=0,0,0,0,8192,\n"\
"32=0,0,0,0,16384,\n"\
"33=0,0,0,0,32768,\n"\
"//Switch13\n"\
"[Couplerswich]\n"\
"36=0,0,16,0,0,\n"\
"37=128,0,0,0,0,\n"\
"//Switch14\n"\
"[Pimswich]\n"\
"32=32,0,0,0,0,\n"\
"33=16,0,0,0,0,\n"\
"//Switch15\n"\
"[Txoutswich]\n"\
"34=0,16,0,0,0,\n"\
"35=0,8,0,0,0,\n"\

#define IMPLEMENT_STRING_HW_EXT12      "[ip]\n"\
"Signalswich         =127.0.0.1:5000\n"\
"Paspecumpwmt       =127.0.0.1:5001\n"\
"PaspecumpwmtP       =127.0.0.1:5002\n"\
"Testmdlte700        =127.0.0.1:5003\n"\
"Testmddd800         =127.0.0.1:5004\n"\
"Testmdgsm900        =127.0.0.1:5005\n"\
"Testmddcs1800       =127.0.0.1:5006\n"\
"Testmdpcs1900       =127.0.0.1:5007\n"\
"Testmdwcdma2100     =127.0.0.1:5008\n"\
"Testmdlte2600       =127.0.0.1:5009\n"\
"Testmd4501          =127.0.0.1:5010\n"\
"Testmd4502          =127.0.0.1:5011\n"\
"Testmd700h          =127.0.0.1:5012\n"\
"Testmd700apt        =127.0.0.1:5013\n"\
"Testmd800cdma       =127.0.0.1:5014\n"\
"namelist=Signalswich,Paspecumpwmt,PaspecumpwmtP,Testmdlte700,Testmddd800,Testmdgsm900,Testmddcs1800,Testmdpcs1900,Testmdwcdma2100,Testmdlte2600,Testmd4501,Testmd4502,Testmd700h,Testmd700apt,Testmd800cdma\n"\
"\n"\
"[switch]\n"\
"\n"\
"//1\n"\
"Signal1swich = PA1in1, PA2in1, PA3in1, PA4in1\n"\
"//2\n"\
"Signal2swich = PA1in2 ,PA2in2 ,PA3in2, PA4in2\n"\
"//3\n"\
"Powermeterswich1  = md4501pwmt,md4502pwmt,mddcs1800pwmt,mdpcs1900pwmt,mdwcdma2100pwmt,mdlte2600pwmt,mdPowermeterswich2com\n"\
"//4\n"\
"Spectrumswich1  = md4501pim,md4502pim,mddcs1800pim,mdpcs1900pim,mdwcdma2100pim,mdlte2600pim,mdSpectrumswich2com\n"\
"//5\n"\
"PA1swich1 = md4501tx1, md4502tx1, md4503tx1\n"\
"//6\n"\
"PA1swich2  = md4501tx2, md4502tx2 ,md4503tx2\n"\
"//7\n"\
"PA2swich1  = md700ltx1, md700htx1 ,md700apttx1, mddd800tx1, mdcdma800tx1, mdgsm900tx1\n"\
"//8\n"\
"PA2swich2  = md700ltx2, md700htx2 ,md700apttx2, mddd800tx2, mdcdma800tx2, mdgsm900tx2\n"\
"//9\n"\
"Powermeterswich2  = md700lpwmt,md700hpwmt,md700aptpwmt,mddd800pwmt,mdcdma800pwmt,mdgsm900pwmt\n"\
"//10\n"\
"Spectrumswich2   = md700lpim,md700hpim,md700aptpim,mddd800pim,mdcdma800pim,mdgsm900pim\n"\
"//11\n"\
"PA3swich1  = mddcs1800tx1, mdpcs1900tx1, mdwcdma2100tx1\n"\
"//12\n"\
"PA3swich2  = mddcs1800tx2, mdpcs1900tx2, mdwcdma2100tx2\n"\
"//13\n"\
"Couplerswich = nc1, no2\n"\
"//14\n"\
"Pimswich  = nc1, no2\n"\
"//15\n"\
"Txoutswich  = nc1, no2\n"\
"namelist=Signal1swich,Signal2swich,Powermeterswich1,Spectrumswich1,PA1swich1,PA1swich2,PA2swich1,PA2swich2,Powermeterswich2,Spectrumswich2,PA3swich1,PA3swich2,Couplerswich,Pimswich,Txoutswich\n"\
"PA1in1=1\n"\
"PA2in1=2\n"\
"PA3in1=3\n"\
"PA4in1=4\n"\
"PA1in2=1\n"\
"PA2in2=2\n"\
"PA3in2=3\n"\
"PA4in2=4\n"\
"md4501pwmt=1\n"\
"md4502pwmt=2\n"\
"mddcs1800pwmt=3\n"\
"mdpcs1900pwmt=4\n"\
"mdwcdma2100pwmt=5\n"\
"mdlte2600pwmt=6\n"\
"mdPowermeterswich2com=7\n"\
"md4501pim=1\n"\
"md4502pim=2\n"\
"mddcs1800pim=3\n"\
"mdpcs1900pim=4\n"\
"mdwcdma2100pim=5\n"\
"mdlte2600pim=6\n"\
"mdSpectrumswich2com=7\n"\
"md4501tx1=1\n"\
"md4502tx1=2\n"\
"md4503tx1=3\n"\
"md4501tx2=1\n"\
"md4502tx2=2\n"\
"md4503tx2=3\n"\
"md700ltx1=1\n"\
"md700htx1=2\n"\
"md700apttx1=3\n"\
"mddd800tx1=4\n"\
"mdcdma800tx1=5\n"\
"mdgsm900tx1=6\n"\
"md700ltx2=1\n"\
"md700htx2=2\n"\
"md700apttx2=3\n"\
"mddd800tx2=4\n"\
"mdcdma800tx2=5\n"\
"mdgsm900tx2=6\n"\
"md700lpwmt=1\n"\
"md700hpwmt=2\n"\
"md700aptpwmt=3\n"\
"mddd800pwmt=4\n"\
"mdcdma800pwmt=5\n"\
"mdgsm900pwmt=6\n"\
"md700lpim=1\n"\
"md700hpim=2\n"\
"md700aptpim=3\n"\
"mddd800pim=4\n"\
"mdcdma800pim=5\n"\
"mdgsm900pim=6\n"\
"mddcs1800tx1=1\n"\
"mdpcs1900tx1=2\n"\
"mdwcdma2100tx1=3\n"\
"mddcs1800tx2=1\n"\
"mdpcs1900tx2=2\n"\
"mdwcdma2100tx2=3\n"\
"nc1=1\n"\
"no2=2\n"\
"\n"\
"[actiontx1]\n"\
"\n"\
"1tx1to4501ant1 =  Signal1swich(Signalswich,PA1in1), PA1swich1(Paspecumpwmt,md4501tx1),Txoutswich(Testmd4501,nc1)\n"\
"2tx1to4501ant2 =  Signal1swich(Signalswich,PA1in1), PA1swich1(Paspecumpwmt,md4501tx1),Txoutswich(Testmd4501,no2)\n"\
"\n"\
"3tx1to4502ant1 =  Signal1swich(Signalswich,PA1in1), PA1swich1(Paspecumpwmt,md4502tx1),Txoutswich(Testmd4502,nc1)\n"\
"4tx1to4502ant2 =  Signal1swich(Signalswich,PA1in1), PA1swich1(Paspecumpwmt,md4502tx1),Txoutswich(Testmd4502,no2)\n"\
"\n"\
"5tx1to700lant1 =  Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,md700ltx1),Txoutswich(Testmdlte700,nc1)\n"\
"6tx1to700lant2 =  Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,md700ltx1),Txoutswich(Testmdlte700,no2)\n"\
"\n"\
"7tx1to700hant1 =  Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,md700htx1),Txoutswich(Testmd700h,nc1)\n"\
"8tx1to700hant2 =  Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,md700htx1),Txoutswich(Testmd700h,no2)\n"\
"\n"\
"9tx1to700aptant1 =  Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,md700apttx1),Txoutswich(Testmd700apt,nc1)\n"\
"10tx1to700aptant2 = Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,md700apttx1),Txoutswich(Testmd700apt,no2)\n"\
"\n"\
"11tx1todd800ant1 = Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,mddd800tx1),Txoutswich(Testmddd800,nc1)\n"\
"12tx1todd800ant2 = Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,mddd800tx1),Txoutswich(Testmddd800,no2)\n"\
"\n"\
"13tx1tocdma800ant1 = Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,mdcdma800tx1),Txoutswich(Testmd800cdma,nc1)\n"\
"14tx1tocdma800ant2 = Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,mdcdma800tx1),Txoutswich(Testmd800cdma,no2)\n"\
"\n"\
"15tx1togsm900ant1 = Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,mdgsm900tx1),Txoutswich(Testmdgsm900,nc1)\n"\
"16tx1togsm900ant2 = Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,mdgsm900tx1),Txoutswich(Testmdgsm900,no2)\n"\
"\n"\
"17tx1todcs1800ant1 = Signal1swich(Signalswich,PA3in1), PA3swich1(Paspecumpwmt,mddcs1800tx1),Txoutswich(Testmddcs1800,nc1)\n"\
"18tx1todcs1800ant2 = Signal1swich(Signalswich,PA3in1), PA3swich1(Paspecumpwmt,mddcs1800tx1),Txoutswich(Testmddcs1800,no2)\n"\
"\n"\
"19tx1topcs1900ant1 = Signal1swich(Signalswich,PA3in1), PA3swich1(Paspecumpwmt,mdpcs1900tx1),Txoutswich(Testmdpcs1900,nc1)\n"\
"20tx1topcs1900ant2 = Signal1swich(Signalswich,PA3in1), PA3swich1(Paspecumpwmt,mdpcs1900tx1),Txoutswich(Testmdpcs1900,no2)\n"\
"\n"\
"21tx1towcdma2100ant1 = Signal1swich(Signalswich,PA3in1), PA3swich1(Paspecumpwmt,mdwcdma2100tx1),Txoutswich(Testmdwcdma2100,nc1)\n"\
"22tx1towcdma2100ant2 = Signal1swich(Signalswich,PA3in1), PA3swich1(Paspecumpwmt,mdwcdma2100tx1),Txoutswich(Testmdwcdma2100,no2)\n"\
"\n"\
"\n"\
"23tx1towcdma2600ant1 = Signal1swich(Signalswich,PA4in1),Txoutswich(Testmdlte2600,nc1)\n"\
"24tx1towcdma2600ant2 = Signal1swich(Signalswich,PA4in1),Txoutswich(Testmdlte2600,no2)\n"\
"namelist=1tx1to4501ant1,2tx1to4501ant2,3tx1to4502ant1,4tx1to4502ant2,5tx1to700lant1,6tx1to700lant2,7tx1to700hant1,8tx1to700hant2,9tx1to700aptant1,10tx1to700aptant2,11tx1todd800ant1,12tx1todd800ant2,13tx1tocdma800ant1,14tx1tocdma800ant2,15tx1togsm900ant1,16tx1togsm900ant2,17tx1todcs1800ant1,18tx1todcs1800ant2,19tx1topcs1900ant1,20tx1topcs1900ant2,21tx1towcdma2100ant1,22tx1towcdma2100ant2,23tx1towcdma2600ant1,24tx1towcdma2600ant2\n"\
"\n"\
"[actiontx2]\n"\
"\n"\
"1tx2to4501ant1 =  Signal2swich(Signalswich,PA1in2), PA1swich2(Paspecumpwmt,md4501tx2),Txoutswich(Testmd4501,nc1)\n"\
"2tx2to4501ant2 =  Signal2swich(Signalswich,PA1in2), PA1swich2(Paspecumpwmt,md4501tx2),Txoutswich(Testmd4501,no2)\n"\
"\n"\
"3tx2to4502ant1 =  Signal2swich(Signalswich,PA1in2), PA1swich2(Paspecumpwmt,md4502tx2),Txoutswich(Testmd4502,nc1)\n"\
"4tx2to4502ant2 =  Signal2swich(Signalswich,PA1in2), PA1swich2(Paspecumpwmt,md4502tx2),Txoutswich(Testmd4502,no2)\n"\
"\n"\
"5tx2to700lant1 =  Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,md700ltx2),Txoutswich(Testmdlte700,nc1)\n"\
"6tx2to700lant2 =  Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,md700ltx2),Txoutswich(Testmdlte700,no2)\n"\
"\n"\
"7tx2to700hant1 =  Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,md700htx2),Txoutswich(Testmd700h,nc1)\n"\
"8tx2to700hant2 =  Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,md700htx2),Txoutswich(Testmd700h,no2)\n"\
"\n"\
"9tx2to700aptant1 =  Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,md700apttx2),Txoutswich(Testmd700apt,nc1)\n"\
"10tx2to700aptant2 = Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,md700apttx2),Txoutswich(Testmd700apt,no2)\n"\
"\n"\
"11tx2todd800ant1 = Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,mddd800tx2),Txoutswich(Testmddd800,nc1)\n"\
"12tx2todd800ant2 = Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,mddd800tx2),Txoutswich(Testmddd800,no2)\n"\
"\n"\
"13tx2tocdma800ant1 = Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,mdcdma800tx2),Txoutswich(Testmd800cdma,nc1)\n"\
"14tx2tocdma800ant2 = Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,mdcdma800tx2),Txoutswich(Testmd800cdma,no2)\n"\
"\n"\
"15tx2togsm900ant1 = Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,mdgsm900tx2),Txoutswich(Testmdgsm900,nc1)\n"\
"16tx2togsm900ant2 = Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,mdgsm900tx2),Txoutswich(Testmdgsm900,no2)\n"\
"\n"\
"17tx2todcs1800ant1 = Signal2swich(Signalswich,PA3in2), PA3swich2(Paspecumpwmt,mddcs1800tx2),Txoutswich(Testmddcs1800,nc1)\n"\
"18tx2todcs1800ant2 = Signal2swich(Signalswich,PA3in2), PA3swich2(Paspecumpwmt,mddcs1800tx2),Txoutswich(Testmddcs1800,no2)\n"\
"\n"\
"19tx2topcs1900ant1 = Signal2swich(Signalswich,PA3in2), PA3swich2(Paspecumpwmt,mdpcs1900tx2),Txoutswich(Testmdpcs1900,nc1)\n"\
"20tx2topcs1900ant2 = Signal2swich(Signalswich,PA3in2), PA3swich2(Paspecumpwmt,mdpcs1900tx2),Txoutswich(Testmdpcs1900,no2)\n"\
"\n"\
"21tx2towcdma2100ant1 = Signal2swich(Signalswich,PA3in2), PA3swich2(Paspecumpwmt,mdwcdma2100tx2),Txoutswich(Testmdwcdma2100,nc1)\n"\
"22tx2towcdma2100ant2 = Signal2swich(Signalswich,PA3in2), PA3swich2(Paspecumpwmt,mdwcdma2100tx2),Txoutswich(Testmdwcdma2100,no2)\n"\
"\n"\
"23tx1towcdma2600ant1 = Signal2swich(Signalswich,PA4in2),Txoutswich(Testmdlte2600,nc1)\n"\
"24tx1towcdma2600ant2 = Signal2swich(Signalswich,PA4in2),Txoutswich(Testmdlte2600,no2)\n"\
"namelist=1tx2to4501ant1,2tx2to4501ant2,3tx2to4502ant1,4tx2to4502ant2,5tx2to700lant1,6tx2to700lant2,7tx2to700hant1,8tx2to700hant2,9tx2to700aptant1,10tx2to700aptant2,11tx2todd800ant1,12tx2todd800ant2,13tx2tocdma800ant1,14tx2tocdma800ant2,15tx2togsm900ant1,16tx2togsm900ant2,17tx2todcs1800ant1,18tx2todcs1800ant2,19tx2topcs1900ant1,20tx2topcs1900ant2,21tx2towcdma2100ant1,22tx2towcdma2100ant2,23tx1towcdma2600ant1,24tx1towcdma2600ant2\n"\
"\n"\
"[actionpim]\n"\
"\n"\
"1pimant1of4501  = Spectrumswich1(Paspecumpwmt,md4501pim), Pimswich (Testmd4501,nc1)\n"\
"2pimant2of4501  = Spectrumswich1(Paspecumpwmt,md4501pim), Pimswich (Testmd4501,no2)\n"\
"\n"\
"3pimant1of4502  = Spectrumswich1(Paspecumpwmt,md4502pim), Pimswich (Testmd4502,nc1)\n"\
"4pimant2of4502  = Spectrumswich1(Paspecumpwmt,md4502pim), Pimswich (Testmd4502,no2)\n"\
"\n"\
"5pimant1of700l  = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,md700lpim), Pimswich(Testmdlte700,nc1)\n"\
"6pimant2of700l  = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,md700lpim), Pimswich(Testmdlte700,no2)\n"\
"\n"\
"7pimant1of700h  = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,md700hpim), Pimswich(Testmd700h,nc1)\n"\
"8pimant2of700h  = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,md700hpim), Pimswich(Testmd700h,no2)\n"\
"\n"\
"9pimant1of700apt  = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,md700aptpim), Pimswich(Testmd700apt,nc1)\n"\
"10pimant2of700apt = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,md700aptpim), Pimswich(Testmd700apt,no2)\n"\
"\n"\
"11pimant1ofdd800 = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,mddd800pim), Pimswich(Testmddd800,nc1)\n"\
"12pimant2ofdd800 = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,mddd800pim), Pimswich(Testmddd800,no2)\n"\
"\n"\
"13pimant1ofcdma800 = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,mdcdma800pim), Pimswich(Testmd800cdma,nc1)\n"\
"14pimant2ofcdma800 = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,mdcdma800pim), Pimswich(Testmd800cdma,no2)\n"\
"\n"\
"15pimant1ofgsm900 = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,mdgsm900pim), Pimswich(Testmdgsm900,nc1)\n"\
"16pimant2ofgsm900 = Spectrumswich1(Paspecumpwmt,mdSpectrumswich2com), Spectrumswich2(PaspecumpwmtP,mdgsm900pim), Pimswich(Testmdgsm900,no2)\n"\
"\n"\
"17pimant1ofdcs1800  = Spectrumswich1(Paspecumpwmt,mddcs1800pim), Pimswich (Testmddcs1800,nc1)\n"\
"18pimant2ofdcs1800  = Spectrumswich1(Paspecumpwmt,mddcs1800pim), Pimswich (Testmddcs1800,no2)\n"\
"\n"\
"19pimant1ofpcs1900 = Spectrumswich1(Paspecumpwmt,mdpcs1900pim), Pimswich (Testmdpcs1900,nc1)\n"\
"20pimant2ofpcs1900 = Spectrumswich1(Paspecumpwmt,mdpcs1900pim), Pimswich (Testmdpcs1900,no2)\n"\
"\n"\
"21pimant1ofwcdam2100 = Spectrumswich1(Paspecumpwmt,mdwcdma2100pim), Pimswich (Testmdwcdma2100,nc1)\n"\
"22pimant2ofwcdma2100 = Spectrumswich1(Paspecumpwmt,mdwcdma2100pim), Pimswich (Testmdwcdma2100,no2)\n"\
"\n"\
"23pimant1oflte2600 = Spectrumswich1(Paspecumpwmt,mdlte2600pim), Pimswich (Testmdlte2600,nc1)\n"\
"24pimant2oflte2600 = Spectrumswich1(Paspecumpwmt,mdlte2600pim), Pimswich (Testmdlte2600,no2)\n"\
"namelist=1pimant1of4501,2pimant2of4501,3pimant1of4502,4pimant2of4502,5pimant1of700l,6pimant2of700l,7pimant1of700h,8pimant2of700h,9pimant1of700apt,10pimant2of700apt,11pimant1ofdd800,12pimant2ofdd800,13pimant1ofcdma800,14pimant2ofcdma800,15pimant1ofgsm900,16pimant2ofgsm900,17pimant1ofdcs1800,18pimant2ofdcs1800,19pimant1ofpcs1900,20pimant2ofpcs1900,21pimant1ofwcdam2100,22pimant2ofwcdma2100,23pimant1oflte2600,24pimant2oflte2600\n"\
"\n"\
"[actiondet]\n"\
"\n"\
"1couptx1of4501  = Powermeterswich1(Paspecumpwmt,md4501pwmt), Couplerswich(Testmd4501,nc1)\n"\
"2couptx2of4501  = Powermeterswich1(Paspecumpwmt,md4501pwmt), Couplerswich(Testmd4501,no2)\n"\
"\n"\
"3couptx1of4502  = Powermeterswich1(Paspecumpwmt,md4502pwmt), Couplerswich(Testmd4502,nc1)\n"\
"4couptx2of4502  = Powermeterswich1(Paspecumpwmt,md4502pwmt), Couplerswich(Testmd4502,no2)\n"\
"\n"\
"5couptx1of700l  = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,md700lpwmt), Couplerswich(Testmdlte700,nc1)\n"\
"6couptx2of700l  = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,md700lpwmt), Couplerswich(Testmdlte700,no2)\n"\
"\n"\
"7couptx1of700h  = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,md700hpwmt), Couplerswich(Testmd700h,nc1)\n"\
"8couptx2of700h  = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,md700hpwmt), Couplerswich(Testmd700h,no2)\n"\
"\n"\
"9couptx1of700apt  = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,md700aptpwmt), Couplerswich(Testmd700apt,nc1)\n"\
"10couptx2of700apt = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,md700aptpwmt), Couplerswich(Testmd700apt,no2)\n"\
"\n"\
"11couptx1ofdd800  = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,mddd800pwmt), Couplerswich(Testmddd800,nc1)\n"\
"12couptx2ofdd800  = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,mddd800pwmt), Couplerswich(Testmddd800,no2)\n"\
"\n"\
"13couptx1ofcdma800 = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,mdcdma800pwmt), Couplerswich(Testmd800cdma,nc1)\n"\
"14couptx2ofcdma800 = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,mdcdma800pwmt), Couplerswich(Testmd800cdma,no2)\n"\
"\n"\
"15couptx1ofgsm900 = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,mdgsm900pwmt), Couplerswich(Testmdgsm900,nc1)\n"\
"16couptx2ofgsm900 = Powermeterswich1(Paspecumpwmt,mdPowermeterswich2com), Powermeterswich2(PaspecumpwmtP,mdgsm900pwmt), Couplerswich(Testmdgsm900,no2)\n"\
"\n"\
"\n"\
"17couptx1ofdcs1800  = Powermeterswich1(Paspecumpwmt,mddcs1800pwmt), Couplerswich(Testmddcs1800,nc1)\n"\
"18couptx2ofdcs1800  = Powermeterswich1(Paspecumpwmt,mddcs1800pwmt), Couplerswich(Testmddcs1800,no2)\n"\
"\n"\
"19couptx1ofpcs1900  = Powermeterswich1(Paspecumpwmt,mdpcs1900pwmt), Couplerswich(Testmdpcs1900,nc1)\n"\
"20couptx2ofpcs1900  = Powermeterswich1(Paspecumpwmt,mdpcs1900pwmt), Couplerswich(Testmdpcs1900,no2)\n"\
"\n"\
"21couptx1ofwcdma2100  = Powermeterswich1(Paspecumpwmt,mdwcdma2100pwmt), Couplerswich(Testmdwcdma2100,nc1)\n"\
"22couptx2ofwcdma2100  = Powermeterswich1(Paspecumpwmt,mdwcdma2100pwmt), Couplerswich(Testmdwcdma2100,no2)\n"\
"\n"\
"23couptx1oflte2600  = Powermeterswich1(Paspecumpwmt,mdlte2600pwmt), Couplerswich(Testmdlte2600,nc1)\n"\
"24couptx2oflte2600  = Powermeterswich1(Paspecumpwmt,mdlte2600pwmt), Couplerswich(Testmdlte2600,no2)\n"\
"namelist=1couptx1of4501,2couptx2of4501,3couptx1of4502,4couptx2of4502,5couptx1of700l,6couptx2of700l,7couptx1of700h,8couptx2of700h,9couptx1of700apt,10couptx2of700apt,11couptx1ofdd800,12couptx2ofdd800,13couptx1ofcdma800,14couptx2ofcdma800,15couptx1ofgsm900,16couptx2ofgsm900,17couptx1ofdcs1800,18couptx2ofdcs1800,19couptx1ofpcs1900,20couptx2ofpcs1900,21couptx1ofwcdma2100,22couptx2ofwcdma2100,23couptx1oflte2600,24couptx2oflte2600\n"\

#endif
