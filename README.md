# Image blurrer

## Algorithm used:

> For this program i went with a Box blur algorithm

## Compiler:

> Visual studio Community 2022 v17.1.5

## Prepareation / Research:

> To prepare for this task i began by looking into how the .tga format was structured. I found this [website](http://www.paulbourke.net/dataformats/tga/) as well as this [website](https://docs.fileformat.com/image/tga/) That explained all i needed to work with .tga files.
> After understanding that it worked about the same as network packets in terms of a header that tells the program how large the file is and what data is within, all i had to do was to begin making the program.
> I also refreshed my memory on [Reading and writing](https://www.geeksforgeeks.org/file-handling-c-classes/) to files from c++ using the standard library. very straight forwward to use and similar to working with networking using WinSock.
> Appart from the progreamming preparation i had to look into how to make .tga images. the best way that i found was through Gimp 2 where the most options was avilable. The application is also multithreaded to speed it up, and so that i could try it out.

## How to use:
> Start the program and input the full file path to the .tga file you want to blur like this *C:\Users\User\Dokument\Image.tga*. If the file does not exist you will just have to try again. When an imager is successfully opened you get the prompt enter blurr ammount, this is a float so input anything from 0.0 to 1 or above.
