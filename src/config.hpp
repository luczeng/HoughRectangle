#include <cereal/cereal.hpp>

struct Config
{
	public:
	int thetaBins;
	int rhoBins;

	//Method for cereal to know which member to serialize
	template <class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(thetaBins),CEREAL_NVP(rhoBins));
	}

};

