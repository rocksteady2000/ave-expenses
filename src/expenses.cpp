/* This program takes csv as input file 
 * containing monthly expenses per category line item and
 * calculates the average monthly cost of an expense per category.
 * Also calculates the average monthly overall cost based on entire year's total expenses. 
 * Using csv parser library, rapidcsv.h from https://github.com/d99kris/rapidcsv 
*/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "../include/rapidcsv.h"

using namespace std;
using namespace rapidcsv;

/*This constant is for limiting length of Columns evaluated for each month of the year. */
const int MAX_SIZE = 12;

string usage = "Usage: make run filename=<filename>";
string details = "Please correct and rerun program. Only enter numbers without any symbols or characters for each expense amount.\nExiting program.\n";

/* The getDoc fcn will open csv file using rapidcsv::Document class
 * This Document class uses input stream ifstream to open and read csv, 
 * and will throw an iostream exception if the stream has failbit or badbit set to true. 
 * Note that it is set it to skip empty lines and accept / char as beginning of line as comment prefix
 * to skip comment lines.
 */
Document getDoc(char* filename)
{
    try
    {
        Document doc( filename,
        /* Using Label parameters to label 1st column and 1st row without comments as headers*/
        LabelParams(0, 0),
        /* Using default delimiter comma */
        SeparatorParams(),
        /*Using default converter parameters, which will not convert invalid numbers and will throw exception by default*/            
        ConverterParams(),
        /*LineReader Params set to true to skip comment lines starting with '/', and also set to skip empty lines*/
        LineReaderParams(true, '/', true) );

        return doc;                   
    }
    
    /*Exception handling for incorrect file name, file type, or missing filename as input parameter. */  
    catch(const ios_base::failure& io_err)
    {
        cerr << "Unable to open file: " << filename <<". Please try again using correct filename and path."<< endl<<usage<<endl;
        cerr <<" Input/Output error reading file: "<< io_err.what(); 
        exit(1);
    }
            
}

/* This fcn will Get Column header names and check values to determine if input file is valid csv with required headers. 
*/
vector<string> getColHeader(Document doc)
{    
    /*Creating vector of strings for month values to validate with column header vector*/
    vector<string> months ={"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
    vector<string> col= doc.GetColumnNames();
       
    if(col.empty())  
    {
        cerr<<"ERROR - this is not a valid csv file. Please enter correct filename. "<<usage<<endl;
        exit(1);
    }
    
    /* checking to make sure no additional column headers were added beyond the 12 months */ 
    if (col.size()>MAX_SIZE)
    {
        /*csv should actually contain 13 columns, but rapidcsv lib does not count first column since it is considered part of row header*/
        cerr<<"Column headers out of range --parsing data only within 13 total columns. Please keep format intact where 1st column is for Expense Category, followed by a column for each month"<<endl;
        col.resize(MAX_SIZE);  
    } 
    
    /* Change formatting of months in csv header to lowercase and 3-ltr abbreviation to validate */
    
    if(col != months) 
    {   for(unsigned i=0; i< col.size(); i++) 
        {   
            if(col.at(i).size() > 3)
                {
                    col.at(i).resize(3);
                }
            transform(col.at(i).begin(), col.at(i).end(), col.at(i).begin(), ::tolower);
            /*debug
            cout<<col.at(i)<<endl;  
            */
        }
    }

    if(col != months)  
    {
        cerr<<"ERROR - this file is not the correct csv! Please enter csv containing expenses for each month. "<<usage<<endl;
        exit(1);
    }  
    
    return col;
}

/* This getRow fn will create vector the row header names to get the expense categories.
 */
            
vector<string> getRowHeader(Document doc)
{
            vector<string> row = doc.GetRowNames();
            return row;
}

/* The setAve fcn will determine average of each expense category.
*  Average is not necessarily based on entire year, but rather on number of values entered in given category.
*  This is to account for values entered mid-year, or if expenses did not start at beginning of year, or end at year-end.
*  Also calculates average total expense per month.
*/
void setAve( Document doc, vector<string> col, vector<string> row)
{
float sum =0 ,ave=0, totalAve=0;
try
{
    for (unsigned i=0; i< row.size();i++)
    {
        /*create vector for each row of values per expense category line item */
        vector<float> cost =doc.GetRow<float>(row.at(i));        
        if(cost.size()>MAX_SIZE)
        {  /* instead of throwing custom exception msg to end program, sending error msg and continuing program to output remaining calculations.
            *   throw out_of_range("out_of_range");
            */
            cerr<<"Out of Range: Need to remove extra items in row:"<<row.at(i)<<". Resizing row to only calculate average of values listed within first 12 columns for each month"<<endl;
            cost.resize(MAX_SIZE);
        }
        for (unsigned j=0; j<cost.size();j++)
        {
            if (!isnan(cost.at(j))|| !isinf(cost.at(j)))
            {
                sum += cost.at(j);
            } 
            else
            {
                throw invalid_argument("Invalid computation");

            }
        }
        /* Average calcualated based on number of values enterd in given row just in case expenses did not last for duration of entire year 
         * denominator will always be non-zero since getRow fcn does not input any empty rows
         */
        ave = sum/cost.size();

        if(!isnan(ave)|| !isinf(ave))
        {
            cout<<"Average monthly expense for "<< row.at(i) << " is $" << fixed<< setprecision(2)<< ave << endl;

        }
        else
        {
           throw invalid_argument("Invalid computation");
        }
        /*aggregate each line item average to get average monthly total for all expenses*/
        totalAve += ave;
        /*reset sum for next row iteration in loop*/
        sum = 0; 
    }       
    cout << "Total monthly average cost for all expenses is $ " << totalAve  << "." << endl;

}//end try

catch(const invalid_argument& inv_err)
{
    cerr<<"Error: "<< inv_err.what()<<".  Unable to convert to float value -Incorrect data entered in csv file!"<<details+usage<<endl;

}

catch(const out_of_range& e)
{
    cerr<<"Invalid value exceeding range of float value in csv! Error:"<<e.what() << details+ usage<< endl;
}


}
int main(int argc,char* argv[])
{
    try
    {
        if (argc==2)
        {
            char* filename = argv[1];
            Document csv = getDoc(filename);
            vector<string> col = getColHeader(csv);
            vector<string> row = getRowHeader(csv);
            setAve(csv,col,row);
         }
        else
        {
            throw invalid_argument("Incorrect command line arguments specified");
        }
    }
    catch (const invalid_argument& e)
    {
        cout << "Error: " << e.what() << endl<<usage<<endl;
    }
    return 0;
}
