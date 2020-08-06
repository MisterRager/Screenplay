#include <iostream>
#include <string.h>
#include <errno.h>

#include <turbojpeg.h>

unsigned char * initBuffer(int width, int height, int nbands) {
    unsigned char * srcBuf = new unsigned char[width * height * nbands];

    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            srcBuf[(j * width + i) * nbands + 0] = (i) % 256;
            srcBuf[(j * width + i) * nbands + 1] = (j) % 256;
            srcBuf[(j * width + i) * nbands + 2] = (j + i) % 256;
        }
    }

    return srcBuf;
}


int saveBuffer(unsigned char * srcBuf, FILE *file, int width, int height, int nbands) {
    //unsigned char *srcBuf; //passed in as a param containing pixel data in RGB pixel interleaved format
    tjhandle handle = tjInitCompress();

    if(handle == NULL) {

        const char *err = (const char *) tjGetErrorStr();
        std::cerr << "TJ Error: " << err << " UNABLE TO INIT TJ Compressor Object\n";
        return -1;
    }

    int jpegQual =92;
    int flags = 0;
    unsigned char* jpegBuf = NULL;
    int pitch = width * nbands;
    int pixelFormat = TJPF_GRAY;
    int jpegSubsamp = TJSAMP_GRAY;

    if(nbands == 3) {
        pixelFormat = TJPF_RGB;
        jpegSubsamp = TJSAMP_411;
    }

    unsigned long jpegSize = 0;

    int tj_stat = tjCompress2( handle, srcBuf, width, pitch, height,
        pixelFormat, &(jpegBuf), &jpegSize, jpegSubsamp, jpegQual, flags);

    if(tj_stat != 0) {
        const char *err = (const char *) tjGetErrorStr();
        std::cerr << "TurboJPEG Error: " << err << " UNABLE TO COMPRESS JPEG IMAGE\n";
        tjDestroy(handle);
        handle = NULL;
        return -1;
    }

    if (fwrite(jpegBuf, jpegSize, 1, file) < 1) {
        std::cerr << "Could not write JPEG file: " << strerror(errno);
        return -1;
    }
    fclose(file);

    //write out the compress date to the image file
    //cleanup
    tj_stat = tjDestroy(handle); //should deallocate data buffer

    if(tj_stat != 0) {
        const char *err = (const char *) tjGetErrorStr();
        std::cerr << "TurboJPEG Error: " << err << " UNABLE TO PROPERLY CLEAN UP\n";
        tjDestroy(handle);
        handle = NULL;
        return -1;
    }

    handle = 0;

    return 0;
}

int main() {
  static int width = 128;
  static int height = 128;
  static int nbands = 3;

  unsigned char * srcBuf = initBuffer(width, height, nbands);

  FILE *file = fopen("out.jpg", "wb");
  if (!file) {
      std::cerr << "Could not open JPEG file: " << strerror(errno);
      return -1;
  }

  saveBuffer(srcBuf, file, width, height, nbands);

  return 0;
}