#!/usr/bin/python
import csv
import statistics
"""
this program takes csv containing monthly expenses
from given csv containing monthly expenses per category line item and 
calculates the average monthly cost of an expense per category.
Also calculates the average monthly overall cost.
"""

#read budget.csv file
with open('budget.csv', 'r') as fp:
    reader = csv.reader(fp)
    #delete header row from parser
    next(reader)
    aveSum=0
    for row in reader: 
        if len(row)!=0: #need to exclude empty rows
            #print(row)
            #pull out expense category from each row
            category = row.pop(0)
            #print(category)
            try:
                #calculate average monthly cost for each line item
                ave = statistics.mean(float(x) for x in row)
                #print(ave)
                #tally all averages to get total sum 
                aveSum += ave   
                print("average monthly cost for {0} is ${1:.2f}".format(category,ave))
            #exception handling if string entered instead of int/float in any field
            except:
                print("unable to convert data in this row")           
   #display final tally of sum of all averages      
    print("The total average monthly expenses is ${0:.2f}".format(aveSum))
