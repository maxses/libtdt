# Liblepto

<div align="center" width="100%" style="vertical-align: middle;" valign="middle">
    <img src="doc/canio_2.jpg" height="150" style="vertical-align: middle;">
</div>

## Overview

TDT is an CAN based protocol for simple sensor values. TDT ( trivial data 
types ) messages contain information about the data type of the payload.
Like on CANOpen each node has an object catalogue.

It is also possible to flash firmware of nodes via can using the MMP 
(multi message packet) layer.

This repository provides:

* Library for Microcontroller
* Library for host PC
* Code generator to provide c++ enumerations of objects and defines for logging

The available objects of the nodes can be specified with JSON files.

