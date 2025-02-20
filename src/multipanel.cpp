﻿// ****** multipanel.cpp **********
// ****  William R. Good  ********


#include "XPLMUtilities.h"
#include "XPLMDataAccess.h"

#include "saitekpanels.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <cmath>
#include <algorithm>

#define testbit(x, y)  ( ( ((const char*)&(x))[(y)>>3] & 0x80 >> ((y)&0x07)) >> (7-((y)&0x07) ) )

// ********************** Multi Panel variables ***********************

int AttrSwitch_1_DataOnValue, AttrSwitch_1_DataOffValue;
int AttrSwitch_2_DataOnValue, AttrSwitch_2_DataOffValue;
int AttrSwitch_3_DataOnValue, AttrSwitch_3_DataOffValue;
int AttrSwitch_4_DataOnValue, AttrSwitch_4_DataOffValue;
int AttrSwitch_5_DataOnValue, AttrSwitch_5_DataOffValue;
int AttrSwitch_6_DataOnValue, AttrSwitch_6_DataOffValue;
int AttrSwitch_7_DataOnValue, AttrSwitch_7_DataOffValue;
int AttrSwitch_8_DataOnValue, AttrSwitch_8_DataOffValue;
int AttrSwitch_9_DataOnValue, AttrSwitch_9_DataOffValue;
int AttrSwitch_10_DataOnValue, AttrSwitch_10_DataOffValue;
int AttrSwitch_11_DataOnValue, AttrSwitch_11_DataOffValue;
int AttrSwitch_12_DataOnValue, AttrSwitch_12_DataOffValue;
int AttrSwitch_13_DataOnValue, AttrSwitch_13_DataOffValue;
int AttrSwitch_14_DataOnValue, AttrSwitch_14_DataOffValue;
int AttrSwitch_15_DataOnValue, AttrSwitch_15_DataOffValue;
int AttrSwitch_16_DataOnValue, AttrSwitch_16_DataOffValue;

static int multinowrite = 0, lastmultiseldis = 0;
static int mulres, multires;

static int multimul = 10;

static int n = 5;

static int lastappos = 0, lastappos2 = 0;
//static int multitestloop = 0;


static int upapalt, upapvs, upaphdg, upapcrs, upapcrs2, neg;
static int apas, apasout, apmas;

static int flashcnt = 0, flashon = 0;

static int altdbncinc = 0, altdbncdec = 0, vsdbncinc = 0, vsdbncdec = 0;
static int iasdbncinc = 0, iasdbncdec = 0, hdgdbncinc = 0, hdgdbncdec = 0;
static int crsdbncinc = 0, crsdbncdec = 0; 

static int altbiginc = 0, altbigdec = 0, vsbiginc = 0, vsbigdec = 0;
static int iasbiginc = 0, iasbigdec = 0, hdgbiginc = 0, hdgbigdec = 0;
static int crsbiginc = 0, crsbigdec = 0;

static float upapaltf, upapvsf, upaphdgf, upapcrsf, upapcrsf2, rhdgf;
static float apasf, apmasf;

static int multiaactv, multiadig1, multiarem1, multiadig2, multiarem2;
static int multiadig3, multiarem3, multiadig4, multiarem4, multiadig5;
static int multibstby, multibdig1, multibdig2, multibrem2;
static int multibdig3, multibrem3, multibdig4, multibrem4, multibdig5;  
static int btnleds = 0, lastbtnleds = 0, multiseldis = 1, multionceonly = 0;
static int lastmultiaactv, lastmultibstby;

static int ALT_SWITCH = 7, VS_SWITCH = 6;
static int IAS_SWITCH = 5, HDG_SWITCH = 4;
static int CRS_SWITCH = 3, AUTO_THROTTLE_SWITCH = 8;
static int AP_MASTER_BUTTON = 0, HDG_BUTTON = 15;
static int NAV_BUTTON = 14, IAS_BUTTON = 13;
static int ALT_BUTTON = 12, VS_BUTTON = 11;
static int APR_BUTTON = 10, REV_BUTTON = 9;
static int FLAPS_UP_SWITCH = 23, FLAPS_DN_SWITCH = 22;
static int TRIM_WHEEL_UP = 20, TRIM_WHEEL_DN = 21;
static int ADJUSTMENT_UP = 2, ADJUSTMENT_DN = 1;

static int Last_Adjustment_Up, Last_Adjustment_Dn;


float MultiAltKnobSpeedThreshold = 0.40f;  // Steve Bootes  Bill Good: speed thresholds for alt knob acceleration
float MultiVsKnobSpeedThreshold =  0.40f;  // Steve Bootes Bill Good: speed thresholds for vs knob acceleration
float MultiIasKnobSpeedThreshold =  0.40f;  // Steve Bootes Bill Good: speed thresholds for ias knob acceleration
float MultiHdgKnobSpeedThreshold =  0.40f;  // Steve Bootes Bill Good: speed thresholds for hdg knob acceleration
float MultiCrsKnobSpeedThreshold =  0.40f;  // Steve Bootes Bill Good: speed thresholds for crs knob acceleration


static unsigned char multibuf[4];
static unsigned char multiwbuf[13];


void process_multi_display()
{
  switch(multiseldis){
    case 1:
    // ***** Setup Display for ALT or VS Switch Position *********
      multiaactv = upapalt;
      multiadig1 = multiaactv/10000, multiarem1 = multiaactv%10000;
      multiadig2 = multiarem1 /1000, multiarem2 = multiarem1%1000;
      multiadig3 = multiarem2/100, multiarem3 = multiarem2%100;
      multiadig4 = multiarem3/10, multiarem4 = multiarem3%10;
      multiadig5 = multiarem4;
      multibstby = upapvs;
      
      if (neg == 1) {
        multibdig1 = 254;
      } else {
        multibdig1 = 15;
      }
      
      multibdig2 = multibstby /1000, multibrem2 = multibstby%1000;
      multibdig3 = multibrem2/100, multibrem3 = multibrem2%100;
      multibdig4 = multibrem3/10, multibrem4 = multibrem3%10;
      multibdig5 = multibrem4;
      break;
    case 2:
    // ***** Setup Display for IAS Switch Position *********
      if (XPLMGetDatai(AirspeedIsMach) == 1) {
          multiaactv = apasout;
          multiadig1 = 15, multiadig2 = 15 ;
          multiadig3 = multiaactv/100, multiarem3 = multiaactv%100;
          multiadig4 = multiarem3/10, multiarem4 = multiarem3%10;
          multiadig5 = multiarem4;

      } else if (XPLMGetDatai(IasIsmachRemapableData) == 1) {

          multiaactv = apasout;
          multiadig1 = 15, multiadig2 = 15 ;
          multiadig3 = multiaactv/100, multiarem3 = multiaactv%100;
          multiadig4 = multiarem3/10, multiarem4 = multiarem3%10;
          multiadig5 = multiarem4;

      } else {
          multiaactv = apasout;
          multiadig1 = 15, multiadig2 = 15 ;
          multiadig3 = multiaactv/100, multiarem3 = multiaactv%100;
          multiadig4 = multiarem3/10, multiarem4 = multiarem3%10;
          multiadig5 = multiarem4;
      }

      break;
    case 3:
    // ***** Setup Display for HDG Switch Position *********
      multiaactv = upaphdg;
      multiadig1 = 15, multiadig2 = 15 ;
      multiadig3 = multiaactv/100, multiarem3 = multiaactv%100;
      multiadig4 = multiarem3/10, multiarem4 = multiarem3%10;
      multiadig5 = multiarem4;
      break;
    case 4:
    // ***** Setup Display for CRS Switch Position *********
      multiaactv = upapcrs;
      multiadig1 = 15, multiadig2 = 15 ;
      multiadig3 = multiaactv/100, multiarem3 = multiaactv%100;
      multiadig4 = multiarem3/10, multiarem4 = multiarem3%10;
      multiadig5 = multiarem4;
      break;
    case 5:
    default:
    // ********** Setup the Display to be Blank *******************
      multiadig1 = 15, multiadig2 = 15, multiadig3 = 15, multiadig4 = 15, multiadig5 = 15;
      multibdig1 = 15, multibdig2 = 15, multibdig3 = 15, multibdig4 = 15, multibdig5 = 15;
      btnleds = 0;
      break;
  }
// ****** Make Message One Digit at A Time and Turn on Button LEDS  *******
  char multiadigit1 = multiadig1, multiadigit2 = multiadig2, multiadigit3 = multiadig3;
  char multiadigit4 = multiadig4, multiadigit5 = multiadig5;
  char multibdigit1 = multibdig1, multibdigit2 = multibdig2, multibdigit3 = multibdig3;
  char multibdigit4 = multibdig4, multibdigit5 = multibdig5;	
  char cdigit1 = btnleds; 

// ****** Load Array with Message of Digits and Button LEDS *************
  multiwbuf[0] = 0;
  multiwbuf[1] = multiadigit1, multiwbuf[2] = multiadigit2, multiwbuf[3] = multiadigit3;
  multiwbuf[4] = multiadigit4, multiwbuf[5] = multiadigit5;
  multiwbuf[6] = multibdigit1, multiwbuf[7] = multibdigit2, multiwbuf[8] = multibdigit3;
  multiwbuf[9] = multibdigit4, multiwbuf[10] = multibdigit5, multiwbuf[11] = cdigit1;

}


// ***************** ALT Switch Position *******************
void process_multi_panel_datareference_values()
{
    if(testbit(multibuf,ALT_SWITCH)) {
        XPLMSetDatai(MultiAltSwitchOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiAltSwitchOwnedDataRef, 0);
    }

    if(testbit(multibuf,VS_SWITCH)) {
        XPLMSetDatai(MultiVsSwitchOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiVsSwitchOwnedDataRef, 0);
    }

    if(testbit(multibuf,IAS_SWITCH)) {
        XPLMSetDatai(MultiIasSwitchOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiIasSwitchOwnedDataRef, 0);
    }

    if(testbit(multibuf,HDG_SWITCH)) {
        XPLMSetDatai(MultiHdgSwitchOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiHdgSwitchOwnedDataRef, 0);
    }

    if(testbit(multibuf,CRS_SWITCH)) {
        XPLMSetDatai(MultiCrsSwitchOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiCrsSwitchOwnedDataRef, 0);
    }

    if(testbit(multibuf,ADJUSTMENT_UP)) {
        XPLMSetDatai(MultiKnobIncOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiKnobIncOwnedDataRef, 0);
    }

    if(testbit(multibuf,ADJUSTMENT_DN)) {
        XPLMSetDatai(MultiKnobDecOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiKnobDecOwnedDataRef, 0);
    }

    if(testbit(multibuf,AUTO_THROTTLE_SWITCH)) {
        XPLMSetDatai(MultiAtOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiAtOwnedDataRef, 0);
    }

    if(testbit(multibuf,TRIM_WHEEL_UP)) {
        XPLMSetDatai(MultiTrimUpOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiTrimUpOwnedDataRef, 0);
    }

    if(testbit(multibuf,TRIM_WHEEL_DN)) {
        XPLMSetDatai(MultiTrimDnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiTrimDnOwnedDataRef, 0);
    }

    if(testbit(multibuf,TRIM_WHEEL_UP)) {
        XPLMSetDatai(MultiTrimUpOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiTrimUpOwnedDataRef, 0);
    }

    if(testbit(multibuf,FLAPS_UP_SWITCH)) {
        XPLMSetDatai(MultiFlapsUpOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiFlapsUpOwnedDataRef, 0);
    }

    if(testbit(multibuf,FLAPS_DN_SWITCH)) {
        XPLMSetDatai(MultiFlapsDnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiFlapsDnOwnedDataRef, 0);
    }


    if(testbit(multibuf,AP_MASTER_BUTTON)) {
        XPLMSetDatai(MultiApBtnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiApBtnOwnedDataRef, 0);
    }


    if(testbit(multibuf,HDG_BUTTON)) {
        XPLMSetDatai(MultiHdgBtnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiHdgBtnOwnedDataRef, 0);
    }

    if(testbit(multibuf,NAV_BUTTON)) {
        XPLMSetDatai(MultiNavBtnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiNavBtnOwnedDataRef, 0);
    }

    if(testbit(multibuf,IAS_BUTTON)) {
        XPLMSetDatai(MultiIasBtnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiIasBtnOwnedDataRef, 0);
    }

    if(testbit(multibuf,ALT_BUTTON)) {
        XPLMSetDatai(MultiAltBtnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiAltBtnOwnedDataRef, 0);
    }

    if(testbit(multibuf,VS_BUTTON)) {
        XPLMSetDatai(MultiVsBtnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiVsBtnOwnedDataRef, 0);
    }

    if(testbit(multibuf,APR_BUTTON)) {
        XPLMSetDatai(MultiAprBtnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiAprBtnOwnedDataRef, 0);
    }

    if(testbit(multibuf,REV_BUTTON)) {
        XPLMSetDatai(MultiRevBtnOwnedDataRef, 1);
    } else {
        XPLMSetDatai(MultiRevBtnOwnedDataRef, 0);
    }

    int adjustment_up = XPLMGetDatai(MultiKnobIncOwnedDataRef);
    if (Last_Adjustment_Up != 0 && adjustment_up == 0)
    {
      XPLMSetDatai(MultiKnobIncTicksOwnedDataRef, XPLMGetDatai(MultiKnobIncTicksOwnedDataRef) + 1);
    }
    int adjustment_dn = XPLMGetDatai(MultiKnobDecOwnedDataRef);
    if (Last_Adjustment_Dn != 0 && adjustment_dn == 0)
    {
      XPLMSetDatai(MultiKnobDecTicksOwnedDataRef, XPLMGetDatai(MultiKnobDecTicksOwnedDataRef) + 1);
    }
}

