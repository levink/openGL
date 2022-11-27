#ifndef __FORMATS_H
#include"formats.h"
#endif

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

unsigned char *LoadIndexedBMPFile(const char *path,int *width,int *height)
{
	unsigned char *result = NULL;
    FILE *in = fopen(path,"rb");
	if(!in)
		return NULL;
    BMPHeader hdr;
    fread(&hdr,sizeof(hdr),1,in);
    if(hdr.type != 0x4D42)           // Not a bitmap file at all
		return NULL;
	BMPInfoHeader	infoHdr;
	fread(&infoHdr,sizeof(infoHdr),1,in);
	if(infoHdr.bitsPerPixel != 8)
		return NULL;
	if(infoHdr.compression)
		return NULL;
	if((result = new unsigned char[infoHdr.width*infoHdr.height*3]) == NULL)
		return NULL;
	BMPPaletteItem palette[256];
	fread(palette,256,sizeof(BMPPaletteItem),in);
    fseek(in,hdr.offset-sizeof(hdr)-sizeof(infoHdr)-1024,SEEK_CUR);   
	unsigned char *dst = result;
	for(int y=0;y<infoHdr.height;y++) {
		for(int x=0;x<infoHdr.width;x++) {
			int idx = (unsigned char)fgetc(in);
			*dst++ = palette[idx].red;
			*dst++ = palette[idx].green;
			*dst++ = palette[idx].blue;
		}
        for(int x=0;x<((4-(infoHdr.width&3))&3);x++)  // Skip alignment bytes
           fgetc(in);
    }
    fclose(in);
	*width = infoHdr.width;
	*height = infoHdr.height;
	return result;
}
