//
// Created by Marc DENTY on 01/06/2016.
//

#include "MemoryTiff.h"

tsize_t MemTiffReadProc(thandle_t handle, tdata_t buf, tsize_t size) {
    return ((MemoryTiff *) handle)->MemTiffReadProc(buf, size);
}
tsize_t MemTiffWriteProc(thandle_t handle, tdata_t buf, tsize_t size) {
    return ((MemoryTiff *) handle)->MemTiffWriteProc(buf, size);
}
toff_t  MemTiffSeekProc(thandle_t handle, toff_t off, int whence) {
    return ((MemoryTiff *) handle)->MemTiffSeekProc(off, whence);
}
int     MemTiffCloseProc(thandle_t handle) {
    return ((MemoryTiff *) handle)->MemTiffCloseProc();
}
toff_t  MemTiffSizeProc(thandle_t handle) {
    return ((MemoryTiff *) handle)->MemTiffSizeProc();
}
int     MemTiffMapProc(thandle_t handle, tdata_t *base, toff_t *psize) {
    return ((MemoryTiff *) handle)->MemTiffMapProc(base, psize);
}
void    MemTiffUnmapProc(thandle_t handle, tdata_t base, toff_t size) {
    return ((MemoryTiff *) handle)->MemTiffUnmapProc(base, size);
}
void    MemTiffFree(thandle_t handle) {
    return ((MemoryTiff *) handle)->MemTiffFree();
}



tsize_t MemoryTiff::MemTiffReadProc(tdata_t buf, tsize_t size) {
    tsize_t n;
    if (((tsize_t) this->fptr + size) <= this->flen) {
        n = size;
    }
    else {
        n = this->flen - this->fptr;
    }
    memcpy(buf, this->data + this->fptr, n);
    this->fptr += n;

    return n;
}
tsize_t MemoryTiff::MemTiffWriteProc(tdata_t buf, tsize_t size) {
    if (((tsize_t) this->fptr + size) > this->size) {
        this->data = (unsigned char *) realloc(this->data,
                                               this->fptr + this->incsiz + size);
        this->size = this->fptr + this->incsiz + size;
    }
    memcpy(this->data + this->fptr, buf, size);
    this->fptr += size;
    if (this->fptr > this->flen) this->flen = this->fptr;

    return size;
}
toff_t  MemoryTiff::MemTiffSeekProc(toff_t off, int whence) {
    switch (whence) {
        case SEEK_SET: {
            if ((tsize_t) off > this->size) {
                this->data = (unsigned char *) realloc(this->data,
                                                         this->size + this->incsiz + off);
                this->size = this->size + this->incsiz + off;
            }
            this->fptr = off;
            break;
        }
        case SEEK_CUR: {
            if ((tsize_t) (this->fptr + off) > this->size) {
                this->data = (unsigned char *) realloc(this->data,
                                                         this->fptr + this->incsiz + off);
                this->size = this->fptr + this->incsiz + off;
            }
            this->fptr += off;
            break;
        }
        case SEEK_END: {
            if ((tsize_t) (this->size + off) > this->size) {
                this->data = (unsigned char *) realloc(this->data,
                                                         this->size + this->incsiz + off);
                this->size = this->size + this->incsiz + off;
            }
            this->fptr = this->size + off;
            break;
        }
    }
    if (this->fptr > this->flen) this->flen = this->fptr;
    return this->fptr;

}
int     MemoryTiff::MemTiffCloseProc() {
    this->fptr = 0;
    return 0;

}
toff_t  MemoryTiff::MemTiffSizeProc() {
    return this->flen;
}
int     MemoryTiff::MemTiffMapProc(tdata_t *base, toff_t *psize) {
    *base = this->data;
    *psize = this->flen;
    return (1);
}
void    MemoryTiff::MemTiffUnmapProc(tdata_t base, toff_t size) {
    // Nothing to do here
}
void    MemoryTiff::MemTiffFree() {
    if (ownedData) {
        FREE(this->data);
    }
    this->data = NULL;
}