// ***************** ALT Switch Position *******************
void process_alt_switch()
{
    if(testbit(multibuf,ALT_SWITCH)) {
        altbiginc = 0;
        altbigdec = 0;
        MultiAltKnobSpeedThreshold = (float)(multiaccelthreshold * .01);

        if (!AvPwrIsOn()) {
            multiseldis = 5;
        } else {
           multiseldis = 1;
        }

        if ((altswitchremap == 2) | (altswitchremap == 1) | (altswitchremap == 3)) {
            upapaltf = XPLMGetDataf(AltSwitchRemapableData);
        } else {
            upapaltf = XPLMGetDataf(ApAlt);
        }
        upapalt = (int)(upapaltf);
        if((Last_Adjustment_Up == 1) && (testbit(multibuf,ADJUSTMENT_UP) == 0)) {
            altdbncinc++;
			if (altdbncinc > multispeed) {
                n = multimul;
                MultiKnobLastCurrentUpTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentUpTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentUpTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentUpTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "Xsaitekpanels:MultiKnobLastCurrentUpTimeDiff = %f MultiAltKnobSpeedThreshold = %f\n",MultiKnobLastCurrentUpTimeDiff, MultiAltKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }
                if (((xpanelsfnbutton == 1) && (altswitchremap != 3)) || (MultiKnobLastCurrentUpTimeDiff < MultiAltKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time
					if (altswitchremap == 1) {
                        float tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                        if (log_enable == 1) {
                            tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                            sprintf(buf, "Xsaitekpanels: tmp_alt_remap_dataref = %f\n", tmp_alt_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(AltSwitchUpRemapableCmd);
                        tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                        tmp_alt_remap_dataref = tmp_alt_remap_dataref + 900;
                        XPLMSetDataf(AltSwitchRemapableData, tmp_alt_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(AltSwitchUpRemapableCmd);\n");
                            XPLMDebugString(buf);
                            sprintf(buf, "Xsaitekpanels: tmp_alt_remap_dataref = %f\n\n", tmp_alt_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        altdbncinc = 0;
                        altbiginc = 1;

                    } else {
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: upapalt = %d ", upapalt);
                            XPLMDebugString(buf);
                        }
                        upapalt = upapalt + 1000;
                        upapalt = (upapalt / 1000);
                        upapalt = (upapalt * 1000);
                        altdbncinc = 0;
                        altbiginc = 1;
                        if (log_enable == 1) {
                            sprintf(buf, "upapalt + 1000 = %d\n\n", upapalt + 1000);
                            XPLMDebugString(buf);
                        }
                    }
                }
                MultiKnobLastCurrentUpTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (altswitchremap == 3)) {
                    if ((altswitchremap == 1) || (altswitchremap == 3)) {
                        if (altbiginc == 0) {
                            float tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                            if (log_enable == 1) {
                                tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                                sprintf(buf, "Xsaitekpanels: tmp_alt_remap_dataref = %f\n", tmp_alt_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(AltSwitchUpRemapableCmd);
                            tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(AltSwitchUpRemapableCmd);\n");
                                XPLMDebugString(buf);
                                sprintf(buf, "Xsaitekpanels: tmp_alt_remap_dataref = %f\n\n", tmp_alt_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            altdbncinc = 0;
                        }

                    } else {
                        if (altbiginc == 0) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: upapalt = %d ", upapalt);
                                XPLMDebugString(buf);
                            }
                            upapalt = upapalt + 100;
                            upapalt = (upapalt / 100);
                            upapalt = (upapalt * 100);
                            altdbncinc = 0;
                            if (log_enable == 1) {
                                sprintf(buf, "upapalt + 100 = %d\n\n", upapalt + 100);
                                XPLMDebugString(buf);
                            }
                        }
                    }
                }
            }
		}
        Last_Adjustment_Up = testbit(multibuf,ADJUSTMENT_UP);
        altbiginc = 0;

        if((Last_Adjustment_Dn == 1) && (testbit(multibuf,ADJUSTMENT_DN) == 0)) {
            altdbncdec++;
			if (altdbncdec > multispeed) {
                n = multimul;
                MultiKnobLastCurrentDnTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentDnTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentDnTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentDnTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "Xsaitekpanels: MultiKnobLastCurrentDnTimeDiff = %f MultiAltKnobSpeedThreshold = %f\n",MultiKnobLastCurrentDnTimeDiff, MultiAltKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }
                if (((xpanelsfnbutton == 1) && (altswitchremap != 3)) || (MultiKnobLastCurrentDnTimeDiff < MultiAltKnobSpeedThreshold)) { // Steve Bootes : add test for MultiKnob rotation time
                    if(altswitchremap == 1) {
                        float tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                        if (log_enable == 1) {
                            tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                            sprintf(buf, "Xsaitekpanels: tmp_alt_remap_dataref = %f\n", tmp_alt_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(AltSwitchDnRemapableCmd);
                        tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                        tmp_alt_remap_dataref = tmp_alt_remap_dataref - 900;
                        XPLMSetDataf(AltSwitchRemapableData, tmp_alt_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(AltSwitchUpRemapableCmd);\n");
                            XPLMDebugString(buf);
                            sprintf(buf, "Xsaitekpanels: tmp_alt_remap_dataref = %f\n\n", tmp_alt_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        altdbncdec = 0;
                        altbigdec = 1;

                    } else {
                        if (upapalt >= 1000) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: upapalt = %d ", upapalt);
                                XPLMDebugString(buf);
                            }
                            upapalt = upapalt - 1000;
                            altbigdec = 1;
                            if (log_enable == 1) {
                                sprintf(buf, "upapalt - 1000 = %d\n\n", upapalt - 1000);
                                XPLMDebugString(buf);
                            }
                        }
                        if(upapalt > 100) {
                            upapalt = (upapalt / 100);
                            upapalt = (upapalt * 100);
                        }
                        altdbncdec = 0;
                    }
                }
                MultiKnobLastCurrentDnTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (altswitchremap == 3)) {
                     if ((altswitchremap == 1) || (altswitchremap == 3)) {
                        if (altbigdec == 0) {
                            float tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                            if (log_enable == 1) {
                                tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                                sprintf(buf, "Xsaitekpanels: tmp_alt_remap_dataref = %f\n", tmp_alt_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(AltSwitchDnRemapableCmd);
                            tmp_alt_remap_dataref = XPLMGetDataf(AltSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(AltSwitchUpRemapableCmd);\n");
                                XPLMDebugString(buf);
                                sprintf(buf, "Xsaitekpanels: tmp_alt_remap_dataref = %f\n\n", tmp_alt_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            altdbncdec = 0;

                        }

                     } else {
                         if (upapalt >= 100) {
                             if (altbigdec == 0) {
                                 if (log_enable == 1) {
                                     sprintf(buf, "Xsaitekpanels: upapalt = %d ", upapalt);
                                     XPLMDebugString(buf);
                                 }
                                 upapalt = upapalt - 100;
                                 if (log_enable == 1) {
                                     sprintf(buf, "upapalt - 100 = %d\n\n", upapalt - 100);
                                     XPLMDebugString(buf);
                                 }
                             }
                         }
                         if(upapalt > 100) {
                             upapalt = (upapalt / 100);
                             upapalt = (upapalt * 100);
                         }
                         altdbncdec = 0;
                     }
                }
            }
		}
        Last_Adjustment_Dn = testbit(multibuf,ADJUSTMENT_DN);
        upapaltf = upapalt;
        altbigdec = 0;

        if ((altswitchremap == 1) || (altswitchremap == 3)) {
            upapaltf = XPLMGetDataf(AltSwitchRemapableData);
        } else if (altswitchremap == 2) {
            XPLMSetDataf(AltSwitchRemapableData, upapaltf);
        } else {
            XPLMSetDataf(ApAlt, upapaltf);
        }

        if ((vsswitchremap == 1) || (vsswitchremap == 3)) {
            upapvsf = XPLMGetDataf(VsSwitchRemapableData);
        } else if (vsswitchremap == 2) {
            upapvsf = XPLMGetDataf(VsSwitchRemapableData);
        } else {
            upapvsf = XPLMGetDataf(ApVs);
        }
        upapvs = (int)(upapvsf);
        if (upapvs < 0) {
            upapvs = (upapvs * -1);
            neg = 1;
        } else {
            neg = 0;
        }

	}
}

// ***************** VS Switch Position *******************
void process_vs_switch()
{
    if(testbit(multibuf,VS_SWITCH)) {
        vsbiginc = 0;
        vsbigdec = 0;
        MultiVsKnobSpeedThreshold = (float)(multiaccelthreshold * .01);
        if (!AvPwrIsOn() || !BatPwrIsOn()) {
            multiseldis = 5;
        } else {
           multiseldis = 1;
        }

        if (vsswitchremap == 2) {
            upapvsf = XPLMGetDataf(VsSwitchRemapableData);
        } else {
            upapvsf = XPLMGetDataf(ApVs);
        }
        upapvs = (int)(upapvsf);
        if((Last_Adjustment_Up == 1) && (testbit(multibuf,ADJUSTMENT_UP) == 0)) {
            vsdbncinc++;
            if (vsdbncinc > multispeed) {
                n = multimul;
                MultiKnobLastCurrentUpTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentUpTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentUpTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentUpTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "MultiKnobLastCurrentUpTimeDiff = %f MultiVsKnobSpeedThreshold = %f\n",MultiKnobLastCurrentUpTimeDiff, MultiVsKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }
                if (((xpanelsfnbutton == 1) && (vsswitchremap != 3)) || (MultiKnobLastCurrentUpTimeDiff < MultiVsKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time)
                    if (vsswitchremap == 1) {
                        float tmp_vs_remap_dataref = XPLMGetDataf(VsSwitchRemapableData);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: tmp_vs_remap_dataref = %f\n", tmp_vs_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(VsSwitchUpRemapableCmd);
                        tmp_vs_remap_dataref = XPLMGetDataf(VsSwitchRemapableData);
                        tmp_vs_remap_dataref = tmp_vs_remap_dataref + 100;
                        XPLMSetDataf(VsSwitchRemapableData, tmp_vs_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(VsSwitchUpRemapableCmd);\n");
                            XPLMDebugString(buf);
                            sprintf(buf, "Xsaitekpanels: tmp_vs_remap_dataref = %f\n\n", tmp_vs_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        vsdbncinc = 0;
                        vsbiginc = 1;

                    } else {
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: upapvs = %d ", upapvs);
                            XPLMDebugString(buf);
                        }
                        upapvs = upapvs + 200;
                        upapvs = (upapvs / 100);
                        upapvs = (upapvs * 100);
                        vsbiginc = 1;
                        if (log_enable == 1) {
                            sprintf(buf, "upapvs + 200 = %d\n\n", upapvs);
                            XPLMDebugString(buf);
                        }
                    }
                    vsdbncinc = 0;

                }
                MultiKnobLastCurrentUpTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (vsswitchremap == 3)) {
                    if ((vsswitchremap == 1) || (vsswitchremap == 3)) {
                        if (vsbiginc == 0) {
                            float tmp_vs_remap_dataref = XPLMGetDataf(VsSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: tmp_vs_remap_dataref = %f\n", tmp_vs_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(VsSwitchUpRemapableCmd);
                            tmp_vs_remap_dataref = XPLMGetDataf(VsSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(VsSwitchUpRemapableCmd);\n");
                                XPLMDebugString(buf);
                                sprintf(buf, "Xsaitekpanels: tmp_vs_remap_dataref = %f\n\n", tmp_vs_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            vsdbncinc = 0;
                        }
                     } else {
                        if (vsbiginc == 0) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: upapvs = %d ", upapvs);
                                XPLMDebugString(buf);
                            }
                            upapvs = upapvs + 100;
                            upapvs = (upapvs / 100);
                            upapvs = (upapvs * 100);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: upapvs + 100 = %d\n\n", upapvs);
                                XPLMDebugString(buf);
                            }
                        }
                    }
                    vsdbncinc = 0;
                }
             }
		}
        Last_Adjustment_Up = testbit(multibuf,ADJUSTMENT_UP);
        vsbiginc = 0;

        if((Last_Adjustment_Dn == 1) && (testbit(multibuf,ADJUSTMENT_DN) == 0)) {
            vsdbncdec++;
            if (vsdbncdec > multispeed) {
                n = multimul;
                MultiKnobLastCurrentDnTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentDnTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentDnTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentDnTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "MultiKnobLastCurrentDnTimeDiff = %f MultiVsKnobSpeedThreshold = %f\n",MultiKnobLastCurrentDnTimeDiff, MultiVsKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }
                if (((xpanelsfnbutton == 1) && (vsswitchremap != 3)) || (MultiKnobLastCurrentDnTimeDiff < MultiVsKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time)
                    if (vsswitchremap == 1) {
                        float tmp_vs_remap_dataref = XPLMGetDataf(VsSwitchRemapableData);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: tmp_vs_remap_dataref = %f\n", tmp_vs_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(VsSwitchDnRemapableCmd);
                        tmp_vs_remap_dataref = XPLMGetDataf(VsSwitchRemapableData);
                        tmp_vs_remap_dataref = tmp_vs_remap_dataref - 100;
                        XPLMSetDataf(VsSwitchRemapableData, tmp_vs_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(VsSwitchDnRemapableCmd);\n");
                            XPLMDebugString(buf);
                            sprintf(buf, "Xsaitekpanels: tmp_vs_remap_dataref = %f\n\n", tmp_vs_remap_dataref);
                            XPLMDebugString(buf);
                        }

                        vsdbncdec = 0;
                        vsbigdec = 1;
                    } else {
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: upapvs = %d ", upapvs);
                            XPLMDebugString(buf);
                        }
                        upapvs = upapvs - 200;
                        if (upapvs > 100) {
                            upapvs = (upapvs / 100);
                            upapvs = (upapvs * 100);
                        }
                        if (upapvs < (-100)) {
                            upapvs = (upapvs / 100);
                            upapvs = (upapvs * 100);
                        }
                        vsbigdec = 1;
                        if (log_enable == 1) {
                            sprintf(buf, "upapvs - 200 = %d\n\n", upapvs);
                            XPLMDebugString(buf);
                        }
                    }
                }
                vsdbncdec = 0;
                MultiKnobLastCurrentDnTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (vsswitchremap == 3)) {
                    if ((vsswitchremap == 1) || (vsswitchremap == 3)) {
                        if (vsbigdec == 0) {
                            float tmp_vs_remap_dataref = XPLMGetDataf(VsSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: tmp_vs_remap_dataref = %f\n", tmp_vs_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(VsSwitchDnRemapableCmd);
                            tmp_vs_remap_dataref = XPLMGetDataf(VsSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(VsSwitchDnRemapableCmd);\n");
                                XPLMDebugString(buf);
                                sprintf(buf, "Xsaitekpanels: tmp_vs_remap_dataref = %f\n\n", tmp_vs_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            vsdbncdec = 0;
                        }
                    } else {
                        if (vsbigdec == 0) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: upapvs = %d ", upapvs);
                                XPLMDebugString(buf);
                            }
                            upapvs = upapvs - 100;
                            if (upapvs > 100) {
                                upapvs = (upapvs / 100);
                                upapvs = (upapvs * 100);
                            }
                            if (upapvs < (-100)) {
                                upapvs = (upapvs / 100);
                                upapvs = (upapvs * 100);
                            }
                            if (log_enable == 1) {
                                sprintf(buf, "upapvs - 100 = %d\n\n", upapvs);
                                XPLMDebugString(buf);
                            }
                        }
                    }
                    vsdbncdec = 0;
                }
            }
        }
        Last_Adjustment_Dn = testbit(multibuf,ADJUSTMENT_DN);
        vsbigdec = 0;

        upapvsf = upapvs;
        if (vsswitchremap == 2) {
            XPLMSetDataf(VsSwitchRemapableData, upapvsf);
        } else if ((vsswitchremap == 1) || (vsswitchremap == 3)) {
            upapvsf = XPLMGetDataf(VsSwitchRemapableData);
            upapvs = (int)(upapvsf);
        } else {
            XPLMSetDataf(ApVs, upapvsf);
        }

        if ((altswitchremap == 1) || (altswitchremap == 3)) {
            upapaltf = XPLMGetDataf(AltSwitchRemapableData);
        } else if (altswitchremap == 2) {
            upapaltf = XPLMGetDataf(AltSwitchRemapableData);
        } else {
            upapaltf = XPLMGetDataf(ApAlt);
        }

        if ((vsswitchremap == 1) || (vsswitchremap == 3)) {
            upapvsf = XPLMGetDataf(VsSwitchRemapableData);
        } else if (vsswitchremap == 2) {
            upapvsf = XPLMGetDataf(VsSwitchRemapableData);
        } else {
            upapvsf = XPLMGetDataf(ApVs);
        }

        upapalt = (int)(upapaltf);
        upapvs = (int)(upapvsf);
        if (upapvs < 0) {
            upapvs = (upapvs * -1);
            neg = 1;
        } else {
            neg = 0;
        }
    }
}

// ***************** IAS Switch Position *******************
void process_ias_switch()
{
    if (testbit(multibuf,IAS_SWITCH)) {
        iasbiginc = 0;
        iasbigdec = 0;
        MultiIasKnobSpeedThreshold = (float)(multiaccelthreshold * .01);
        //multiseldis = 2;
        if (!AvPwrIsOn() || !BatPwrIsOn()) {
            multiseldis = 5;
        } else {
           multiseldis = 2;
        }

        if (iasismachremap == 1) {
            if (XPLMGetDatai(IasIsmachRemapableData) == iasismachvalue) {
                apmasf = XPLMGetDataf(IasSwitchRemapableData);
            } else {
                apasf = XPLMGetDataf(IasSwitchRemapableData);
            }
        } else if (XPLMGetDatai(AirspeedIsMach) == 1) {
            apmasf = XPLMGetDataf(ApAs);
        } else if ((iasswitchremap == 1) | (iasswitchremap == 2) | (iasswitchremap == 3)) {
            apasf = XPLMGetDataf(IasSwitchRemapableData);
            apas = (int)(apasf);
        } else {
            apasf = XPLMGetDataf(ApAs);
            apas = (int)(apasf);
        }

        if ((Last_Adjustment_Up == 1) && (testbit(multibuf,ADJUSTMENT_UP) == 0)) {
            iasdbncinc++;
            if (iasdbncinc > multispeed) {
                n = multimul;
                MultiKnobLastCurrentUpTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentUpTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentUpTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentUpTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "MultiKnobLastCurrentUpTimeDiff = %f MultiIasKnobSpeedThreshold = %f\n",MultiKnobLastCurrentUpTimeDiff, MultiIasKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }

                if (((xpanelsfnbutton == 1) && (iasswitchremap != 3)) || (MultiKnobLastCurrentUpTimeDiff < MultiIasKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time)
                    if (iasswitchremap == 1) {
                        float tmp_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                        if (log_enable == 1) {
                            tmp_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                            sprintf(buf, "Xsaitekpanels: tmp_ias_remap_dataref = %f\n", tmp_ias_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(IasSwitchUpRemapableCmd);
                        tmp_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                        if (XPLMGetDatai(AirspeedIsMach) == 1) {
                            tmp_ias_remap_dataref = tmp_ias_remap_dataref + .09;
                        } else {
                            tmp_ias_remap_dataref = tmp_ias_remap_dataref + 9;
                        }

                        XPLMSetDataf(IasSwitchRemapableData, tmp_ias_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(IasSwitchUpRemapableCmd);\n");
                            XPLMDebugString(buf);
                            tmp_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                            sprintf(buf, "Xsaitekpanels: tmp_ias_remap_dataref = %f\n\n", tmp_ias_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        iasdbncinc = 0;
                        iasbiginc = 1;

                    } else {
                        if (iasismachremap == 1) {
                            if (XPLMGetDatai(IasIsmachRemapableData) == iasismachvalue) {
                                apmasf = apmasf + .10;
                            } else {
                                apas = apas + 1;
                            }
                        } else if (XPLMGetDatai(AirspeedIsMach) == 1) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: apmasf = %f ", apmasf);
                                XPLMDebugString(buf);
                            }
                            apmasf = apmasf + .10;
                            if (log_enable == 1) {
                                sprintf(buf, "apmasf + .10 = %f\n\n", apmasf);
                                XPLMDebugString(buf);
                            }
                        } else {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: apas = %d ", apas);
                                XPLMDebugString(buf);
                            }
                            apas = apas + 10;
                            iasbiginc = 1;
                            if (log_enable == 1) {
                                sprintf(buf, "apas + 10 = %d\n\n", apas);
                                XPLMDebugString(buf);
                            }
                        }
                    }
                    iasdbncinc = 0;
                }
                MultiKnobLastCurrentUpTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (iasswitchremap == 3)) {
                    if ((iasswitchremap == 1) || (iasswitchremap == 3)) {
                        if (iasbiginc == 0) {
                            if (log_enable == 1) {
                                float log_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                                sprintf(buf, "Xsaitekpanels: log_ias_remap_dataref = %f\n", log_ias_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(IasSwitchUpRemapableCmd);
                            iasdbncinc = 0;
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(IasSwitchUpRemapableCmd);\n");
                                XPLMDebugString(buf);
                                float log_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                                sprintf(buf, "Xsaitekpanels: log_ias_remap_dataref = %f\n\n", log_ias_remap_dataref);
                                XPLMDebugString(buf);
                            }
                        }
                    } else if (iasswitchremap == 2) {
                        apas = apas + 1;
                    } else {
                        if (iasismachremap == 1) {
                            if (XPLMGetDatai(IasIsmachRemapableData) == iasismachvalue) {
                                apmasf = apmasf + .01;
                            } else {
                                apas = apas + 1;
                            }
                        } else if (XPLMGetDatai(AirspeedIsMach) == 1) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: apmasf = %f ", apmasf);
                                XPLMDebugString(buf);
                            }
                            apmasf = apmasf + .01;
                            if (log_enable == 1) {
                                sprintf(buf, "apmasf + .01 = %f\n\n", apmasf);
                                XPLMDebugString(buf);
                            }
                        } else {
                            if (iasbiginc == 0) {
                                if (log_enable == 1) {
                                    sprintf(buf, "Xsaitekpanels: apas = %d ", apas);
                                    XPLMDebugString(buf);
                                }
                                apas = apas + 1;
                                if (log_enable == 1) {
                                    sprintf(buf, "apas + 1 = %d\n\n", apas);
                                    XPLMDebugString(buf);
                                }
                            }
                        }
                    }
                    iasdbncinc = 0;
                }
            }
        }
        Last_Adjustment_Up = testbit(multibuf,ADJUSTMENT_UP);
        iasbiginc = 0;

        if ((Last_Adjustment_Dn == 1) && (testbit(multibuf,ADJUSTMENT_DN) == 0)) {
            iasdbncdec++;
            if (iasdbncdec > multispeed) {
                n = multimul;
                MultiKnobLastCurrentDnTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentDnTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentDnTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentDnTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "MultiKnobLastCurrentDnTimeDiff = %f MultiIasKnobSpeedThreshold = %f\n",MultiKnobLastCurrentDnTimeDiff, MultiIasKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }
                if (((xpanelsfnbutton == 1) && (iasswitchremap != 3)) || (MultiKnobLastCurrentDnTimeDiff < MultiIasKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time)
                    if (iasswitchremap == 1) {
                        float tmp_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: log_ias_remap_dataref = %f\n", tmp_ias_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(IasSwitchDnRemapableCmd);
                        tmp_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                        if (XPLMGetDatai(AirspeedIsMach) == 1) {
                            tmp_ias_remap_dataref = tmp_ias_remap_dataref - .09;
                        } else {
                            tmp_ias_remap_dataref = tmp_ias_remap_dataref - 9;
                        }
                        XPLMSetDataf(IasSwitchRemapableData, tmp_ias_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(IasSwitchDnRemapableCmd);\n");
                            XPLMDebugString(buf);
                            tmp_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                            sprintf(buf, "Xsaitekpanels: tmp_ias_remap_dataref = %f\n\n", tmp_ias_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        iasdbncdec = 0;
                        iasbigdec = 1;
                    } else {
                       if (iasismachremap == 1) {
                           if (XPLMGetDatai(IasIsmachRemapableData) == iasismachvalue) {
                               apmasf = apmasf - .01;
                           } else {
                               apas = apas - 1;
                           }
                       } else if (XPLMGetDatai(AirspeedIsMach) == 1) {
                           if (log_enable == 1) {
                               sprintf(buf, "Xsaitekpanels: apmasf = %f ", apmasf);
                               XPLMDebugString(buf);
                           }
                           apmasf = apmasf - .01;
                           if (log_enable == 1) {
                               sprintf(buf, "apmasf - .01 = %f\n\n", apmasf);
                               XPLMDebugString(buf);
                           }
                       } else {
                           if (log_enable == 1) {
                               sprintf(buf, "Xsaitekpanels: apas = %d ", apas);
                               XPLMDebugString(buf);
                           }
                           apas = apas - 10;
                           iasbigdec = 1;
                           if (log_enable == 1) {
                               sprintf(buf, "apas - 10 = %d\n\n", apas);
                               XPLMDebugString(buf);
                           }
                       }
                    }
                    iasdbncdec = 0;
                }
                MultiKnobLastCurrentDnTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (iasswitchremap == 3)) {
                    if ((iasswitchremap == 1) || (iasswitchremap == 3)) {
                        if (iasbigdec == 0) {
                            if (log_enable == 1) {
                                float log_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                                sprintf(buf, "Xsaitekpanels: log_ias_remap_dataref = %f\n", log_ias_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(IasSwitchDnRemapableCmd);
                            iasdbncdec = 0;
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(IasSwitchDnRemapableCmd);\n");
                                XPLMDebugString(buf);
                                float log_ias_remap_dataref = XPLMGetDataf(IasSwitchRemapableData);
                                sprintf(buf, "Xsaitekpanels: log_ias_remap_dataref = %f\n\n", log_ias_remap_dataref);
                                XPLMDebugString(buf);
                            }
                        }
                    } else if (iasswitchremap == 2) {
                        apas = apas - 1;
                    } else {
                        if (iasismachremap == 1) {
                            if (XPLMGetDatai(IasIsmachRemapableData) == iasismachvalue) {
                                apmasf = apmasf - .01;
                            } else {
                                apas = apas - 1;
                            }
                        } else if (XPLMGetDatai(AirspeedIsMach) == 1) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: apmasf = %f ", apmasf);
                                XPLMDebugString(buf);
                            }
                            apmasf = apmasf - .01;
                            if (log_enable == 1) {
                                sprintf(buf, "apmasf - .01 = %f\n\n", apmasf);
                                XPLMDebugString(buf);
                            }
                        } else {
                            if (iasbigdec == 0) {
                                if (log_enable == 1) {
                                    sprintf(buf, "Xsaitekpanels: apas = %d ", apas);
                                    XPLMDebugString(buf);
                                }
                                apas = apas - 1;
                                if (log_enable == 1) {
                                    sprintf(buf, "apas - 1 = %d\n\n", apas);
                                    XPLMDebugString(buf);
                                }
                            }
                        }
                    }
                    iasdbncdec = 0;
                }
            }
        }
        Last_Adjustment_Dn = testbit(multibuf,ADJUSTMENT_DN);
        iasbigdec = 0;


        if ((iasswitchremap == 1) || (iasswitchremap == 3)) {
            if (XPLMGetDatai(AirspeedIsMach) == 1) {
                apmasf = XPLMGetDataf(IasSwitchRemapableData);
                apmas = (int)(apmasf * 100);
                apasout = apmas;
            } else {
                apasf = XPLMGetDataf(IasSwitchRemapableData);
                apas = (int)(apasf);
                apasout = apas;
            }


        } else if (iasswitchremap == 2) {
            if (XPLMGetDatai(AirspeedIsMach) == 1) {

                apmas = (int)(apmasf * 100);
                apasout = apmas;
                XPLMSetDataf(ApAs, apmasf);
            } else {
                apasout = apas;
                apasf = apas;
                XPLMSetDataf(IasSwitchRemapableData, apasf);
            }

        } else {
            if (XPLMGetDatai(AirspeedIsMach) == 1) {

                apmas = (int)(apmasf * 100);
                apasout = apmas;
                XPLMSetDataf(ApAs, apmasf);
            } else {
                apasout = apas;
                apasf = apas;
                XPLMSetDataf(ApAs, apasf);
            }
        }
    }
}

// ***************** HDG Switch Position *******************
void process_hdg_switch()
{

    if(testbit(multibuf,HDG_SWITCH)) {
        hdgbiginc = 0;
        hdgbigdec = 0;
        MultiHdgKnobSpeedThreshold = (float)(multiaccelthreshold * .01);
        //multiseldis = 3;
        if (!AvPwrIsOn() || !BatPwrIsOn()) {
            multiseldis = 5;
        } else {
           multiseldis = 3;
        }

        if (hdgswitchremap == 2) {
            upaphdgf = XPLMGetDataf(HdgSwitchRemapableData);
        } else {
            upaphdgf = XPLMGetDataf(ApHdg);
        }

        upaphdg = (int)(upaphdgf);
        if((Last_Adjustment_Up == 1) && (testbit(multibuf,ADJUSTMENT_UP) == 0)) {
            hdgdbncinc++;
            if (hdgdbncinc > multispeed) {
                n = multimul;
                MultiKnobLastCurrentUpTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentUpTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentUpTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentUpTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "MultiKnobLastCurrentUpTimeDiff = %f MultiHdgKnobSpeedThreshold = %f\n",MultiKnobLastCurrentUpTimeDiff, MultiHdgKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }

                if (((xpanelsfnbutton == 1) && (hdgswitchremap != 3)) || (MultiKnobLastCurrentUpTimeDiff < MultiHdgKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time)
                    if (hdgswitchremap == 1) {
                        float tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: tmp_hdg_remap_dataref = %f\n", tmp_hdg_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(HdgSwitchUpRemapableCmd);
                        tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                        tmp_hdg_remap_dataref = tmp_hdg_remap_dataref + 9;
                        XPLMSetDataf(HdgSwitchRemapableData, tmp_hdg_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(HdgSwitchUpRemapableCmd);\n");
                            XPLMDebugString(buf);
                            sprintf(buf, "Xsaitekpanels: tmp_hdg_remap_dataref = %f\n\n", tmp_hdg_remap_dataref);
                            XPLMDebugString(buf);
                        }

                        hdgbiginc = 1;

                    } else {
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: upaphdg = %d ", upaphdg);
                            XPLMDebugString(buf);
                        }
                        upaphdg = upaphdg + 10;
                        hdgbiginc = 1;
                        if (log_enable == 1) {
                            sprintf(buf, "upaphdg + 10 = %d\n\n", upaphdg);
                            XPLMDebugString(buf);
                        }
                    }
                    hdgdbncinc = 0;

                }
                MultiKnobLastCurrentUpTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (hdgswitchremap == 3)) {
                    if ((hdgswitchremap == 1) || (hdgswitchremap == 3)) {
                        if (hdgbiginc == 0) {
                            float tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: tmp_hdg_remap_dataref = %f\n", tmp_hdg_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(HdgSwitchUpRemapableCmd);
                            tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(HdgSwitchUpRemapableCmd);\n");
                                XPLMDebugString(buf);
                                sprintf(buf, "Xsaitekpanels: tmp_hdg_remap_dataref = %f\n\n", tmp_hdg_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            hdgdbncinc = 0;
                        }

                    } else {
                        if (hdgbiginc == 0) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: upaphdg = %d ", upaphdg);
                                XPLMDebugString(buf);
                            }
                            upaphdg = upaphdg + 1;
                            if (log_enable == 1) {
                                sprintf(buf, "upaphdg + 1 = %d\n\n", upaphdg);
                                XPLMDebugString(buf);
                            }
                        }
                    }
                    hdgdbncinc = 0;
                }
            }
        }
        Last_Adjustment_Up = testbit(multibuf,ADJUSTMENT_UP);
        hdgbiginc = 0;

        if((Last_Adjustment_Dn == 1) && (testbit(multibuf,ADJUSTMENT_DN) == 0)) {
            hdgdbncdec++;
            if (hdgdbncdec > multispeed) {
                n = multimul;
                MultiKnobLastCurrentDnTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentDnTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentDnTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentDnTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "MultiKnobLastCurrentDnTimeDiff = %f MultiHdgKnobSpeedThreshold = %f\n",MultiKnobLastCurrentDnTimeDiff, MultiHdgKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }
                if (((xpanelsfnbutton == 1) && (hdgswitchremap != 3)) || (MultiKnobLastCurrentDnTimeDiff < MultiHdgKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time)
                    if (hdgswitchremap == 1) {
                        float tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                        if (log_enable == 1) {
                            tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                            sprintf(buf, "Xsaitekpanels: tmp_hdg_remap_dataref = %f\n", tmp_hdg_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(HdgSwitchDnRemapableCmd);
                        tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                        tmp_hdg_remap_dataref = tmp_hdg_remap_dataref - 9;
                        XPLMSetDataf(HdgSwitchRemapableData, tmp_hdg_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(HdgSwitchDnRemapableCmd);\n");
                            XPLMDebugString(buf);
                            sprintf(buf, "Xsaitekpanels: tmp_hdg_remap_dataref = %f\n\n", tmp_hdg_remap_dataref);
                            XPLMDebugString(buf);
                        }

                        hdgbigdec = 1;

                    } else {
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: upaphdg = %d ", upaphdg);
                            XPLMDebugString(buf);
                        }
                        upaphdg = upaphdg - 10;
                        hdgbigdec = 1;
                        if (log_enable == 1) {
                            sprintf(buf, "upaphdg - 10 = %d\n\n", upaphdg);
                            XPLMDebugString(buf);
                        }
                    }
                    hdgdbncdec = 0;
                }
                MultiKnobLastCurrentDnTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (hdgswitchremap == 3)) {
                    if ((hdgswitchremap == 1) || (hdgswitchremap == 3)) {
                        if (hdgbigdec == 0) {
                            float tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                            if (log_enable == 1) {
                                tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                                sprintf(buf, "Xsaitekpanels: tmp_hdg_remap_dataref = %f\n", tmp_hdg_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(HdgSwitchDnRemapableCmd);
                            tmp_hdg_remap_dataref = XPLMGetDataf(HdgSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(HdgSwitchDnRemapableCmd);\n");
                                XPLMDebugString(buf);
                                sprintf(buf, "Xsaitekpanels: tmp_hdg_remap_dataref = %f\n\n", tmp_hdg_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            hdgdbncdec = 0;

                        }

                    } else {
                        if (hdgbigdec == 0) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: upaphdg = %d ", upaphdg);
                                XPLMDebugString(buf);
                            }
                            upaphdg = upaphdg - 1;
                            if (log_enable == 1) {
                                sprintf(buf, "upaphdg - 1 = %d\n\n", upaphdg);
                                XPLMDebugString(buf);
                            }
                        }
                    }
                    hdgdbncdec = 0;
                }
            }
        }
        Last_Adjustment_Dn = testbit(multibuf,ADJUSTMENT_DN);
        hdgbigdec = 0;

        if(upaphdg > 360) {
            upaphdg = 1;
        }
        if(upaphdg < 0) {
            upaphdg = 359;
        }
        upaphdgf = upaphdg;
        if (hdgswitchremap == 2) {
            XPLMSetDataf(HdgSwitchRemapableData, upaphdgf);
        } else if ((hdgswitchremap == 1) || (hdgswitchremap == 3)) {
            upaphdgf = XPLMGetDataf(HdgSwitchRemapableData);
            upaphdg = (int)(upaphdgf);
        } else {
            XPLMSetDataf(ApHdg, upaphdgf);
        }

    }
}

