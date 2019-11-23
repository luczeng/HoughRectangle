#ifndef CONFIG_H
#define CONFIG_H
#include <cereal/cereal.hpp>

struct Config
{
	public:
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

	//Method for cereal to know which member to serialize
	template <class Archive>
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
            CEREAL_NVP(min_side_length));
	}

};

#endif
