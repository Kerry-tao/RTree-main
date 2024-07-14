#include "./RTree/Node.h"
#include "./RTree/RTree.h"
// #include "./RTree/DataProcessor.h"
//  #include "RTree2.h"
//  #include "DataProcessor.h"
using namespace std;

int main()
{

    // string locFilePath = "./RTree/yelp-loc.txt";
    // string docFilePath = "./RTree/yelp-doc.txt";
    // string outputFilePath = "./RTree/dataPoint.txt";

    // int maxLines = 10000; // Control the number of lines to read
    // int maxDimension = 1300;
    // DataProcessor processor(locFilePath, docFilePath);
    // processor.processFiles(maxLines, maxDimension);

    // RTree rtree;
    // rtree.calculateDistances(processor.dataList);
    // processor.writeDataToFile(outputFilePath);

    // for (const DataPoint &data_point : processor.dataList)
    // {
    //     rtree.insert(DataPoint(data_point));
    // }
    // rtree.printTree2();
    return 0;
    // cout<<"---------\n";
    // RTree rtree2;
    // std::vector<DataPoint> data_points = {
    //     {"id1",{4,1},{1, 0, 0, 1}},
    //     {"id2",{5,2},{0, 1, 0, 1}},
    //     {"id3",{4,5},{0, 0, 1, 0}},
    //     {"id4",{9,10},{1, 0, 0, 0}},
    //     {"id5",{10,9},{0, 1, 1, 0}},
    //     {"id6",{9,9},{0, 1, 0, 0}},
    //     {"id7",{2,4},{1, 0, 0, 1}},
    //     {"id8",{8,4},{0, 1, 1, 0}},
    //     {"id9",{4,7},{0, 0, 1, 1}}
    // };
    // rtree2.calculateDistances(data_points);
    // for(const DataPoint& data_point :data_points) {
    //     rtree2.insert(DataPoint(data_point));
    // }
    // rtree2.printTree2();
}
