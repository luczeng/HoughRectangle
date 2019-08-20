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

	//Method for cereal to know which member to serialize
	template <class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(thetaBins),
			CEREAL_NVP(rhoBins),
			CEREAL_NVP(thetaMin),
			CEREAL_NVP(thetaMax));
	}

};
#endif
