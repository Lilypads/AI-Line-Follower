#include"limbic-system-model.h"

#include <stdio.h>

#include <filter.h>

float LHB_BIAS = 0;
float DRN_SUPPRESSION = 0;
float OFC_5HTR1_OFFSET = 0;
float OFC_5HTR2_OFFSET = 0;
float DRN_OFFSET = 0;

// constructor
Limbic_system::Limbic_system()
{
	flog = fopen("log.dat","wt");

	// filter which creates a default behaviour when touching the landmarks
	on_contact_direction_Green_filter = new SecondOrderLowpassFilter(0.1);
	on_contact_direction_Blue_filter = new SecondOrderLowpassFilter(0.1);

	// filter which smears out the visual input in time as a 1st attempt
	// to create persistent activity in the cortex
	visual_direction_Green_mPFC_filter = new SecondOrderLowpassFilter(0.01);
	visual_direction_Blue_mPFC_filter = new SecondOrderLowpassFilter(0.01);

	// input step number
	step = 0;
};


// destructor
Limbic_system::~Limbic_system()
{
	fclose(flog);
}


// changes a synaptic weight "w" by the amount "delta"
// and makes sure it never goes below 0 and above 1
void Limbic_system::weightChange(float &w, float delta) {
	w += delta;
	if (w>1) w = 1;
	if (w<0) w = 0;
}

