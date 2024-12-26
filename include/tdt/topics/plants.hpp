#ifndef TDT_TOPIC_PLANTS_HPP
#define TDT_TOPIC_PLANTS_HPP
//----------------------------------------------------------------------------
///
/// \brief  TDT topic 'plant sensors'
///
///         TDT objects for plant sensors.
///
/// \date   20241219
/// \author Maximilian Seesslen <mes@seesslen.net>
///
//----------------------------------------------------------------------------


//--- Includes ----------------------------------------------------------------


#include <tdt/topic.hpp>

#if defined USE_LEPTO
   // #include <lepto/events.h>
   // #include <lepto/log.h>
#endif


//--- Declarations ------------------------------------------------------------


namespace Tdt
{

enum class EObjectPlants: uint16_t
{
   Sensor                = 0x08 + OBJECT_OFFSET_PLANT,
   Sensor0               = 0x08 + OBJECT_OFFSET_PLANT,
   Sensor1               = 0x09 + OBJECT_OFFSET_PLANT,
   Sensor2               = 0x0A + OBJECT_OFFSET_PLANT,
   Sensor3               = 0x0B + OBJECT_OFFSET_PLANT,
   Sensor4               = 0x0C + OBJECT_OFFSET_PLANT,
   SensorRaw             = 0x8008 + OBJECT_OFFSET_PLANT,
   SensorRaw0            = 0x8008 + OBJECT_OFFSET_PLANT,
   SensorRaw1            = 0x8009 + OBJECT_OFFSET_PLANT,
   SensorRaw2            = 0x800A + OBJECT_OFFSET_PLANT,
   SensorRaw3            = 0x800B + OBJECT_OFFSET_PLANT,
   SensorRaw4            = 0x800C + OBJECT_OFFSET_PLANT,
};

TDT_TOPIC( Plants );

extern CTopic topicPlants;

}


//---fin-----------------------------------------------------------------------
#endif // ? ! TDT_TOPIC_PLANTS_HPP