// ***************** CRS Switch Position *******************
void process_crs_switch()
{
    float cur_apcrsf = 0;
    int   cur_apcrs  = 0;

    // if the toggle is selected, use nav2, otherwise, nav1
    XPLMDataRef crs_dataref =  !xpanelscrstoggle ? ApCrs : ApCrs2;

	if(testbit(multibuf,CRS_SWITCH)) {
        crsbiginc = 0;
        crsbigdec = 0;
        MultiCrsKnobSpeedThreshold = (float)(multiaccelthreshold * .01);
        //multiseldis = 4;
        if (!AvPwrIsOn() || !BatPwrIsOn()) {
            multiseldis = 5;
        } else {
           multiseldis = 4;
        }

        upapcrsf = XPLMGetDataf(ApCrs);
        upapcrs = (int)(upapcrsf);

        if ((crsswitchremap == 1) || (crsswitchremap == 2) || (crsswitchremap == 3)) {
            cur_apcrsf = XPLMGetDataf(CrsSwitchRemapableData);
        } else {
            //  get the appropriate course setting depending on if the toggle is down
            cur_apcrsf = XPLMGetDataf(crs_dataref);
        }
        cur_apcrs = (int)(cur_apcrsf);
        if((Last_Adjustment_Up == 1) && (testbit(multibuf,ADJUSTMENT_UP) == 0)) {
            crsdbncinc++;
            if (crsdbncinc > multispeed) {
                n = multimul;
                MultiKnobLastCurrentUpTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentUpTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentUpTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentUpTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "MultiKnobLastCurrentUpTimeDiff = %f MultiCrsKnobSpeedThreshold = %f\n",MultiKnobLastCurrentUpTimeDiff, MultiCrsKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }

                if (((xpanelsfnbutton == 1) && (crsswitchremap != 3)) || (MultiKnobLastCurrentUpTimeDiff < MultiCrsKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time)
                    if (crsswitchremap == 1) {
                        float tmp_crs_remap_dataref = XPLMGetDataf(CrsSwitchRemapableData);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: tmp_crs_remap_dataref = %f\n", tmp_crs_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(CrsSwitchUpRemapableCmd);
                        tmp_crs_remap_dataref = XPLMGetDataf(CrsSwitchRemapableData);
                        tmp_crs_remap_dataref = tmp_crs_remap_dataref + 9;
                        XPLMSetDataf(CrsSwitchRemapableData, tmp_crs_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(CrsSwitchUpRemapableCmd);\n");
                            XPLMDebugString(buf);
                            sprintf(buf, "Xsaitekpanels: tmp_crs_remap_dataref = %f\n\n", tmp_crs_remap_dataref);
                            XPLMDebugString(buf);
                        }

                        crsdbncinc = 0;
                        crsbiginc = 1;

                    } else {
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: cur_apcrs = %d ", cur_apcrs);
                            XPLMDebugString(buf);
                        }
                        cur_apcrs = cur_apcrs + 10;
                        crsbiginc = 1;
                        if (log_enable == 1) {
                            sprintf(buf, "cur_apcrs + 10 = %d\n\n", cur_apcrs);
                            XPLMDebugString(buf);
                        }
                    }
                }
                MultiKnobLastCurrentUpTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (crsswitchremap == 3)) {
                    if ((crsswitchremap == 1) || (crsswitchremap == 3)) {
                        if (crsbiginc == 0) {
                            float tmp_crs_remap_dataref = XPLMGetDataf(CrsSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: tmp_crs_remap_dataref = %f\n", tmp_crs_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(CrsSwitchUpRemapableCmd);
                            tmp_crs_remap_dataref = XPLMGetDataf(CrsSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(CrsSwitchUpRemapableCmd);\n");
                                XPLMDebugString(buf);
                                sprintf(buf, "Xsaitekpanels: tmp_crs_remap_dataref = %f\n\n", tmp_crs_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            crsdbncinc = 0;

                        }
                    } else {
                        if (crsbiginc == 0) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: cur_apcrs = %d ", cur_apcrs);
                                XPLMDebugString(buf);
                            }
                            cur_apcrs = cur_apcrs + 1;
                            if (log_enable == 1) {
                                sprintf(buf, "cur_apcrs + 1 = %d\n\n", cur_apcrs);
                                XPLMDebugString(buf);
                            }
                        }
                    }
                }
                crsdbncinc = 0;
            }
        }
        Last_Adjustment_Up = testbit(multibuf,ADJUSTMENT_UP);
        crsbiginc = 0;

        if((Last_Adjustment_Dn == 1) && (testbit(multibuf,ADJUSTMENT_DN) == 0)) {
            crsdbncdec++;
            if (crsdbncdec > multispeed) {
                n = multimul;
                MultiKnobLastCurrentDnTimeDiff = wrgCurrentTime -  MultiKnobLastCurrentDnTime;
                if (log_enable == 1) {
                    sprintf(buf, "Xsaitekpanels: wrgCurrentTime = %f MultiKnobLastCurrentDnTime = %f\n",wrgCurrentTime, MultiKnobLastCurrentDnTime);
                    XPLMDebugString(buf);
                    sprintf(buf, "MultiKnobLastCurrentDnTimeDiff = %f MultiCrsKnobSpeedThreshold = %f\n",MultiKnobLastCurrentDnTimeDiff, MultiCrsKnobSpeedThreshold);
                    XPLMDebugString(buf);
                }

                if (((xpanelsfnbutton == 1) && (crsswitchremap != 3)) || (MultiKnobLastCurrentDnTimeDiff < MultiCrsKnobSpeedThreshold)) {  // Steve Bootes : add test for MultiKnob rotation time)
                    if (crsswitchremap == 1) {
                        float tmp_crs_remap_dataref = XPLMGetDataf(CrsSwitchRemapableData);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: tmp_crs_remap_dataref = %f\n", tmp_crs_remap_dataref);
                            XPLMDebugString(buf);
                        }
                        XPLMCommandOnce(CrsSwitchDnRemapableCmd);
                        tmp_crs_remap_dataref = XPLMGetDataf(CrsSwitchRemapableData);
                        tmp_crs_remap_dataref = tmp_crs_remap_dataref - 9;
                        XPLMSetDataf(CrsSwitchRemapableData, tmp_crs_remap_dataref);
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(CrsSwitchDnRemapableCmd);\n");
                            XPLMDebugString(buf);
                            sprintf(buf, "Xsaitekpanels: tmp_crs_remap_dataref = %f\n\n", tmp_crs_remap_dataref);
                            XPLMDebugString(buf);
                        }

                        crsbigdec = 1;
                        crsdbncdec = 0;

                    } else {
                        if (log_enable == 1) {
                            sprintf(buf, "Xsaitekpanels: cur_apcrs = %d ", cur_apcrs);
                            XPLMDebugString(buf);
                        }
                        cur_apcrs = cur_apcrs - 10;
                        crsbigdec = 1;
                        if (log_enable == 1) {
                            sprintf(buf, "cur_apcrs - 10 = %d\n\n", cur_apcrs);
                            XPLMDebugString(buf);
                        }
                    }
                }
                MultiKnobLastCurrentDnTime = wrgCurrentTime; // Steve Bootes  Bill Good: set last current time to current time
                if ((xpanelsfnbutton == 0) || (crsswitchremap == 3)) {
                    if ((crsswitchremap == 1) || (crsswitchremap == 3)) {
                        if (crsbigdec == 0) {
                            float tmp_crs_remap_dataref = XPLMGetDataf(CrsSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: tmp_crs_remap_dataref = %f\n", tmp_crs_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            XPLMCommandOnce(CrsSwitchDnRemapableCmd);
                            tmp_crs_remap_dataref = XPLMGetDataf(CrsSwitchRemapableData);
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: XPLMCommandOnce(CrsSwitchDnRemapableCmd);\n");
                                XPLMDebugString(buf);
                                sprintf(buf, "Xsaitekpanels: tmp_crs_remap_dataref = %f\n\n", tmp_crs_remap_dataref);
                                XPLMDebugString(buf);
                            }
                            crsdbncdec = 0;
                        }
                    } else {
                        if (crsbigdec == 0) {
                            if (log_enable == 1) {
                                sprintf(buf, "Xsaitekpanels: cur_apcrs = %d ", cur_apcrs);
                                XPLMDebugString(buf);
                            }
                            cur_apcrs = cur_apcrs - 1;
                            if (log_enable == 1) {
                                sprintf(buf, "cur_apcrs - 1 = %d\n\n", cur_apcrs);
                                XPLMDebugString(buf);
                            }
                        }
                    }
                }
                crsdbncdec = 0;
            }
        }
        Last_Adjustment_Dn = testbit(multibuf,ADJUSTMENT_DN);
        crsbigdec = 0;

        if(cur_apcrs > 360) {
            cur_apcrs = 1;
        }

        if(cur_apcrs < 0) {
            cur_apcrs = 359;
        }

        cur_apcrsf = cur_apcrs;
        if ((crsswitchremap == 1) || (crsswitchremap == 3)) {
            upapcrsf = XPLMGetDataf(CrsSwitchRemapableData);
            upapcrs = (int)(upapcrsf);
        } else if (crsswitchremap == 2) {
            XPLMSetDataf(CrsSwitchRemapableData, cur_apcrsf);
         } else {
            XPLMSetDataf(crs_dataref, cur_apcrsf);
        }

        if ((crsswitchremap == 1) || (crsswitchremap == 3)) {


        } else {
            //  set the appropriate global based on whether the crs toggle is on
            if( !xpanelscrstoggle ) {
                //  toggle off - nav1
                upapcrsf = cur_apcrsf;
                upapcrs  = cur_apcrs;
            } else {
                //  toggle on - nav2
                upapcrsf2 = cur_apcrsf;
                upapcrs2   = cur_apcrs;
                upapcrs   = cur_apcrs;
            }
        }

	}
}

