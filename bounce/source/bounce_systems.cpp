#include "bounce_systems.hpp"

bool nothing_system(hades::job_system&, hades::system_job_data)
{
	return true;
}

void register_bounce_systems(hades::data::data_manager &data)
{
	//create a system that does nothing;
	hades::make_system("nothing_system", 
		nothing_system,
		nothing_system,
		nothing_system,
		nothing_system,
		nothing_system,
		data);
}
