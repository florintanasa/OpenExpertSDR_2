/*
 * This file is part of ExpertSDR
 *
 * ExpertSDR is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * ExpertSDR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 *
 *
 * Copyright (C) 2012 Valery Mikhaylovsky
 * The authors can be reached by email at maksimus1210@gmail.com
 */

#ifndef EXPERTSDR_VA2_1_H
#define EXPERTSDR_VA2_1_H

#include <search.h>
#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QtWidgets>
#include <QtWidgets/QWidget>
#include <QtConcurrent/qtconcurrentrun.h>
#else
#include <QtGui>
#include <QtGui/QWidget>
#include <qtconcurrentrun.h>
#endif

#include <QMainWindow>
#include <QActionGroup>
#include <QList>
#include <QMenu>

#include "FreqScale/Didgit.h"
#include "Options.h"
#include "DttSP.h"
#include "WdgGraph.h"
#include "WdgMem.h"
#include "About.h"
#include "Defines.h"
#include "FreqScale/Didgit.h"
#include "SdrPlugin/SdrPlugin.h"
#include "SdrPlugin/PortAudio/pa19.h"
#ifdef Q_OS_WIN
#include "Valcoder/Panel.h"
#include "qt_windows.h"
#endif
#include "CalibrateSC/progresscalibrate.h"
#include "CalibrateSC/Calibrator/Calibrator.h"
#include "Cat/CatManager.h"
#include "CalibrateSC/progresscalibrate.h"
#include "SmeterGUI/SMeterSDR.h"
#include "S-Meter/S_Meter.h"
#include "Vac.h"
#include "CW/CwMacro.h"
#include "scale/Scale.h"
#include "AudioThread.h"
#include "CalibrateSC/Calibrator/Calibrator.h"
#include "Wav/VoiceRecorderWave.h"
#include "Timer/Timer.h"
#include "bands/BandManager.h"
#include "PanaramDefines.h"
#include "ui_expertsdr_va2_1.h"

using namespace QtConcurrent;

#define GetCurrentPos(Pos, CurrentPos)	((Pos < qAbs(CurrentPos)) ? (MAX_DSP_MEMORY_BUF + Pos + CurrentPos) : (Pos + CurrentPos))

#define TX_ZERO_IN_BUFS			2
#define TX_ZERO_BUFS			2

#define MAX_FREQ_MEMORIES		16
#define NOMINAL_OUT_VOLTAGE 	1.3

#define NUMS_SAMPLES_FOR_MEANE	21

#define MUTE_TX_RX 5

typedef enum {
	CH2 = 0,
	CH4 = 1,
	CH2x2 = 2
} CbFormat;

class QextSerialPort;
class CatManager;
class QThread;
class AudioThread;

void VacCallBack(const void *In, void *Out, unsigned long FrameCount, void *UserData);

//namespace Ui {
//class ExpertSDR_vA2_1;
//}

class ExpertSDR_vA2_1 : public QWidget
{
	Q_OBJECT

	friend void VacCallBack(const void *In, void *Out, unsigned long FrameCount, void *UserData);
	friend class AudioThread;
	friend class Vac;
	friend class CatManager;

	CatManager *pCatManager;
	ProgressCalibrate *pCalibrateSC;

	int SaveDDS;
	int SaveFilter;
	bool RxMuteStatus;

	Timer timerClickMem;
	Timer timerClickMode;
	Timer timerClickFilter;
	Timer timerChangePanelIF;
	Timer timerChangePanelFilterWidth;
	Timer timerChangePanelFilterShift;
	Timer timerChangePanelFilterLow;
	Timer timerChangePanelFilterHigh;

	QVector<float> vecRamp;
	bool IsRampRelay;
	QVector<float> vecRampDelay;
	int cntSwitchTxRx;
	int cntSwitchRxTx;
	bool  saveStateBrakeIn;
	bool isGenTx;
	bool isStarted;

	public:
		ExpertSDR_vA2_1(QWidget *parent = 0);
		~ExpertSDR_vA2_1();
		void audioCallBack(float **In, float **Out, unsigned long FrameCount, CbFormat format);
		CbFormat getAudioCardMode();
	private:

		int flagLock;
		void setLock(int flags);
		int getLock() {return flagLock;};
		bool writeSettings();
		void readSettings();
		void closeEvent(QCloseEvent *event);
		void resizeEvent(QResizeEvent *event);
		void showEvent(QShowEvent *event);
		void timerEvent(QTimerEvent *);

		int MainStep();
		int MainMod();
		int MainAGC();
		int MainSmeter();

		void calcVectorRamp(int samplerate, int silenceTime, int rampTime);

		int FreqNum;

   //     Ui::ExpertSDR_vA2_1 *ui;
        Ui::ExpertSDR_vA2_1Class ui;
		Vac	*pVac;
		AudioThread *pAudThr;
		int rxGlCnt;
		bool CalibrateSoundCard;
		double PhaseAccumulator;
		double PhaseAccumulator2;
		int RxInputSignal;
		double RxFreqSignal;
		int TxInputSignal;
		double MicScale;
		double DriveScale;
		double LineInGainScale;
		double MicGainScale;
		double ScaleOutBuffSCard;
		bool bSwapLineIn;
		bool bSwapLineOut;
		TRXMODE TrxMode;
		bool StateTone;
		bool DspMute;
		int indexAgc;
		Scale	*pVfoB;
		bool    VacEnable;
		pa19	*pPaVac;
#ifdef Q_OS_WIN
		bool winEvent(MSG *msg, long *result);
        Panel *pPanel;
#endif
		int  UpdatesTimerID;
		QAbstractEventDispatcher * m_EvtDispatch;
		QextSerialPort 	*pPort;
		S_Meter 		*pSM;
		QButtonGroup 	*pTxSplitPb;
		BandManager bandManager;
		int			FilterPosFreq;
		QTimer *pTimerTxOutKeyer;
		Options *pOpt;
		About   *pAbout;
		QTimer  *pTimer;
		DttSP   *pDsp;
		SdrPlugin *pSdrCtrl;
		SMeterSDR  *pSmeter;
		Calibrator *pCalibrator;
		QSize MainWindowSize;
		QPoint MainWindowPos;
		float RxFilterHigh;
		float RxFilterLow;
		float CalibrateLevel[5];
		QByteArray splitOpt;
		float smeter2mCorr;
		float smeter07mCorr;
		QButtonGroup *pModeBut;
		QButtonGroup *pBandBut;
		QButtonGroup *pFiltBut;
		QButtonGroup *pEqBut;
		WdgGraph  	 *pGraph;
		WdgMem		 *pMem;
		// voice recorder
		VoiceRecorder *pVoiceRec;
		QMenu *pMenuPreamp;
		QActionGroup *pAgPreamp;
		QMenu *pMenuAgc;
		QActionGroup *pAgAgc;
		QMenu *pMenuStep;
		QActionGroup *pAgStep;
		QMenu *pMenuFreqUpDown;
		QActionGroup *pFupDownAct;
		CwMacro *pCwMacro;
		qreal Volume;
		qreal MonVolume;
		int glitchCoeff;
		double panRxOffset;
		double panRxScale;
		double panTxOffset;
		double panTxScale;
		QTimer *pMnFudTimer;
		QSplitter *Splitter;
		bool IsUtcTime;
		bool EqTxOn;
		bool EqRxOn;
		bool EqTxMode;
		Didgit *pDg0;
		Didgit *pDg1;
		Didgit *pDg2;
		Didgit *pDg3;
		Didgit *pDg4;
		Didgit *pDg5;
		Didgit *pDg6;
		Didgit *pDg7;
		Didgit *pDg8;
		Didgit *pDg9;
		int FreqMin;
		int FreqMax;
		void SetFreq(int Freq);
		void SetFreqMin(int Value);
		void SetFreqMax(int Value);
		void SetScaleNum(long int Num);
		int  LoadNum();
		void LoadChangeNum();
		void MnAddNewFreq(int Freq, int FilterFreq);
		int  MnMainFreq;
		bool TxMode;
		double RxFilterBandLow;
		double RxFilterBandHigh;
		void MainFreqChange(int Freq);
		double CalibrateFreq;
		int  saveVfoA;
		int  saveFilter;
		int  saveFiltId;
		int saveMode;
		int savePreamp;
		int saveWinType;
		int saveDdsFreq;
		int saveSampleRate;
		int saveVolume;
		int numBandVfoB;
		int vfoBVal[3];
		SDRMODE sdrMode;

