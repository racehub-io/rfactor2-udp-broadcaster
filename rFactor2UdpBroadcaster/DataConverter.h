#pragma once

#include "InternalsPlugin.hpp";
#include "telemetry.pb.h";
#include "stringConverter.h";

using namespace rFactor2UdpBroadcaster;


/// Conversion class from rFactor 2 types to our own types

TelemetryVector3* ToProtoType(const TelemVect3& info)
{
    TelemetryVector3* vect3 = new TelemetryVector3();

    vect3->set_x(info.x);
    vect3->set_y(info.y);
    vect3->set_z(info.z);

    return vect3;
}

VehicleScoringInfo* ToProtoType(const VehicleScoringInfoV01& info)
{
    VehicleScoringInfo* data = new VehicleScoringInfo();

    data->set_mid(info.mID);
    data->set_mdrivername(info.mDriverName);
    data->set_mvehiclename(info.mVehicleName);
    data->set_mtotallaps(info.mTotalLaps);
    data->set_msector((int)info.mSector);
    data->set_mfinishstatus((int)info.mFinishStatus);
    data->set_mlapdist(info.mLapDist);
    data->set_mpathlateral(info.mPathLateral);
    data->set_mtrackedge(info.mTrackEdge);

    data->set_mbestsector1(info.mBestSector1);
    data->set_mbestsector2(info.mBestSector2);
    data->set_mbestlaptime(info.mBestLapTime);
    data->set_mlastsector1(info.mLastSector1);
    data->set_mlastsector2(info.mLastSector2);
    data->set_mlastlaptime(info.mLastLapTime);
    data->set_mcursector1(info.mCurSector1);
    data->set_mcursector2(info.mCurSector2);

    data->set_mnumpitstops(info.mNumPitstops);
    data->set_mnumpenalties(info.mNumPenalties);
    data->set_misplayer(info.mIsPlayer);
    data->set_mcontrol((int)info.mControl);
    data->set_minpits(info.mInPits);
    data->set_mplace((int)info.mPlace);
    data->set_mvehicleclass(info.mVehicleClass);

    data->set_mtimebehindnext(info.mTimeBehindNext);
    data->set_mlapsbehindnext(info.mLapsBehindNext);
    data->set_mtimebehindleader(info.mTimeBehindLeader);
    data->set_mlapsbehindleader(info.mLapsBehindLeader);
    data->set_mlapstartet(info.mLapStartET);

    data->set_allocated_mpos(ToProtoType(info.mPos));
    data->set_allocated_mlocalvel(ToProtoType(info.mLocalVel));
    data->set_allocated_mlocalaccel(ToProtoType(info.mLocalAccel));

    for (TelemVect3 vect : info.mOri)
    {
        TelemetryVector3* ori = ToProtoType(vect);
        TelemetryVector3* newOri = data->add_mori();
        *newOri = *ori;
    }
    data->set_allocated_mlocalrot(ToProtoType(info.mLocalRot));
    data->set_allocated_mlocalrotaccel(ToProtoType(info.mLocalRotAccel));

    data->set_mheadlights((int)info.mHeadlights);
    data->set_mpitstate((int)info.mPitState);
    data->set_mserverscored((int)info.mServerScored);
    data->set_mindividualphase((int)info.mIndividualPhase);
    data->set_mqualification(info.mQualification);
    data->set_mtimeintolap(info.mTimeIntoLap);
    data->set_mestimatedlaptime(info.mEstimatedLapTime);
    data->set_mpitgroup(info.mPitGroup);
    data->set_mflag((int)info.mFlag);
    data->set_munderyellow(info.mUnderYellow);
    data->set_mcountlapflag((int)info.mCountLapFlag);
    data->set_mingaragestall(info.mInGarageStall);
    for (int val : info.mUpgradePack)
    {
        data->add_mupgradepack(val);
    }

    return data;
}

TelemWheel* ToProtoType(const TelemWheelV01& info)
{
    TelemWheel* data = new TelemWheel();

    data->set_msuspensiondeflection(info.mSuspensionDeflection);
    data->set_mrideheight(info.mRideHeight);
    data->set_msuspforce(info.mSuspForce);
    data->set_mbraketemp(info.mBrakeTemp);
    data->set_mbrakepressure(info.mBrakePressure);
    data->set_mrotation(info.mRotation);
    data->set_mlateralpatchvel(info.mLateralPatchVel);
    data->set_mlongitudinalpatchvel(info.mLongitudinalPatchVel);
    data->set_mlateralgroundvel(info.mLateralGroundVel);
    data->set_mlongitudinalgroundvel(info.mLongitudinalGroundVel);
    data->set_mcamber(info.mCamber);
    data->set_mlateralforce(info.mLateralForce);
    data->set_mlongitudinalforce(info.mLongitudinalForce);
    data->set_mtireload(info.mTireLoad);
    data->set_mgripfract(info.mGripFract);
    data->set_mpressure(info.mPressure);
    for (double val : info.mTemperature)
    {
        data->add_mtemperature(val);
    }
    data->set_mwear(info.mWear);
    data->set_mterrainname(info.mTerrainName);
    data->set_msurfacetype((int)info.mSurfaceType);
    data->set_mflat(info.mFlat);
    data->set_mdetached(info.mDetached);
    data->set_mverticaltiredeflection(info.mVerticalTireDeflection);
    data->set_mwheelylocation(info.mWheelYLocation);
    data->set_mtoe(info.mToe);
    data->set_mtirecarcasstemperature(info.mTireCarcassTemperature);
    for (double val : info.mTireInnerLayerTemperature)
    {
        data->add_mtireinnerlayertemperature(val);
    }

    return data;
}

