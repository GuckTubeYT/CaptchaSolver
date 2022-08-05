#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "httpReq.h"

struct {
    char headerCheck[6];
    int height;
    int width;
    char useAlpha;
    uint8_t* pixelData;
} RTTEXHeaders;

struct {
    int score;
    int scoreChessBoard;
    int bestScore;
    int bestX;
    int bestY;
    char result[20];
} solvingResult;

struct algPuzzle {
    int x;
    int y;
    int expect;
};

struct algPuzzle yellowLineCheck[] = {
    { 38, 1, 0 }, { 29, 2, 0 }, { 45, 2, 0 }, { 36, 3, 0 }, { 27, 4, 0 }, { 43, 4, 30 }, { 34, 5, 30 }, { 50, 5, 0 }, { 25, 6, 0 }, { 41, 6, 0 }, { 32, 7, 0 }, { 48, 7, 30 }, { 23, 8, 0 }, { 55, 8, 0 }, { 30, 9, 0 }, { 53, 10, 0 }, { 28, 11, 30 }, { 51, 12, 30 }, { 26, 13, 0 }, { 33, 14, 0 }, { 49, 14, 30 }, { 24, 15, 0 }, { 31, 16, 30 }, { 47, 16, 30 }, { 13, 18, 0 }, { 29, 18, 0 }, { 45, 18, 0 }, { 61, 18, 0 }, { 4, 19, 0 }, { 20, 19, 0 }, { 52, 19, 0 }, { 68, 19, 0 }, { 11, 20, 0 }, { 27, 20, 0 }, { 59, 20, 0 }, { 75, 20, 0 }, { 2, 21, 0 }, { 18, 21, 0 }, { 50, 21, 0 }, { 66, 21, 0 }, { 9, 22, 30 }, { 25, 22, 30 }, { 57, 22, 30 }, { 73, 22, 30 }, { 0, 23, 0 }, { 16, 23, 30 }, { 32, 23, 0 }, { 48, 23, 0 }, { 64, 23, 30 }, { 7, 24, 0 }, { 23, 24, 0 }, { 55, 24, 0 }, { 71, 24, 0 }, { 78, 25, 0 }, { 5, 26, 30 }, { 3, 28, 0 }, { 74, 29, 30 }, { 1, 30, 0 }, { 72, 31, 0 }, { 6, 33, 0 }, { 77, 34, 0 }, { 4, 35, 30 }, { 75, 36, 0 }, { 2, 37, 0 }, { 73, 38, 30 }, { 0, 39, 0 }, { 78, 41, 0 }, { 5, 42, 30 }, { 85, 42, 0 }, { 3, 44, 0 }, { 19, 44, 0 }, { 83, 44, 0 }, { 74, 45, 30 }, { 1, 46, 0 }, { 17, 46, 30 }, { 81, 46, 0 }, { 72, 47, 0 }, { 88, 47, 30 }, { 15, 48, 0 }, { 79, 48, 0 }, { 6, 49, 30 }, { 22, 49, 0 }, { 13, 50, 30 }, { 77, 50, 30 }, { 20, 51, 0 }, { 11, 52, 30 }, { 18, 53, 0 }, { 9, 54, 0 }, { 23, 56, 30 }, { 21, 58, 0 }, { 12, 59, 0 }, { 3, 60, 0 }, { 19, 60, 0 }, { 10, 61, 0 }, { 1, 62, 0 }, { 17, 62, 0 }, { 8, 63, 30 }, { 24, 63, 0 }, { 15, 64, 0 }, { 79, 64, 30 }, { 6, 65, 0 }, { 22, 65, 0 }, { 13, 66, 30 }, { 77, 66, 0 }, { 4, 67, 30 }, { 20, 67, 0 }, { 84, 67, 0 }, { 75, 68, 0 }, { 2, 69, 0 }, { 18, 69, 0 }, { 82, 69, 30 }, { 73, 70, 30 }, { 0, 71, 0 }, { 80, 71, 0 }, { 39, 72, 0 }, { 87, 72, 0 }, { 46, 73, 0 }, { 78, 73, 0 }, { 5, 74, 30 }, { 44, 75, 30 }, { 3, 76, 0 }, { 35, 76, 0 }, { 42, 77, 0 }, { 74, 77, 30 }, { 1, 78, 0 }, { 33, 78, 0 }, { 49, 78, 30 }, { 72, 79, 0 }, { 31, 80, 0 }, { 47, 80, 0 }, { 6, 81, 0 }, { 29, 82, 30 }, { 45, 82, 0 }, { 77, 82, 0 }, { 4, 83, 30 }, { 36, 83, 0 }, { 43, 84, 0 }, { 75, 84, 0 }, { 2, 85, 0 }, { 34, 85, 30 }, { 41, 86, 0 }, { 73, 86, 30 }, { 0, 87, 0 }, { 32, 87, 0 }, 
 };