	private slots:
		void ForceStop();
		void calcRampDelay(int value);
		void setRampDelayEnable(bool state);
		void changedVfoBUp();
		void changedVfoBDown();
		void defaultSettings();
		void defaultCalibrations();
		void onOptionsApply();
		void OnLoad();
		void OnSaveAs();
		void OnLoadCalibration();
		void OnSaveAsCalibration();
		void OnStartCalibration();
		void OnAmCarrier(int Val);
		void paramForCalibration();
		void ChangeValcoder(int val);
		void ChangeRxInputSignal(int index);
		void ChangeTxInputSignal(int index);
		void ChangeRxFreqSignal(int val);
		void onBreakInDelay(int val);
		void onBreakIn(int en);
		void onSwapKeys(int en);
		void onKeyAutoMode(int en);
		void onCatTx();
		void onCatRx();
		void SetLockMode(bool state);
		void SetLockBand(bool state);
		void SetLockFilters(bool state);
		void SetEnableFreqPanel(bool state);
		void ChangedCalibrateNum(double Num);
		void SetMemUpFreq();
		void SetMemDownFreq();
		void calibrateSCard();
		void endCalibrateCard();
		void ChangedScardUotVoltage(double val);
		void ChangedTimeMachine(int value);
		void ChangeSwapLineIn(int val);
		void ChangeSwapLineOut(int val);
		void SMeterClosed();
		void onSm2mCorr(double val);
		void onSm07mCorr(double val);
		void IsKeyTrue(bool State);
		void OnTune();
		void ChangedCWMon(int State);
		void ChangedCWMon(bool State);
		void onPaVacDriverChanged(int Index);
		void onCwSpeed(int speed);
		void adcMeters(int Ufwd, int Uref);
		void onXvtrxEnable(bool enable);
		void onCalibrationGen(bool status);
		void setLockIsRec(bool state);
		void setLockIsPlay(bool state);
		void setIsPause(bool state);
		void changedPlayFile(bool state);
		void setWavePath(QString path);
		void onPlayVoice(bool state);
		void onRecVoice(bool state);
		void setSunSDRState(QString ms);
		void pushState();
		void popState();
		void SetUhfOsc(double MHz);
		void SetVhfOsc(double MHz);
		void OnSoundOptChanged();
		void OnShowSM(bool IsShow);
		void OnUpdateDataTime();
		void OnShowFullScreen(bool Full);
		void OnClose();
		void OnLpanel(bool Status);
		void OnMemPanel(bool Status);
		void OnOptions();
		void OnAbout();
		void OnTimeClick();
		void onCwMacro();
		void OnVacRxGain(int Val);
		void OnVacTxGain(int Val);
		void OnPbVacEn(bool Stat);
		void onChangeDigLSb(int val);
		void onChangeDigUSb(int val);
		void OnPaOn(bool Status);
		void OnPreampChanged(QAction * Action);
		void OnAgcChanged(QAction * Action);
		void OnStepChanged(QAction * Action);
		void OnStepChanged(int val);
		void OnChangeMode(int Mode);
		void OnChangeBand(int mode);
		void OnChangeFilter(int Filter);
		void OnEqOn(bool IsOn);
		void OnEqModeChanged(int Number);
		void OnEqTxChanged(int Value);
		void OnEqRxChanged(int Value);
		void OnDspEq(bool);
		void OnSql(bool State);
		void OnSql(int Value);
		void OnGate(bool State);
		void OnGate(int Value);
		void OnComp(bool State);
		void OnComp(int Value);
		void OnCpdr(bool State);
		void OnCpdr(int Value);
		void OnPitch(int Val);
		void OnNr(bool Val);
		void OnAnf(bool Val);
		void OnLock(bool Val);
		void OnNb(bool Val);
		void OnNb2(bool Val);
		void OnBin(bool Val);
		void OnStart(bool Start);
		void OnSetPolyphase(int Status);
		void OnMox(bool Tx);
		void SetSdrTrx(bool Tx);
		void SetDspTrx(bool Tx);
		void OnMute(bool State);
		void OnVolume(int Val);
		void OnMonVolume(int Val);
		void OnAgc(int Val);
		void OnDrive(int Val);
		void OnMic(int Val);
		void OnLineInGain(int Val);
		void OnMicInGain(int Val);
		void SetTxFilter();
		void ExControl(int Band, TRXMODE Mode);
		void onXvAntSwitch(int num);
		void OnChangeFilterFreq(int Freq);
		void LowDig0();
		void LowDig1();
		void LowDig2();
		void LowDig3();
		void LowDig4();
		void LowDig5();
		void LowDig6();
		void LowDig7();
		void LowDig8();
		void LowDig9();
		void HighDig0();
		void HighDig1();
		void HighDig2();
		void HighDig3();
		void HighDig4();
		void HighDig5();
		void HighDig6();
		void HighDig7();
		void HighDig8();
		void HighDig9();
		void OnChangeTxGainRej(double Rej);
		void OnChangeTxPhaseRej(double Rej);
		void OnChangeTxGainRejCw(double Rej);
		void OnChangeTxPhaseRejCw(double Rej);
		void OnMnNext();
		void OnMnPrev();
		void OnMenuFreqUpDownChanged(QAction* pAct);
		void OnMnFudTimeout();
		void OnChangeMainFreq(int Freq);
		void OnChangeStationFreq(int Freq);
		void OnChangeMainFreqWithWdgMem(int Freq, int Mode);
		void OnChangeLowFilterFreq(int FLow);
		void OnChangeHighFilterFreq(int Fhigh);
		void OnSbTxFilterChanged(int Param);
		void OnTimeChange();
		void OnTimePlus();
		void OnTimeMinus();
		void OnTimeUp();
		void OnPanChangeBand(int Val);
		void OnPanChangeMode(int Val);
		void OnPanChangeFilter(int Val);
		void OnPanChangeAgc(int Val);
		void OnPanChangePreamp(int Val);
		void OnPanChangeStep(int Val);
		void OnPanChangeBin();
		void OnPanChangeNr();
		void OnPanChangeAnf();
		void OnPanChangeNb();
		void OnPanChangeNb2();
		void OnPanChangeLock();
		void OnPanChangeMemory(int val);
		void OnPanChangeSqlState();
		void OnPanChangePa();
		void OnPanChangeStation(int val);
		void OnChangeBandFilter(int value);
		void OnPanChangeMute();
		void OnPanChangeStart();
		void OnPanVolume(int Val);
		void OnPanMicGain(int Val);
		void OnPanSquelch(int Val);
		void OnPanFilterHigh(int Val);
		void OnPanFilterLow(int Val);
		void OnPanFilterWidth(int Val);
		void OnPanFilterShift(int Val);
		void OnPanDrive(int Val);
		void OnPanIF(int Val);
		void OnPanRfGain(int Val);
		void OnPanZoom(int Val);
		void OnIF(int Val);
		void OnChangePreamp(int Val);
		void OnOptChangeNrVals(int val);
		void OnOptChangeAgcVals(int val);
		void OnOptChangeNbsVals(int val);
		void OnOptChangeAnfVals(int val);
		void OnOptChangeSampleRate(int index);
		void OnOptChangeDspBufLen(int index);
		void OnOptAgcSlope(int val);
		void OnOptAgcAttak(int val);
		void OnOptAgcDecay(int val);
		void OnOptAgcHang(int val);
		void OnOptAgcHangThr(int val);
		void OnOptChangeWindow(int Window);
		void OnSplit(bool stat);
		void OnAtoB();
		void OnAfromB();
		void OnAswapB();
		void OnRx2(bool stat);
		void OnFilter2Hide(bool stat);
		void OnChangeFilter2Freq(int Freq);
		void OnVfoBChanged(int Freq);
		void OnSwitchVfo(int VfoNum);
		void OnBal1();
		void OnBal2();
		void freqDbm();
		void setCalibrFreqCoeff(float val);
		void getSmeter();
		void setCalibrDbmCoeff(int num, float val);
		void resetRitValue();
		void setUpdateTime(int ms);
		void setRxEnable(bool state);
		void OnNewTune(long freq);
		void OnNewDDS(long freq);

	signals:
		void emitFreqDbm(int freq, float dbm);
		void emitDbm(float val);
		void TuneChanged(int Freq);
		void ModeChanged(int mode);
		void SoundCardSampleRateChanged(int rate);
};

#endif // EXPERTSDR_VA2_1_H