TelemInfo* ToProtoType(const TelemInfoV01& info)
{
    TelemInfo* data = new TelemInfo();

    data->set_mid(info.mID);
    data->set_mdeltatime(info.mDeltaTime);
    data->set_melapsedtime(info.mElapsedTime);
    data->set_mlapnumber(info.mLapNumber);
    data->set_mlapstartet(info.mLapStartET);
    data->set_mvehiclename(info.mVehicleName);
    data->set_mtrackname(info.mTrackName);
    data->set_allocated_mpos(ToProtoType(info.mPos));
    data->set_allocated_mlocalvel(ToProtoType(info.mLocalVel));
    data->set_allocated_mlocalaccel(ToProtoType(info.mLocalAccel));
    for (TelemVect3 vect : info.mOri)
    {
        TelemetryVector3* ori = ToProtoType(vect);
        TelemetryVector3* newOri = data->add_mori();
        *newOri = *ori;
    }
    data->set_allocated_mlocalrot(ToProtoType(info.mLocalRot));
    data->set_allocated_mlocalrotaccel(ToProtoType(info.mLocalRotAccel));

    data->set_mgear(info.mGear);
    data->set_menginerpm(info.mEngineRPM);
    data->set_menginewatertemp(info.mEngineWaterTemp);
    data->set_mengineoiltemp(info.mEngineOilTemp);
    data->set_mclutchrpm(info.mClutchRPM);
    data->set_munfilteredthrottle(info.mUnfilteredThrottle);
    data->set_munfilteredbrake(info.mUnfilteredBrake);
    data->set_munfilteredsteering(info.mUnfilteredSteering);
    data->set_munfilteredclutch(info.mUnfilteredClutch);
    data->set_mfilteredthrottle(info.mFilteredThrottle);
    data->set_mfilteredbrake(info.mFilteredBrake);
    data->set_mfilteredsteering(info.mFilteredSteering);
    data->set_mfilteredclutch(info.mFilteredClutch);
    data->set_msteeringshafttorque(info.mSteeringShaftTorque);
    data->set_mfront3rddeflection(info.mFront3rdDeflection);
    data->set_mrear3rddeflection(info.mRear3rdDeflection);

    data->set_mfrontwingheight(info.mFrontWingHeight);
    data->set_mfrontrideheight(info.mFrontRideHeight);
    data->set_mrearrideheight(info.mRearRideHeight);
    data->set_mdrag(info.mDrag);
    data->set_mfrontdownforce(info.mFrontDownforce);
    data->set_mreardownforce(info.mRearDownforce);

    data->set_mfuel(info.mFuel);
    data->set_menginemaxrpm(info.mEngineMaxRPM);
    data->set_mscheduledstops((int)info.mScheduledStops);
    data->set_moverheating(info.mOverheating);
    data->set_mdetached(info.mDetached);
    data->set_mheadlights(info.mHeadlights);
    for (int val : info.mDentSeverity)
    {
        data->add_mdentseverity(val);
    }
    data->set_mlastimpactet(info.mLastImpactET);
    data->set_mlastimpactmagnitude(info.mLastImpactMagnitude);
    data->set_allocated_mlastimpactpos(ToProtoType(info.mLastImpactPos));

    data->set_menginetorque(info.mEngineTorque);
    data->set_mcurrentsector(info.mCurrentSector);
    data->set_mspeedlimiter((int)info.mSpeedLimiter);
    data->set_mmaxgears((int)info.mMaxGears);
    data->set_mfronttirecompoundindex((int)info.mFrontTireCompoundIndex);
    data->set_mreartirecompoundindex((int)info.mRearTireCompoundIndex);
    data->set_mfuelcapacity(info.mFuelCapacity);
    data->set_mfrontflapactivated((int)info.mFrontFlapActivated);
    data->set_mrearflapactivated((int)info.mRearFlapActivated);
    data->set_mrearflaplegalstatus((int)info.mRearFlapLegalStatus);
    data->set_mignitionstarter((int)info.mIgnitionStarter);
    data->set_mfronttirecompoundname(info.mFrontTireCompoundName);
    data->set_mreartirecompoundname(info.mRearTireCompoundName);
    data->set_mspeedlimiteravailable((int)info.mSpeedLimiterAvailable);
    data->set_mantistallactivated((int)info.mAntiStallActivated);
    for (int val : info.mUnused)
    {
        data->add_munused(val);
    }
    data->set_mvisualsteeringwheelrange(info.mVisualSteeringWheelRange);
    data->set_mrearbrakebias(info.mRearBrakeBias);
    data->set_mturboboostpressure(info.mTurboBoostPressure);
    for (float val : info.mPhysicsToGraphicsOffset)
    {
        data->add_mphysicstographicsoffset(val);
    }
    data->set_mphysicalsteeringwheelrange(info.mPhysicalSteeringWheelRange);
    for (TelemWheelV01 wheel : info.mWheel)
    {
        TelemWheel* cvr = ToProtoType(wheel);
        TelemWheel* newObj = data->add_mwheel();
        *newObj = *cvr;
    }


    return data;
}