char* captchaSolving(char* webLink) {
    memset(&solvingResult, 0, sizeof(solvingResult));
    struct httpReq rttex = httpRequest(webLink);

    if (!rttex.data) {
        printf("RTTEX File not found! exiting...\n");
        return 0;
    }
    
    for (int a = 0; a < 6; a++) RTTEXHeaders.headerCheck[a] = rttex.data[a];
    if (!strstr(RTTEXHeaders.headerCheck, "RTTXTR")) {
        printf("This is not a RTTEX File! Exiting...\n");
        return 0;
    }
    memcpy(&RTTEXHeaders.height, rttex.data + 0x08, 4);
    memcpy(&RTTEXHeaders.width, rttex.data + 0x0c, 2);
    RTTEXHeaders.useAlpha = rttex.data[0x1c];
    int memPos = 0x7c;
    printf("Packed height: %d\nPacked width: %d\nUsing alpha: %d\n", RTTEXHeaders.height, RTTEXHeaders.width, RTTEXHeaders.useAlpha);
    RTTEXHeaders.pixelData = malloc(RTTEXHeaders.height * RTTEXHeaders.width * (RTTEXHeaders.useAlpha ? 4 : 3));

    //Thanks ZTzTopia for helping me making Flip algorithm & Solving algorithm
    for (int h = 0; h < RTTEXHeaders.height / 2; h++) {
        for (int w = 0; w < RTTEXHeaders.width; w++) {
            RTTEXHeaders.pixelData[(h * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 1] = rttex.data[memPos + ((RTTEXHeaders.height - h - 1) * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 1];
            RTTEXHeaders.pixelData[((RTTEXHeaders.height - h - 1) * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 1] = rttex.data[memPos + (h * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 1];
            RTTEXHeaders.pixelData[(h * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 2] = rttex.data[memPos + ((RTTEXHeaders.height - h - 1) * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 2];
            RTTEXHeaders.pixelData[((RTTEXHeaders.height - h - 1) * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 2] = rttex.data[memPos + (h * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 2];
            RTTEXHeaders.pixelData[(h * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 3] = rttex.data[memPos + ((RTTEXHeaders.height - h - 1) * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 3];
            RTTEXHeaders.pixelData[((RTTEXHeaders.height - h - 1) * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 3] = rttex.data[memPos + (h * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 3];
            if (RTTEXHeaders.useAlpha) {
                RTTEXHeaders.pixelData[(h * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 4] = rttex.data[memPos + ((RTTEXHeaders.height - h - 1) * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 4];
                RTTEXHeaders.pixelData[((RTTEXHeaders.height - h - 1) * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 4] = rttex.data[memPos + (h * RTTEXHeaders.width + w) * (RTTEXHeaders.useAlpha ? 4 : 3) + 4];
            }
        }
    }

    int filterDistance = 16;
    int yellowLineCount = 0;

    uint8_t* pixelsFiltered = malloc(RTTEXHeaders.width * RTTEXHeaders.height);
    for (int i = 0; i < RTTEXHeaders.width * RTTEXHeaders.height; i++) {
        int x = i % RTTEXHeaders.width;
        int y = floor(i / RTTEXHeaders.width);

        int bytesPerPixel = (RTTEXHeaders.useAlpha ? 4 : 3);
        int address = ((y * RTTEXHeaders.width) * bytesPerPixel) + (x) * bytesPerPixel;
        int addressUp = (((y - 1) * RTTEXHeaders.width) * bytesPerPixel) + (x) * bytesPerPixel;
        int addressRight = ((y * RTTEXHeaders.width) * bytesPerPixel) + (x + 1) * bytesPerPixel;
        int addressBottom = (((y + 1) * RTTEXHeaders.width) * bytesPerPixel) + (x) * bytesPerPixel;
        int addressLeft = ((y * RTTEXHeaders.width) * bytesPerPixel) + (x - 1) * bytesPerPixel;

        int filter = 0;
        for (int j = 0; j < 3; j++) {
            if (x + 1 <= RTTEXHeaders.width && abs(RTTEXHeaders.pixelData[address + j] - RTTEXHeaders.pixelData[addressRight + j]) > filterDistance ||
                y + 1 <= RTTEXHeaders.height && abs(RTTEXHeaders.pixelData[address + j] - RTTEXHeaders.pixelData[addressBottom + j]) > filterDistance ||
                x - 1 > 0 && abs(RTTEXHeaders.pixelData[address + j] - RTTEXHeaders.pixelData[addressLeft + j]) > filterDistance ||
                y - 1 > 0 && abs(RTTEXHeaders.pixelData[address + j] - RTTEXHeaders.pixelData[addressUp + j]) > filterDistance) {
                filter = 1;
            }
        }
         
        if ((RTTEXHeaders.pixelData[address + 0] >= 200 && RTTEXHeaders.pixelData[address + 1] >= 200 && RTTEXHeaders.pixelData[address + 2] >= 200) &&
            (RTTEXHeaders.pixelData[address + 0] != 255 && RTTEXHeaders.pixelData[address + 1] != 255 && RTTEXHeaders.pixelData[address + 2] != 255)) {
            filter = 0;
        }

        // Detect yellow.
        int r = RTTEXHeaders.pixelData[address + 0] > 215;
        int g = abs(RTTEXHeaders.pixelData[address + 1] - RTTEXHeaders.pixelData[address + 0]) < 3;
        int b = RTTEXHeaders.pixelData[address + 2] >= 30 && RTTEXHeaders.pixelData[address + 2] < 50;
        if (r && g && b) {
            if (filter == 1) {
                yellowLineCount++;
                filter = 2;
            }
        }
        pixelsFiltered[i] = (uint8_t)filter;
    }

    

    for (int i = 0; i < RTTEXHeaders.width * RTTEXHeaders.height; i++) {
        int x = i % RTTEXHeaders.width;
        int y = floor(i / RTTEXHeaders.width);  //3.9 = 3

        solvingResult.score = 0;
        solvingResult.scoreChessBoard = 0;

        if (yellowLineCount > 512) {
            for (int j = 0; j < sizeof(yellowLineCheck) / sizeof(struct algPuzzle); j++) {
                if (yellowLineCheck[j].x + x > RTTEXHeaders.width || yellowLineCheck[j].y + y > RTTEXHeaders.height) {
                    continue;
                }

                int address = ((y + yellowLineCheck[j].y) * RTTEXHeaders.width) + (x + yellowLineCheck[j].x);
                int expect = (yellowLineCheck[j].expect == 0) ? 1 : 2;
                if (pixelsFiltered[address] == expect) {
                    solvingResult.score++;
                }
            }
        }

        if (solvingResult.score > solvingResult.bestScore) {
            solvingResult.bestScore = solvingResult.score;
            solvingResult.bestX = x;
            solvingResult.bestY = y;
        }
    }

    printf("[CaptchaSolver] score: %f, x: %f, y: %f\n", 
        (float)(solvingResult.bestScore / (sizeof(yellowLineCheck) / sizeof(struct algPuzzle))) * 100.0f, 
        (float)solvingResult.bestX / (float)RTTEXHeaders.width,
        (float)solvingResult.bestY / (float)RTTEXHeaders.height);
    
    sprintf(solvingResult.result, "%f", (float)solvingResult.bestX / (float)RTTEXHeaders.width);
    return solvingResult.result;
}