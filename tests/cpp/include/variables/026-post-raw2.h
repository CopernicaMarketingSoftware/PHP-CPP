/**
 *
 *  Test superglobal variables _POST
 *      026-post-raw2.phpt
 *
 */


#include <iostream>
#include <fstream>

/**
 *  Set up namespace
 */
namespace TestVariables {
    using namespace Php;


    /*
     * Test 
     */
    void post_raw2(void)
    {
        out << "name1 : "<< FILES["flnm"]["name"][0] << std::endl;
        out << "name2 : "<< FILES["flnm"]["name"][1] << std::endl;

        out << "type1 : "<< FILES["flnm"]["type"][0] << std::endl;
        out << "type2 : "<< FILES["flnm"]["type"][1] << std::endl;

        out << "error1 : "<< FILES["flnm"]["error"][0] << std::endl;
        out << "error2 : "<< FILES["flnm"]["error"][1] << std::endl;

        out << "size1 : "<< FILES["flnm"]["size"][0] << std::endl;
        out << "size2 : "<< FILES["flnm"]["size"][1] << std::endl;


        int length0 = FILES["flnm"]["size"][0];
        int length1 = FILES["flnm"]["size"][1];
        char *buffer0, *buffer1;
       
        std::ifstream file0, file1;
        std::string filename0 = FILES["flnm"]["tmp_name"][0];
        std::string filename1 = FILES["flnm"]["tmp_name"][1];

        file0.open(filename0, std::ios::in | std::ios::binary);
        file1.open(filename1, std::ios::in | std::ios::binary);

        if(!file0.is_open() || !file1.is_open()) {
            out << "Cannot open file." << std::endl;
            return;
        }
       
        //allocate memory
        buffer0 = new char[length0];
        buffer1 = new char[length1];
       
        //read data as a block to buffer
        file0.read(buffer0, length0);
        file1.read(buffer1, length1);
        file0.close();
        file1.close();
       
        out << "content1 : ";
        out.write(buffer0, length0);
        out << std::endl;
        out << "content2 : ";
        out.write(buffer1, length1);
        out << std::endl;
       
        delete[] buffer0;
        delete[] buffer1;
    }

/**
 *  End of namespace
 */
}

