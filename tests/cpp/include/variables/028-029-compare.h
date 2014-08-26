/**
 *
 *  Test variables
 *  phptname.phpt
 *
 */




/**
 *  Set up namespace
 */
namespace TestVariables {
    
    /*
     * Test bool Value::operator==(const Value &value) const 
     */
    void test_compare1()
    {
        Php::Value v1(5), v2(5.0), v3("5"), v4("5.0");

        Php::out << "true:" << std::endl;
        Php::out << (v1 == v2) << std::endl;
        Php::out << (v1 == v3) << std::endl;
        Php::out << (v1 == v4) << std::endl;
        Php::out << (v2 == v1) << std::endl;
        Php::out << (v2 == v3) << std::endl;
        Php::out << (v2 == v4) << std::endl;
        Php::out << (v3 == v1) << std::endl;
        Php::out << (v3 == v2) << std::endl;
        Php::out << (v3 == v4) << std::endl;
        Php::out << (v4 == v1) << std::endl;
        Php::out << (v4 == v2) << std::endl;
        Php::out << (v4 == v3) << std::endl;
        
        Php::Value v5(6), v6(6.0), v7("6"), v8("6.0");

        Php::out << "false:" << std::endl;
        Php::out << (v1 == v5) << std::endl;
        Php::out << (v1 == v6) << std::endl;
        Php::out << (v1 == v7) << std::endl;
        Php::out << (v1 == v8) << std::endl;
        
        Php::out << (v2 == v5) << std::endl;
        Php::out << (v2 == v6) << std::endl;
        Php::out << (v2 == v7) << std::endl;
        Php::out << (v2 == v8) << std::endl;
        
        Php::out << (v3 == v5) << std::endl;
        Php::out << (v3 == v6) << std::endl;
        Php::out << (v3 == v7) << std::endl;
        Php::out << (v3 == v8) << std::endl;
        
        Php::out << (v4 == v5) << std::endl;
        Php::out << (v4 == v6) << std::endl;
        Php::out << (v4 == v7) << std::endl;
        Php::out << (v4 == v8) << std::endl;

        Php::Value v9, v10, v11, v12;
        v9[0] = 5;
        v9[1] = 6;

        v10[0] = 5;
        v10[1] = "Hello!";

        v11[0] = 5;
        v11[1] = 6;

        v12[0] = 5;

        Php::out << "Compare array:" << std::endl;
        Php::out << (v1 == v9) << std::endl;
        Php::out << (v5 == v9) << std::endl;
        Php::out << (v9 == v10) << std::endl;
        Php::out << (v11 == v9) << std::endl;
        Php::out << (v12 == v9) << std::endl;

        Php::Value v13 = false, v14, v15 = 0;
        Php::out << "Compare NULL:" << std::endl;
        Php::out << (v1 == v13) << std::endl;
        Php::out << (v1 == v14) << std::endl;
        Php::out << (v1 == v15) << std::endl;

        Php::out << (v13 == v14) << std::endl;
        Php::out << (v13 == v15) << std::endl;
        Php::out << (v14 == v15) << std::endl;
    }
    
    /*
     * Test bool Value::operator< (const Value &value) const
     */
    void test_compare2()
    {
        Php::Value v1(5), v2(5.0), v3("5"), v4("5.0");

        Php::out << "false:" << std::endl;
        Php::out << (v1 < v2) << std::endl;
        Php::out << (v1 < v3) << std::endl;
        Php::out << (v1 < v4) << std::endl;
        Php::out << (v2 < v1) << std::endl;
        Php::out << (v2 < v3) << std::endl;
        Php::out << (v2 < v4) << std::endl;
        Php::out << (v3 < v1) << std::endl;
        Php::out << (v3 < v2) << std::endl;
        Php::out << (v3 < v4) << std::endl;
        Php::out << (v4 < v1) << std::endl;
        Php::out << (v4 < v2) << std::endl;
        Php::out << (v4 < v3) << std::endl;
        
        Php::Value v5(6), v6(6.0), v7("6"), v8("6.0");

        Php::out << "true:" << std::endl;
        Php::out << (v1 < v5) << std::endl;
        Php::out << (v1 < v6) << std::endl;
        Php::out << (v1 < v7) << std::endl;
        Php::out << (v1 < v8) << std::endl;
        
        Php::out << (v2 < v5) << std::endl;
        Php::out << (v2 < v6) << std::endl;
        Php::out << (v2 < v7) << std::endl;
        Php::out << (v2 < v8) << std::endl;
        
        Php::out << (v3 < v5) << std::endl;
        Php::out << (v3 < v6) << std::endl;
        Php::out << (v3 < v7) << std::endl;
        Php::out << (v3 < v8) << std::endl;
        
        Php::out << (v4 < v5) << std::endl;
        Php::out << (v4 < v6) << std::endl;
        Php::out << (v4 < v7) << std::endl;
        Php::out << (v4 < v8) << std::endl;

        Php::out << "false:" << std::endl;
        Php::out << (v1 > v5) << std::endl;
        Php::out << (v1 > v6) << std::endl;
        Php::out << (v1 > v7) << std::endl;
        Php::out << (v1 > v8) << std::endl;
        
        Php::out << (v2 > v5) << std::endl;
        Php::out << (v2 > v6) << std::endl;
        Php::out << (v2 > v7) << std::endl;
        Php::out << (v2 > v8) << std::endl;
        
        Php::out << (v3 > v5) << std::endl;
        Php::out << (v3 > v6) << std::endl;
        Php::out << (v3 > v7) << std::endl;
        Php::out << (v3 > v8) << std::endl;
        
        Php::out << (v4 > v5) << std::endl;
        Php::out << (v4 > v6) << std::endl;
        Php::out << (v4 > v7) << std::endl;
        Php::out << (v4 > v8) << std::endl;

        Php::Value v9, v10, v11, v12;
        v9[0] = 5;
        v9[1] = 6;

        v10[0] = 5;
        v10[1] = "Hello!";

        v11[0] = 5;
        v11[1] = 6;

        v12[0] = 5;

        Php::out << "Compare array:" << std::endl;
        Php::out << (v1 < v9) << std::endl;
        Php::out << (v5 < v9) << std::endl;
        Php::out << (v9 < v10) << std::endl;
        Php::out << (v9 > v10) << std::endl;
        Php::out << (v11 < v9) << std::endl;
        Php::out << (v12 < v9) << std::endl;

        Php::Value v13 = false, v14, v15 = 0;
        Php::out << "Compare NULL:" << std::endl;
        Php::out << (v1 < v13) << std::endl;
        Php::out << (v1 < v14) << std::endl;
        Php::out << (v1 < v15) << std::endl;

        Php::out << (v1 > v13) << std::endl;
        Php::out << (v1 > v14) << std::endl;
        Php::out << (v1 > v15) << std::endl;

        Php::out << (v13 < v14) << std::endl;
        Php::out << (v13 < v15) << std::endl;
        Php::out << (v14 < v15) << std::endl;
    }

/**
 *  End of namespace
 */
}

