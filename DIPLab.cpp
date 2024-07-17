#include <stdio.h>
#include <iostream>
#include "ImageManager.h"
#include "ImageManager.cpp"
using namespace std;

int main()
{
    ImageManager* im = new ImageManager();
    im->read("mandril.bmp");

    im->convertToGrayscale();
    im->write("grayscale.bmp");


    /*im->convertToGreen();
    im->write("green.bmp");*/

    /*im->convertToBlue();
    im->write("blue.bmp");*/

    

    return 0;
}