// ***************** Auto Throttle Switch Position *******************
void process_autothrottle_switch()
{
    if(autothrottleswitchenable == 0) {
        return;
    }

    bool IsAutoThottleEnabled = testbit(multibuf, AUTO_THROTTLE_SWITCH);

    if (autothrottleswitcharmedvalue == 0)
        IsAutoThottleEnabled = !IsAutoThottleEnabled;

    static bool AutoThrottleAllreadyOn = IsAutoThottleEnabled;
    static bool AutoThrottleAllreadyOff = !IsAutoThottleEnabled;

    if (IsAutoThottleEnabled && AutoThrottleAllreadyOn)
        return;
    if (!IsAutoThottleEnabled && AutoThrottleAllreadyOff)
        return;

    if (autothrottleswitchenable == 1)
    {
        if (IsAutoThottleEnabled)
        {
            XPLMSetDatai(ApAutThr, 1);

            AutoThrottleAllreadyOn = true;
            AutoThrottleAllreadyOff = false;
        }
        else
        {
            XPLMSetDatai(ApAutThr, 0);

            AutoThrottleAllreadyOn = true;
            AutoThrottleAllreadyOff = false;
        }
    }
    else if (autothrottleswitchenable == 2)
    {
        if (IsAutoThottleEnabled)
        {
            XPLMSetDatai(AttrSwitchRemapableData, AttrSwitch_1_DataOnValue);
            XPLMSetDatai(Attr2SwitchRemapableData, AttrSwitch_2_DataOnValue);
            XPLMSetDatai(Attr3SwitchRemapableData, AttrSwitch_3_DataOnValue);
            XPLMSetDatai(Attr4SwitchRemapableData, AttrSwitch_4_DataOnValue);
            XPLMSetDatai(Attr5SwitchRemapableData, AttrSwitch_5_DataOnValue);
            XPLMSetDatai(Attr6SwitchRemapableData, AttrSwitch_6_DataOnValue);
            XPLMSetDatai(Attr7SwitchRemapableData, AttrSwitch_7_DataOnValue);
            XPLMSetDatai(Attr8SwitchRemapableData, AttrSwitch_8_DataOnValue);
            XPLMSetDatai(Attr9SwitchRemapableData, AttrSwitch_9_DataOnValue);
            XPLMSetDatai(Attr10SwitchRemapableData, AttrSwitch_10_DataOnValue);
            XPLMSetDatai(Attr11SwitchRemapableData, AttrSwitch_11_DataOnValue);
            XPLMSetDatai(Attr12SwitchRemapableData, AttrSwitch_12_DataOnValue);
            XPLMSetDatai(Attr13SwitchRemapableData, AttrSwitch_13_DataOnValue);
            XPLMSetDatai(Attr14SwitchRemapableData, AttrSwitch_14_DataOnValue);
            XPLMSetDatai(Attr15SwitchRemapableData, AttrSwitch_15_DataOnValue);
            XPLMSetDatai(Attr16SwitchRemapableData, AttrSwitch_16_DataOnValue);

            AutoThrottleAllreadyOn = true;
            AutoThrottleAllreadyOff = false;
        }
        else
        {
            XPLMSetDatai(AttrSwitchRemapableData, AttrSwitch_1_DataOffValue);
            XPLMSetDatai(Attr2SwitchRemapableData, AttrSwitch_2_DataOffValue);
            XPLMSetDatai(Attr3SwitchRemapableData, AttrSwitch_3_DataOffValue);
            XPLMSetDatai(Attr4SwitchRemapableData, AttrSwitch_4_DataOffValue);
            XPLMSetDatai(Attr5SwitchRemapableData, AttrSwitch_5_DataOffValue);
            XPLMSetDatai(Attr6SwitchRemapableData, AttrSwitch_6_DataOffValue);
            XPLMSetDatai(Attr7SwitchRemapableData, AttrSwitch_7_DataOffValue);
            XPLMSetDatai(Attr8SwitchRemapableData, AttrSwitch_8_DataOffValue);
            XPLMSetDatai(Attr9SwitchRemapableData, AttrSwitch_9_DataOffValue);
            XPLMSetDatai(Attr10SwitchRemapableData, AttrSwitch_10_DataOffValue);
            XPLMSetDatai(Attr11SwitchRemapableData, AttrSwitch_11_DataOffValue);
            XPLMSetDatai(Attr12SwitchRemapableData, AttrSwitch_12_DataOffValue);
            XPLMSetDatai(Attr13SwitchRemapableData, AttrSwitch_13_DataOffValue);
            XPLMSetDatai(Attr14SwitchRemapableData, AttrSwitch_14_DataOffValue);
            XPLMSetDatai(Attr15SwitchRemapableData, AttrSwitch_15_DataOffValue);
            XPLMSetDatai(Attr16SwitchRemapableData, AttrSwitch_16_DataOffValue);

            AutoThrottleAllreadyOn = false;
            AutoThrottleAllreadyOff = true;
        }
    }
    else if (autothrottleswitchenable == 3)
    {
        if (IsAutoThottleEnabled)
        {
            XPLMCommandOnce(AttrSwitch_1_OnCmd);
            XPLMCommandOnce(AttrSwitch_2_OnCmd);
            XPLMCommandOnce(AttrSwitch_3_OnCmd);
            XPLMCommandOnce(AttrSwitch_4_OnCmd);
            XPLMCommandOnce(AttrSwitch_5_OnCmd);
            XPLMCommandOnce(AttrSwitch_6_OnCmd);
            XPLMCommandOnce(AttrSwitch_7_OnCmd);
            XPLMCommandOnce(AttrSwitch_8_OnCmd);
            XPLMCommandOnce(AttrSwitch_9_OnCmd);
            XPLMCommandOnce(AttrSwitch_10_OnCmd);
            XPLMCommandOnce(AttrSwitch_11_OnCmd);
            XPLMCommandOnce(AttrSwitch_12_OnCmd);
            XPLMCommandOnce(AttrSwitch_13_OnCmd);
            XPLMCommandOnce(AttrSwitch_14_OnCmd);
            XPLMCommandOnce(AttrSwitch_15_OnCmd);
            XPLMCommandOnce(AttrSwitch_16_OnCmd);

            AutoThrottleAllreadyOn = true;
            AutoThrottleAllreadyOff = false;
        }
        else
        {
            XPLMCommandOnce(AttrSwitch_2_OffCmd);
            XPLMCommandOnce(AttrSwitch_1_OffCmd);
            XPLMCommandOnce(AttrSwitch_3_OffCmd);
            XPLMCommandOnce(AttrSwitch_4_OffCmd);
            XPLMCommandOnce(AttrSwitch_5_OffCmd);
            XPLMCommandOnce(AttrSwitch_6_OffCmd);
            XPLMCommandOnce(AttrSwitch_7_OffCmd);
            XPLMCommandOnce(AttrSwitch_8_OffCmd);
            XPLMCommandOnce(AttrSwitch_9_OffCmd);
            XPLMCommandOnce(AttrSwitch_10_OffCmd);
            XPLMCommandOnce(AttrSwitch_11_OffCmd);
            XPLMCommandOnce(AttrSwitch_12_OffCmd);
            XPLMCommandOnce(AttrSwitch_13_OffCmd);
            XPLMCommandOnce(AttrSwitch_14_OffCmd);
            XPLMCommandOnce(AttrSwitch_15_OffCmd);
            XPLMCommandOnce(AttrSwitch_16_OffCmd);

            AutoThrottleAllreadyOn = false;
            AutoThrottleAllreadyOff = true;
        }
    }
    else if (autothrottleswitchenable == 4)
    {
        if (IsAutoThottleEnabled)
        {
            XPLMSetDataf(AttrSwitchRemapableData, ((float)AttrSwitch_1_DataOnValue / 100.0f));
            XPLMSetDataf(Attr2SwitchRemapableData, AttrSwitch_2_DataOnValue / 100.0f);
            XPLMSetDataf(Attr3SwitchRemapableData, AttrSwitch_3_DataOnValue / 100.0f);
            XPLMSetDataf(Attr4SwitchRemapableData, AttrSwitch_4_DataOnValue / 100.0f);
            XPLMSetDataf(Attr5SwitchRemapableData, AttrSwitch_5_DataOnValue / 100.0f);
            XPLMSetDataf(Attr6SwitchRemapableData, AttrSwitch_6_DataOnValue / 100.0f);
            XPLMSetDataf(Attr7SwitchRemapableData, AttrSwitch_7_DataOnValue / 100.0f);
            XPLMSetDataf(Attr8SwitchRemapableData, AttrSwitch_8_DataOnValue / 100.0f);
            XPLMSetDataf(Attr9SwitchRemapableData, AttrSwitch_9_DataOnValue / 100.0f);
            XPLMSetDataf(Attr10SwitchRemapableData, AttrSwitch_10_DataOnValue / 100.0f);
            XPLMSetDataf(Attr11SwitchRemapableData, AttrSwitch_11_DataOnValue / 100.0f);
            XPLMSetDataf(Attr12SwitchRemapableData, AttrSwitch_12_DataOnValue / 100.0f);
            XPLMSetDataf(Attr13SwitchRemapableData, AttrSwitch_13_DataOnValue / 100.0f);
            XPLMSetDataf(Attr14SwitchRemapableData, AttrSwitch_14_DataOnValue / 100.0f);
            XPLMSetDataf(Attr15SwitchRemapableData, AttrSwitch_15_DataOnValue / 100.0f);
            XPLMSetDataf(Attr16SwitchRemapableData, AttrSwitch_16_DataOnValue / 100.0f);

            AutoThrottleAllreadyOn = true;
            AutoThrottleAllreadyOff = false;
        }
        else
        {
            XPLMSetDataf(AttrSwitchRemapableData, AttrSwitch_1_DataOffValue / 100.0f);
            XPLMSetDataf(Attr2SwitchRemapableData, AttrSwitch_2_DataOffValue / 100.0f);
            XPLMSetDataf(Attr3SwitchRemapableData, AttrSwitch_3_DataOffValue / 100.0f);
            XPLMSetDataf(Attr4SwitchRemapableData, AttrSwitch_4_DataOffValue / 100.0f);
            XPLMSetDataf(Attr5SwitchRemapableData, AttrSwitch_5_DataOffValue / 100.0f);
            XPLMSetDataf(Attr6SwitchRemapableData, AttrSwitch_6_DataOffValue / 100.0f);
            XPLMSetDataf(Attr7SwitchRemapableData, AttrSwitch_7_DataOffValue / 100.0f);
            XPLMSetDataf(Attr8SwitchRemapableData, AttrSwitch_8_DataOffValue / 100.0f);
            XPLMSetDataf(Attr9SwitchRemapableData, AttrSwitch_9_DataOffValue / 100.0f);
            XPLMSetDataf(Attr10SwitchRemapableData, AttrSwitch_10_DataOffValue / 100.0f);
            XPLMSetDataf(Attr11SwitchRemapableData, AttrSwitch_11_DataOffValue / 100.0f);
            XPLMSetDataf(Attr12SwitchRemapableData, AttrSwitch_12_DataOffValue / 100.0f);
            XPLMSetDataf(Attr13SwitchRemapableData, AttrSwitch_13_DataOffValue / 100.0f);
            XPLMSetDataf(Attr14SwitchRemapableData, AttrSwitch_14_DataOffValue / 100.0f);
            XPLMSetDataf(Attr15SwitchRemapableData, AttrSwitch_15_DataOffValue / 100.0f);
            XPLMSetDataf(Attr16SwitchRemapableData, AttrSwitch_16_DataOffValue / 100.0f);

            AutoThrottleAllreadyOn = false;
            AutoThrottleAllreadyOff = true;
        }
    }
}

