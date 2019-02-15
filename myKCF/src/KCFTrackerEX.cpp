#include "KCFTrackerEX.h"
#define  KCF_MIN_POINT	0 
using namespace std;
using namespace cv;
KCFTrackerEx::KCFTrackerEx()
{
 	m_bHog = true;
	m_bFixedWindow = true;
	m_bMultiScale = true;
	m_bLab = false;
	m_fKCFLevel = 0.3;
	memset(m_stKCFTrackerInfos, 0x00, sizeof(m_stKCFTrackerInfos));	 
 	for (int i = 0; i < KCF_MAX_NUM; i ++)
 	{
 		m_pKCFTrackers[i] = new KCFTracker(m_bHog, m_bFixedWindow, m_bMultiScale, m_bLab);
 	}	
}

KCFTrackerEx::~KCFTrackerEx()
{
 	for (int i = 0; i < KCF_MAX_NUM; i ++)
 	{
 		delete m_pKCFTrackers[i];
 	}	
	//m_mFDMatImg.release();
}
 
int KCFTrackerEx::KCFTrackerUpdate(Mat p_mtImageMat)
{
	Rect 	l_rtTrackRet;
	float 	l_fPeakValue = 0;
	//dzlog_debug ("-> KCFTrackerUpdate()\n");

	//m_mFDMatImg = p_mtImageMat;
	m_mFDMatImg = p_mtImageMat.clone();

	for (int i = 0; i < KCF_MAX_NUM; i++)
	{
		l_fPeakValue = 0;

		//cout << "iHandel = " << m_stKCFTrackerInfos[i].iHandel << endl;
		if (m_stKCFTrackerInfos[i].iHandel != 0)
		{
			long 	l_lCurrTime = getTickCount();
			double  d_lTimeout = (l_lCurrTime - m_stKCFTrackerInfos[i].lTimeout)/getTickFrequency();
			if (d_lTimeout > 30)		// 30s
			{
				m_stKCFTrackerInfos[i].iHandel = 0;
				m_stKCFTrackerInfos[i].lTimeout = 0;
				printf("AntionChan: %02d tracker timeout\n", i);
				continue;
			}
			l_rtTrackRet = m_pKCFTrackers[i]->update(p_mtImageMat, l_fPeakValue);
			cout << "l_fPeakValue = " << l_fPeakValue << endl;

			if (l_fPeakValue < m_fKCFLevel)
			{
				m_stKCFTrackerInfos[i].iHandel = 0;

				printf("AntionChan: %02d tracker lost the target, peak=%f\n", i, l_fPeakValue);
			}
			else 
			{
				m_stKCFTrackerInfos[i].fPeak = l_fPeakValue;

				m_stKCFTrackerInfos[i].iCenterX = l_rtTrackRet.x + l_rtTrackRet.width / 2;
				m_stKCFTrackerInfos[i].iCenterY = l_rtTrackRet.y + l_rtTrackRet.height / 2;
				printf("AntionChan: %02d tracker update(%d, %d, %f)\n", i, m_stKCFTrackerInfos[i].iCenterX, m_stKCFTrackerInfos[i].iCenterY, l_fPeakValue);
			}
		}
	}
	printf("<- KCFTrackerUpdate(0)\n");
	return 0;
  }

  int KCFTrackerEx::iKCFTrackerCheck(int x, int y, int p_iWidth, int p_iHeight)
  {
	  int	  	l_iIndex = -1;
	  int	  	l_iTrackerID = 0;
	  int		l_iCenterX = 0;
	  int		l_iCenterY = 0;
	  int		l_iStander = 0;
	  printf("AntionChan: -> iKCFTrackerCheck(%d, %d, %d, %d)\n", x, y, p_iWidth, p_iHeight);
	  l_iCenterX = x + p_iWidth/2;
	  l_iCenterY = y + p_iHeight/2;
	  l_iStander = p_iWidth / 2;
	  Rect 	l_oRect;
	  l_oRect.x = x;
  	  l_oRect.y = y;
  	  l_oRect.width = p_iWidth;
  	  l_oRect.height = p_iHeight;
	  Mat		    l_mRoi(m_mFDMatImg, l_oRect);
	  char 			l_acFilePath[256] = {0};
	  for (int i = 0; i < KCF_MAX_NUM; i ++)
	  {
		  if (abs(l_iCenterX - m_stKCFTrackerInfos[i].iCenterX) < l_iStander &&
			  abs(l_iCenterY - m_stKCFTrackerInfos[i].iCenterY) < l_iStander &&
			  m_stKCFTrackerInfos[i].iHandel != 0)
			  {
				  l_iIndex = i;
				  //break;
				  printf("AntionChan: <- iKCFTrackerCheck(1), Target[%d, %d, %d, %d] is in the list\n", x, y, p_iWidth, p_iHeight);
				  //sprintf (l_acFilePath, "acPic/List00Frame%02d-%d0000%d.jpg", p_iPts, getTickCount(), l_iIndex);
				  //imwrite(l_acFilePath, l_mRoi);
				  //l_mRoi.release();
				  return 1;
			  }
	  }
	  l_iTrackerID = iKCFTrackerFind();
	cout << "l_iTrackerID = " << l_iTrackerID << endl;
	  if (l_iTrackerID >= 0)
	  {
			if (((l_oRect.x + l_oRect.width) > (m_mFDMatImg.cols - KCF_MIN_POINT)) || (l_oRect.x < KCF_MIN_POINT) || ((l_oRect.y + l_oRect.height) > (m_mFDMatImg.rows - KCF_MIN_POINT)) || (l_oRect.y < KCF_MIN_POINT))
			{
				printf("AntionChan: What is going on?\n");
				//l_mRoi.release();
				return 0;
			}
			m_pKCFTrackers[l_iTrackerID]->init(l_oRect, m_mFDMatImg);
			cout << "Enter -------------- Enter" << endl;
			m_stKCFTrackerInfos[l_iTrackerID].iHandel = 1;
			m_stKCFTrackerInfos[l_iTrackerID].lTimeout = getTickCount();
			//sprintf (l_acFilePath, "acPic/New00Frame%02d-%d0000%d.jpg", p_iPts, getTickCount(), l_iTrackerID);
			//imwrite(l_acFilePath, l_mRoi);
			//dzlog_debug ("AntionChan: %d: 1 Initial Bounding face_rect = x:%d y:%d w:%d h:%d\n", l_iTrackerID, l_oRect.x, l_oRect.y, l_oRect.width, l_oRect.height);
	  } 

	  printf("AntionChan: <- iKCFTrackerCheck(0)\n");
	  //l_mRoi.release();
	  return 0;
}

int KCFTrackerEx::iKCFTrackerFind(void)
{
	int	l_iIndex = -1;
   
	for (int i = 0; i < KCF_MAX_NUM; i++)
	{
		if (m_stKCFTrackerInfos[i].iHandel == 0)
		{
			l_iIndex = i;
			break;
		}
	}

	printf("AntionChan: <- iKCFTrackerFind(l_iIndex:%d)\n", l_iIndex);
	
	return l_iIndex;
   }

int KCFTrackerEx::iKCFTrackerClear(void)
{
	  int	  l_iRet = 0;

	  for (int i = 0; i < KCF_MAX_NUM; i ++)
	  {
		   m_stKCFTrackerInfos[i].iHandel =0;
	  }

	  //dzlog_debug ("AntionChan: <- iKCFTrackerClear()\n");
	  
	  ////m_mFDMatImg.release();
	  return l_iRet;
}
