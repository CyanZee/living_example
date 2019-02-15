/*************************************************************************************
 *
 * Name: KCFTrackerEX.h
 *
 * Desc: 对KCF目标跟踪算法的二次封�?
 *
 * Auth: AntionChan, 07Aug2018
 *
 **************************************************************************************/
 #ifndef __ANTIONCHAN_20180807_KCFTRACKEREX_HEADER__
 #define __ANTIONCHAN_20180807_KCFTRACKEREX_HEADER__

 #include "kcftracker.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"

 
 using namespace cv;


 #define KCF_MAX_NUM		50


 typedef struct
 {
	int		iIndex;
	int 	iHandel;
	int		iCenterX;
	int		iCenterY;
	float	fPeak;
	long	lTimeout;
 }stKCFTrackerInfo;
 
 
 class KCFTrackerEx
 {
 public:
 	bool			m_bHog;
	bool			m_bFixedWindow;
	bool 			m_bMultiScale;
	bool			m_bLab;
	float			m_fKCFLevel;

	Mat					m_mFDMatImg;
 	KCFTracker*  		m_pKCFTrackers[KCF_MAX_NUM];
 	stKCFTrackerInfo	m_stKCFTrackerInfos[KCF_MAX_NUM];
	
 public:
 	KCFTrackerEx();
	~KCFTrackerEx();
	
	int KCFTrackerUpdate(Mat p_mtImageMat);
	int iKCFTrackerCheck(int x, int y, int p_iWidth, int p_iHeight);
	int iKCFTrackerFind(void);
	int iKCFTrackerClear(void);
 };


 #endif //__ANTIONCHAN_20180807_KCFTRACKEREX_HEADER__

