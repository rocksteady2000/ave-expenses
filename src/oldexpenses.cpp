#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>
#include "../include/rapidcsv.h"

using namespace std;

/* this program takes csv containing monthly expenses
from given csv containing monthly expenses per category line item and
calculates the average monthly cost of an expense per category.
Also calculates the average monthly overall cost. */


int main(int argc,char* argv[])
    {
        float sum,ave,totalAve = 0;
        try{ //using csv parser library, rapidcsv.h. this doc fcn uses input stream ifstream to open and read csv, and will throw an iostream exception if the stream has failbit or badbit set to true. Note that i set it to skip empty lines and accept $ char.

            rapidcsv::Document doc( argv[1],
            rapidcsv::LabelParams(0, 0),
            rapidcsv::SeparatorParams() /* default is comma*/,
            rapidcsv::ConverterParams(),
            rapidcsv::LineReaderParams(true /* SkipCommentLines */,
                                        '/' /* CommentPrefix */,
                                        true /* SkipEmptyLines */));

            vector<string> col= doc.GetColumnNames();
            //checking to make sure no additional column headers were added
            if (col.size()>12){//actually contains 13 columns, but csv lib does not count first column since it is considered part of row header
                cerr<<"Column headers out of range --resizing to 13 columns, Must keep format intact where 1st column is for Expense Category, followed by a column for each month"<<endl;
                col.resize(12);
            }
            vector<string> row = doc.GetRowNames();
            /*create vector for each row of float values in csv
                    empty fields will be treated as 0, improper format will be caught by invalid_arg exception */
            for (unsigned i=0; i< row.size();i++){
                vector<float> cost(12);cost =doc.GetRow<float>(row.at(i));

                if(cost.size()>12){//instead of throwing custom exception msg to end program, sending error msg and continuing program to output remaining calculations.
                    cerr<<"Out of Range: Need to remove extra items in row:"<<row.at(i)<<". Resizing row to only calculate average of first 12 values"<<endl;
                    cost.resize(12);
                }
                // traverse vector to calculate sum and average for each row line item
                for (unsigned j=0; j<cost.size(); j++){
                    //this exception already caught by vector constructor aasignment so is no longer needed.
                    // if(cost.at(j)==NAN) throw i;
                    sum += cost.at(j);
                }//end inner for loop
                ave = sum/12;
                cout<<" Average monthly expense for "<< row.at(i) << " is $" << fixed<< setprecision(2)<< ave << endl;
                //aggregate each line item average to get average monthly total for all expenses
                totalAve += ave;
                sum = 0; //reset sum for next row
            }//end outer for loop
            cout << "Total monthly average cost for all expenses is $ " << totalAve  << "." << endl;
    }//end try


    catch(const ios_base::failure& io_err){
         cerr << "error: Unable to open file: " << argv[1] <<". Please try again using correct filename and path."<< endl<<"Usage:"<<argv[0]<<" <filename>"<<endl;
        cerr<<" iostream error reading file: "<< io_err.what();
    }

    catch(const invalid_argument& inv_err){
        cerr<<"Error: "<< inv_err.what()<<".  Incorrect data entered in csv file! Please correct and rerun program. Only enter numbers without any symbols or characters for each expense amount"<< endl;

    }

    // commenting out this exception. Since rapidscsv library ingests row into resizable vectors, GetRow and GetCol functions won't allow params to restrict length of output vector. So instead chose to output custom out of range messages, which will also allow option to continue with program instead of breaking loop.
       catch(const out_of_range& e){
            cerr<<"Error: data in incorrect field in csv! "<<e.what() <<  endl;
        }

    return 0;
}
