//
// Created by Marc DENTY on 01/06/2016.
//

#ifndef ANDROID_TIFFBITMAPFACTORY_MEMORYTIFF_H
#define ANDROID_TIFFBITMAPFACTORY_MEMORYTIFF_H


#include <tiffio.h>
#include <stdlib.h>
#include <tiffiop.h>

extern "C" {
tsize_t MemTiffReadProc(thandle_t handle, tdata_t buf, tsize_t size);
tsize_t MemTiffWriteProc(thandle_t handle, tdata_t buf, tsize_t size);
toff_t  MemTiffSeekProc(thandle_t handle, toff_t off, int whence);
int     MemTiffCloseProc(thandle_t handle);
toff_t  MemTiffSizeProc(thandle_t handle);
int     MemTiffMapProc(thandle_t handle, tdata_t *base, toff_t *psize);
void    MemTiffUnmapProc(thandle_t handle, tdata_t base, toff_t size);
void    MemTiffFree(thandle_t handle);
};

#define MALLOC(ptr, type, number, action) {\
if (((ptr) = (type*) malloc ((number)*sizeof(type))) == NULL) {\
    (void) fprintf (stderr, "[%s: #%04d] ERROR : malloc of %lu bytes failed !\n", __FILE__, __LINE__, number*sizeof(type));\
    perror ("Operating system message");\
    action;}}

#define REALLOC(ptr, type, number, action) {\
if (((ptr) = (type*) realloc ((ptr), (number)*sizeof(type))) == NULL) {\
    (void) fprintf (stderr, "[%s: #%04d] ERROR : realloc of %lu bytes failed!\n", __FILE__, __LINE__, number*sizeof(type));\
    perror ("Operating system message");\
    action;}}

#define FREE(ptr) { if (ptr != NULL) free (ptr); ptr = NULL; }


class MemoryTiff {
private:
    unsigned char* data;
    tsize_t size;
    tsize_t incsiz;
    tsize_t flen;
    toff_t fptr;
    bool ownedData;

public:
    MemoryTiff(u_char* data, tsize_t dataSize) {
        this->incsiz = 0; // Is this even possible ?
        this->size = dataSize;
        this->data = data;
        this->flen = dataSize;
        this->fptr = 0;
        ownedData = FALSE;
    }

    MemoryTiff(tsize_t incsiz = 20480, tsize_t initsiz = 20480) {
        this->incsiz = incsiz;
        if (initsiz == 0) {
            initsiz = incsiz;
        }
        MALLOC(this->data, unsigned char, initsiz, exit(-1));
        ownedData = TRUE;
        this->size = initsiz;
        this->flen = 0;
        this->fptr = 0;
    }

    virtual ~MemoryTiff() {
        if (ownedData) {
            FREE(this->data);
        }
    }

    tsize_t MemTiffReadProc(tdata_t buf, tsize_t size);
    tsize_t MemTiffWriteProc(tdata_t buf, tsize_t size);
    toff_t  MemTiffSeekProc(toff_t off, int whence);
    int     MemTiffCloseProc();
    toff_t  MemTiffSizeProc();
    int     MemTiffMapProc(tdata_t *base, toff_t *psize);
    void    MemTiffUnmapProc(tdata_t base, toff_t size);
    void    MemTiffFree();

};


#endif //ANDROID_TIFFBITMAPFACTORY_MEMORYTIFF_H
