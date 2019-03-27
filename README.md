# Xml-Csv-Json-Converter

Program convert xml , json and csv file each others on terminal.

### Usage of Project
 Project can compile and run from terminal screen .Program takes 5 parameter. 1-Name of file 
 2-Input file name (It must be typed -i begining of file name
  3-Output file name (It must be typed -o begining of file name
   4-True means compressed mode is active 5-Ouptut format (It must be typed -t begining of format

### Sample of usage 
concube [-i inputfilename] [-o outputfilename] [-true] [-t outputfile format] 
concube -i students.json -o students.xml -t xml
### Compressed Mode
 In the process of conversion to XML format, program must provide two alternative ways.First way is defining a tag named "root" and appending all values in JSON as new tags underthe root tag. Second way, which is also named as compressed-mode, defines a "root" tag and describes the remaining data as attributes. In order to work in compressed mode while converting from json to xml, program will look for a run time variable as "true", meaning that the compressed mode is activated.
