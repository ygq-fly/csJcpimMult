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
#ifndef __SWITCH_CONFIG_HW_EXT8_H__
#define __SWITCH_CONFIG_HW_EXT8_H__
#define IO_STRING_HW_EXT8      "[STATE]\n"\
"//???????????????8��???????????????????????��??????????\n"\
"//??????????1????????8��??J15 1-7\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"//??????????2????????4��??J3 1-3\n"\
"7=0,0,0,128,0,\n"\
"8=0,0,0,64,0,\n"\
"9=0,0,0,32,0,\n"\
"//??????????3????????8��??J16 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"//??????????4??????J4??4��?? 1-3\n"\
"17=0,0,1024,0,0,\n"\
"18=32768,0,0,0,0,\n"\
"19=16384,0,0,0,0,\n"\
"//??????????1??????J5??4��?? 1-3\n"\
"20=16,0,0,0,0,\n"\
"21=32,0,0,0,0,\n"\
"22=64,0,0,0,0,\n"\
"//??????????2??????J6??4��?? 1-3\n"\
"23=128,0,0,0,0,\n"\
"24=0,0,16,0,0,\n"\
"25=0,0,32,0,0,\n"\
"//??????????3??????J7??4��?? 1-2\n"\
"26=0,1,0,0,0,\n"\
"27=0,2,0,0,0,\n"\
"//??????????4??????J8??8��?? 1-3\n"\
"28=0,0,0,0,1024,\n"\
"29=0,0,0,0,2048,\n"\
"30=0,0,0,0,4096,\n"\
"//??????????5??????J9??4��?? 1-3\n"\
"31=0,0,0,0,8192,\n"\
"32=0,0,0,0,16384,\n"\
"33=0,0,0,0,32768,\n"\
"//??????????6??????J10??4��?? 1-2\n"\
"34=0,1024,0,0,0,\n"\
"35=0,2048,0,0,0,\n"\
"//////////////////////////////////////////////////////////////////////////////////////\n"\
"//Switch1\n"\
"[Signal1swich]\n"\
"//???????????????8��???????????????????????��??????????\n"\
"//??????????1????????8��??J15 1-7\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"//Switch2\n"\
"[Signal2swich]\n"\
"//??????????3????????8��??J16 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"//Switch3\n"\
"[Powermeterswich1]\n"\
"//??????????3????????8��??J16 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"17=0,0,1024,0,0,\n"\
"18=32768,0,0,0,0,\n"\
"19=16384,0,0,0,0,\n"\
"//Switch4\n"\
"[Spectrumswich1]\n"\
"//???????????????8��???????????????????????��??????????\n"\
"//??????????1????????8��??J15 1-7\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"7=0,0,0,128,0,\n"\
"8=0,0,0,64,0,\n"\
"9=0,0,0,32,0,\n"\
"//Switch5\n"\
"[PA1swich1]\n"\
"//??????????1??????J5??4��?? 1-3\n"\
"20=16,0,0,0,0,\n"\
"21=32,0,0,0,0,\n"\
"22=64,0,0,0,0,\n"\
"//Switch6\n"\
"[PA1swich2]\n"\
"//??????????2??????J6??4��?? 1-3\n"\
"23=128,0,0,0,0,\n"\
"24=0,0,16,0,0,\n"\
"25=0,0,32,0,0,\n"\
"//Switch7\n"\
"[PA3swich1]\n"\
"//??????????1??????J5??6��?? 1-6\n"\
"20=16,0,0,0,0,\n"\
"21=32,0,0,0,0,\n"\
"22=64,0,0,0,0,\n"\
"23=0,1,0,0,0,\n"\
"24=0,0,0,0,1024,\n"\
"25=0,0,0,0,2048,\n"\
"//Switch8\n"\
"[PA3swich2]\n"\
"//??????????2??????J6??6��?? 1-6\n"\
"23=128,0,0,0,0,\n"\
"24=0,0,16,0,0,\n"\
"25=0,0,32,0,0,\n"\
"26=0,2,0,0,0,\n"\
"27=0,0,0,0,8192,\n"\
"28=0,0,0,0,16384,\n"\
"//Switch9\n"\
"[Powermeterswich2]\n"\
"//??????????3????????8��??J16 1-7\n"\
"//Switch10\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"[Spectrumswich2]\n"\
"//???????????????8��???????????????????????��??????????\n"\
"//??????????1????????8��??J15 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"//Switch11\n"\
"[PA2swich1]\n"\
"//??????????4??????J8??8��?? 1-3\n"\
"28=0,0,0,0,1024,\n"\
"29=0,0,0,0,2048,\n"\
"30=0,0,0,0,4096,\n"\
"//Switch12\n"\
"[PA2swich2]\n"\
"//??????????5??????J9??4��?? 1-3\n"\
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

