# tdtGen

<div align="center" width="100%" style="vertical-align: middle;" valign="middle">
    <img src="doc/canio_2.jpg" height="150" style="vertical-align: middle;">
</div>

## Overview

tdtGen is a code generator for TDT protocol. The input are JSON files 
describing TDT-objects, units and logs.

## Usage

When libtdt is used as a cmake subdirectory ( e.g. 'add_subdirectory(libtdt)' ),
the tool is automatically created and integrated.
Only the directories containing custom JSON files have to be specified in the 
CMake variable 'tdtProfiles'.

Example:
```
list( APPEND
    tdtProfiles
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/node.json
    ${CMAKE_CURRENT_SOURCE_DIR}/tdt/environment.json
)
```

Or with the use of wildcards:
```
file( GLOB 
   tdtProfiles 
      ${CMAKE_CURRENT_SOURCE_DIR}/libtdt/tdtBase/*.json
      ${CMAKE_CURRENT_SOURCE_DIR}/tdtCampo/*.json
)
```