// ***************** AP Master Button and light *******************
void process_ap_master_switch()
{
    if (apbuttonremap == 1) {
        if (multires > 0) {
            if (testbit(multibuf, AP_MASTER_BUTTON)) {
                XPLMCommandOnce(ApButtonRemapableCmd);
                lastappos = 1;
            }
        }
        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(ApLightRemapableData) > .50) {
                aplightdata = 1;
            } else if (XPLMGetDataf(ApLightRemapableData) < .50) {
                aplightdata = 0;
            }
            switch(aplightdata) {
            case 1:
                btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(ApLightFlashRemapableData) > .50) {
                    aplightflashdata = 1;
                } else if (XPLMGetDataf(ApLightFlashRemapableData) < .50) {
                    aplightflashdata = 0;
                }
                if (aplightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                }
                break;
            }
        } else if (lightdatareferencetype == 2) {
                switch(XPLMGetDatai(ApLightRemapableData)) {
                case 1:
                    btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                    break;
                case 0:
                    aplightflashdata = XPLMGetDatai(ApLightFlashRemapableData);
                    if (aplightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                    }
                    break;
                }

        } else if (lightdatareferencetype == 3) {
            aplightdata = XPLMGetDatai(ApLightRemapableData);
            switch(aplightdata) {
            case 0:
                btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                break;
            case 1:
                if (flashon == 1) {
                    btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                } else {
                    btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                }
                break;
            case 2:
                btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                break;
            }
        }

    } else if (apbuttonremap == 2) {
            if (multires > 0) {
                if (testbit(multibuf, AP_MASTER_BUTTON)) {
                    XPLMSetDatai(ApButtonRemapableData, 1);
                    lastappos = 1;
                } else if (!testbit(multibuf, AP_MASTER_BUTTON)) {
                    XPLMSetDatai(ApButtonRemapableData, 0);
                }
            }

            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(ApLightRemapableData) > .50) {
                    aplightdata = 1;
                } else if (XPLMGetDataf(ApLightRemapableData) < .50) {
                    aplightdata = 0;
                }
                switch(aplightdata) {
                case 1:
                    btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                    break;
                case 0:
                    if (XPLMGetDataf(ApLightFlashRemapableData) > .50) {
                        aplightflashdata = 1;
                    } else if (XPLMGetDataf(ApLightFlashRemapableData) < .50) {
                            aplightflashdata = 0;
                    }
                    if (aplightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                    }
                    break;
                }
            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(ApLightRemapableData)) {
                    case 1:
                        btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                        break;
                    case 0:
                        aplightflashdata = XPLMGetDatai(ApLightFlashRemapableData);

                        if (aplightflashdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                            } else {
                                btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                            }
                        } else {
                            btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                        }
                        break;
                    }

            } else if (lightdatareferencetype == 3) {
                aplightdata = XPLMGetDatai(ApLightRemapableData);
                switch(XPLMGetDatai(ApMstrStat)) {
                case 0:
                    btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                    break;
                case 1:
                    if (flashon == 1) {
                        btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                    }
                    break;
                case 2:
                    btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                    break;
                }
            }

    } else if (apbuttonremap == 3) {
        if (multires > 0) {
            if (testbit(multibuf, AP_MASTER_BUTTON)) {
                lastappos = XPLMGetDatai(ApButtonRemapableData);
                if (lastappos == 1) {
                    XPLMSetDatai(ApButtonRemapableData, 0);
                } else {
                    XPLMSetDatai(ApButtonRemapableData, 1);

                }

            }
        }

        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(ApLightRemapableData) > .50) {
                aplightdata = 1;
            } else if (XPLMGetDataf(ApLightRemapableData) < .50) {
                aplightdata = 0;
            }
            switch(aplightdata) {
            case 1:
                btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(ApLightFlashRemapableData) > .50) {
                    aplightflashdata = 1;
                } else if (XPLMGetDataf(ApLightFlashRemapableData) < .50) {
                        aplightflashdata = 0;
                }
                if (aplightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                }
                break;
            }
        } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(ApLightRemapableData)) {
                case 1:
                    btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                    break;
                case 0:
                    aplightflashdata = XPLMGetDatai(ApLightFlashRemapableData);

                    if (aplightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                    }
                    break;
                }

        } else if (lightdatareferencetype == 3) {
            aplightdata = XPLMGetDatai(ApLightRemapableData);
            switch(XPLMGetDatai(ApMstrStat)) {
            case 0:
                btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                break;
            case 1:
                if (flashon == 1) {
                    btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                } else {
                    btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
                }
                break;
            case 2:
                btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
                break;
            }
        }
    } else {
        if (multires > 0) {
          switch(XPLMGetDatai(ApMstrStat)){
          case 0:
              if(testbit(multibuf, AP_MASTER_BUTTON)) {
                  XPLMSetDatai(ApMstrStat, 1);
                  lastappos = 1;
              }
              break;
          case 1:
              if(testbit(multibuf, AP_MASTER_BUTTON)) {
                  if (lastappos == 1) {
                      XPLMSetDatai(ApMstrStat, 2);
                  }
                  if (lastappos == 2) {
                      XPLMSetDatai(ApMstrStat, 0);
                  }
              }
              break;
          case 2:
              if(testbit(multibuf, AP_MASTER_BUTTON)) {
                  XPLMSetDatai(ApMstrStat, 1);
                  lastappos = 2;
              }
              break;
          }
        }


        
        switch(XPLMGetDatai(ApMstrStat)) {
        case 0:
            btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
            break;
        case 1:
            if (flashon == 1) {
                btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
            } else {
                btnleds &= ~(1<<0);   // * clear bit 0 in btnleds to 0 *
            }
            break;
        case 2:
            btnleds |= (1<<0);   // * set bit 0 in btnleds to 1 *
            break;
        }
    }
}

// ***************** HDG Button and light *******************
void process_hdg_button()
{
    if (hdgbuttonremap == 1) {
            if (multires > 0) {
                if (testbit(multibuf, HDG_BUTTON)) {
                    XPLMCommandOnce(HdgButtonRemapableCmd);
                    lastappos = 1;
                }
            }
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(HdgLightRemapableData) > .50) {
                    hdglightdata = 1;
                } else if (XPLMGetDataf(HdgLightRemapableData) < .50) {
                    hdglightdata = 0;
                }
                switch(hdglightdata) {
                case 1:
                    btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                    break;
                case 0:
                    if (XPLMGetDataf(HdgLightFlashRemapableData) > .50) {
                        hdglightflashdata = 1;
                    } else if (XPLMGetDataf(HdgLightFlashRemapableData) < .50) {
                        hdglightflashdata = 0;
                    }
                    if (hdglightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                    }
                    break;
                }

              } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(HdgLightRemapableData)) {
                    case 1:
                        btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                        break;
                    case 0:
                        hdglightflashdata = XPLMGetDatai(HdgLightFlashRemapableData);

                        if (hdglightflashdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                            } else {
                                btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                            }
                        } else {
                            btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                        }
                        break;
                    }

            } else if (lightdatareferencetype == 3) {
                hdglightdata = XPLMGetDatai(HdgLightRemapableData);
                switch(hdglightdata) {
                  case 2:
                    btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                    break;
                  case 1:
                    if (flashon == 1) {
                      btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                }else{
                      btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                }
                    break;
                  case 0:
                    btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                    break;
                }
            }

    } else if (hdgbuttonremap == 2) {
        if (multires > 0) {
            if (testbit(multibuf, HDG_BUTTON)) {
                XPLMSetDatai(HdgButtonRemapableData, 1);
                lastappos = 1;
            } else if (!testbit(multibuf, HDG_BUTTON)) {
                XPLMSetDatai(HdgButtonRemapableData, 0);
                lastappos = 1;
            }
        }
        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(HdgLightRemapableData) > .50) {
                hdglightdata = 1;
            } else if (XPLMGetDataf(HdgLightRemapableData) < .50) {
                hdglightdata = 0;
            }
            switch(hdglightdata) {
            case 1:
                btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(HdgLightFlashRemapableData) > .50) {
                    hdglightflashdata = 1;
                } else if (XPLMGetDataf(HdgLightFlashRemapableData) < .50) {
                    hdglightflashdata = 0;
                }
                if (hdglightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                }
                break;
            }

        } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(HdgLightRemapableData)) {
                case 1:
                    btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                    break;
                case 0:
                    hdglightflashdata = XPLMGetDatai(HdgLightFlashRemapableData);

                    if (hdglightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                    }
                    break;
                }

        } else if (lightdatareferencetype == 3) {
            hdglightdata = XPLMGetDatai(HdgLightRemapableData);
            switch(hdglightdata) {
              case 2:
                btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                break;
              case 1:
                if (flashon == 1) {
                  btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
            }else{
                  btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
            }
                break;
              case 0:
                btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                break;
            }
        }

    } else if (hdgbuttonremap == 3) {
        if (multires > 0) {
            if (testbit(multibuf, HDG_BUTTON)) {
                lastappos = XPLMGetDatai(HdgButtonRemapableData);
                if (lastappos == 1) {
                    XPLMSetDatai(HdgButtonRemapableData, 0);
                } else {
                    XPLMSetDatai(HdgButtonRemapableData, 1);
                }

            }
        }
        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(HdgLightRemapableData) > .50) {
                hdglightdata = 1;
            } else if (XPLMGetDataf(HdgLightRemapableData) < .50) {
                hdglightdata = 0;
            }
            switch(hdglightdata) {
            case 1:
                btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(HdgLightFlashRemapableData) > .50) {
                    hdglightflashdata = 1;
                } else if (XPLMGetDataf(HdgLightFlashRemapableData) < .50) {
                    hdglightflashdata = 0;
                }
                if (hdglightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                }
                break;
            }

        } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(HdgLightRemapableData)) {
                case 1:
                    btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                    break;
                case 0:
                    hdglightflashdata = XPLMGetDatai(HdgLightFlashRemapableData);

                    if (hdglightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                    }
                    break;
                }

        } else if (lightdatareferencetype == 3) {
            hdglightdata = XPLMGetDatai(HdgLightRemapableData);
            switch(hdglightdata) {
              case 2:
                btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
                break;
              case 1:
                if (flashon == 1) {
                  btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
            }else{
                  btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
            }
                break;
              case 0:
                btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
                break;
            }
        }
    } else {
        if (multires > 0) {
          if(testbit(multibuf,HDG_BUTTON)) {

            if(xpanelsfnbutton == 1) {
                rhdgf = XPLMGetDataf(MHdg);
                XPLMSetDataf(ApHdg, rhdgf);
            }
            if(xpanelsfnbutton == 0) {
                XPLMCommandOnce(ApHdgBtn);
                lastappos = 1;
            }
          }
        }
        
        switch(XPLMGetDatai(ApHdgStat)) {
          case 2:
            btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
            break;
          case 1:
            if (flashon == 1) {
              btnleds |= (1<<1);   // * set bit 1 in btnleds to 1 *
        }else{
              btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
	    }
            break;
          case 0:
            btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
            break;
        }
        if (XPLMGetDatai(ApMstrStat) == 0) {
            btnleds &= ~(1<<1);   // * clear bit 1 in btnleds to 0 *
        }
    }
}

