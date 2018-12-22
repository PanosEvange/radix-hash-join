#include "join/parse.hpp"
#include "join/inputManager.hpp" //for ignoreLine

#include "join/stats.hpp"

//global
Relation * r;
uint64_t relationsSize;

// Intermediate IR;

void executeQueries() {
    char * line = NULL;
    size_t s = 0;
    while (getline(&line, &s, stdin) > 0) {
         if(ignoreLine(line)){
             continue;
         }

        if (strcmp(line, "F\n") == 0) {
            // std::cout << "**End of Batch**" << '\n';
            // free(line);
            continue;
        }

        QueryInfo * queryInfo = parseInput(line);

        // std::cout << "RE-ORDERED PREDICATES" << std::endl;
        // for (uint64_t i = 0; i < queryInfo->predicatesCount; i++) {
        //     std::cout << "\t";
        //     printPredicate(&queryInfo->predicates[i]);
        // }

        Intermediate * IR = new Intermediate;
        for(uint64_t i=0; i<4; i++)
            IR->results[i] = NULL;
        IR->length = 0;

        for (uint64_t i = 0; i < queryInfo->predicatesCount; i++) {
            execute(&queryInfo->predicates[i], queryInfo->relations, IR);
        }

        calculateSums(queryInfo, IR);

        // std::cout << "Intermediate Results after query execution:" << '\n';
        // printResult(IR->results, IR->relCount);

        deleteIntermediate(IR);


        deleteQueryInfo(queryInfo);
        // free(line);
        // line = NULL;
    }
    free(line);
}

int main(void){
    mapAllData(&r, &relationsSize);

    if( r == NULL ){ //no file found
        std::cout << "No file found. Please try again. Bye!" << '\n';
        return -1;
    }

    // Print the data from a given file
    // if(relationsSize) printData(r[0]);
    std::cout << '\n';

    //execute queries etc
    executeQueries();



        std::cout   << "Before\n"
                    << "0.0" << ": l=" << r[0].l[0]
                       << "  u=" << r[0].u[0]
                       << "  f=" << r[0].f[0]
                       << "  d=" << r[0].d[0] << "\n"
                    << "0.1" << ": l=" << r[0].l[1]
                       << "  u=" << r[0].u[1]
                       << "  f=" << r[0].f[1]
                       << "  d=" << r[0].d[1] << "\n"
                    << "0.2" << ": l=" << r[0].l[2]
                       << "  u=" << r[0].u[2]
                       << "  f=" << r[0].f[2]
                       << "  d=" << r[0].d[2] << "\n";
        equalFilter(0,1,9403);
        std::cout   << "\n\nAfter\n"
                    << "0.0" << ": l=" << r[0].l[0]
                       << "  u=" << r[0].u[0]
                       << "  f=" << r[0].f[0]
                       << "  d=" << r[0].d[0] << "\n"
                    << "0.1" << ": l=" << r[0].l[1]
                       << "  u=" << r[0].u[1]
                       << "  f=" << r[0].f[1]
                       << "  d=" << r[0].d[1] << "\n"
                    << "0.2" << ": l=" << r[0].l[2]
                       << "  u=" << r[0].u[2]
                       << "  f=" << r[0].f[2]
                       << "  d=" << r[0].d[2] << "\n";

    unMapAllData(r, relationsSize);

    return 0;
}
