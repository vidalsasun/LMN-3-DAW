# LMN-3-DAW FORK
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

This repository is a fork for the great work of @FundamentalFrequency

https://github.com/FundamentalFrequency

I am not a C++ programmer, I have tried to use the project methodology, but I am sure there is code I have added that could be greatly improved.

#Changelog

  -  Added Load / Save tracks.

   ![alt text](https://github.com/vidalsasun/LMN-3-DAW/assets/23163594/7eabe772-995c-4199-ae2f-b5999457683c)
   ![alt text](https://github.com/vidalsasun/LMN-3-DAW/assets/23163594/0fc9b7a4-0d95-4af8-9a11-ab6e3a8f3648)

    Since we do not have an alphabetical keyboard, I have decided to name the tracks with the format 'edit_+ddmmyyyyhhMMss'. If you change the name of the file after there are no problem
    If you decide to change the file name later, it will load without any problem.
    
    There are 2 new folders on /home/pi/.config/LMN-3:
      -  Saved: where all tracks are stored

      -  load_project: the actual project

      When you press the save button the load_project xml file is moved to saved folder, if there are a file whith the same name, it is overwriten
      When you Add a new track, the load_project is cleaned and a new file is created on it and the actual project are moved to saved folder, if there are a file whith the same name, it is overwriten
      When you load a track,  the load_project is cleaned and the track selected is moved to load_project
      

  
  -  Remove metronome on play (not on record)



    IMPORTANT: You cannot move the saved xml to another LMN-3 because need the same samples and drumkits, if you hace the same folders on the other LMN-3, the track will load ok.