ScoringInfo* ToProtoType(const ScoringInfoV01& info)
{
    ScoringInfo* data = new ScoringInfo();

    data->set_mtrackname(info.mTrackName);
    data->set_msession(info.mSession);
    data->set_mcurrentet(info.mCurrentET);
    data->set_mendet(info.mEndET);
    data->set_mmaxlaps(info.mMaxLaps);
    data->set_mlapdist(info.mLapDist);
    data->set_mnumvehicles(info.mNumVehicles);
    data->set_mgamephase((int)info.mGamePhase);
    data->set_myellowflagstate((int)info.mYellowFlagState);
    for (int val : info.mSectorFlag)
    {
        data->add_msectorflag(val);
    }
    data->set_mstartlight((int)info.mStartLight);
    data->set_mnumredlights((int)info.mNumRedLights);
    data->set_minrealtime(info.mInRealtime);
    data->set_mplayername(info.mPlayerName);
    data->set_mplrfilename(info.mPlrFileName);
    data->set_mdarkcloud(info.mDarkCloud);
    data->set_mraining(info.mRaining);
    data->set_mambienttemp(info.mAmbientTemp);
    data->set_mtracktemp(info.mTrackTemp);
    data->set_allocated_mwind(ToProtoType(info.mWind));
    data->set_mminpathwetness(info.mMinPathWetness);
    data->set_mmaxpathwetness(info.mMaxPathWetness);

    VehicleScoringInfoV01* ptrScoringInfo = info.mVehicle;
    for (int i = 0; i < info.mNumVehicles; ++i)
    {
        VehicleScoringInfo* scoringInfo = ToProtoType(*ptrScoringInfo);
        VehicleScoringInfo* newVehicle = data->add_mvehicle();
        *newVehicle = *scoringInfo;
        ptrScoringInfo++;
    }


    return data;
}

GraphicsInfo* ToProtoType(const GraphicsInfoV02& info)
{
    GraphicsInfo* data = new GraphicsInfo();

    data->set_allocated_mcampos(ToProtoType(info.mCamPos));
    for (TelemVect3 vect : info.mCamOri)
    {
        TelemetryVector3* ori = ToProtoType(vect);
        TelemetryVector3* newOri = data->add_mcamori();
        *newOri = *ori;
    }
    data->set_mambientblue(info.mAmbientBlue);
    data->set_mambientgreen(info.mAmbientGreen);
    data->set_mambientred(info.mAmbientRed);
    data->set_mid(info.mID);
    data->set_mcameratype(info.mCameraType);

    return data;
}

PhysicsInfo* ToProtoType(const PhysicsOptionsV01 info)
{
    PhysicsInfo* data = new PhysicsInfo();

    data->set_mtractioncontrol(info.mTractionControl);
    data->set_mantilockbrakes(info.mAntiLockBrakes);
    data->set_mstabilitycontrol(info.mStabilityControl);
    data->set_mautoshift(info.mAutoShift);
    data->set_mautoclutch(info.mAutoClutch);
    data->set_minvulnerable(info.mInvulnerable);
    data->set_moppositelock(info.mOppositeLock);
    data->set_msteeringhelp(info.mSteeringHelp);
    data->set_mbrakinghelp(info.mBrakingHelp);
    data->set_mspinrecovery(info.mSpinRecovery);
    data->set_mautopit(info.mAutoPit);
    data->set_mautolift(info.mAutoLift);
    data->set_mautoblip(info.mAutoBlip);
    data->set_mfuelmult(info.mFuelMult);
    data->set_mtiremult(info.mTireMult);
    data->set_mmechfail(info.mMechFail);
    data->set_mallowpitcrewpush(info.mAllowPitcrewPush);
    data->set_mrepeatshifts(info.mRepeatShifts);
    data->set_mholdclutch(info.mHoldClutch);
    data->set_mautoreverse(info.mAutoReverse);
    data->set_malternateneutral(info.mAlternateNeutral);
    data->set_maicontrol(info.mAIControl);
    data->set_munused1(info.mUnused1);
    data->set_munused2(info.mUnused2);
    data->set_mmanualshiftoverridetime(info.mManualShiftOverrideTime);
    data->set_mautoshiftoverridetime(info.mAutoShiftOverrideTime);
    data->set_mspeedsensitivesteering(info.mSpeedSensitiveSteering);
    data->set_msteerratiospeed(info.mSteerRatioSpeed);

    return data;
}