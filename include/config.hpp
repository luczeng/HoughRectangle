
// TODO(luczeng): alternative to header guards: '#pragma once' is understood by almost all compilers
//                and you don't need the last '#ifdef' then.
#ifndef CONFIG_H
#define CONFIG_H
#include <cereal/cereal.hpp>

// TODO(luczeng): you should probably wrap your library in a seperate namespace, as follows:
// namespace Hough {

struct Config
{
	public: // TODO(luczeng): in a struct, all data members and member functions are public so this is unnecessary
	int thetaBins;
	int rhoBins;

	int thetaMin;
	int thetaMax;

    int h;
    int w;

    int L_window;

    int r_min;
    int r_max;

    int min_side_length;
    float T_theta;
    float T_rho;
    float T_l;
    float T_alpha;

	//Method for cereal to know which member to serialize
	template <class Archive> // TODO(luczeng): I prefer 'typename' for templates but that's a personal choice
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(thetaBins),
			CEREAL_NVP(rhoBins),
			CEREAL_NVP(thetaMin),
			CEREAL_NVP(thetaMax),
            CEREAL_NVP(h),
            CEREAL_NVP(w),
            CEREAL_NVP(L_window),
            CEREAL_NVP(r_min),
            CEREAL_NVP(r_max),
            CEREAL_NVP(min_side_length),
            CEREAL_NVP(T_theta),
            CEREAL_NVP(T_rho),
            CEREAL_NVP(T_l),
            CEREAL_NVP(T_alpha));
	}

};

#endif
