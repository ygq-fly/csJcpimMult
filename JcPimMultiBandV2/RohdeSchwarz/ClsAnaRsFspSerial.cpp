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

using namespace std;

bool ClsAnaRsFspSerial::InstrConnect(const char* c_addr)
{
	bool isconn = AgConnect(c_addr);
	//连接成功即开始初始化
	//if (isconn)
	//	InstrInit();
	return isconn;
}

void ClsAnaRsFspSerial::InstrSession(unsigned long viConnectedSession, const char* cIdn)
{
	AgSession(viConnectedSession, cIdn);
	//连接成功即开始初始化
	//InstrInit();
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
    AgWrite("*CLS\n");
    AgWrite("*RST\n");
	//rs仪表需要实时显示
    AgWrite("SYST:DISP:UPD ON\n");
	AgWrite("CALC:MARK1 ON\n");
	if (Util::strFind(_strIDN, "FSC"))
		AgWrite("ROSC:SOUR EXT\n");
    Preset (preset_default);

	//------------------------write by san-------------------
	InstrSetCenterFreq(637 * 1000);
	//请注意!
	//RS仪表在连续测量模式下无法使用主动触发检测信号
	//aglient的支持！
	//以下条命令必须执行
	AgWrite("INIT:CONT OFF\n");
	//开始预放
	AgWrite("INP:GAIN:STAT ON\n");
	//------------------------write by san-------------------
}

double ClsAnaRsFspSerial::InstrGetAnalyzer(double freq_khz, bool isMax)
{
	char const *mark_x = "CALC:MARK1:X %lf%s\n";
	char const *mark_max = "CALC:MARK1:MAX\n";
	char const *mark_y = "CALC:MARK1:Y?\n";

	//------------------------write by san-------------------
	//请注意!
	//必须添加主动触发检测信号
	InstrSetCenterFreq(freq_khz);
	bool isCmdSucc = AgWrite("*CLS\n");
	isCmdSucc = AgWrite("INIT\n");
	if (false == AgWait())
		return -10000;
	//------------------------write by san-------------------

	if (isMax == true)
		CommonSet(mark_max);
	//else
	//	CommonSet(mark_x, freq_khz, "KHz");

	char buf[1024] = { 0 };
	long retCount = AgWriteAndRead(mark_y, buf);

	double val = -10000;

	if (retCount)
		val = atof(buf);

	return val;
}
void ClsAnaRsFspSerial::InstrSetAvg(const int& avg_time)
{
    //[SENSe<1|2>:]AVERage:COUNt 0 to 32767
    //char const *set_aver_count = "AVER:COUN %d%s\n";

	//------------------------write by san-------------------
	if (avg_time > 0) {
		//AgWrite("AVER ON\n"); //当前华为不使用
		CommonSet("AVER:COUN %d\n", avg_time);
	}
	else
		InstrClosgAvg();
	//------------------------write by san-------------------
}
void ClsAnaRsFspSerial::InstrClosgAvg()
{
    //[SENSe<1|2>:]AVERage[:STATe<1...3>] ON | OFF
    //char const *set_aver_state = "AVER OFF\n";
	CommonSet("AVER OFF\n");
}
void ClsAnaRsFspSerial::InstrSetOffset(const double& pow_db)
{
    //[SENSe<1|2>:]FREQuency:OFFSet <numeric_value>
    //char const *set_freq_offset = "FREQ:OFFS %.0f%s\n";

	//------------------------write by san-------------------
	//注意 OFFSET的单位为dB，非dBm
	CommonSet("DISP:TRAC:Y:RLEV:OFFS %lf dB\n", pow_db);
	//------------------------write by san-------------------
}

// void InstrSetAttRef(const int& att, const int& reflevel) = 0;
// void InstrSetRbwSpan(const int& rbw_hz, const int& span_hz) = 0;

void ClsAnaRsFspSerial::InstrSetAtt(const int& att)
{//INPut<1|2>:ATTenuation 0 to 70/75dB
    //char const *set_att = "INP:ATT %d%s\n";
	CommonSet("INP:ATT %d dB\n", att);
}
void ClsAnaRsFspSerial::InstrSetRef(const int& reflevel)
{
    //DISPlay[:WINDow<1|2>]:TRACe<1...3>:Y[:SCALe]:RLEVel -130dBm to 30dBm
    //char const *set_disp_rlevel = "DISP:TRAC:Y:RLEV %d%s\n";
	CommonSet("DISP:TRAC:Y:RLEV %d dBm\n", reflevel);
}
void ClsAnaRsFspSerial::InstrSetRbw(const double& rbw_hz)
{
    //[SENSe<1|2>:]BANDwidth|BWIDth[:RESolution] 10 Hz to 10 MHz
    char const *set_rbw = "BAND %.0f%s\n";
    CommonSet (set_rbw, rbw_hz, "Hz");
}
void ClsAnaRsFspSerial::InstrSetVbw(const double& vbw_hz)
{
    //[SENSe<1|2>:]BANDwidth|BWIDth:VIDeo 1 Hz to 10 MHz
    char const *set_vbw = "BAND:VID %.0f%s\n";
    CommonSet (set_vbw, vbw_hz, "Hz");
}
void ClsAnaRsFspSerial::InstrSetSpan(const double& span_hz)
{
    //[SENSe<1|2>:]FREQuency:SPAN 0 to fmax
    char const *set_freq_span = "FREQ:SPAN %.0f%s\n";
    CommonSet (set_freq_span, span_hz, "Hz");
}
void ClsAnaRsFspSerial::InstrSetCenterFreq(const double& freq_khz)
{
    //[SENSe<1|2>:]FREQuency:CENTer 0 to fmax
    char const *set_freq_center = "FREQ:CENT %lf%s\n";
    CommonSet (set_freq_center, freq_khz, "KHz");
	char const *mark_x = "CALC:MARK1:X %lf%s\n";
	CommonSet(mark_x, freq_khz, "KHz");
}