#define IMPLEMENT_STRING_HW_EXT8     "[ip]\n"\
"Signalswich         =127.0.0.1:5000\n"\
"Paspecumpwmt       =127.0.0.1:5001\n"\
"PaspecumpwmtP       =127.0.0.1:5002\n"\
"Testmd800CA          =127.0.0.1:5003\n"\
"Testmd800CB          =127.0.0.1:5004\n"\
"Testmdpcs1900A      =127.0.0.1:5005\n"\
"Testmdpcs1900B      =127.0.0.1:5006\n"\
"Testmdlte2600A      =127.0.0.1:5007\n"\
"Testmdlte2600B      =127.0.0.1:5008\n"\
"Testmdlte2600C      =127.0.0.1:5009\n"\
"Testmdlte2600D      =127.0.0.1:5010\n"\
"namelist=Signalswich,Paspecumpwmt,PaspecumpwmtP,Testmd800CA,Testmd800CB,Testmdpcs1900A,Testmdpcs1900B,Testmdlte2600A,Testmdlte2600B,Testmdlte2600C,Testmdlte2600D\n"\
"[switch]\n"\
"\n"\
"//1\n"\
"Signal1swich = PA1in1, PA2in1, PA3in1, PA4in1\n"\
"//2\n"\
"Signal2swich = PA1in2 ,PA2in2 ,PA3in2, PA4in2\n"\
"//3\n"\
"Powermeterswich1  = md800apwmt,md800bpwmt,md1900apwmt,md1900bpwmt,md2600apwmt,md2600bpwmt,md2600cpwmt,md2600dpwmt\n"\
"//4\n"\
"Spectrumswich1  = md800apim,md800bpim,md1900apim,md1900bpim,md2600apim,md2600bpim,md2600cpim,md2600dpim\n"\
"//5\n"\
"PA1swich1 = md800atx1, md800btx1\n"\
"//6\n"\
"PA1swich2  = md800atx2, md800btx2\n"\
"//7\n"\
"PA2swich1  = md1900atx1, md1900btx1\n"\
"//8\n"\
"PA2swich2  = md1900atx2, md1900btx2\n"\
"//9\n"\
"Powermeterswich2  = md700lpwmt,md700hpwmt,md700aptpwmt,mddd800pwmt,mdcdma800pwmt,mdgsm900pwmt\n"\
"//10\n"\
"Spectrumswich2   = md700lpim,md700hpim,md700aptpim,mddd800pim,mdcdma800pim,mdgsm900pim\n"\
"//11\n"\
"PA3swich1  = md2600atx1, md2600btx1, md2600ctx1,md2600dtx1\n"\
"//12\n"\
"PA3swich2  = md2600atx2, md2600btx2, md2600ctx2,md2600dtx2\n"\
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
"md800apwmt=1\n"\
"md800bpwmt=2\n"\
"md1900apwmt=3\n"\
"md1900bpwmt=4\n"\
"md2600apwmt=5\n"\
"md2600bpwmt=6\n"\
"md2600cpwmt=7\n"\
"md2600dpwmt=8\n"\
"md800apim=1\n"\
"md800bpim=2\n"\
"md1900apim=3\n"\
"md1900bpim=4\n"\
"md2600apim=5\n"\
"md2600bpim=6\n"\
"md2600cpim=7\n"\
"md2600dpim=8\n"\
"md800atx1=1\n"\
"md800btx1=2\n"\
"md800atx2=1\n"\
"md800btx2=2\n"\
"md1900atx1=1\n"\
"md1900btx1=2\n"\
"md1900atx2=1\n"\
"md1900btx2=2\n"\
"md2600atx1=1\n"\
"md2600btx1=2\n"\
"md2600ctx1=3\n"\
"md2600dtx1=4\n"\
"md2600atx2=1\n"\
"md2600btx2=2\n"\
"md2600ctx2=3\n"\
"md2600dtx2=4\n"\
"\n"\
"[actiontx1]\n"\
"1tx1to800aant1 =  Signal1swich(Signalswich,PA1in1), PA1swich1(Paspecumpwmt,md800atx1),Txoutswich(Testmd800CA,nc1)\n"\
"2tx1to800aant2 =  Signal1swich(Signalswich,PA1in1), PA1swich1(Paspecumpwmt,md800atx1),Txoutswich(Testmd800CA,no2)\n"\
"3tx1to800bant1 =  Signal1swich(Signalswich,PA1in1), PA1swich1(Paspecumpwmt,md800btx1),Txoutswich(Testmd800CB,nc1)\n"\
"4tx1to800bant2 =  Signal1swich(Signalswich,PA1in1), PA1swich1(Paspecumpwmt,md800btx1),Txoutswich(Testmd800CB,no2)\n"\
"5tx1to1900aant1 = Signal1swich(Signalswich,PA2in1), PA2swich1(Paspecumpwmt,md1900atx1),Txoutswich(Testmdpcs1900A,nc1)\n"\
"6tx1to1900aant2 = Signal1swich(Signalswich,PA2in1), PA2swich1(Paspecumpwmt,md1900atx1),Txoutswich(Testmdpcs1900A,no2)\n"\
"7tx1to1900bant1 = Signal1swich(Signalswich,PA2in1), PA2swich1(Paspecumpwmt,md1900btx1),Txoutswich(Testmdpcs1900B,nc1)\n"\
"8tx1to1900bant2 = Signal1swich(Signalswich,PA2in1), PA2swich1(Paspecumpwmt,md1900btx1),Txoutswich(Testmdpcs1900B,no2)\n"\
"9tx1to2600aant1 = Signal1swich(Signalswich,PA3in1), PA3swich1(PaspecumpwmtP,md2600atx1),Txoutswich(Testmdlte2600A,nc1)\n"\
"10tx1to2600aant2 = Signal1swich(Signalswich,PA3in1), PA3swich1(PaspecumpwmtP,md2600atx1),Txoutswich(Testmdlte2600A,no2)\n"\
"11tx1to2600bant1 = Signal1swich(Signalswich,PA3in1), PA3swich1(PaspecumpwmtP,md2600btx1),Txoutswich(Testmdlte2600B,nc1)\n"\
"12tx1to2600bant2 = Signal1swich(Signalswich,PA3in1), PA3swich1(PaspecumpwmtP,md2600btx1),Txoutswich(Testmdlte2600B,no2)\n"\
"13tx1to2600cant1 = Signal1swich(Signalswich,PA3in1), PA3swich1(PaspecumpwmtP,md2600ctx1),Txoutswich(Testmdlte2600C,nc1)\n"\
"14tx1to2600cant2 = Signal1swich(Signalswich,PA3in1), PA3swich1(PaspecumpwmtP,md2600ctx1),Txoutswich(Testmdlte2600C,no2)\n"\
"15tx1to2600dant1 = Signal1swich(Signalswich,PA3in1), PA3swich1(PaspecumpwmtP,md2600dtx1),Txoutswich(Testmdlte2600D,nc1)\n"\
"16tx1to2600dant2 = Signal1swich(Signalswich,PA3in1), PA3swich1(PaspecumpwmtP,md2600dtx1),Txoutswich(Testmdlte2600D,no2)\n"\
"namelist=1tx1to800aant1,2tx1to800aant2,3tx1to800bant1,4tx1to800bant2,5tx1to1900aant1,6tx1to1900aant2,7tx1to1900bant1,8tx1to1900bant2,9tx1to2600aant1,10tx1to2600aant2,11tx1to2600bant1,12tx1to2600bant2,13tx1to2600cant1,14tx1to2600cant2,15tx1to2600dant1,16tx1to2600dant2\n"\
"\n"\
"[actiontx2]\n"\
"namelist=1tx2to800aant1,2tx2to800aant2,3tx2to800bant1,4tx2to800bant2,5tx2to1900aant1,6tx2to1900aant2,7tx2to1900bant1,8tx2to1900bant2,9tx2to2600aant1,10tx2to2600aant2,11tx2to2600bant1,12tx2to2600bant2,13tx2to2600cant1,14tx2to2600cant2,15tx2to2600dant1,16tx2to2600dant2\n"\
"1tx2to800aant1 =  Signal2swich(Signalswich,PA1in2), PA1swich2(Paspecumpwmt,md800atx2),Txoutswich(Testmd800CA,nc1)\n"\
"2tx2to800aant2 =  Signal2swich(Signalswich,PA1in2), PA1swich2(Paspecumpwmt,md800atx2),Txoutswich(Testmd800CA,no2)\n"\
"3tx2to800bant1 =  Signal2swich(Signalswich,PA1in2), PA1swich2(Paspecumpwmt,md800btx2),Txoutswich(Testmd800CB,nc1)\n"\
"4tx2to800bant2 =  Signal2swich(Signalswich,PA1in2), PA1swich2(Paspecumpwmt,md800btx2),Txoutswich(Testmd800CB,no2)\n"\
"5tx2to1900aant1 = Signal2swich(Signalswich,PA2in2), PA2swich2(Paspecumpwmt,md1900atx2),Txoutswich(Testmdpcs1900A,nc1)\n"\
"6tx2to1900aant2 = Signal2swich(Signalswich,PA2in2), PA2swich2(Paspecumpwmt,md1900atx2),Txoutswich(Testmdpcs1900A,no2)\n"\
"7tx2to1900bant1 = Signal2swich(Signalswich,PA2in2), PA2swich2(Paspecumpwmt,md1900btx2),Txoutswich(Testmdpcs1900B,nc1)\n"\
"8tx2to1900bant2 = Signal2swich(Signalswich,PA2in2), PA2swich2(Paspecumpwmt,md1900btx2),Txoutswich(Testmdpcs1900B,no2)\n"\
"9tx2to2600aant1 = Signal2swich(Signalswich,PA3in2), PA3swich2(PaspecumpwmtP,md2600atx2),Txoutswich(Testmdlte2600A,nc1)\n"\
"10tx2to2600aant2 = Signal2swich(Signalswich,PA3in2), PA3swich2(PaspecumpwmtP,md2600atx2),Txoutswich(Testmdlte2600A,no2)\n"\
"11tx2to2600bant1 = Signal2swich(Signalswich,PA3in2), PA3swich2(PaspecumpwmtP,md2600btx2),Txoutswich(Testmdlte2600B,nc1)\n"\
"12tx2to2600bant2 = Signal2swich(Signalswich,PA3in2), PA3swich2(PaspecumpwmtP,md2600btx2),Txoutswich(Testmdlte2600B,no2)\n"\
"13tx2to2600cant1 = Signal2swich(Signalswich,PA3in2), PA3swich2(PaspecumpwmtP,md2600ctx2),Txoutswich(Testmdlte2600C,nc1)\n"\
"14tx2to2600cant2 = Signal2swich(Signalswich,PA3in2), PA3swich2(PaspecumpwmtP,md2600ctx2),Txoutswich(Testmdlte2600C,no2)\n"\
"15tx2to2600dant1 = Signal2swich(Signalswich,PA3in2), PA3swich2(PaspecumpwmtP,md2600dtx2),Txoutswich(Testmdlte2600D,nc1)\n"\
"16tx2to2600dant2 = Signal2swich(Signalswich,PA3in2), PA3swich2(PaspecumpwmtP,md2600dtx2),Txoutswich(Testmdlte2600D,no2)\n"\
"\n"\
"[actionpim]\n"\
"1pimant1of800a  = Spectrumswich1(Paspecumpwmt,md800apim), Pimswich (Testmd800CA,nc1)\n"\
"2pimant2of800a  = Spectrumswich1(Paspecumpwmt,md800apim), Pimswich (Testmd800CA,no2)\n"\
"3pimant1of800b  = Spectrumswich1(Paspecumpwmt,md800bpim), Pimswich (Testmd800CB,nc1)\n"\
"4pimant2of800b  = Spectrumswich1(Paspecumpwmt,md800bpim), Pimswich (Testmd800CB,no2)\n"\
"5pimant1of1900a = Spectrumswich1(Paspecumpwmt,md1900apim), Pimswich (Testmdpcs1900A,nc1)\n"\
"6pimant2of1900a = Spectrumswich1(Paspecumpwmt,md1900apim), Pimswich (Testmdpcs1900A,no2)\n"\
"7pimant1of1900b = Spectrumswich1(Paspecumpwmt,md1900bpim), Pimswich (Testmdpcs1900B,nc1)\n"\
"8pimant2of1900b = Spectrumswich1(Paspecumpwmt,md1900bpim), Pimswich (Testmdpcs1900B,no2)\n"\
"9pimant1of2600a = Spectrumswich1(Paspecumpwmt,md2600apim), Pimswich (Testmdlte2600A,nc1)\n"\
"10pimant2of2600a = Spectrumswich1(Paspecumpwmt,md2600apim), Pimswich (Testmdlte2600A,no2)\n"\
"11pimant1of2600b = Spectrumswich1(Paspecumpwmt,md2600bpim), Pimswich (Testmdlte2600B,nc1)\n"\
"12pimant2of2600b = Spectrumswich1(Paspecumpwmt,md2600bpim), Pimswich (Testmdlte2600B,no2)\n"\
"13pimant1of2600c = Spectrumswich1(Paspecumpwmt,md2600cpim), Pimswich (Testmdlte2600C,nc1)\n"\
"14pimant2of2600c = Spectrumswich1(Paspecumpwmt,md2600cpim), Pimswich (Testmdlte2600C,no2)\n"\
"15pimant1of2600d = Spectrumswich1(Paspecumpwmt,md2600dpim), Pimswich (Testmdlte2600D,nc1)\n"\
"16pimant2of2600d = Spectrumswich1(Paspecumpwmt,md2600dpim), Pimswich (Testmdlte2600D,no2)\n"\
"namelist=1pimant1of800a,2pimant2of800a,3pimant1of800b,4pimant2of800b,5pimant1of1900a,6pimant2of1900a,7pimant1of1900b,8pimant2of1900b,9pimant1of2600a,10pimant2of2600a,11pimant1of2600b,12pimant2of2600b,13pimant1of2600c,14pimant2of2600c,15pimant1of2600d,16pimant2of2600d\n"\
"\n"\
"[actiondet]\n"\
"1couptx1of800a  = Powermeterswich1(Paspecumpwmt,md800apwmt), Couplerswich(Testmd800CA,nc1)\n"\
"2couptx2of800a  = Powermeterswich1(Paspecumpwmt,md800apwmt), Couplerswich(Testmd800CA,no2)\n"\
"3couptx1of800b  = Powermeterswich1(Paspecumpwmt,md800bpwmt), Couplerswich(Testmd800CB,nc1)\n"\
"4couptx2of800b  = Powermeterswich1(Paspecumpwmt,md800bpwmt), Couplerswich(Testmd800CB,no2)\n"\
"5couptx1of1900a  = Powermeterswich1(Paspecumpwmt,md1900apwmt), Couplerswich(Testmdpcs1900A,nc1)\n"\
"6couptx2of1900a  = Powermeterswich1(Paspecumpwmt,md1900apwmt), Couplerswich(Testmdpcs1900A,no2)\n"\
"7couptx1of1900b  = Powermeterswich1(Paspecumpwmt,md1900bpwmt), Couplerswich(Testmdpcs1900B,nc1)\n"\
"8couptx2of1900b  = Powermeterswich1(Paspecumpwmt,md1900bpwmt), Couplerswich(Testmdpcs1900B,no2)\n"\
"9couptx1oflte2600a  = Powermeterswich1(Paspecumpwmt,md2600apwmt), Couplerswich(Testmdlte2600A,nc1)\n"\
"10couptx2oflte2600a  = Powermeterswich1(Paspecumpwmt,md2600apwmt), Couplerswich(Testmdlte2600A,no2)\n"\
"11couptx1oflte2600b  = Powermeterswich1(Paspecumpwmt,md2600bpwmt), Couplerswich(Testmdlte2600B,nc1)\n"\
"12couptx2oflte2600b  = Powermeterswich1(Paspecumpwmt,md2600bpwmt), Couplerswich(Testmdlte2600B,no2)\n"\
"13couptx1oflte2600c  = Powermeterswich1(Paspecumpwmt,md2600cpwmt), Couplerswich(Testmdlte2600C,nc1)\n"\
"14couptx2oflte2600c  = Powermeterswich1(Paspecumpwmt,md2600cpwmt), Couplerswich(Testmdlte2600C,no2)\n"\
"15couptx1oflte2600d  = Powermeterswich1(Paspecumpwmt,md2600dpwmt), Couplerswich(Testmdlte2600D,nc1)\n"\
"16couptx2oflte2600d  = Powermeterswich1(Paspecumpwmt,md2600dpwmt), Couplerswich(Testmdlte2600D,no2)\n"\
"namelist=1couptx1of800a,2couptx2of800a,3couptx1of800b,4couptx2of800b,5couptx1of1900a,6couptx2of1900a,7couptx1of1900b,8couptx2of1900b,9couptx1oflte2600a,10couptx2oflte2600a,11couptx1oflte2600b,12couptx2oflte2600b,13couptx1oflte2600c,14couptx2oflte2600c,15couptx1oflte2600d,16couptx2oflte2600d\n"\

#endif