// ***************** NAV Button and light *******************
void process_nav_button()
{
    if (navbuttonremap == 1) {
        if (multires > 0) {
            if(testbit(multibuf,NAV_BUTTON)) {
                switch (XPLMGetDatai(HsiSelector)) {
                case 0:
                    XPLMCommandOnce(NavButtonVorlocRemapableCmd);
                case 1:
                    XPLMCommandOnce(NavButtonVorlocRemapableCmd);
                    break;
                case 2:
                    XPLMCommandOnce(NavButtonLnavRemapableCmd);
                    break;
                }
                lastappos = 1;
            }
        }
        switch (XPLMGetDatai(HsiSelector)) {
        case 0:
            // Use LED to match power state
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightVorlocRemapableData) > .50) {
                    navlightvorlocdata = 1;
                } else if (XPLMGetDataf(NavLightVorlocRemapableData) < .50) {
                    navlightvorlocdata = 0;
                }
                switch(navlightvorlocdata) {
                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightVorlocFlashRemapableData) > .50) {
                        navlightflashvorlocdata = 1;
                    } else if (XPLMGetDataf(NavLightVorlocFlashRemapableData) < .50) {
                        navlightflashvorlocdata = 0;
                    }
                    if (navlightflashvorlocdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(NavLightVorlocRemapableData)) {
                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashvorlocdata = XPLMGetDatai(NavLightVorlocFlashRemapableData);

                        if (navlightflashvorlocdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }

            } else if (lightdatareferencetype == 3) {
                navlightvorlocdata = XPLMGetDatai(NavLightVorlocRemapableData);
                switch(navlightvorlocdata) {
                case 2:
                    btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                    break;
                case 1:
                    if (flashon == 1) {
                          btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    }else{
                          btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                case 0:
                    btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
            }

            break;
        case 1:
            // Use LED to match power state
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightVorlocRemapableData) > .50) {
                    navlightvorlocdata = 1;
                } else if (XPLMGetDataf(NavLightVorlocRemapableData) < .50) {
                    navlightvorlocdata = 0;
                }
                switch(navlightvorlocdata) {
                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightVorlocFlashRemapableData) > .50) {
                        navlightflashvorlocdata = 1;
                    } else if (XPLMGetDataf(NavLightVorlocFlashRemapableData) < .50) {
                        navlightflashvorlocdata = 0;
                    }
                    if (navlightflashvorlocdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(NavLightVorlocRemapableData)) {
                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashvorlocdata = XPLMGetDatai(NavLightVorlocFlashRemapableData);

                        if (navlightflashvorlocdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }

            } else if (lightdatareferencetype == 3) {
                navlightvorlocdata = XPLMGetDatai(NavLightVorlocRemapableData);
                switch(navlightvorlocdata) {
                case 2:
                    btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                    break;
                case 1:
                    if (flashon == 1) {
                          btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    }else{
                          btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                case 0:
                    btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
            }

            break;
        case 2:
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightLnavRemapableData) > .50) {
                    navlightlnavdata = 1;
                } else if (XPLMGetDataf(NavLightLnavRemapableData) < .50) {
                    navlightlnavdata = 0;
                }
                switch(navlightlnavdata) {

                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightLnavFlashRemapableData) > .50) {
                        navlightflashlnavdata = 1;
                    } else if (XPLMGetDataf(NavLightLnavFlashRemapableData) < .50) {
                         navlightflashlnavdata = 0;
                    }
                    if (navlightflashlnavdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(NavLightLnavRemapableData)) {

                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashlnavdata = XPLMGetDatai(NavLightLnavFlashRemapableData);

                        if (navlightflashlnavdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }


            } else if (lightdatareferencetype == 3) {
                navlightlnavdata = XPLMGetDatai(NavLightLnavRemapableData);
                switch(XPLMGetDatai(ApNavStat)) {
                case 2:
                    btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                    break;
                case 1:
                    if (flashon == 1) {
                          btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    }else{
                          btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                  case 0:
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
            }
            break;
        }


    } else if (navbuttonremap == 2) {
        if (multires > 0) {
            if(testbit(multibuf,NAV_BUTTON)) {
                switch (XPLMGetDatai(HsiSelector)) {
                case 0:
                    XPLMSetDatai(NavButtonVorlocRemapableData, 1);
                case 1:
                    XPLMSetDatai(NavButtonVorlocRemapableData, 1);
                    break;
                case 2:
                    XPLMSetDatai(NavButtonLnavRemapableData, 1);
                    break;
                }
                lastappos = 1;
            }
        } else if(!testbit(multibuf,NAV_BUTTON)) {
                switch (XPLMGetDatai(HsiSelector)) {
                case 0:
                    XPLMSetDatai(NavButtonVorlocRemapableData, 0);
                case 1:
                    XPLMSetDatai(NavButtonVorlocRemapableData, 0);
                    break;
                case 2:
                    XPLMSetDatai(NavButtonLnavRemapableData, 0);
                    break;
                }
                lastappos = 1;
            }

        switch (XPLMGetDatai(HsiSelector)) {
        case 0:
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightVorlocRemapableData) > .50) {
                    navlightvorlocdata = 1;
                } else if (XPLMGetDataf(NavLightVorlocRemapableData) < .50) {
                    navlightvorlocdata = 0;
                }
                switch(navlightvorlocdata) {
                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightVorlocFlashRemapableData) > .50) {
                        navlightflashvorlocdata = 1;
                    } else if (XPLMGetDataf(NavLightVorlocFlashRemapableData) < .50) {
                        navlightflashvorlocdata = 0;
                    }
                    if (navlightflashvorlocdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(NavLightVorlocRemapableData)) {
                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashvorlocdata = XPLMGetDatai(NavLightVorlocFlashRemapableData);

                        if (navlightflashvorlocdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }

            } else if (lightdatareferencetype == 3) {
                navlightvorlocdata = XPLMGetDatai(NavLightVorlocRemapableData);
                switch(navlightvorlocdata) {
                  case 2:
                        btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                        break;
                      case 1:
                    if (flashon == 1) {
                          btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    }else{
                          btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                  case 0:
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
            }

            break;
        case 1:
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightVorlocRemapableData) > .50) {
                    navlightvorlocdata = 1;
                } else if (XPLMGetDataf(NavLightVorlocRemapableData) < .50) {
                    navlightvorlocdata = 0;
                }
                switch(navlightvorlocdata) {
                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightVorlocFlashRemapableData) > .50) {
                        navlightflashvorlocdata = 1;
                    } else if (XPLMGetDataf(NavLightVorlocFlashRemapableData) < .50) {
                        navlightflashvorlocdata = 0;
                    }
                    if (navlightflashvorlocdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(NavLightVorlocRemapableData)) {
                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashvorlocdata = XPLMGetDatai(NavLightVorlocFlashRemapableData);

                        if (navlightflashvorlocdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }

            } else if (lightdatareferencetype == 3) {
                navlightvorlocdata = XPLMGetDatai(NavLightVorlocRemapableData);
                switch(navlightvorlocdata) {
                  case 2:
                        btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                        break;
                      case 1:
                    if (flashon == 1) {
                          btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    }else{
                          btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                  case 0:
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
            }

            break;
        case 2:
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightLnavRemapableData) > .50) {
                    navlightlnavdata = 1;
                } else if (XPLMGetDataf(NavLightLnavRemapableData) < .50) {
                    navlightlnavdata = 0;
                }
                switch (navlightlnavdata) {
                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightLnavFlashRemapableData) > .50) {
                        navlightflashlnavdata = 1;
                    } else if (XPLMGetDataf(NavLightLnavFlashRemapableData) < .50) {
                        navlightflashlnavdata = 0;
                    }
                    if (navlightflashlnavdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch (XPLMGetDatai(NavLightLnavRemapableData)) {
                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashlnavdata = XPLMGetDatai(NavLightLnavFlashRemapableData);

                        if (navlightflashlnavdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }


            } else if (lightdatareferencetype == 3) {
                navlightlnavdata = XPLMGetDatai(NavLightLnavRemapableData);
                switch(XPLMGetDatai(ApNavStat)) {
                  case 2:
                        btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                        break;
                      case 1:
                    if (flashon == 1) {
                          btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    }else{
                          btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                  case 0:
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
           }
           break;
        }

    } else if (navbuttonremap == 3) {
        if (multires > 0) {
            if(testbit(multibuf,NAV_BUTTON)) {
                lastappos = XPLMGetDatai(NavButtonVorlocRemapableData);
                lastappos2 = XPLMGetDatai(NavButtonLnavRemapableData);
                switch (XPLMGetDatai(HsiSelector)) {
                case 0:
                    if (lastappos == 1) {
                        XPLMSetDatai(NavButtonVorlocRemapableData, 0);
                    } else {
                        XPLMSetDatai(NavButtonVorlocRemapableData, 1);
                    }
                    break;
                case 1:
                    if (lastappos == 1) {
                        XPLMSetDatai(NavButtonVorlocRemapableData, 0);
                    } else {
                        XPLMSetDatai(NavButtonVorlocRemapableData, 1);
                    }
                    break;
                case 2:
                    if (lastappos2 == 1) {
                        XPLMSetDatai(NavButtonLnavRemapableData, 0);
                    } else {
                        XPLMSetDatai(NavButtonLnavRemapableData, 1);
                    }

                    break;
                }

            }
        }

        switch (XPLMGetDatai(HsiSelector)) {
        case 0:
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightVorlocRemapableData) > .50) {
                    navlightvorlocdata = 1;
                } else if (XPLMGetDataf(NavLightVorlocRemapableData) < .50) {
                    navlightvorlocdata = 0;
                }
                switch(navlightvorlocdata) {
                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightVorlocFlashRemapableData) > .50) {
                        navlightflashvorlocdata = 1;
                    } else if (XPLMGetDataf(NavLightVorlocFlashRemapableData) < .50) {
                        navlightflashvorlocdata = 0;
                    }
                    if (navlightflashvorlocdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(NavLightVorlocRemapableData)) {
                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashvorlocdata = XPLMGetDatai(NavLightVorlocFlashRemapableData);

                        if (navlightflashvorlocdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }

            } else if (lightdatareferencetype == 3) {
                navlightvorlocdata = XPLMGetDatai(NavLightVorlocRemapableData);
                switch(XPLMGetDatai(NavLightVorlocRemapableData)) {
                  case 2:
                    btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                    break;
                  case 1:
                    if (flashon == 1) {
                    btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                  case 0:
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
            }

            break;
        case 1:
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightVorlocRemapableData) > .50) {
                    navlightvorlocdata = 1;
                } else if (XPLMGetDataf(NavLightVorlocRemapableData) < .50) {
                    navlightvorlocdata = 0;
                }
                switch(navlightvorlocdata) {
                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightVorlocFlashRemapableData) > .50) {
                        navlightflashvorlocdata = 1;
                    } else if (XPLMGetDataf(NavLightVorlocFlashRemapableData) < .50) {
                        navlightflashvorlocdata = 0;
                    }
                    if (navlightflashvorlocdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(NavLightVorlocRemapableData)) {
                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashvorlocdata = XPLMGetDatai(NavLightVorlocFlashRemapableData);

                        if (navlightflashvorlocdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }

            } else if (lightdatareferencetype == 3) {
                navlightvorlocdata = XPLMGetDatai(NavLightVorlocRemapableData);
                switch(XPLMGetDatai(NavLightVorlocRemapableData)) {
                  case 2:
                    btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                    break;
                  case 1:
                    if (flashon == 1) {
                    btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                  case 0:
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
            }

            break;
        case 2:
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(NavLightLnavRemapableData) > .50) {
                    navlightlnavdata = 1;
                } else if (XPLMGetDataf(NavLightLnavRemapableData) < .50) {
                    navlightlnavdata = 0;
                }
                switch (navlightlnavdata) {
                case 1:
                    btnleds |= (1<<2);
                    break;
                case 0:
                    if (XPLMGetDataf(NavLightLnavFlashRemapableData) > .50) {
                        navlightflashlnavdata = 1;
                    } else if (XPLMGetDataf(NavLightLnavFlashRemapableData) < .50) {
                        navlightflashlnavdata = 0;
                    }
                    if (navlightflashlnavdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<2);
                        } else {
                            btnleds &= ~(1<<2);
                        }
                    } else {
                        btnleds &= ~(1<<2);
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch (XPLMGetDatai(NavLightLnavRemapableData)) {
                    case 1:
                        btnleds |= (1<<2);
                        break;
                    case 0:
                        navlightflashlnavdata = XPLMGetDatai(NavLightLnavFlashRemapableData);

                        if (navlightflashlnavdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<2);
                            } else {
                                btnleds &= ~(1<<2);
                            }
                        } else {
                            btnleds &= ~(1<<2);
                        }
                        break;
                    }


            } else if (lightdatareferencetype == 3) {
                navlightlnavdata = XPLMGetDatai(NavLightLnavRemapableData);
                switch(XPLMGetDatai(ApNavStat)) {
                  case 2:
                        btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
                        break;
                      case 1:
                    if (flashon == 1) {
                          btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
                    }else{
                          btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    }
                    break;
                  case 0:
                        btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
                    break;
                }
           }
           break;
        }

    } else {
        if (multires > 0) {
          if(testbit(multibuf,NAV_BUTTON)) {
            if(xpanelsfnbutton == 1) {
                xpanelscrstoggle = !xpanelscrstoggle;
            }
            if(xpanelsfnbutton == 0) {
                XPLMCommandOnce(ApNavBtn);
                lastappos = 1;
            }

          }
        }

    switch(XPLMGetDatai(ApNavStat)) {
	  case 2:
            btnleds |= (1<<2);   // * set bit 2 in btnleds to 1 *
            break;
          case 1:
	    if (flashon == 1) {
              btnleds |= (1<<2);   // * set bit 1 in btnleds to 1 *
	    }else{
              btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
	    }
	    break;
	  case 0:
            btnleds &= ~(1<<2);   // * clear bit 2 in btnleds to 0 *
	    break;
	}
   }
}

// ***************** IAS Button and light ******************
void process_ias_button()
{

       if (iasbuttonremap == 1) {
           if (multires > 0) {
               if (testbit(multibuf, IAS_BUTTON)) {
                   if (xpanelsfnbutton == 1) {
                       XPLMCommandOnce(IasChangeoverButtonRemapableCmd);
                   }
                   if (xpanelsfnbutton == 0) {
                       XPLMCommandOnce(IasButtonRemapableCmd);
                       lastappos = 1;
                   }

               }
           }
           if (lightdatareferencetype == 1) {
               if (XPLMGetDataf(IasLightRemapableData) > .50) {
                   iaslightdata = 1;

               } else if (XPLMGetDataf(IasLightRemapableData) < .50) {
                   iaslightdata = 0;
               }
               switch(iaslightdata) {
               case 1:
                   btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   break;
               case 0:
                   if (XPLMGetDataf(IasLightFlashRemapableData) > .50) {
                       iaslightflashdata = 1;
                   } else if (XPLMGetDataf(IasLightFlashRemapableData) < .50) {
                       iaslightflashdata = 0;
                   }
                   if (iaslightflashdata) {
                       if (flashon == 1) {
                           btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                       } else {
                           btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                       }
                   } else {
                       btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   }
                   break;
               }

           } else if (lightdatareferencetype == 2) {

                   switch(XPLMGetDatai(IasLightRemapableData)) {
                   case 1:
                       btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                       break;
                   case 0:
                       iaslightflashdata = XPLMGetDatai(IasLightFlashRemapableData);

                       if (iaslightflashdata) {
                           if (flashon == 1) {
                               btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                           } else {
                               btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                           }
                       } else {
                           btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                       }
                       break;
                   }

            } else if (lightdatareferencetype == 3) {
               iaslightdata = XPLMGetDatai(IasLightRemapableData);
               switch(iaslightdata) {
               case 2:
                   btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   break;
                 case 1:
                   if (flashon == 1) {
                     btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   }
                   if (flashon == 0) {
                     btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   }
                   break;
                 case 0:
                   btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   break;
               }

            }


       } else  if (iasbuttonremap == 2) {
           if (multires > 0) {
               if (testbit(multibuf, IAS_BUTTON)) {
                   if (xpanelsfnbutton == 1) {
                       XPLMSetDatai(IasChangeoverButtonRemapableData, 1);
                   }
                   if (xpanelsfnbutton == 0) {
                       XPLMSetDatai(IasButtonRemapableData, 1);
                       lastappos = 1;
                   }
               }
           } else if (!testbit(multibuf, IAS_BUTTON)) {
                   if (xpanelsfnbutton == 1) {
                       XPLMSetDatai(IasChangeoverButtonRemapableData, 0);
                   }
                   if (xpanelsfnbutton == 0) {
                       XPLMSetDatai(IasButtonRemapableData, 0);
                       lastappos = 1;
                   }
           }
           if (lightdatareferencetype == 1) {
               if (XPLMGetDataf(IasLightRemapableData) > .50) {
                   iaslightdata = 1;
               } else if (XPLMGetDataf(IasLightRemapableData) < .50) {
                   iaslightdata = 0;
               }
               switch(iaslightdata) {
               case 1:
                   btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   break;
               case 0:
                   if (XPLMGetDataf(IasLightFlashRemapableData) > .50) {
                       iaslightflashdata = 1;
                   } else if (XPLMGetDataf(IasLightFlashRemapableData) < .50) {
                       iaslightflashdata = 0;
                   }
                   if (iaslightflashdata) {
                       if (flashon == 1) {
                           btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                       } else {
                           btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                       }
                   } else {
                       btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   }
                   break;
               }

           } else if (lightdatareferencetype == 2) {

                   switch(XPLMGetDatai(IasLightRemapableData)) {
                   case 1:
                       btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                       break;
                   case 0:
                       iaslightflashdata = XPLMGetDatai(IasLightFlashRemapableData);

                       if (iaslightflashdata) {
                           if (flashon == 1) {
                               btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                           } else {
                               btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                           }
                       } else {
                           btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                       }
                       break;
                   }

           } else if (lightdatareferencetype == 3) {
               iaslightdata = XPLMGetDatai(IasLightRemapableData);
               switch(iaslightdata) {
               case 2:
                   btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   break;
               case 1:
                   if (flashon == 1) {
                       btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   }
                   if (flashon == 0) {
                       btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   }
                   break;
               case 0:
                   btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   break;
               }
           }


       } else  if (iasbuttonremap == 3) {
           if (multires > 0) {
               if (xpanelsfnbutton == 1) {
                   if (testbit(multibuf, IAS_BUTTON)) {
                       XPLMSetDatai(IasChangeoverButtonRemapableData, 1);
                   } else if (!testbit(multibuf, IAS_BUTTON)) {
                       XPLMSetDatai(IasChangeoverButtonRemapableData, 0);
                   }
               }
               if (xpanelsfnbutton == 0) {
                   if (testbit(multibuf, IAS_BUTTON)) {
                       lastappos = XPLMGetDatai(IasButtonRemapableData);
                       if (lastappos == 1) {
                           XPLMSetDatai(IasButtonRemapableData, 0);
                       } else {
                           XPLMSetDatai(IasButtonRemapableData, 1);

                       }
                   }
               }
           }
           if (lightdatareferencetype == 1) {
               if (XPLMGetDataf(IasLightRemapableData) > .50) {
                   iaslightdata = 1;
               } else if (XPLMGetDataf(IasLightRemapableData) < .50) {
                   iaslightdata = 0;
               }
               switch(iaslightdata) {
               case 1:
                   btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   break;
               case 0:
                   if (XPLMGetDataf(IasLightFlashRemapableData) > .50) {
                       iaslightflashdata = 1;
                   } else if (XPLMGetDataf(IasLightFlashRemapableData) < .50) {
                       iaslightflashdata = 0;
                   }
                   if (iaslightflashdata) {
                       if (flashon == 1) {
                           btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                       } else {
                           btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                       }
                   } else {
                       btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   }
                   break;
               }

           } else if (lightdatareferencetype == 2) {

                   switch(XPLMGetDatai(IasLightRemapableData)) {
                   case 1:
                       btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                       break;
                   case 0:
                       iaslightflashdata = XPLMGetDatai(IasLightFlashRemapableData);

                       if (iaslightflashdata) {
                           if (flashon == 1) {
                               btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                           } else {
                               btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                           }
                       } else {
                           btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                       }
                       break;
                   }

           } else if (lightdatareferencetype == 3) {
               iaslightdata = XPLMGetDatai(IasLightRemapableData);
               switch(iaslightdata) {
               case 2:
                   btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   break;
               case 1:
                   if (flashon == 1) {
                       btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
                   }
                   if (flashon == 0) {
                       btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   }
                   break;
               case 0:
                   btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
                   break;
               }
           }


     } else {
        if (multires > 0) {
          if(testbit(multibuf,IAS_BUTTON)) {
            if(xpanelsfnbutton == 1) {
                 if (XPLMGetDatai(AirspeedIsMach) == 1) {
                     XPLMCommandOnce(ApKnotsMachTgl);
                 } else {
                     XPLMCommandOnce(ApKnotsMachTgl);
                 }
             }
             if(xpanelsfnbutton == 0) {
                 XPLMCommandOnce(ApIasBtn);
                 lastappos = 1;
             }

          }
        }
        switch(XPLMGetDatai(ApIasStat)) {
          case 2:
            btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
            break;
          case 1:
            if (flashon == 1) {
              btnleds |= (1<<3);   // * set bit 3 in btnleds to 1 *
            }
            if (flashon == 0) {
              btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
            }
            break;
          case 0:
            btnleds &= ~(1<<3);   // * clear bit 3 in btnleds to 0 *
            break;
        }
    }
}

