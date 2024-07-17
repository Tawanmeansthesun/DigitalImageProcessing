#ifndef __IMAGE_MANAGER_
#define __IMAGE_MANAGER_

#define BYTE 8
#define BMP_COLOR_TABLE_SIZE 1024
#define BMP_HEADER_SIZE 54

class ImageManager
{
    public:
        ImageManager();
        virtual ~ImageManager();
        bool read(const char* fileName);
        bool write(const char* fileName);
        int getRGB(int x,int y);
        void setRGB(int x, int y, int color);
        void convertToRed();
        void convertToBlue();
        void convertToGreen();
        void restoreToOriginal();

        int width;
        int height;
        int bitDepth;


    protected:
    
    private:
        unsigned char * header;
        unsigned char * colorTable;
        unsigned char * buf;
        unsigned char * original;
};
#endif