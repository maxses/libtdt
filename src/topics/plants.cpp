//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------


#include <tdt/topic.hpp>
#include <tdt/topics/plants.hpp>


namespace Tdt
{

CTopic topicPlants
(
   "Plants",
   {
   ENUM_MAP( +Tdt::EObjectPlants,      Sensor ),
   ENUM_MAP_TEXT( +Tdt::EObjectPlants, Sensor0, "plantSensor[0]" ),
   ENUM_MAP_TEXT( +Tdt::EObjectPlants, Sensor1, "plantSensor[1]" ),
   ENUM_MAP_TEXT( +Tdt::EObjectPlants, Sensor2, "plantSensor[2]" ),
   ENUM_MAP_TEXT( +Tdt::EObjectPlants, Sensor3, "plantSensor[3]" ),
   ENUM_MAP_TEXT( +Tdt::EObjectPlants, Sensor4, "plantSensor[4]" ),
   }
);

} // namespace Tdt

//---fin-----------------------------------------------------------------------
