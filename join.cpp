#include "join.hpp"

extern int numberOfBuckets;

void join(relation * A, relation * B){
    // Order given touples bucket by bucket (basically produces A' and B')
    uint32_t * histogramA;
    uint32_t * startingPosA;
    int * bucketArray;
    int * chainArray;
    tuple * orderedA = bucketify(A,&histogramA,&startingPosA);

    uint32_t * histogramB;
    uint32_t * startingPosB;
    tuple * orderedB = bucketify(B,&histogramB,&startingPosB);

    Result * result;

    // Print out the hashed values of A' and B'
    // to confirm that they are in order
    // std::cout << std::endl << "A\'\t|\tB\'" << std::endl;
    // for(uint32_t i=0; i<A->size; i++){
    //     std::cout << h1(orderedA[i].value) << "\t|\t";
    //     std::cout << h1(orderedB[i].value) << std::endl;
    // }

    // bucketify2(A, histogramA[0], startingPosA[0], &bucketArray, &chainArray);
    // bucketify2(B, histogramB[1], startingPosB[1], &bucketArray, &chainArray);

    for (int i = 0; i < numberOfBuckets; i++) {
        // std::cout << "Checking for bucket " << i << '\n';

        // For each bucket find the smaller one and make an index with h2 for
        // that one. Then find the equal values and store them in result
        if (histogramA[i] >= histogramB[i]) {
            // std::cout << "Bucket A is bigger\n\n";
            // std::cout << std::endl;
            // std::cout << "Ordered bucket big:" << std::endl;
            // for (uint32_t j = 0; j < histogramA[i]; j++) {
            //     std::cout << "\t"<< j << ": " << A->column[j + startingPosA[i]].value << std::endl;
            // }
            // std::cout << std::endl;
            // std::cout << "Ordered bucket small:" << std::endl;
            // for (uint32_t j = 0; j < histogramB[i]; j++) {
            //     std::cout << "\t"<< j << ": " << B->column[j + startingPosB[i]].value << std::endl;
            // }
            // std::cout << std::endl;
            bucketify2(B, histogramB[i], startingPosB[i], &bucketArray, &chainArray);
            compare(A, B,histogramA[i], startingPosA[i], histogramB[i], \
                startingPosB[i], bucketArray, chainArray, &result, 0);
        }
        else {
            // std::cout << "Bucket B is bigger\n\n";
            // std::cout << std::endl;
            // std::cout << "Ordered bucket big:" << std::endl;
            // for (uint32_t j = 0; j < histogramB[i]; j++) {
            //     std::cout << "\t"<< j << ": " << B->column[j + startingPosB[i]].value << std::endl;
            // }
            // std::cout << std::endl;
            // std::cout << "Ordered bucket small:" << std::endl;
            // for (uint32_t j = 0; j < histogramA[i]; j++) {
            //     std::cout << "\t"<< j << ": " << A->column[j + startingPosA[i]].value << std::endl;
            // }
            // std::cout << std::endl;
            bucketify2(A, histogramA[i], startingPosA[i], &bucketArray, &chainArray);
            compare(B, A, histogramB[i], startingPosB[i], histogramA[i], \
                startingPosA[i], bucketArray, chainArray, &result, 1);
        }

        delete[] bucketArray;
        delete[] chainArray;
        // std::cout << "----------------------------------------------------------" << '\n';
    }

    deleteRelation(A);
    deleteRelation(B);

    delete[] orderedA;
    delete[] histogramA;
    delete[] startingPosA;

    delete[] orderedB;
    delete[] histogramB;
    delete[] startingPosB;

}

void compare(relation * relBig,
            relation * relSmall,
            uint32_t bucketSizeSmall,
            uint32_t startIndexSmall,
            uint32_t bucketSizeBig,
            uint32_t startIndexBig,
            int * bucketArray,
            int * chainArray,
            Result ** result,
            char flag) {
    uint32_t i;
    int32_t valueA;
    int hash_value, prime = nextPrime(bucketSizeBig);
    // Compare every value of the bigger relation with the values of the smaller
    // one but are on the same bucket of h2
    for (i = startIndexSmall; i < bucketSizeSmall + startIndexSmall; i++) {
        valueA = relBig->column[i].value;
        hash_value = h2(valueA, prime);
        checkEquals(valueA, hash_value, relSmall, bucketSizeBig, startIndexBig, \
            bucketArray, chainArray, result, flag, i + startIndexBig);
    }
}

void checkEquals(int32_t valueA,
                int hash_value,
                relation * relB,
                uint32_t bucketSizeBig,
                uint32_t startIndexBig,
                int * bucketArray,
                int * chainArray,
                Result ** result,
                char flag,
                int rowIdA) {
    int32_t valueB;
    // Get the rowId of the first value in the current bucket
    int rowId = bucketArray[hash_value], normRowId;
    while (rowId != -1) {
        // We need to normalse the rowid because it has values from -1 to
        // the size of the bucket but we are using the actual array
        normRowId = rowId + startIndexBig;
        // Get the value from the bigger array
        valueB = relB->column[normRowId].value;
        // Compare the values and add them in the list if they are wqual
        if (valueA == valueB) {
            // We need this flag so we will know which rowId goes first and
            // which second
            if (flag == 0) {
                std::cout << "Equal:" << valueA << " = " << valueB << ", " <<\
                rowId << ":" << rowIdA << '\n';
            }
            else {
                std::cout << "Equal:" << valueA << " = " << valueB << ", " <<\
                rowIdA << ":" << rowId << '\n';
            }
        }
        // Find the next element in the current bucket
        rowId = chainArray[rowId];
    }

}