// ***************** ALT Button and light *******************
void process_alt_button()
{
     if (altbuttonremap == 1) {
         if (multires > 0) {
             if (testbit(multibuf, ALT_BUTTON)) {
                 XPLMCommandOnce(AltButtonRemapableCmd);
                 lastappos = 1;
             }
         }
         if (lightdatareferencetype == 0) {
             switch(XPLMGetDatai(ApAltStat)) {
             case 2:
                 btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 break;
             case 1:
                 if (flashon == 1) {
                     btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 }else{
                     btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 }
                 break;
             case 0:
                 btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 break;
             }
             if (XPLMGetDatai(ApMstrStat) == 0) {
                   btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
             }

         } else if (lightdatareferencetype == 1) {
             if (XPLMGetDataf(AltLightRemapableData) > .50) {
                 altlightdata = 1;
             } else if (XPLMGetDataf(AltLightRemapableData) < .50) {
                 altlightdata = 0;
             }
             switch(altlightdata) {
             case 1:
                 btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 break;
             case 0:
                 if (XPLMGetDataf(AltLightFlashRemapableData) > .50) {
                     altlightflashdata = 1;
                 } else if (XPLMGetDataf(AltLightFlashRemapableData) < .50) {
                     altlightflashdata = 0;
                 }
                 if (altlightflashdata) {
                     if (flashon == 1) {
                         btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                     } else {
                         btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                     }
                 } else {
                     btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 }
                 break;
             }

         } else if (lightdatareferencetype == 2) {

                 switch(XPLMGetDatai(AltLightRemapableData)) {
                 case 1:
                     btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                     break;
                 case 0:
                     altlightflashdata = XPLMGetDatai(AltLightFlashRemapableData);

                     if (altlightflashdata) {
                         if (flashon == 1) {
                             btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                         } else {
                             btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                         }
                     } else {
                         btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                     }
                     break;
                 }

         } else if (lightdatareferencetype == 3) {
             altlightdata = XPLMGetDatai(AltLightRemapableData);
             switch(altlightdata) {
             case 2:
                 btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 break;
             case 1:
                 if (flashon == 1) {
                     btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 } else {
                     btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 }
                 break;
             case 0:
                 btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 break;
             }
         }

     } else if (altbuttonremap == 2) {
         if (multires > 0) {
             if (testbit(multibuf, ALT_BUTTON)) {
                 XPLMSetDatai(AltButtonRemapableData, 1);
                 lastappos = 1;
             } else if (!testbit(multibuf, ALT_BUTTON)) {
                 XPLMSetDatai(AltButtonRemapableData, 0);
                 lastappos = 1;
             }
         }
         if (lightdatareferencetype == 1) {
             if (XPLMGetDataf(AltLightRemapableData) > .50) {
                 altlightdata = 1;
             } else if (XPLMGetDataf(AltLightRemapableData) < .50) {
                 altlightdata = 0;
             }
             switch(altlightdata) {
             case 1:
                 btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 break;
             case 0:
                 if (XPLMGetDataf(AltLightFlashRemapableData) > .50) {
                     altlightflashdata = 1;
                 } else if (XPLMGetDataf(AltLightFlashRemapableData) < .50) {
                     altlightflashdata = 0;
                 }
                 if (altlightflashdata) {
                     if (flashon == 1) {
                         btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                     } else {
                         btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                     }
                 } else {
                     btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 }
                 break;
             }

        } else if (lightdatareferencetype == 2) {

                 switch(XPLMGetDatai(AltLightRemapableData)) {
                 case 1:
                     btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                     break;
                 case 0:
                     altlightflashdata = XPLMGetDatai(AltLightFlashRemapableData);

                     if (altlightflashdata) {
                         if (flashon == 1) {
                             btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                         } else {
                             btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                         }
                     } else {
                         btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                     }
                     break;
                 }

         } else if (lightdatareferencetype == 3) {
             altlightdata = XPLMGetDatai(AltLightRemapableData);
             switch(altlightdata) {
             case 2:
                 btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 break;
             case 1:
                 if (flashon == 1) {
                     btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 }else{
                     btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 }
                 break;
             case 0:
                 btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 break;
             }

         }

     } else if (altbuttonremap == 3) {
         if (multires > 0) {
             if (testbit(multibuf, ALT_BUTTON)) {
                 lastappos = XPLMGetDatai(AltButtonRemapableData);
                 if (lastappos == 1) {
                     XPLMSetDatai(AltButtonRemapableData, 0);
                 } else {
                     XPLMSetDatai(AltButtonRemapableData, 1);
                 }

             }
         }
         if (lightdatareferencetype == 1) {
             if (XPLMGetDataf(AltLightRemapableData) > .50) {
                 altlightdata = 1;
             } else if (XPLMGetDataf(AltLightRemapableData) < .50) {
                 altlightdata = 0;
             }
             switch(altlightdata) {
             case 1:
                 btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 break;
             case 0:
                 if (XPLMGetDataf(AltLightFlashRemapableData) > .50) {
                     altlightflashdata = 1;
                 } else if (XPLMGetDataf(AltLightFlashRemapableData) < .50) {
                     altlightflashdata = 0;
                 }
                 if (altlightflashdata) {
                     if (flashon == 1) {
                         btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                     } else {
                         btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                     }
                 } else {
                     btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 }
                 break;
             }

        } else if (lightdatareferencetype == 2) {

                 switch(XPLMGetDatai(AltLightRemapableData)) {
                 case 1:
                     btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                     break;
                 case 0:
                     altlightflashdata = XPLMGetDatai(AltLightFlashRemapableData);

                     if (altlightflashdata) {
                         if (flashon == 1) {
                             btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                         } else {
                             btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                         }
                     } else {
                         btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                     }
                     break;
                 }

         } else if (lightdatareferencetype == 3) {
             altlightdata = XPLMGetDatai(AltLightRemapableData);
             switch(altlightdata) {
             case 2:
                 btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 break;
             case 1:
                 if (flashon == 1) {
                     btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
                 }else{
                     btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 }
                 break;
             case 0:
                 btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
                 break;
             }

         }

     } else {
        if (multires > 0) {
          if(testbit(multibuf,ALT_BUTTON)) {
              XPLMCommandOnce(ApAltBtn);
              lastappos = 1;
          }
        }
    switch(XPLMGetDatai(ApAltStat)) {
	  case 2:
            btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
            break;
          case 1:
	    if (flashon == 1) {
              btnleds |= (1<<4);   // * set bit 4 in btnleds to 1 *
	    }else{    
              btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
	    }
            break;
          case 0:
            btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
            break;
	} 
	
	if (XPLMGetDatai(ApMstrStat) == 0) {
          btnleds &= ~(1<<4);   // * clear bit 4 in btnleds to 0 *
	}
    }
}

// ***************** VS Button and light *******************
void process_vs_button()
{

    if (vsbuttonremap == 1) {
        if (multires > 0) {
            if (testbit(multibuf, VS_BUTTON)) {
                XPLMCommandOnce(VsButtonRemapableCmd);
                lastappos = 1;
            }
        }
        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(VsLightRemapableData) > .50) {
                vslightdata = 1;
            } else if (XPLMGetDataf(VsLightRemapableData) < .50) {
                vslightdata = 0;

            }
            switch(vslightdata) {
            case 1:
                btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(VsLightFlashRemapableData) > .50) {
                    vslightflashdata = 1;
                } else if (XPLMGetDataf(VsLightFlashRemapableData) < .50) {
                    vslightflashdata = 0;
                }
                if (vslightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                }
                break;
            }

         } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(VsLightRemapableData)) {
                case 1:
                    btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                    break;
                case 0:
                    vslightflashdata = XPLMGetDatai(VsLightFlashRemapableData);

                    if (vslightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                    }
                    break;
                }


        } else if (lightdatareferencetype == 3) {
            vslightdata = XPLMGetDatai(VsLightRemapableData);
            switch(vslightdata) {
            case 2:
                btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                break;
            case 1:
                if (flashon == 0) {
                    btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                } else {
                    btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                }
                break;
            case 0:
                btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                break;
            }

        }


    } else if (vsbuttonremap == 2) {
            if (multires > 0) {
                if (testbit(multibuf, VS_BUTTON)) {
                    XPLMSetDatai(VsButtonRemapableData, 1);
                    lastappos = 1;
                } else if (!testbit(multibuf, VS_BUTTON)) {
                    XPLMSetDatai(VsButtonRemapableData, 0);
                    lastappos = 1;

                }
            }
            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(VsLightRemapableData) > .50) {
                    vslightdata = 1;
                } else if (XPLMGetDataf(VsLightRemapableData) < .50) {
                    vslightdata = 0;
                }
                switch(vslightdata) {
                case 1:
                    btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                    break;
                case 0:
                    if (XPLMGetDataf(VsLightFlashRemapableData) > .50) {
                        vslightflashdata = 1;
                    } else if (XPLMGetDataf(VsLightFlashRemapableData) < .50) {
                        vslightflashdata = 0;
                    }
                    if (vslightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                    switch(XPLMGetDatai(VsLightRemapableData)) {
                    case 1:
                        btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                        break;
                    case 0:
                        vslightflashdata = XPLMGetDatai(VsLightFlashRemapableData);

                        if (vslightflashdata) {
                            if (flashon == 1) {
                                btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                            } else {
                                btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                            }
                        } else {
                            btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                        }
                        break;
                    }


            } else if (lightdatareferencetype == 3) {
                vslightdata = XPLMGetDatai(VsLightRemapableData);
                switch(vslightdata) {
                case 2:
                    btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                    break;
                case 1:
                    if (flashon == 0) {
                        btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                    }
                    break;
                case 0:
                    btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                    break;
                }


            }

    } else if (vsbuttonremap == 3) {

        if (multires > 0) {
            if (testbit(multibuf, VS_BUTTON)) {
                lastappos = XPLMGetDatai(VsButtonRemapableData);
                if (lastappos == 1) {
                    XPLMSetDatai(VsButtonRemapableData, 0);
                } else {
                    XPLMSetDatai(VsButtonRemapableData, 1);
                }
            }
        }
        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(VsLightRemapableData) > .50) {
                vslightdata = 1;
            } else if (XPLMGetDataf(VsLightRemapableData) < .50) {
                vslightdata = 0;
            }
            switch(vslightdata) {
            case 1:
                btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(VsLightFlashRemapableData) > .50) {
                    vslightflashdata = 1;
                } else if (XPLMGetDataf(VsLightFlashRemapableData) < .50) {
                    vslightflashdata = 0;
                }
                if (vslightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                }
                break;
            }

        } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(VsLightRemapableData)) {
                case 1:
                    btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                    break;
                case 0:
                    vslightflashdata = XPLMGetDatai(VsLightFlashRemapableData);

                    if (vslightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                    }
                    break;
                }


        } else if (lightdatareferencetype == 3) {
            vslightdata = XPLMGetDatai(VsLightRemapableData);
            switch(vslightdata) {
            case 2:
                btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                break;
            case 1:
                if (flashon == 0) {
                    btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
                } else {
                    btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                }
                break;
            case 0:
                btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
                break;
            }


        }

    } else {
        if (multires > 0) {
            if(testbit(multibuf,VS_BUTTON)) {
                XPLMCommandOnce(ApVsBtn);
                upapvsf = 0;
                XPLMSetDataf(ApVs, upapvsf);
                lastappos = 1;
             }
        }
        switch(XPLMGetDatai(ApVsStat)) {
        case 2:
            btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
            break;
        case 1:
            if (flashon == 0) {
                btnleds |= (1<<5);   // * set bit 5 in btnleds to 1 *
            } else {
                btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
            }
            break;
        case 0:
            btnleds &= ~(1<<5);   // * clear bit 5 in btnleds to 0 *
            break;
        }
    }
}

// ***************** APR Button and light *******************
void process_apr_button()
{

    if (aprbuttonremap == 1) {
        if (multires > 0) {
          if(testbit(multibuf,APR_BUTTON)) {
              XPLMCommandOnce(AprButtonRemapableCmd);
              lastappos = 1;
          }
        }

        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(AprLightRemapableData) > .50) {
                aprlightdata = 1;
            } else if (XPLMGetDataf(AprLightRemapableData) < .50) {
                aprlightdata = 0;
            }
            switch(aprlightdata) {
            case 1:
                btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(AprLightFlashRemapableData) > .50) {
                    aprlightflashdata = 1;
                } else if (XPLMGetDataf(AprLightFlashRemapableData) < .50) {
                        aprlightflashdata = 0;
                }

                if (aprlightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                }
                break;
            }

        } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(AprLightRemapableData)) {
                case 1:
                    btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                    break;
                case 0:
                    aprlightflashdata = XPLMGetDatai(AprLightFlashRemapableData);


                    if (aprlightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                    }
                    break;
                }

        } else if (lightdatareferencetype == 3) {
            aprlightdata = XPLMGetDatai(AprLightRemapableData);
            switch(aprlightdata) {
            case 2:
                btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                break;
            case 1:
                if (flashon == 1) {
                    btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                } else {
                    btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                }
                break;
            case 0:
                btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                break;
            }
        }


    } else if (aprbuttonremap == 2) {
            if (multires > 0) {
              if(testbit(multibuf,APR_BUTTON)) {
                  XPLMSetDatai(AprButtonRemapableData, 1);
                  lastappos = 1;

              } else if(!testbit(multibuf,APR_BUTTON)) {
                 XPLMSetDatai(AprButtonRemapableData, 0);
                 lastappos = 1;

              }
            }

            if (lightdatareferencetype == 1) {
                if (XPLMGetDataf(AprLightRemapableData) > .50) {
                    aprlightdata = 1;
                } else if (XPLMGetDataf(AprLightRemapableData) < .50) {
                    aprlightdata = 0;
                }
                switch(aprlightdata) {
                case 1:
                    btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                    break;
                case 0:
                    if (XPLMGetDataf(AprLightFlashRemapableData) > .50) {
                        aprlightflashdata = 1;
                    } else if (XPLMGetDataf(AprLightFlashRemapableData) < .50) {
                         aprlightflashdata = 0;
                    }

                    if (aprlightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                    }
                    break;
                }

            } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(AprLightRemapableData)) {
                case 1:
                    btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                    break;
                case 0:
                    aprlightflashdata = XPLMGetDatai(AprLightFlashRemapableData);

                    if (aprlightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                    }
                    break;
                }


            } else if (lightdatareferencetype == 3) {
                aprlightdata = XPLMGetDatai(AprLightRemapableData);
                switch(XPLMGetDatai(ApAprStat)) {
                case 2:
                    btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                    break;
                case 1:
                    if (flashon == 1) {
                        btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                    }else{
                          btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                    }
                    break;
                case 0:
                    btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                    break;
                }
            }

    } else if (aprbuttonremap == 3) {

        if (multires > 0) {
          if(testbit(multibuf,APR_BUTTON)) {
              lastappos = XPLMGetDatai(AprButtonRemapableData);
              if (lastappos == 1) {
                  XPLMSetDatai(AprButtonRemapableData, 0);
              } else {
                  XPLMSetDatai(AprButtonRemapableData, 1);
              }
          }
        }

        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(AprLightRemapableData) > .50) {
                aprlightdata = 1;
            } else if (XPLMGetDataf(AprLightRemapableData) < .50) {
                aprlightdata = 0;
            }
            switch(aprlightdata) {
            case 1:
                btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(AprLightFlashRemapableData) > .50) {
                    aprlightflashdata = 1;
                } else if (XPLMGetDataf(AprLightFlashRemapableData) < .50) {
                     aprlightflashdata = 0;
                }

                if (aprlightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                }
                break;
            }

        } else if (lightdatareferencetype == 2) {

            switch(XPLMGetDatai(AprLightRemapableData)) {
            case 1:
                btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                break;
            case 0:
                aprlightflashdata = XPLMGetDatai(AprLightFlashRemapableData);

                if (aprlightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                }
                break;
            }


        } else if (lightdatareferencetype == 3) {
            aprlightdata = XPLMGetDatai(AprLightRemapableData);
            switch(XPLMGetDatai(ApAprStat)) {
            case 2:
                btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                break;
            case 1:
                if (flashon == 1) {
                    btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
                }else{
                      btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                }
                break;
            case 0:
                btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
                break;
            }
        }

    } else {
        if (multires > 0) {
          if(testbit(multibuf,APR_BUTTON)) {
            if(xpanelsfnbutton == 1) {
                rhdgf = XPLMGetDataf(MHdg);
                XPLMSetDataf(ApCrs, rhdgf);
            }
            if(xpanelsfnbutton == 0) {
                XPLMCommandOnce(ApAprBtn);
                lastappos = 1;
            }
          }
        }
    switch(XPLMGetDatai(ApAprStat)) {
	  case 2:
            btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
            break;
          case 1:
	    if (flashon == 1) {
              btnleds |= (1<<6);   // * set bit 6 in btnleds to 1 *
	    }else{   
              btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
	    }
            break;
          case 0:
            btnleds &= ~(1<<6);   // * clear bit 6 in btnleds to 0 *
            break;
        }
    }
}

