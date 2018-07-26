# mse2cdb
Tool for importing cards from Magic Set Editor files into a YGOPro cdb. Can be configured to support various languages and templates via Lua.

## How to Build
Requires sqlite3, lua5.3, and libzip.
### On Linux
Run these commands in the project's parent directory:
```
mkdir build
cd build
cmake ..
make
```
This will create the executable `mse2cdb` inside `build`, which can then be run according to the instructions in the "How to Use" section.

If you need a Windows build, you can try modifying CMakeLists.txt by: 

* uncommenting the block of `set` commands under the first "Windows Build" comment
* uncommenting the `target_link_libraries` command under the second "Windows Build" comment
* turning the other `target_link_libraries` command into a comment

Then you should be able to modify the various library paths in the file to point to wherever you have your Windows libraries and maybe it will build.  
(I'm pretty sure this isn't how you're supposed to do it, but eh, works on my machine so I'll leave it like this until I eventually figure out the correct approach.)
### On Windows
I barely managed to find a way to build FOR Windows, but doing so ON Windows is an entirely different question for which I don't have an answer right now. Sorry.

## How to Use
The program is called from the command line as follows:  
`mse2cdb [-l language] [-t template] msePath cdbPath`

Parameters (see "How to Configure" for more information about the mentioned Lua scripts):

* `-l language`: Specifies the language used on the cards in the MSE file, which influences type, attribute, and archetype recognition. `language` must be the name a subdirectory of `lua/lang` that contains the files `archetypes.lua` and `dict.lua`. If the parameter is omitted, the program attempts to determine the language based on the `set info => language` value of the MSE set file. 
* `-t template`: Specifies the template used by the MSE file, which influences how the various database fields are read. `template` must be the name (without extension) of a .lua file inside the `lua` directory. If the parameter is omitted, the program attempts to determine the language based on the `stylesheet` value of the MSE set file.
* `msePath`: Full path to the .mse-set file from which you want to read cards.
* `cdbPath`: Full path to the .cdb file in which cards should be written. Must already exist and have the standard YGOPro database structure.

## How to Configure
The way data from the MSE file is translated for YGOPro can be freely configured using the various .lua files located in the `lua` directory next to the executable.

Types of configuration files:

* `constant.lua`: Defines the type, attribute, and race constants that are also used in YGOPro scripts to preserve a familiar environment for card developers. In fact, this is literally just copied from YGOPro's own `constant.lua` file.
* `utility.lua`: Defines various helper functions that can be used by template scripts, mostly concerned with string manipulation and non-trivial calculations. Like in YGOPro, these functions can be accessed with `aux.<Function Name>`.
* `lang\<language>\dict.lua`: Defines associative arrays used to translate race, subtype, and attribute name into the corresponding constants. Also defines language-specific strings like the "treated as" clause.
* `lang\<language>\archetypes.lua`: Defines an associative array used to translate archetype names into the corresponding setcodes. Kept in a separate file because this will need to be changed much more frequently than other translations.
* `<template>.lua`: Defines a function for each column in YGOPro's cdb tables. These functions, with the MSE data of a single card as their parameter, are called by the program to generate the values that are written in the corresponding columns for that card.