void ClsAnaRsFspSerial::InstrSetSweepTime(int count_ms) {
	if (count_ms > 0) {
		CommonSet("SWE:TIME:AUTO OFF\n");
		CommonSet("SWE:TIME %d ms\n", count_ms);
	}
	else
		CommonSet("SWE:TIME:AUTO ON\n");

}

void ClsAnaRsFspSerial::InstrSetPreamp(bool isOn) {
	if (!_isNeedPreamp) {
		AgWrite("INP:GAIN:STAT OFF\n");
		return;
	}
	if (isOn){
		//开始预放
		AgWrite("INP:GAIN:STAT ON\n");
	}
	else {
		//off预放
		AgWrite("INP:GAIN:STAT OFF\n");
	}
}

void ClsAnaRsFspSerial::Preset(enum preset_parameter pp)
{
	//2015-5-5/vco: 15khz-300hz-1000hz
	//
	//------------------------write by san-------------------
	//SPAN:       0 HZ		,	400*1000 HZ		,	1000
	//BW:         30 HZ		,	10*1000			,	100
	//VBW:        100 HZ	,	10*1000			,	100
	//SWEEP TIME: 1 ms		,	auto			,	auto

	//AVERGE:     0			,	0				,	0
	//REFLEVEL:   -60		,	-60				,	20
	//OFFSET:     0			,	0				,	0
	//ATT:        0			,	0				,	30
	//------------------------write by san-------------------
	static const double freq_span[PRESET_PARAMETER_TOTAL] = { 0, 15000, 1000 };
	static const double freq_rbw[PRESET_PARAMETER_TOTAL] = { 10, 300, 100 };
	static const double freq_vbw[PRESET_PARAMETER_TOTAL] = { 30, 1000, 100 };
	static const int    sweep_time[PRESET_PARAMETER_TOTAL] = { 1, 0, 0 };

	static const int    freq_aver[PRESET_PARAMETER_TOTAL] = { 0, 0, 0 };
	static const int    disp_rlev[PRESET_PARAMETER_TOTAL] = { -60, -60, 10 };
	static const double disp_rlev_offset[PRESET_PARAMETER_TOTAL] = { 0, 0, 0 };
	static const int    input_att[PRESET_PARAMETER_TOTAL] = { 0, 0, 40 };


	if (pp < 0 || pp >= PRESET_PARAMETER_TOTAL || pp_ == pp)
		return;

	pp_ = pp;

	InstrSetOffset(disp_rlev_offset[pp]);
	if (Util::strFind(_strIDN, "FSC") && pp == preset_default) {
		InstrSetSpan(500);
		InstrSetVbw(30);
		InstrSetRbw(30);
	}
	else {
		InstrSetSpan(freq_span[pp]);
		InstrSetVbw(freq_vbw[pp]);
		InstrSetRbw(freq_rbw[pp]);
	}
	//------------------------write by san-------------------
	//fsp仪表不支持模拟FFT模式，sweeptime必须设置为auto
	//------------------------write by san-------------------
	if (Util::strFind(_strIDN, "FSV"))
		InstrSetSweepTime(sweep_time[pp]);
	else
		InstrSetSweepTime(0);

	if (pp == preset_calibration)
		InstrSetAtt(m_offset_att < 0 ? input_att[pp] : m_offset_att);
	else
		InstrSetAtt(m_pim_att < 0 ? input_att[pp] : m_pim_att);

	InstrSetAvg(freq_aver[pp]);
	//------------------------write by san-------------------
	//ref level需放最后执行！
	//------------------------write by san-------------------
	InstrSetRef(disp_rlev[pp]);

	//以下条命令必须执行
	AgWrite("INIT:CONT OFF\n");
}

bool ClsAnaRsFspSerial::CommonSet(char const *command, ...)
{
    
    char command_data[1024] = {0};
    
    va_list ap;
    va_start(ap, command);
    vsnprintf_s(command_data, 1023, command, ap);
    va_end(ap);
    
    //cout<<command_data<<endl;
    
    return InstrWrite(command_data);
}

//------------------------write by san-------------------
void ClsAnaRsFspSerial::InstrPimSetting() {
	InstrSetPreamp(true);
	Preset(preset_parameter::preset_default);
}

void ClsAnaRsFspSerial::InstrVcoSetting() {
	InstrSetPreamp(true);
	Preset(preset_parameter::preset_mensuration);
}

void ClsAnaRsFspSerial::InstrTxOffsetSetting() {
	InstrSetPreamp(false);
	Preset(preset_parameter::preset_calibration);
}

void ClsAnaRsFspSerial::InstrRxOffsetSetting() {
	InstrPimSetting();
}

void ClsAnaRsFspSerial::InstrSetModeAtt(int pim_att, int offset_att) {
	m_pim_att = pim_att;
	m_offset_att = offset_att;
}
//------------------------write by san-------------------