// that's what our limbic system does!
// remember there are two landmarks/place fields: light green and dark green
// only one of them contains the reward (controlled by the main program)
//
// It gets as inputs:
// - the reward
// - two place field signals (placefield*) which go from 0 to 1 when in the place field (where the reward shows up)
// - two signals when the agent touches the landmark: (on_contact_direction_*)
// - visual inputs when the agent sees a landmark which goes from 0 to 1 the closer the agent gets (_visual_direction_*)
// - visual inputs when the agent sees the reward (_visual_reward_*)
//
// It needs to set the outputs:
// - CoreGreenOut and CoreBlueOut which when set to non-zero generates a navigation behaviour towards
//   the landmarks
// - mPFC2CoreExploreLeft and - mPFC2CoreExploreRight to generate exploration behaviour
//   that is inhibited with other activities.
void Limbic_system::doStep(float _reward,
		float _placefieldGreen,
		float _placefieldBlue,
		float _on_contact_direction_Green,
		float _on_contact_direction_Blue,
		float _visual_direction_Green,
		float _visual_direction_Blue,
		float _visual_reward_Green,
		float _visual_reward_Blue ) {

	reward = _reward;
	placefieldGreen = _placefieldGreen;
	placefieldBlue = _placefieldBlue;
	on_contact_direction_Green = _on_contact_direction_Green;
	on_contact_direction_Blue = _on_contact_direction_Blue;
	visual_direction_Green = _visual_direction_Green;
	visual_direction_Blue = _visual_direction_Blue;
	visual_reward_Green = _visual_reward_Green;
	visual_reward_Blue = _visual_reward_Blue;
	//fprintf(stderr,"%f,%f\n",_visual_reward_Green,_visual_reward_Blue);

	visual_direction_Green_trace = visual_direction_Green_mPFC_filter->filter(visual_direction_Green);
	visual_direction_Blue_trace = visual_direction_Blue_mPFC_filter->filter(visual_direction_Blue);

	double mPFC_Green_spont_act = 0;
	double mPFC_Blue_spont_act = 0;
	double mPFC_spont_act_value = 0.2;

	if (mPFCspontGreen>0) {
		mPFCspontGreen--;
		mPFC_Green_spont_act = mPFC_spont_act_value;
	} else {
		mPFC_Green_spont_act = 0;
		if (random() < (RAND_MAX/1000)) {
			mPFCspontGreen = 500;
		}
	}

	if (mPFCspontBlue>0) {
		mPFCspontBlue--;
		mPFC_Blue_spont_act = mPFC_spont_act_value;
	} else {
		mPFC_Blue_spont_act = 0;
		if (random() < (RAND_MAX/1000)) {
			mPFCspontBlue = 500;
		}
	}

	// this is also generated in the mPFC and then fed down to the NAcc core with the command
	// to explore
	switch (exploreState) {
	case EXPLORE_LEFT:
		mPFC2CoreExploreLeft = 0.1;
		mPFC2CoreExploreRight = 0;
		if (((float)random()/((float)RAND_MAX))<0.03) {
			exploreState = (ExploreStates)floor((float)random()/(float)RAND_MAX*EXPLORE_NUM_ITEMS);
		}
		//printf("left\n");
		break;
	case EXPLORE_RIGHT:
		mPFC2CoreExploreLeft = 0;
		mPFC2CoreExploreRight = 0.1;
		if (((float)random()/((float)RAND_MAX))<0.03) {
			exploreState = (ExploreStates)floor((float)random()/(float)RAND_MAX*EXPLORE_NUM_ITEMS);
		}
		//printf("right\n");
		break;
	case EXPLORE_STOP:
		mPFC2CoreExploreLeft = 0;
		mPFC2CoreExploreRight = 0;
		if (((float)random()/((float)RAND_MAX))<0.01) {
			exploreState = (ExploreStates)floor((float)random()/(float)RAND_MAX*EXPLORE_NUM_ITEMS);
		}
		break;
	case EXPLORE_STRAIGHT:
	default:
		mPFC2CoreExploreLeft = 0.1;
		mPFC2CoreExploreRight = 0.1;
		if (((float)random()/((float)RAND_MAX))<0.05) {
			exploreState = (ExploreStates)floor((float)random()/(float)RAND_MAX*EXPLORE_NUM_ITEMS);
		}
		break;
	}

//#define HT5DEBUG
#ifdef HT5DEBUG
	mPFC_Green = visual_direction_Green_trace + visual_reward_Green + mPFC_Green_spont_act;
	mPFC_Blue = visual_direction_Blue_trace + visual_reward_Blue + mPFC_Blue_spont_act;
#else
	mPFC_Green = ofc5HTreceptors(visual_direction_Green_trace + visual_reward_Green*2 + mPFC_Green_spont_act,1+DRN,2+DRN);
	mPFC_Blue = ofc5HTreceptors(visual_direction_Blue_trace + visual_reward_Blue*2 + mPFC_Blue_spont_act,1+DRN,2+DRN);
#endif

	// the activity in the LH is literally that of the reward
	LH = reward;

	// the VTA gets its activity from the LH and is ihibited by the RMTg
	VTA = (LH + VTA_baseline_activity) / (1+RMTg * shunting_inhibition_factor);

	////////////////////////////////////////////////////////////////////
	// OFC
	// lace field -> Orbitofrontal Cortex weight.
	// The OFC remembers the reward value of a place.
	// So the higher the weight the higher the OFC activity
	// when the animal is inside that place field where there has been
	// reward experienced in the past.
	OFC = pfLg2OFC * placefieldGreen + pfDg2OFC * placefieldBlue;
	if (((placefieldGreen-OFCprev) > 0.01)&&(OFCpersist==0)) OFCpersist = 200;
	OFCprev = placefieldGreen;
	if (LH > 0.5) {
		OFCpersist = 0;
	}
	if (OFCpersist>0) OFCpersist--;
	//fprintf(stderr,"%d\n",OFCpersist);
	// weight change
	weightChange(pfLg2OFC, learning_rate_OFC * DRN * placefieldGreen);
	weightChange(pfDg2OFC, learning_rate_OFC * DRN * placefieldBlue);


	// massive weight decay if there is no reward after a long period!
//	if ((OFCpersist>0)&&(OFCpersist<100)) {
//		fprintf(stderr,"--");
//		pfLg2OFC = pfLg2OFC * 0.999;
//	}

	// the dorsal raphe activity is driven by the OFC in a positive way
	DRN = (LH + OFC * 4) / (1+RMTg * shunting_inhibition_factor + DRN_SUPPRESSION) + DRN_OFFSET;

	//printf("%f\n",DRN);

	////////////////////////////////////////////////////
	// lateral shell activity
	// the place field feeds into the Nacc shell for the time being.
	lShell = placefieldGreen * lShell_weight_pflg + placefieldBlue * lShell_weight_pfdg;

	// Let's do heterosynaptic plasticity in the shell
	shell_DA = VTA;
	// shell plasticity can experience a "dip" where then the weights
	// decrease. That's when it's below its baseline.
	shell_plasticity = shell_DA - VTA_zero_val;
	weightChange(lShell_weight_pflg, learning_rate_lshell * shell_plasticity * placefieldGreen);
	weightChange(lShell_weight_pfdg, learning_rate_lshell * shell_plasticity * placefieldBlue);

	// the shell inhibits the dlVP
	dlVP = 1/(1+lShell * shunting_inhibition_factor);

	// another inhibition: the dlVP inhibits the EP
	EP = 1/(1+dlVP * shunting_inhibition_factor);

	// the EP excites the LHb
	LHb = EP + LHB_BIAS;

	// and the LHb excites the RMTg
	RMTg = LHb;

	////////////////////////////////////////////////////////////////////////////////////
	// core
	// we have two core units
	// if the Green is high then the rat approaches the Green marker
	CoreGreenOut= (mPFC_Green * core_weight_lg2lg);
	// of the Blue is high then the rat approaches the Blue marker
	CoreBlueOut= (mPFC_Blue * core_weight_dg2dg);

	float core_threshold = 0.25;
	if (CoreGreenOut < core_threshold) CoreGreenOut = 0;
	if (CoreBlueOut < core_threshold) CoreBlueOut = 0;

	// plasticity
	core_DA = VTA;
	core_plasticity = core_DA - VTA_zero_val;
	weightChange(core_weight_lg2lg, learning_rate_core * core_plasticity * mPFC_Green);
	weightChange(core_weight_dg2dg, learning_rate_core * core_plasticity * mPFC_Blue);

	// we assume that the Core performs lateral inhibtion to shut down exploration
	if ((CoreGreenOut > 0.05)&&(CoreBlueOut > 0.05)) {
		mPFC2CoreExploreLeft = 0; // (float)random()/(float)RAND_MAX;
		mPFC2CoreExploreRight = 0; //(float)random()/(float)RAND_MAX;
		//printf("dir! mPFC_Green = %f\n",visual_direction_Green);
	}

	logging();

	step++;  

}	




void Limbic_system::logging() {
	fprintf(flog,"%ld %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
		step, //0
		reward, //1
		placefieldGreen, //2
		placefieldBlue, //3
		on_contact_direction_Green, //4
		on_contact_direction_Blue, //5
		visual_direction_Green, //6
		visual_direction_Blue, //7
		core_weight_lg2lg, //8
		mPFC_Green, //9
		mPFC_Blue, //10
		core_weight_dg2dg, //11
		CoreGreenOut, //12
		CoreBlueOut, //13
		VTA,//14
		core_plasticity,//15
		shell_plasticity,//16
		lShell_weight_pflg,//17
		lShell_weight_pfdg,//18
		dlVP,//19
		EP,//20
		LHb,//21
		RMTg,//22
		pfLg2OFC,//23
		pfDg2OFC, //24
		DRN, //25
		visual_reward_Green, // 26
		visual_reward_Blue //27
		);
	fflush(flog);
}

