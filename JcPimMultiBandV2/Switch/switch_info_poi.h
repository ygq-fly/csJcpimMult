/*******************************************************************************
*POI开关矩阵配置
*log:
2016.1.27 -> 增加2600MHz模块
*******************************************************************************/

#ifndef __SWITCH_CONFIG_POI_H__
#define __SWITCH_CONFIG_POI_H__
#define IO_STRING_POI	         "[STATE]\n"\
"//铁塔POI测试平台\n"\
"//*********//\n"\
"//开关箱1\n"\
"//开关1 \n"\
"//通道1-->0\n"\
"//通道2-->1\n"\
"//通道3-->2\n"\
"//通道4-->3\n"\
"[Signal1swich]\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"//开关2 \n"\
"//通道1-->10\n"\
"//通道2-->11\n"\
"//通道3-->12\n"\
"//通道4-->13\n"\
"[Signal2swich]\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"//*********//\n"\
"\n"\
"//*********//\n"\
"//开关箱2\n"\
"//开关3 \n"\
"//通道1-->0\n"\
"//通道2-->1\n"\
"//通道3-->2\n"\
"//通道4-->3\n"\
"//通道5-->4\n"\
"//通道6-->5\n"\
"//通道7-->6\n"\
"[Powermeterswich]\n"\
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
"//开关4 \n"\
"//通道1-->10\n"\
"//通道2-->11\n"\
"//通道3-->12\n"\
"//通道4-->13\n"\
"//通道5-->14\n"\
"//通道6-->15\n"\
"//通道3-->16\n"\
"[Spectrumswich]\n"\
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
"//开关5 \n"\
"//通道1-->20\n"\
"//通道2-->21\n"\
"//通道3-->22\n"\
"//通道4-->26\n"\
"[PA2swich1]\n"\
"20=16,0,0,0,0,\n"\
"21=32,0,0,0,0,\n"\
"22=64,0,0,0,0,\n"\
"23=0,1,0,0,0,\n"\
"24=0,0,0,0,1024,\n"\
"25=0,0,0,0,2048,\n"\
"//开关6 \n"\
"//通道1-->23\n"\
"//通道2-->24\n"\
"//通道3-->25\n"\
"//通道4-->27\n"\
"[PA2swich2]\n"\
"23=128,0,0,0,0,\n"\
"24=0,0,16,0,0,\n"\
"25=0,0,32,0,0,\n"\
"26=0,2,0,0,0,\n"\
"27=0,0,0,0,8192,\n"\
"28=0,0,0,0,16384,\n"\
"//*********//\n"\
"\n"\
"//*********//\n"\
"//开关箱3\n"\
"//开关1\n"\
"//通道1-->21\n"\
"//通道2-->20\n"\
"[Pimswich1]\n"\
"21=16,0,0,0,0,\n"\
"20=32,0,0,0,0,\n"\
"//开关2\n"\
"//通道1-->24\n"\
"//通道2-->23\n"\
"[Couplerswich2]\n"\
"24=128,0,0,0,0,\n"\
"23=0,0,16,0,0,\n"\
"//开关3\n"\
"//通道1-->27\n"\
"//通道2-->26\n"\
"[Tx1swich3]\n"\
"27=0,1,0,0,0,\n"\
"26=0,2,0,0,0,\n"\
"//开关4\n"\
"//通道1-->29\n"\
"//通道2-->28\n"\
"[Tx2swich4]\n"\
"29=0,0,0,0,1024,\n"\
"28=0,0,0,0,2048,\n"\
"//开关5\n"\
"//通道1-->32\n"\
"//通道2-->31\n"\
"[Antmodeswich5]\n"\
"32=0,0,0,0,8192,\n"\
"31=0,0,0,0,16384,\n"\
"//开关6\n"\
"//通道1-->35\n"\
"//通道2-->34\n"\
"[Antmodeswich6]\n"\
"35=0,1024,0,0,0,\n"\
"34=0,2048,0,0,0,\n"\
"//*********//\n"\
"[ABC]\n"\
"//以电源输入为左边，有8芯插座一边为上方，每个插座内每芯计数从左到右\n"\
"//上排左数第1个插座（8芯）J15 1-7\n"\
"0=0,0,0,0,2,\n"\
"1=0,0,0,0,1,\n"\
"2=0,512,0,0,0,\n"\
"3=0,256,0,0,0,\n"\
"4=0,32,0,0,0,\n"\
"5=0,16,0,0,0,\n"\
"6=0,8,0,0,0,\n"\
"//上排左数第2个插座（4芯）J3 1-3\n"\
"7=0,0,0,128,0,\n"\
"8=0,0,0,64,0,\n"\
"9=0,0,0,32,0,\n"\
"//上排左数第3个插座（8芯）J16 1-7\n"\
"10=0,0,0,16,0,\n"\
"11=0,0,0,8,0,\n"\
"12=0,0,0,4,0,\n"\
"13=0,0,0,2,0,\n"\
"14=0,0,0,1,0,\n"\
"15=0,0,4096,0,0,\n"\
"16=0,0,2048,0,0,\n"\
"//上排左数第4个插座J4（4芯） 1-3\n"\
"17=0,0,1024,0,0,\n"\
"18=32768,0,0,0,0,\n"\
"19=16384,0,0,0,0,\n"\
"//下排左数第1个插座J5（4芯） 1-3\n"\
"20=16,0,0,0,0,\n"\
"21=32,0,0,0,0,\n"\
"22=64,0,0,0,0,\n"\
"//下排左数第2个插座J6（4芯） 1-3\n"\
"23=128,0,0,0,0,\n"\
"24=0,0,16,0,0,\n"\
"25=0,0,32,0,0,\n"\
"//下排左数第3个插座J7（4芯） 1-2\n"\
"26=0,1,0,0,0,\n"\
"27=0,2,0,0,0,\n"\
"//下排左数第4个插座J8（8芯） 1-3\n"\
"28=0,0,0,0,1024,\n"\
"29=0,0,0,0,2048,\n"\
"30=0,0,0,0,4096,\n"\
"//下排左数第5个插座J9（4芯） 1-3\n"\
"31=0,0,0,0,8192,\n"\
"32=0,0,0,0,16384,\n"\
"33=0,0,0,0,32768,\n"\
"//下排左数第6个插座J10（4芯） 1-2\n"\
"34=0,1024,0,0,0,\n"\
"35=0,2048,0,0,0,\n"\
"\n"\
""
#define IMPLEMENT_STRING_POI  "[ip]\n"\
"namelist =Signalswich,PaspecumpwmtP,Testmdcdmagsm,Testmdfdd18,Testmdfdd21,Testmdtdftda,Testmdtdd23,Testmdtdd26,Testmdlte700p,Testmdpcs1900p\n"\
"Signalswich       =127.0.0.1:5001\n"\
"PaspecumpwmtP      =127.0.0.1:5002\n"\
"Testmdcdmagsm     =127.0.0.1:5003\n"\
"Testmdfdd18       =127.0.0.1:5004\n"\
"Testmdfdd21       =127.0.0.1:5005\n"\
"Testmdtdftda      =127.0.0.1:5006\n"\
"Testmdtdd23       =127.0.0.1:5007\n"\
"Testmdtdd26       =127.0.0.1:5008\n"\
"Testmdlte700p      =127.0.0.1:5009\n"\
"Testmdpcs1900p    =127.0.0.1:5010\n"\
"\n"\
"[switch]\n"\
"namelist = Signal1swich,Signal2swich,PA2swich1,PA2swich2,Powermeterswich,Spectrumswich,Couplerswich2,Pimswich1,Tx1swich3,Tx2swich4,Antmodeswich5,Antmodeswich6\n"\
"Signal1swich =PA1in1,PA2in1,PA3in1,PA4in1\n"\
"PA1in1=1\n"\
"PA2in1=2\n"\
"PA3in1=3\n"\
"PA4in1=4\n"\
"Signal2swich =PA1in2,PA2in2,PA3in2,PA4in2\n"\
"PA1in2=1\n"\
"PA2in2=2\n"\
"PA3in2=3\n"\
"PA4in2=4\n"\
"PA2swich1    =fdd18tx1,fdd21tx1,tdftdatx1,PA2swich1Bak1,pcs1900tx1,PA2S1_6\n"\
"fdd18tx1=1\n"\
"fdd21tx1=2\n"\
"tdftdatx1=3\n"\
"PA2swich1Bak1=4\n"\
"pcs1900tx1=5\n"\
"PA2S1_6=6\n"\
"PA2swich2    =fdd18tx2,fdd21tx2,tdftdatx2,tdftdatx3,pcs1900tx2,PA2S2_6\n"\
"fdd18tx2=1\n"\
"fdd21tx2=2\n"\
"tdftdatx2=3\n"\
"tdftdatx3=4\n"\
"pcs1900tx2=5\n"\
"PA2S2_6=6\n"\
"Spectrumswich    =cdmagsmpim,fdd18pim,fdd21pim,tdftdapim,SpecBak1,tdd23pim,tdd26pim,lte700pim,pcs1900pim,SPEC_10\n"\
"cdmagsmpim=1\n"\
"fdd18pim=2\n"\
"fdd21pim=3\n"\
"tdftdapim=4\n"\
"SpecBak1=5\n"\
"tdd23pim=6\n"\
"tdd26pim=7\n"\
"lte700pim=8\n"\
"pcs1900pim=9\n"\
"SPEC_10=10\n"\
"Powermeterswich  =cdmagsmpwmt,fdd18pwmt,fdd21pwmt,tdftdapwmt,tdftdapwmt2,tdd23pwmt,tdd26pwmt,lte700pwmt,pcs1900pwmt,POWER_10\n"\
"cdmagsmpwmt=1\n"\
"fdd18pwmt=2\n"\
"fdd21pwmt=3\n"\
"tdftdapwmt=4\n"\
"tdftdapwmt2=5\n"\
"tdd23pwmt=6\n"\
"tdd26pwmt=7\n"\
"lte700pwmt=8\n"\
"pcs1900pwmt=9\n"\
"POWER_10=10\n"\
"Couplerswich2    =nc1,no2\n"\
"Pimswich1        =nc1,no2\n"\
"Tx1swich3        =nc1,no2\n"\
"Tx2swich4        =nc1,no2\n"\
"Antmodeswich5    =nc1,no2\n"\
"Antmodeswich6    =nc1,no2\n"\
"nc1 = 1\n"\
"no2 = 2\n"\
"\n"\
"[actiontx1]\n"\
"namelist = 1Cmgsmtx1,2Cucdmatx1,3Ctfd18tx1,4Cufd18tx1,5Ctfd21tx1,6Cuw21tx1,7Cmdcstx1,8Cmtdftx1,9null,10Cmtdetx1,11Cttdetx1,12null,13Cmtdetx1,14lte700tx1,15pcs1900tx1\n"\
"\n"\
"1Cmgsmtx1= Signal1swich(Signalswich,PA1in1), Tx1swich3(Testmdcdmagsm , nc1),Tx1swich3(Testmdlte700p,nc1)\n"\
"2Cucdmatx1= Signal1swich(Signalswich,PA1in1), Tx1swich3(Testmdcdmagsm , no2),Tx1swich3(Testmdlte700p,nc1)\n"\
"3Ctfd18tx1= Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,fdd18tx1), Tx1swich3(Testmdfdd18, nc1)   \n"\
"4Cufd18tx1= Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,fdd18tx1), Tx1swich3(Testmdfdd18, no2)\n"\
"5Ctfd21tx1= Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,fdd21tx1), Tx1swich3(Testmdfdd21, nc1)    \n"\
"6Cuw21tx1= Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,fdd21tx1), Tx1swich3(Testmdfdd21 , no2)    \n"\
"7Cmdcstx1= Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,tdftdatx1), Tx1swich3(Testmdtdftda , nc1)  \n"\
"8Cmtdftx1= Signal1swich(Signalswich,PA2in1), PA2swich1(PaspecumpwmtP,tdftdatx1), Tx1swich3(Testmdtdftda , no2), Antmodeswich5(Testmdtdftda,nc1)\n"\
"9null=\n"\
"10Cmtdetx1= Signal1swich(Signalswich,PA3in1), Tx1swich3(Testmdtdd23, nc1),Antmodeswich6(Testmdtdd23,nc1),Tx1swich3(Testmdtdd26, nc1)\n"\
"11Cttdetx1= Signal1swich(Signalswich,PA3in1), Tx1swich3(Testmdtdd23, no2), Antmodeswich5(Testmdtdd23,nc1),Tx1swich3(Testmdtdd26, nc1)\n"\
"12null=\n"\
"13Cmtdetx1= Signal1swich(Signalswich,PA3in1),Tx1swich3(Testmdtdd26, no2), Antmodeswich5(Testmdtdd26,nc1) \n"\
"14lte700tx1=  Signal1swich(Signalswich,PA1in1),Tx1swich3(Testmdlte700p,no2)\n"\
"15pcs1900tx1=Signal1swich(Signalswich,PA2in1),PA2swich1(PaspecumpwmtP,pcs1900tx1)\n"\
"\n"\
"[actiontx2]\n"\
"namelist = 1Cmgsmtx2,2Cucdmatx2,3Ctfd18tx2,4Cufd18tx2,5Ctfd21tx2,6Cuw21tx2,7Cmdcstx2,8Cmtdftx2,9Cmtdatx2,10Cmtdetx2,11null,12Cutdetx2,13Cmtdetx2,14lte700tx2,15pcs1900tx2\n"\
"\n"\
"1Cmgsmtx2= Signal2swich(Signalswich,PA1in2),  Tx2swich4(Testmdcdmagsm , nc1),Tx2swich4(Testmdlte700p,nc1)\n"\
"2Cucdmatx2= Signal2swich(Signalswich,PA1in2), Tx2swich4(Testmdcdmagsm , no2),Tx2swich4(Testmdlte700p,nc1)\n"\
"3Ctfd18tx2= Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,fdd18tx2), Tx2swich4(Testmdfdd18, nc1)    \n"\
"4Cufd18tx2= Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,fdd18tx2), Tx2swich4(Testmdfdd18, no2) \n"\
"5Ctfd21tx2= Signal2swich(Signalswich,PA2in2), PA2swich2(PaspecumpwmtP,fdd21tx2), Tx2swich4(Testmdfdd21, nc1)     \n"\
"6Cuw21tx2= Signal2swich(Signalswich,PA2in2),  PA2swich2(PaspecumpwmtP,fdd21tx2), Tx2swich4(Testmdfdd21, no2)    \n"\
"7Cmdcstx2= Signal2swich(Signalswich,PA2in2),  PA2swich2(PaspecumpwmtP,tdftdatx2),Tx2swich4(Testmdtdftda, nc1)   \n"\
"8Cmtdftx2= Signal2swich(Signalswich,PA2in2),  PA2swich2(PaspecumpwmtP,tdftdatx2),Tx2swich4(Testmdtdftda, no2), Antmodeswich5(Testmdtdftda,nc1)\n"\
"9Cmtdatx2= Signal2swich(Signalswich,PA2in2),  PA2swich2(PaspecumpwmtP,tdftdatx3) \n"\
"10Cmtdetx2= Signal2swich(Signalswich,PA3in2), Tx2swich4(Testmdtdd23, nc1),Antmodeswich6(Testmdtdd23,nc1) ,Tx2swich4(Testmdtdd26, nc1)\n"\
"11null=\n"\
"12Cutdetx2= Signal2swich(Signalswich,PA3in2), Tx2swich4(Testmdtdd23, no2), Tx2swich4(Testmdtdd26, nc1)    \n"\
"13Cmtdetx2= Signal2swich(Signalswich,PA3in2),  Tx2swich4(Testmdtdd26, no2), Antmodeswich5(Testmdtdd26,nc1) \n"\
"14lte700tx2=  Signal2swich(Signalswich,PA1in2),  Tx2swich4(Testmdlte700p  ,  no2)\n"\
"15pcs1900tx2=Signal2swich(Signalswich,PA2in2),  PA2swich2(PaspecumpwmtP,  pcs1900tx2)\n"\
"\n"\
"[actiondet]\n"\
"namelist = Cdmagsmcp1,Cdmagsmcp2,fdd18cp1,fdd18cp2,fdd21cp1,fdd21cp2,tdftdacp1,tdftdacp2,tdftdacp3,tde23cp1,tde23cp2,tdd26cp1,tdd26cp2,lte700cp1,lte700cp2,pcs1900cp1,pcs1900cp2\n"\
"\n"\
"Cdmagsmcp1= Powermeterswich(PaspecumpwmtP,cdmagsmpwmt), Couplerswich2(Testmdcdmagsm,nc1)\n"\
"Cdmagsmcp2= Powermeterswich(PaspecumpwmtP,cdmagsmpwmt), Couplerswich2(Testmdcdmagsm,no2)\n"\
"\n"\
"fdd18cp1= Powermeterswich(PaspecumpwmtP,fdd18pwmt), Couplerswich2(Testmdfdd18,nc1)\n"\
"fdd18cp2= Powermeterswich(PaspecumpwmtP,fdd18pwmt), Couplerswich2(Testmdfdd18,no2)\n"\
"\n"\
"fdd21cp1= Powermeterswich(PaspecumpwmtP,fdd21pwmt), Couplerswich2(Testmdfdd21,nc1)\n"\
"fdd21cp2= Powermeterswich(PaspecumpwmtP,fdd21pwmt), Couplerswich2(Testmdfdd21,no2)\n"\
"\n"\
"tdftdacp1= Powermeterswich(PaspecumpwmtP,tdftdapwmt), Couplerswich2(Testmdtdftda,nc1)\n"\
"tdftdacp2= Powermeterswich(PaspecumpwmtP,tdftdapwmt), Couplerswich2(Testmdtdftda,no2)\n"\
"\n"\
"tdftdacp3= Powermeterswich(PaspecumpwmtP,tdftdapwmt2)\n"\
"\n"\
"tde23cp1= Powermeterswich(PaspecumpwmtP,tdd23pwmt), Couplerswich2(Testmdtdd23,nc1)\n"\
"tde23cp2= Powermeterswich(PaspecumpwmtP,tdd23pwmt), Couplerswich2(Testmdtdd23,no2)\n"\
"\n"\
"tdd26cp1= Powermeterswich(PaspecumpwmtP,tdd26pwmt), Couplerswich2(Testmdtdd26,nc1)\n"\
"tdd26cp2= Powermeterswich(PaspecumpwmtP,tdd26pwmt), Couplerswich2(Testmdtdd26,no2)\n"\
"\n"\
"lte700cp1=Powermeterswich(PaspecumpwmtP,lte700pwmt),  Couplerswich2(Testmdlte700p,nc1)\n"\
"lte700cp2=Powermeterswich(PaspecumpwmtP,lte700pwmt),  Couplerswich2(Testmdlte700p,no2)\n"\
"\n"\
"pcs1900cp1=Powermeterswich(PaspecumpwmtP,pcs1900pwmt),  Couplerswich2(Testmdpcs1900p,nc1)\n"\
"pcs1900cp2=Powermeterswich(PaspecumpwmtP,pcs1900pwmt),  Couplerswich2(Testmdpcs1900p,no2)\n"\
"\n"\
"[actionpim]\n"\
"namelist = 1gsmpim,2cdmapim,3ctfd18pim,4cufd18pim,5ctfd21pim,6cuw21pim,7cmdcspim,8cmtdfpim,9null,10cmtde23pim,11cttde23pim,12null,13cttdd26pim,14lte700pim,15pcs1900pim\n"\
"\n"\
"1gsmpim=  Spectrumswich(PaspecumpwmtP,cdmagsmpim), Pimswich1(Testmdcdmagsm,nc1)\n"\
"2cdmapim= Spectrumswich(PaspecumpwmtP,cdmagsmpim), Pimswich1(Testmdcdmagsm,no2)\n"\
"\n"\
"3ctfd18pim= Spectrumswich(PaspecumpwmtP,fdd18pim), Pimswich1(Testmdfdd18,nc1)\n"\
"4cufd18pim= Spectrumswich(PaspecumpwmtP,fdd18pim), Pimswich1(Testmdfdd18,no2)\n"\
"\n"\
"5ctfd21pim= Spectrumswich(PaspecumpwmtP,fdd21pim), Pimswich1(Testmdfdd21,nc1)\n"\
"6cuw21pim= Spectrumswich(PaspecumpwmtP,fdd21pim), Pimswich1(Testmdfdd21,no2)\n"\
"\n"\
"7cmdcspim= Spectrumswich(PaspecumpwmtP,tdftdapim), Pimswich1(Testmdtdftda,nc1)\n"\
"8cmtdfpim= Spectrumswich(PaspecumpwmtP,tdftdapim), Pimswich1(Testmdtdftda,no2),Antmodeswich5(Testmdtdftda,no2)\n"\
"\n"\
"9null=\n"\
"\n"\
"10cmtde23pim= Spectrumswich(PaspecumpwmtP,tdd23pim), Pimswich1(Testmdtdd23,nc1),Antmodeswich6(Testmdtdd23,no2)\n"\
"11cttde23pim= Spectrumswich(PaspecumpwmtP,tdd23pim), Pimswich1(Testmdtdd23,no2),Antmodeswich5(Testmdtdd23,no2)\n"\
"12=null\n"\
"13cttdd26pim= Spectrumswich(PaspecumpwmtP,tdd26pim), Pimswich1(Testmdtdd26,nc1),Antmodeswich5(Testmdtdd26,no2)\n"\
"14lte700pim=Spectrumswich(PaspecumpwmtP,lte700pim)\n"\
"15pcs1900pim=Spectrumswich(PaspecumpwmtP,pcs1900pim)\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
"\n"\
""

#endif
