/**
 * @file ClaAnaRsFspSerial.cpp
 * @brief
 *
 *
 * @author
 * @note
 * @copyright Jointcom
 * @date 2015.4.10
 * @version v0.1 alpha
 */


#include "ClsAnaRsFspSerial.h"

bool ClsAnaRsFspSerial::InstrConnect(const char* c_addr)
{
	bool isconn = AgConnect(c_addr);
	//连接成功即开始初始化
	if (isconn)
		InstrInit();
	return isconn;
}

void ClsAnaRsFspSerial::InstrSession(unsigned long viConnectedSession)
{
	AgSession(viConnectedSession);
	//连接成功即开始初始化
	InstrInit();
}

bool ClsAnaRsFspSerial::InstrWrite(const char* c_cmd)
{
	return AgWrite(c_cmd);
}

long ClsAnaRsFspSerial::InstrWriteAndRead(const char* c_cmd, char* rbuf)
{
	return AgWriteAndRead(c_cmd, rbuf);
}

bool ClsAnaRsFspSerial::InstrConnStatus()const
{
	return AgConnStatus();
}

void ClsAnaRsFspSerial::InstrClose()
{
	AgClose();
}

void ClsAnaRsFspSerial::InstrInit()
{
    AgWrite("*RST\n");
    Preset (preset_default);
}

double ClsAnaRsFspSerial::InstrGetAnalyzer(double freq_khz, bool isMax)
{
    
}
void ClsAnaRsFspSerial::InstrSetAvg(const int& avg_time)
{
//    [SENSe<1|2>:]AVERage:COUNt 0 to 32767
}
void ClsAnaRsFspSerial::InstrClosgAvg()
{
    
}
void ClsAnaRsFspSerial::InstrSetOffset(const double& pow_dbm)
{
//    [SENSe<1|2>:]FREQuency:OFFSet <numeric_value>
}
// void InstrSetAttRef(const int& att, const int& reflevel) = 0;
// void InstrSetRbwSpan(const int& rbw_hz, const int& span_hz) = 0;
void ClsAnaRsFspSerial::InstrSetAtt(const int& att)
{
//    INPut<1|2>:ATTenuation 0 to 70/75dB
}
void ClsAnaRsFspSerial::InstrSetRef(const int& reflevel)
{
//    [SENSe<1|2>:]LIST:POWer[:SEQuence]
}
void ClsAnaRsFspSerial::InstrSetRbw(const double& rbw_hz)
{
//    [SENSe<1|2>:]LIST:POWer[:SEQuence]
}
void ClsAnaRsFspSerial::InstrSetVbw(const double& vbw_hz)
{
//    [SENSe<1|2>:]LIST:POWer[:SEQuence]
}
void ClsAnaRsFspSerial::InstrSetSpan(const double& span_hz)
{
//    [SENSe<1|2>:]FREQuency:SPAN 0 to fmax
}
void ClsAnaRsFspSerial::SetCenterFreq(const double& freq_khz)
{
//    [SENSe<1|2>:]FREQuency:CENTer 0 to fmax
}

void ClsAnaRsFspSerial::Preset(enum preset_parameter pp)
{
    static const double freq_span[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
    static const double freq_center[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
    static const double freq_offset[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
    static const double list_vbw[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
    static const double list_rbw[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
//    static const double list_sweep[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
    static const double list_rlev[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
    static const double list_att[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
    static const double adverage[PRESET_PARAMETER_TOTAL] = {0 , 0 , 0};
    
    
    if (pp < 0 || pp >= PRESET_PARAMETER_TOTAL)
        return ;
    
    InstrSetSpan (freq_span[pp]);
    SetCenterFreq (freq_center[pp]);
    InstrSetVbw (list_vbw[pp]);
    InstrSetRbw (list_rbw[pp]);
    InstrSetRef (list_rlev[pp]);
    InstrSetAtt (list_att[pp]);
    InstrSetOffset (freq_offset[pp]);
    InstrSetAvg (adverage[pp]);
    
}

