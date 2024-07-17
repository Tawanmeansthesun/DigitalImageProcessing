#include <stdio.h>
#include <iostream>
#include "ImageManager.h"
using namespace std;

ImageManager::ImageManager()
{
    header = new unsigned char[BMP_HEADER_SIZE];
    colorTable = new unsigned char[BMP_COLOR_TABLE_SIZE];
}
ImageManager::~ImageManager()
{
    delete[] header;
    delete[] colorTable;
    delete[] buf;
    delete[] original;
}

bool ImageManager::read(const char* fileName) //READ
{
    FILE *fi = fopen(fileName,"rb");

    if(fi ==(FILE*)0)
    {
        cout << "Unable to open file" << endl;
        return false;
    }

    for(int i = 0; i < BMP_HEADER_SIZE; i++) //54 byte
    {
        header[i] = getc(fi);
    }
    width = *(int *)&header[18]; //position of width(int) on header
    height = *(int *)&header[22]; //same with height and bitDepth
    bitDepth = *(int *)&header[28];

    if(bitDepth <= 8){
        fread(colorTable,sizeof(unsigned char), BMP_COLOR_TABLE_SIZE, fi);
    }

    //buf is the info of image by comprising height, width and bitDepth(is 24 bit)
    buf = new unsigned char[height * width * (bitDepth / BYTE)];//got divided by 8, so we have 3 color channels
    fread(buf,sizeof(unsigned char), height * width * (bitDepth / BYTE), fi);
    //read on buffer, size, array , read by fi

    original = new unsigned char[height * width * (bitDepth / BYTE)];
    for (int i = 0; i < (height * width * (bitDepth / BYTE)); i++)
    {
        original[i] = buf[i];
    }

    cout << "Image " << fileName << " with " << width << " x " << height << " pixels (" << bitDepth << " bits per pixel) has been read!" << endl;

    fclose(fi);

    return true;
}

bool ImageManager::write(const char* fileName) //WRITE
{
    FILE *fo = fopen(fileName,"wb");
    if(fo ==(FILE*)0){
        cout << "Unable to create file" << endl;
        return false;
    }
    fwrite(header,sizeof(unsigned char),BMP_HEADER_SIZE,fo);//WRITING HEADER

    if(bitDepth <= 8)
    {
        fwrite(colorTable, sizeof(unsigned char), BMP_COLOR_TABLE_SIZE, fo);//COLOUR TABLE
    }

    fwrite(buf,sizeof(unsigned char), height * width * (bitDepth / BYTE), fo);//BUF

    cout << "Image " << fileName << " has been written!" << endl;
    fclose(fo);

    return true;
}

int ImageManager::getRGB(int x, int y)
{
    
    int i = y * width * (bitDepth / BYTE) + x * (bitDepth / BYTE);
    int b = buf[i];
    int g = buf[i + 1];
    int r = buf[i + 2];
    int color = (r << 16) | (g << 8) | b;

    return color;
}

void ImageManager::setRGB(int x, int y, int color)
{
    int r = (color >> 16) & 0xff;//shift-right bit for 16
    int g = (color >> 8) & 0xff; //shift-right bit for 8
    int b = color & 0xff; //when you & with 0xff, you'll get blue channel!
    int i = y * width * (bitDepth / BYTE) + x * (bitDepth / BYTE);
    buf[i] = b;
    buf[i + 1] = g;
    buf[i + 2] = r;
}

void ImageManager::convertToRed()
{
    //looping to get all pixels
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int color = getRGB(x, y);
            int r = (color >> 16) & 0xff;
            int g = (color >> 8) & 0xff;
            int b = color & 0xff;
            color = (r << 16) | (0 << 8) | 0;
            //<< shift left because we need to get r
            setRGB(x, y, color);
        }
    }
}

void ImageManager::convertToBlue()
{
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int color = getRGB(x, y);
            int r = (color >> 16) & 0xff;
            int g = (color >> 8) & 0xff;
            int b = color & 0xff;
            color = (0 << 16) | (0 << 8) | b;
            setRGB(x, y, color);
        }
    }
}

void ImageManager::convertToGreen()
{
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int color = getRGB(x, y);
            int r = (color >> 16) & 0xff;
            int g = (color >> 8) & 0xff;
            int b = color & 0xff;
            color = (0 << 16) | (g << 8) | 0;
            setRGB(x, y, color);
        }
    }
} 

void ImageManager::restoreToOriginal(){
for (int i = 0; i < (height * width * (bitDepth / BYTE)); i++){
    buf[i] = original[i];
    }
}

void ImageManager::convertToGrayscale()
{
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            int color = getRGB(x, y);
            int r = (color >> 16) & 0xff;
            int g = (color >> 8) & 0xff;
            int b = color & 0xff;
            int m = (r+g+b)/3;
        
            color = (m << 16) | (m << 8) | m;
            setRGB(x, y, color);
        }
    }
} 
