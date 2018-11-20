#include "memmap/memmap.hpp"

const char ** hardcodedInputFiles(int * fileCount){
    *fileCount = 14;

    const char ** inputFiles = new const char*[*fileCount];

    inputFiles[0] = "workload/r0";
    inputFiles[1] = "workload/r1";
    inputFiles[2] = "workload/r2";
    inputFiles[3] = "workload/r3";
    inputFiles[4] = "workload/r4";
    inputFiles[5] = "workload/r5";
    inputFiles[6] = "workload/r6";
    inputFiles[7] = "workload/r7";
    inputFiles[8] = "workload/r8";
    inputFiles[9] = "workload/r9";
    inputFiles[10] = "workload/r10";
    inputFiles[11] = "workload/r11";
    inputFiles[12] = "workload/r12";
    inputFiles[13] = "workload/r13";

    return inputFiles;
}

int main(void){
    int fileCount;
    const char ** inputFiles = hardcodedInputFiles(&fileCount);

    Relation * r = new Relation[fileCount];

    for(int i=0; i<fileCount; i++){
        r[i] = mapFile(inputFiles[i]);
    }
    
    printData(r[4]);

    for(int i=0; i<fileCount; i++){
        unmapData(r[i]);
    }
    
    delete[] inputFiles;
    delete[] r;
}
