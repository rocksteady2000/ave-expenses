#include <iostream>
#include <iomanip> 
#include <vector>
#include "../include/rapidcsv.h"

using namespace std;

/* this program takes csv containing monthly expenses
from given csv containing monthly expenses per category line item and
calculates the average monthly cost of an expense per category.
Also calculates the average monthly overall cost. */


int main()
    {
     float sum,ave,totalAve = 0;
     //use csv parser
     rapidcsv::Document doc("../data/budget.csv", 
			    rapidcsv::LabelParams(0, 0), 
			    rapidcsv::SeparatorParams() /* default is comma*/, 
			    rapidcsv::ConverterParams(),
                            rapidcsv::LineReaderParams(false /* SkipCommentLines */,
                                                      '/' /* CommentPrefix */,
                                                      true /* SkipEmptyLines */));
    vector<string> col = doc.GetColumnNames();
    vector<string> row = doc.GetRowNames();

    try{
	//create vector of float values for each row in csv
        for (unsigned i=0; i< row.size();i++){
	    vector<float> cost =doc.GetRow<float>(row.at(i));    
	    	// traverse vector to calculate sum and average for each row line item
		for (unsigned j=0; j<cost.size(); j++){
		    sum += cost.at(j);
		}
      	    ave = sum/12;
 	    cout<<" Average monthly expense for "<< row.at(i) << " is $" << fixed<< setprecision(2)<< ave << endl;
      	    //aggregate each line item average to get average monthly total for all expenses
	    totalAve += ave;
      	    sum = 0; //reset sum for next row      
	}
    }

    catch(const std::out_of_range& e){
        std::cout<<"Error: missing or incorrect data in csv! "<<  endl;
    }
      cout << "Total monthly average cost for all expenses is $ " << totalAve  << "." << endl;
    }
