# Liblepto

<div align="center" width="100%" style="vertical-align: middle;" valign="middle">
    <img src="doc/canio_2.jpg" height="150" style="vertical-align: middle;">
</div>

## Overview

TDT is an CAN based protocol for simple sensor values. TDT ( trivial data 
types ) messages contain information about the data type of the payload.
Like on CANOpen each node has an object catalogue.

<div align="center" width="100%" style="vertical-align: middle;" valign="middle">
    <img src="doc/overview.png" style="vertical-align: middle;">
</div>

Ont the first level, the data flow resembles a disorganized yelling. Each node 
can send measured values ​​without a destination. Another node can evaluate them 
if interested. There is not directly central master.
At a further layer, there is the Multi-Message Packet Protocol. This defines 
the protocol for exchanging larger amounts of data between two nodes. This can 
be used, for example, to perform firmware updates via CAN.

This repository provides:

* Library for Microcontroller
* Library for host PC
* Code generator to provide c++ enumerations of objects and defines for logging

The available objects of the nodes can be specified with custom JSON files.

Example:
```
{
   "profiles": [
      {
         "name": "Environment",
         "description": "Environment sensors like temperatures, VOC, humidity",
         "base": "0x400",
         "size": "0x10",
         "objects": [
            {
               "offset": "0x0014",
               "name": "Air humidity",
               "description": "Air humidity",
               "example": "40 %"
            },
            {
               "offset": "0x0018",
               "name": "Temperature",
               "example": "22.35°C",
               "size": "8",
               "unit": "centi degree"
            }
         ]
      }
   ]
}
```