// ***************** REV Button and light *******************
void process_rev_button()
{
    if (revbuttonremap == 1) {
        if (multires > 0) {
            if(testbit(multibuf,REV_BUTTON)) {
                XPLMCommandOnce(RevButtonRemapableCmd);
                lastappos = 1;
            }
        }


        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(RevLightRemapableData) > .50) {
                revlightdata = 1;
            } else if (XPLMGetDataf(RevLightRemapableData) < .50) {
                revlightdata = 0;
            }
            switch(revlightdata) {
            case 1:
                btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(RevLightFlashRemapableData) > .50) {
                    revlightflashdata = 1;
                } else if (XPLMGetDataf(RevLightFlashRemapableData) < .50) {
                    revlightflashdata = 0;
                }
                if (revlightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                }
                break;
            }

          } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(RevLightRemapableData)) {
                case 1:
                    btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                    break;
                case 0:
                    revlightflashdata = XPLMGetDatai(RevLightFlashRemapableData);

                    if (revlightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                    }
                    break;
                }


        } else if (lightdatareferencetype == 3) {
            revlightdata = XPLMGetDatai(RevLightRemapableData);
            switch(revlightdata) {
            case 2:
                btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                break;
            case 1:
                if (flashon == 1) {
                    btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                } else {
                    btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                }

                break;
            case 0:
                btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                break;
            }

        }

    } else if (revbuttonremap == 2) {
        if (multires > 0) {
            if(testbit(multibuf,REV_BUTTON)) {
                XPLMSetDatai(RevButtonRemapableData, 1);
                lastappos = 1;
            } else if(!testbit(multibuf,REV_BUTTON)) {
                XPLMSetDatai(RevButtonRemapableData, 0);
                lastappos = 1;
            }
        }

        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(RevLightRemapableData) > .50) {
                revlightdata = 1;
            } else if (XPLMGetDataf(RevLightRemapableData) < .50) {
                revlightdata = 0;
            }
            switch(revlightdata) {
            case 1:
                btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(RevLightFlashRemapableData) > .50) {
                    revlightflashdata = 1;
                } else if (XPLMGetDataf(RevLightFlashRemapableData) < .50) {
                    revlightflashdata = 0;
                }
                if (revlightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                }
                break;
            }

        } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(RevLightRemapableData)) {
                case 1:
                    btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                    break;
                case 0:
                    revlightflashdata = XPLMGetDatai(RevLightFlashRemapableData);

                    if (revlightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                    }
                    break;
                }

        } else if (lightdatareferencetype == 3) {
            revlightdata = XPLMGetDatai(RevLightRemapableData);
            switch(revlightdata) {
            case 2:
                btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                break;
            case 1:
                if (flashon == 1) {
                    btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                } else {
                    btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                }

                break;
            case 0:
                btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                break;
            }

        }

    } else if (revbuttonremap == 3) {

        if (multires > 0) {
            if(testbit(multibuf,REV_BUTTON)) {
                lastappos = XPLMGetDatai(RevButtonRemapableData);
                if (lastappos == 1) {
                    XPLMSetDatai(RevButtonRemapableData, 0);
                } else {
                    XPLMSetDatai(RevButtonRemapableData, 1);
                }
            }
        }

        if (lightdatareferencetype == 1) {
            if (XPLMGetDataf(RevLightRemapableData) > .50) {
                revlightdata = 1;
            } else if (XPLMGetDataf(RevLightRemapableData) < .50) {
                revlightdata = 0;
            }
            switch(revlightdata) {
            case 1:
                btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                break;
            case 0:
                if (XPLMGetDataf(RevLightFlashRemapableData) > .50) {
                    revlightflashdata = 1;
                } else if (XPLMGetDataf(RevLightFlashRemapableData) < .50) {
                    revlightflashdata = 0;
                }
                if (revlightflashdata) {
                    if (flashon == 1) {
                        btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                    } else {
                        btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                    }
                } else {
                    btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                }
                break;
            }

        } else if (lightdatareferencetype == 2) {

                switch(XPLMGetDatai(RevLightRemapableData)) {
                case 1:
                    btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                    break;
                case 0:
                    revlightflashdata = XPLMGetDatai(RevLightFlashRemapableData);

                    if (revlightflashdata) {
                        if (flashon == 1) {
                            btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                        } else {
                            btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                        }
                    } else {
                        btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                    }
                    break;
                }

        } else if (lightdatareferencetype == 3) {
            revlightdata = XPLMGetDatai(RevLightRemapableData);
            switch(revlightdata) {
            case 2:
                btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                break;
            case 1:
                if (flashon == 1) {
                    btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
                } else {
                    btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                }

                break;
            case 0:
                btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
                break;
            }

        }

    } else {
        if (multires > 0) {
            if(testbit(multibuf,REV_BUTTON)) {
                XPLMCommandOnce(ApRevBtn);
                lastappos = 1;
            }
        }
        switch(XPLMGetDatai(ApRevStat)) {
        case 2:
            btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
            break;
        case 1:
            if (flashon == 1) {
                btnleds |= (1<<7);   // * set bit 7 in btnleds to 1 *
            } else {
                btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
            }

            break;
        case 0:
            btnleds &= ~(1<<7);   // * clear bit 7 in btnleds to 0 *
            break;
        }

    }

}

// ***************** Flaps Switch *******************
void process_flaps_switch()
{
    if (multires > 0) {
        if(testbit(multibuf,FLAPS_UP_SWITCH)) {
            if (flapsupremap == 1) {
                XPLMCommandOnce(FlapsUpRemapableCmd);
            } else {
                if (BatPwrIsOn()) {
                    XPLMCommandOnce(FlapsUp);
                }
            }
        }
        if(testbit(multibuf,FLAPS_DN_SWITCH)) {
            if (flapsdnremap == 1) {
                XPLMCommandOnce(FlapsDnRemapableCmd);
            } else {
                if (BatPwrIsOn()) {
                    XPLMCommandOnce(FlapsDn);
                }
            }
        }
    }
 }

// *************** Setupapi.libSetupapi.lib Wheel *********************

double lerp(double a, double b, double t)
{
    return a + (b - a) * t;
}

void process_trim_wheel(float dt)
{
    static double timeFromLastUpTrimInput = 0.0f;
    static double timeFromLastDownTrimInput = 0.0f;

    timeFromLastUpTrimInput += dt;
    timeFromLastDownTrimInput += dt;

    float TrimWheelDeflectionTime = XPLMGetDataf(XPLMFindDataRef("sim/aircraft/controls/acf_elev_trim_time"));
    double MaxWithDeflection = dynamicTrimMaxVal * ((double)TrimWheelDeflectionTime / 8.0);

    int i;
    if (multires > 0) {
        if(testbit(multibuf,TRIM_WHEEL_UP)) {
            int commandMult = 1;
            if (dynamicTrimWheel == 1) {
                if (timeFromLastUpTrimInput < dynamicTrimAccelerationPoint) {
                    double lerpFactor = (fmin(dynamicTrimAccelerationPoint, timeFromLastUpTrimInput)) / (dynamicTrimAccelerationPoint);
                    commandMult = (int) floor(lerp(MaxWithDeflection, dynamicTrimMinVal, lerpFactor) + 0.5f);
                }
            }
			else {
				commandMult = trimspeed;
			}

#ifdef TRIM_DEBUG_LOG
            char buf[500];
            sprintf(buf, "timeFromLastUpTrimInput: %f, speedMult: %d, MaxWithDeflection %f, TrimWheelDeflectionTime %f\n", timeFromLastUpTrimInput, commandMult, MaxWithDeflection, TrimWheelDeflectionTime);
            XPLMDebugString(buf);
#endif //TRIM_DEBUG_LOG

            timeFromLastUpTrimInput = 0.0f;

            for(i = 0; i < commandMult; ++i) {
                if (trimupremap == 1) {
                    XPLMCommandOnce(TrimUpRemapableCmd);
                } else {
                    XPLMCommandOnce(PitchTrimUp);
                }
            }
        }
        if(testbit(multibuf,TRIM_WHEEL_DN)) {
            int commandMult = 1;
            if (dynamicTrimWheel == 1) {
                if (timeFromLastDownTrimInput < dynamicTrimAccelerationPoint) {
                    double lerpFactor = (fmin(dynamicTrimAccelerationPoint, timeFromLastDownTrimInput)) / (dynamicTrimAccelerationPoint);
                    commandMult = (int)floor(lerp(MaxWithDeflection, dynamicTrimMinVal, lerpFactor) + 0.5f);
                }
            }
			else {
				commandMult = trimspeed;
			}

            timeFromLastDownTrimInput = 0.0f;

            for(i = 0; i < commandMult; ++i) {
                if (trimdnremap == 1) {
                    XPLMCommandOnce(TrimDnRemapableCmd);
                } else {
                    XPLMCommandOnce(PitchTrimDn);
                }
            }
        }
    }
}

// ***************** Flasher for Button LEDS *******************
void process_multi_flash()
{
    flashcnt++;
    if (flashcnt < 50) {
        flashon = 0;
    }
    if (flashcnt > 50) {
        flashon = 1;
    }
    if (flashcnt == 100) {
        flashcnt = 0;
    }
}

// ***************** Blank Display *******************
void process_multi_blank_display()
{
    if (!AvPwrIsOn()) {
        multiseldis = 5;
    }
    if (!BatPwrIsOn()) {
        multiseldis = 5;
    }
    if(switchcnt > 0)  {
        if (XPLMGetDatai(SwitchAvOwnedDataRef) == 0) {
            if (AvPwrOnConfig == 0) {
               multiseldis = 5;
            }
        }

    } else {

    }



}


// ***** Multi Panel Process ******
void process_multi_panel(float dt)

{

    XPLMSetDatai(MultiKnobIncTicksOwnedDataRef, 0);
    XPLMSetDatai(MultiKnobDecTicksOwnedDataRef, 0);

    // ******* Only do a read if something new to be read ********
    hid_set_nonblocking(multihandle, 1);
    int multi_safety_cntr = 30;
    do {
        multires = hid_read(multihandle, multibuf, sizeof(multibuf));
        process_multi_panel_datareference_values();
        if (multibuf[0] == 0) {
            multiseldis = multiswitchpos;
            if (multiswitchpos == 1) {
                upapaltf = XPLMGetDataf(ApAlt);
                upapalt = (int)(upapaltf);
                upapvsf = XPLMGetDataf(ApVs);
                upapvs = (int)(upapvsf);
            } else if (multiswitchpos == 2) {
                apasf = XPLMGetDataf(ApAs);
                apasout = (int)(apasf);
            } else if (multiswitchpos == 3) {
                upaphdgf = XPLMGetDataf(ApHdg);
                upaphdg = (int)(upaphdgf);
            } else if (multiswitchpos == 4) {
                upapcrsf = XPLMGetDataf(ApCrs);
                upapcrs = (int)(upapcrsf);
            }
        }
        process_alt_switch();
        process_vs_switch();
        process_ias_switch();
        process_hdg_switch();
        process_crs_switch();
        process_autothrottle_switch();
        process_ap_master_switch();
        process_hdg_button();
        process_nav_button();
        process_ias_button();
        process_alt_button();
        process_vs_button();
        process_apr_button();
        process_rev_button();
        process_flaps_switch();
        process_trim_wheel(dt);
        if(multires > 0) {
            process_multi_flash();
            process_multi_blank_display();
            process_multi_display();

            if (log_enable == 3) {
                XPLMDebugString("Xsaitekpanels: MULTI: Read buffer Byte3: Bits 7-4: Not used,  Bits 3-0: Pitch Up, Pitch Dn,  Flaps Dn, Flaps Up\n");
                XPLMDebugString("Xsaitekpanels: MULTI: Read buffer Byte2: Bits 7-0: Switch: AT,  Buttons: REV, APR, VS, ALT, IAS, NAV, HDG\n");
                XPLMDebugString("Xsaitekpanels: MULTI: Read buffer Byte1: Bits 7-0: Button: AP,  Knob: Dn, Up,  Switch: CRS, HDG, IAS, VS, ALT\n");
                sprintf(buf, "Xsaitekpanels: MULTI: Read buffer status = [%d, %d, %d]\n", multibuf[0], multibuf[1], multibuf[2]);
                XPLMDebugString(buf);
                sprintf(buf, "Xsaitekpanels: MULTI: Upper display buffer = [%d, %d, %d, %d, %d]\n", multiwbuf[1], multiwbuf[2], multiwbuf[3], multiwbuf[4], multiwbuf[5]);
                XPLMDebugString(buf);
                sprintf(buf, "Xsaitekpanels: MULTI: Lower display buffer = [%d, %d, %d, %d, %d]          (15 = Blank character, 254 = minus sign)\n", multiwbuf[6], multiwbuf[7], multiwbuf[8], multiwbuf[9], multiwbuf[10]);
                XPLMDebugString(buf);
                sprintf(buf, "Xsaitekpanels: MULTI: LED button status = [%d]                         (Bits 7-0: REV, APR, VS, ALT, IAS, NAV, HDG, AP)\n\n",multiwbuf[11]);
                XPLMDebugString(buf);
            }
        }
        --multi_safety_cntr;
    } while((multires > 0) && (multi_safety_cntr > 0));

    process_multi_flash();
    process_multi_blank_display();
    process_multi_display();


    // ******* Write on changes ********
    if ((lastmultiseldis != multiseldis) || (lastbtnleds != btnleds) || (lastmultiaactv != multiaactv) || (lastmultibstby != multibstby) || (updatedispmul > 0)) {
        mulres = hid_send_feature_report(multihandle, multiwbuf, sizeof(multiwbuf));
        if (log_enable == 3) {
            sprintf(buf, "Xsaitekpanels: MULTI: lastmultiseldis = %d, multiseldis = %d,  |  lastbtnleds = %d, btnleds = %d  |  updatedispmul = %d\n",lastmultiseldis, multiseldis, lastbtnleds, btnleds, updatedispmul);
            XPLMDebugString(buf);
            sprintf(buf, "Xsaitekpanels: MULTI: lastmultiaactv = %d, multiaactv = %d  |  lastmultibstby = %d, multibstby = %d\n\n",lastmultiaactv, multiaactv, lastmultibstby, multibstby);
            XPLMDebugString(buf);
        }
        lastmultiaactv = multiaactv;
        lastmultibstby = multibstby;
        lastmultiseldis = multiseldis;
        lastbtnleds = btnleds;
        if (updatedispmul > 0) {
           --updatedispmul;
        }
    }

    /*
    printf("multitestloop = %d AvPwrOn = %d BatPwrOn = %d multiseldis  %d\n", multitestloop, XPLMGetDatai(AvPwrOn), XPLMGetDatai(BatPwrOn), multiseldis);
    multitestloop ++;
    if (multitestloop > 5000) {
        multitestloop = 0;
    }

    */

    return;
}
