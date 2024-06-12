# LMN-3-DAW FORK
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

This repository is a fork for the great work of @FundamentalFrequency

https://github.com/FundamentalFrequency

I am not a C++ programmer, I have tried to use the project methodology, but I am sure there is code I have added that could be greatly improved.

When this version of the program loads for the first time, it opens an empty project. If you want to retrieve your project from LMN-3, copy the 'edit' file from /home/pi/.config/LMN-3 to the 'saved' folder, rename it as you like, and give it the .xml extension

#Changelog

  -  Added Load / Save tracks.

   ![alt text](https://github.com/vidalsasun/LMN-3-DAW/assets/23163594/7eabe772-995c-4199-ae2f-b5999457683c)
   ![alt text](https://github.com/vidalsasun/LMN-3-DAW/assets/23163594/0fc9b7a4-0d95-4af8-9a11-ab6e3a8f3648)

    Since we do not have an alphabetical keyboard, I have decided to name the tracks with the format 'edit_+ddmmyyyyhhMMss'. If you change the name of the file after there are no problem.
    
    If you decide to change the file name later, it will load without any problem.

  ![alt text](https://github.com/vidalsasun/LMN-3-DAW/assets/23163594/45ec9ece-3594-42f7-93a2-b191dc9e8a48)


  ![alt text](https://github.com/vidalsasun/LMN-3-DAW/assets/23163594/5a8cdfd5-ec5f-42e3-a554-8586018bbe9b)

    
    
    There are 2 new folders on /home/pi/.config/LMN-3:
      -  Saved: where all tracks are stored

      -  load_project: the actual project

      When you press the save button:
        -  the load_project xml file is moved to saved folder, if there are a file whith the same name, it is overwriten
      When you Add a new track: 
        -  Temporary folder is removed (the program will create another on load)
        -  Load_project folder is cleaned 
        -  A new file is created on Load_project folder and the actual project xml file are moved to saved folder, if there are a file whith the same name, it is overwriten
        -  LMN-3 program restart
      When you load a track:
        -  Temporary folder is removed (the program will create another on load)
        -  Load_project folder is cleaned and the track selected is moved to load_project folder
        -  LMN-3 program restart    

  
  -  Remove metronome on play (not on record)
  
    IMPORTANT: You cannot move the saved xml to another LMN-3 because need the same samples and drumkits, if you have the same folders on the other LMN-3, the track will load ok.

#Install
  
  -  Download latest release (actual tested release is v0.2.3) zip.
  -  Go to /home/pi/ folder
  -  Rename LMN-3 file to backup it (IMPORTANT!)
  -  Copy zip file to /home/pi/ folder
  ![alt text](https://github.com/vidalsasun/LMN-3-DAW/assets/23163594/4c27929b-c196-46a3-ae09-c2ff6dccd2da)

  -  Unzip it
    
    $ unzip LMN-3-aarch64-linux-gnu.zip
    
    the result file is LMN-3 (IMPORTANT! if you dont backup the original file, it will be replaced)

    When you load this fork program first time, an empty project will be created. If you want to
    
  -  Done!






