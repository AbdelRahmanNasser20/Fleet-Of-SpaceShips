// UMBC - CMSC 341 - Spring 2022 - Proj2
#include "fleet.h"
#include <random>
#include <string>
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{
public:
    /******************************************
    * Test function declarations go here! *
    ******************************************/
    bool testInsertionNormalCase();
    bool testInsertionEdgeCase();
    bool testInsertionErrorCase();

    bool testHeightOfEachNode();
    bool testBalancePropertyCheckValue();
    bool testBalancePropertyCheckValueError();

    bool testRemoveFunctionNormalCase();
    bool testRemoveFunctionEdgeCase();
    bool testRemoveRootCaseWithLeftNode();
    bool testRemoveRootCaseWithOutLeftNode();

    bool testBalanceHeightAfterMultipleRemovals();
    bool testBSTPropertyIsPreservedAfterMultipleRemovals();

    bool testRemoveLost();

    bool testBasicRightRotation();
    bool testBasicLeftRotation();
    bool testBasicRightLeftRotation();
    bool testBasicLeftRightRotation();

    bool testIntermidateRightRotation();
    bool testIntermidateLeftRotation();
    bool testIntermidateRightLeftRotation();
    bool testIntermidateLeftRightRotation();

    bool testIntermidateRightLeftRotationAfterDeletion();
    bool testRandomDeletion();
    void measureInsertionTime();
    void measureDeletionTime();
};

void assertTest(bool flag, string statement);
bool postOrderHeight(Ship *node);
bool postOrderCompareValues(Ship *node);

int main(){
      Tester tester;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    {
        Fleet fleet;
        int fleetSize = 10;
        int tempID = 0;
        int ID = 0;
        for(int i=0;i<fleetSize;i++){
            ID = idGen.getRandNum();
            if (i == fleetSize / 2) tempID = ID;//we store this ID for later use
            Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
            fleet.insert(ship);
        }
        cout << "\nDump after inserting " << fleetSize << " nodes:\n\n";
        fleet.dumpTree();
        cout << "\n\nList of Ships after inserting " << fleetSize << " nodes:\n";
        fleet.listShips();
        cout << endl;

        cout << tempID << endl;
        fleet.remove(tempID);
        cout << "\nDump after removig the node with ID: " << tempID << "\n\n";
        fleet.dumpTree();
        cout << "\n\nList of Ships after removing the node with ID: " << tempID << "\n";
        fleet.listShips();
        cout << endl;

    }

    {
        assertTest(tester.testInsertionNormalCase(), "Insertion Normal Case");
    }

    {
        assertTest(tester.testInsertionErrorCase(), "Insertion Error Case");
    }

    {
        assertTest(tester.testInsertionEdgeCase(), "Insertion Edge Case");
    }


    {
        assertTest(tester.testBasicRightRotation(), "Basic Right rotation");
    }

    {
        assertTest(tester.testBasicLeftRotation(), "Basic left rotation");
    }
    {
        assertTest(tester.testBasicLeftRightRotation(), "Basic left right rotation");
    }
    {
        assertTest(tester.testBasicRightLeftRotation(), "Basic Right left rotation");
    }
    {
        assertTest(tester.testIntermidateRightRotation(), "intermediate Right rotation");
    }
    {
        assertTest(tester.testIntermidateLeftRotation(), "intermediate left rotation");
    }

    {
        assertTest(tester.testIntermidateLeftRightRotation(), "intermediate left right rotation");
    }
    {
        assertTest(tester.testIntermidateRightLeftRotation(), "intermediate right left rotation");
    }
    {
        assertTest(tester.testRemoveFunctionNormalCase(), "Remove Normal Case");
    }

    {
        assertTest(tester.testRemoveFunctionEdgeCase(), "Remove Edge Case");
    }
    {
        assertTest(tester.testRemoveRootCaseWithLeftNode(), "Remove node with two children");
    }

    {
        assertTest(tester.testRemoveRootCaseWithOutLeftNode(), "Remove node in tree with one left child");
    }

    {
        assertTest(tester.testRemoveLost(), "Remove All Lost ship");
    }

    {
        assertTest(tester.testBalancePropertyCheckValue(), " Normal Case Balance Property at each node ");
    }

    {
        //assertTest(tester.testBalancePropertyCheckValueError(), " Error Case Balance Property at each node ");
    }

    {
        assertTest(tester.testBSTPropertyIsPreservedAfterMultipleRemovals(), " Balance Property at each node after multiple Removes ");
    }
    {
        assertTest(tester.testHeightOfEachNode(), "Tested height of all ships without removal");
    }

    {
        assertTest(tester.testBalanceHeightAfterMultipleRemovals(), "HEIGHT OF EACH NODE AFTER MULTIPLE REMOVAL");
    }

    {
        assertTest(tester.testIntermidateRightLeftRotationAfterDeletion(), "RIGHT LEFT AFTER DELETION");
    }

    {
        //assertTest(tester.testRandomDeletion(), " Random Deletion");
    }

    {
        tester.measureInsertionTime();
    }

    {
        tester.measureDeletionTime();
    }

    return 0;
}

void assertTest(bool flag, string statement){
    if (flag)
        cout << "Test Passed " << statement << "\n" << endl;

    else
        cout << "Test failed " << statement << "\n" << endl;
}

// A utility function to do inorder traversal of BST
bool postOrderHeight(Ship *aShip)
{

    if (aShip == nullptr) {
        return true;
    }

    else {
        // finds the imbalance in each node
        // set the default left/right height to -1
        int leftHeight = -1;
        int rightHeight = -1;

        // if the left node is not nullptr
        if (aShip->getLeft() != nullptr)
            leftHeight = aShip->getLeft()->getHeight(); // set left height to lef node height

        if (aShip->getRight() != nullptr)
            rightHeight = aShip->getRight()->getHeight();

        int imbalance = (leftHeight - rightHeight);


        if (abs(imbalance) > 1){
            cout << "the imbalance is " << imbalance << " At Ship " << aShip->getID() << "at height " << aShip->getHeight() << endl;
            if (aShip->getRight() != nullptr){
                cout << "right child" << endl;
            }
            if (aShip->getLeft() != nullptr){
                cout << "left child" << endl;
            }

            return false;}


    }

    if (!postOrderHeight(aShip->getLeft()))
        return false;


    else if (!postOrderHeight(aShip->getRight()))
        return false;
    return true;


}

bool postOrderCompareValues(Ship *node){

    if (node == nullptr) {
        return true;
    }

    else{
            if (node->getRight() != nullptr and node->getRight()->getID() < node->getID()) {
                return false;
            }

            if (node->getLeft() != nullptr and node->getLeft()->getID() > node->getID()) {
                return false;
            }
        }

    bool leftFlag = postOrderCompareValues(node->getLeft());

    bool rightFlag = postOrderCompareValues(node->getRight());

    if (!leftFlag)
        return false;

    else if (!rightFlag)
        return false;

    else
        return true;

}

bool Tester::testInsertionNormalCase() {
    Fleet fleet;
    fleet.insert(Ship(20000));
    fleet.insert(Ship(10000));
    fleet.insert(Ship(30000));
    if (fleet.m_root != nullptr and fleet.m_root->m_left != nullptr and fleet.m_root->m_right != nullptr)
        return true;
    return false;
}

bool Tester::testInsertionErrorCase() {
    Fleet fleet;
    fleet.insert(Ship(20000));
    fleet.insert(Ship(10000));
    fleet.insert(Ship(30000));
    fleet.insert(Ship(20000));
    fleet.insert(Ship(30000));
    fleet.insert(Ship(MAXID+1));
    fleet.insert(Ship(MINID-1));

    // test if there are any nodes at height 3
    if (fleet.m_root->m_left->m_left == nullptr and fleet.m_root->m_left->m_right == nullptr
    and fleet.m_root->m_right->m_left == nullptr and fleet.m_root->m_right->m_right == nullptr)
        return true;
    return false;
}

bool Tester::testInsertionEdgeCase() {
    Fleet fleet;
    fleet.insert(Ship(10000));
    if (fleet.m_root != nullptr)
        return true;
    return false;
}

bool Tester::testRemoveLost(){
    Fleet fleet;
    fleet.insert(Ship(20000));
    fleet.insert(Ship(10000));
    fleet.insert(Ship(30000));
    fleet.m_root->m_left->setState(static_cast<STATE>(1));
    fleet.m_root->m_right->setState(static_cast<STATE>(1));
    fleet.m_root->setState(static_cast<STATE>(1));

    fleet.removeLost();
    fleet.dumpTree();
    if (fleet.m_root == nullptr)
        return true;
    return false;
}

bool Tester::testRemoveFunctionNormalCase() {
    Fleet fleet;
    fleet.insert(Ship(20000));
    fleet.insert(Ship(10000));
    fleet.insert(Ship(30000));

    fleet.remove(10000);
    fleet.remove(30000);

    if (fleet.m_root->m_left == nullptr and fleet.m_root->m_right == nullptr)
        return true;
    return false;
}

bool Tester::testRemoveFunctionEdgeCase() {
    Fleet fleet;
    fleet.insert(Ship(20000));

    fleet.remove(20000);

    if (fleet.m_root == nullptr)
        return true;
    return false;
}

bool Tester::testBalancePropertyCheckValue() {
    Fleet fleet;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    int ID = 0;


    for (int i = 0; i < 1000000; i++){
        ID = idGen.getRandNum();
        fleet.insert(Ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum())));
    }


    bool flag = postOrderCompareValues(fleet.m_root);

    if (!flag)
        return false;
    return true;

}

bool Tester::testBalancePropertyCheckValueError() {
    Fleet fleet;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    int ID = 0;


    for (int i = 0; i < 7; i++){
        ID = idGen.getRandNum();
        fleet.insert(Ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum())));
    }

    //fleet.m_root->m_right = (new Ship(95000));
    //fleet.m_root->m_right->m_right = (new Ship(93000));
    //fleet.m_root->m_left->m_left->m_left = (new Ship(30000));

    bool flag = true;

    flag = postOrderCompareValues(fleet.m_root);
    delete fleet.m_root->m_left->m_left->m_left;
    if (!flag)
      //  delete(fleet.m_root->m_left->m_left->m_left);
        return true;
    //delete(fleet.m_root->m_left->m_left->m_left);
    return false;

}

bool Tester::testHeightOfEachNode(){
    Fleet fleet;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    int ID = 0;

    for (int i = 0; i < 1000000; i++){
        ID = idGen.getRandNum();
        fleet.insert(Ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum())));
    }

    if (!postOrderHeight(fleet.m_root))
        return false;
    return true;

}

bool Tester::testBalanceHeightAfterMultipleRemovals(){
    Fleet fleet;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    int ID = 0;
    int NumToRemove = 5000;
    int shipsToRemove[NumToRemove];

    for (int i = 0; i < 10000; i++){
        ID = idGen.getRandNum();
        fleet.insert(Ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum())));
        // start removing once half the ships are in tree
        if (i >= NumToRemove)
            shipsToRemove[i-NumToRemove] = ID;
    }
    for (int i =0 ; i < NumToRemove;i++){
        fleet.remove((shipsToRemove[i]));
    }

    if (!postOrderHeight(fleet.m_root))
        return false;
    return true;
}

bool Tester::testBSTPropertyIsPreservedAfterMultipleRemovals(){
    Fleet fleet;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    int ID = 0;


    for (int i = 0; i < 10000; i++){
        ID = idGen.getRandNum();
        fleet.insert(Ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum())));
        // start removing once half the ships are in tree
        if (i > 5000)
            fleet.remove(ID);
    }

    if (!postOrderCompareValues(fleet.m_root))
        return false;
    return true;
}

bool Tester::testBasicRightRotation(){
    Fleet fleet;
    fleet.insert(Ship(30000));
    fleet.insert(Ship(20000));
    fleet.insert(Ship(10000));

    if (fleet.m_root->getID() == 20000 and fleet.m_root->getLeft()->getID() == 10000
        and fleet.m_root->getRight()->getID() == 30000)
        return true;
    return false;

}

bool Tester::testBasicLeftRotation() {
    Fleet fleet;
    fleet.insert(Ship(10000));
    fleet.insert(Ship(20000));
    fleet.insert(Ship(30000));

    if (!(fleet.m_root->getHeight() == 1 and fleet.m_root->getLeft()->getHeight() == 0 and fleet.m_root->getRight()->getHeight() == 0))
        return false;

    if ( !(fleet.m_root->getID() == 20000 and fleet.m_root->getLeft()->getID() == 10000
        and fleet.m_root->getRight()->getID() == 30000))
        return false;
    return true;

}

bool Tester::testBasicLeftRightRotation() {
    Fleet fleet;
    fleet.insert(Ship(30000));
    fleet.insert(Ship(10000));
    fleet.insert(Ship(20000));

    if (!(fleet.m_root->getHeight() == 1 and fleet.m_root->getLeft()->getHeight() == 0 and fleet.m_root->getRight()->getHeight() == 0))
        return false;

    if ( !(fleet.m_root->getID() == 20000 and fleet.m_root->getLeft()->getID() == 10000
           and fleet.m_root->getRight()->getID() == 30000))
        return false;
    return true;

}

bool Tester::testBasicRightLeftRotation() {
    Fleet fleet;

    fleet.insert(Ship(10000));
    fleet.insert(Ship(30000));
    fleet.insert(Ship(20000));

    if (!(fleet.m_root->getHeight() == 1 and fleet.m_root->getLeft()->getHeight() == 0 and fleet.m_root->getRight()->getHeight() == 0))
        return false;

    if ( !(fleet.m_root->getID() == 20000 and fleet.m_root->getLeft()->getID() == 10000
           and fleet.m_root->getRight()->getID() == 30000))
        return false;
    return true;
}

bool Tester::testIntermidateRightRotation(){
    Fleet fleet;
    fleet.insert(Ship(60000));
    fleet.insert(Ship(70000));
    fleet.insert(Ship(40000));
    fleet.insert(Ship(50000));
    fleet.insert(Ship(30000));
    fleet.insert(Ship(20000));


    if (fleet.m_root->getID() == 40000 and fleet.m_root->getLeft()->getID() == 30000
        and fleet.m_root->getRight()->getID() == 60000 and fleet.m_root->getLeft()->getLeft()->getID() == 20000
        and fleet.m_root->getRight()->getRight()->getID() == 70000 and fleet.m_root->getRight()->getLeft()->getID() == 50000)
        return true;
    return false;

}

bool Tester::testIntermidateLeftRotation() {
    Fleet fleet;
    fleet.insert(Ship(30000));
    fleet.insert(Ship(20000));
    fleet.insert(Ship(50000));
    fleet.insert(Ship(40000));
    fleet.insert(Ship(60000));
    fleet.insert(Ship(70000));

    if (fleet.m_root->getID() == 50000 and fleet.m_root->getLeft()->getID() == 30000
        and fleet.m_root->getRight()->getID() == 60000 and fleet.m_root->getLeft()->getLeft()->getID() == 20000
        and fleet.m_root->getRight()->getRight()->getID() == 70000 and fleet.m_root->getLeft()->getRight()->getID() == 40000)
        return true;
    return false;

}

bool Tester::testIntermidateLeftRightRotation() {
    Fleet fleet;
    fleet.insert(Ship(60000));
    fleet.insert(Ship(70000));
    fleet.insert(Ship(40000));
    fleet.insert(Ship(50000));
    fleet.insert(Ship(20000));
    fleet.insert(Ship(45000));

    if (fleet.m_root->getID() == 50000 and fleet.m_root->getLeft()->getRight()->getID() == 45000
        and fleet.m_root->getRight()->getID() == 60000 and fleet.m_root->getLeft()->getLeft()->getID() == 20000
        and fleet.m_root->getRight()->getRight()->getID() == 70000)
        return true;
    return false;
}

bool Tester::testIntermidateRightLeftRotation() {
    Fleet fleet;
    fleet.insert(Ship(60000));
    fleet.insert(Ship(80000));
    fleet.insert(Ship(40000));
    fleet.insert(Ship(70000));
    fleet.insert(Ship(90000));
    fleet.insert(Ship(65000));

    if (fleet.m_root->getID() == 70000 and
        fleet.m_root->getLeft()->getID() == 60000 and fleet.m_root->getLeft()->getRight()->getID() == 65000
        and fleet.m_root->getRight()->getID() == 80000 and fleet.m_root->getLeft()->getLeft()->getID() == 40000
        and fleet.m_root->getRight()->getRight()->getID() == 90000)
        return true;
    return false;
}

bool Tester::testIntermidateRightLeftRotationAfterDeletion() {
    Fleet fleet;
    fleet.insert(Ship(60000));
    fleet.insert(Ship(80000));
    fleet.insert(Ship(40000));
    fleet.insert(Ship(70000));
    fleet.insert(Ship(90000));
    fleet.insert(Ship(20000));
    fleet.insert(Ship(65000));


    fleet.remove(20000);

    if (fleet.m_root->getID() == 70000 and
        fleet.m_root->getLeft()->getID() == 60000 and fleet.m_root->getLeft()->getRight()->getID() == 65000
        and fleet.m_root->getRight()->getID() == 80000 and fleet.m_root->getLeft()->getLeft()->getID() == 40000
        and fleet.m_root->getRight()->getRight()->getID() == 90000)
        return true;
    return false;
}

bool Tester::testRandomDeletion(){
    Fleet fleet;
    fleet.insert(Ship(60000));
    fleet.insert(Ship(80000));
    fleet.insert(Ship(40000));
    fleet.insert(Ship(70000));
    fleet.insert(Ship(90000));
    fleet.insert(Ship(20000));
    fleet.insert(Ship(35000));
    fleet.insert(Ship(75000));
//
//    fleet.insert(Ship(10000));
//    fleet.insert(Ship(65000));
//
//    fleet.insert(Ship(25000));
//    fleet.insert(Ship(75000));
//
//    fleet.dumpTree();
//    cout << endl;
//
//    fleet.insert(Ship(30000));
//    fleet.insert(Ship(45000));
//
//    fleet.insert(Ship(85000));
//    fleet.insert(Ship(95000));
    fleet.remove(60000);

    fleet.dumpTree();
    cout << endl;

    return true;




}

bool Tester::testRemoveRootCaseWithLeftNode(){
    Fleet fleet;
    fleet.insert(Ship(50000));
    fleet.insert(Ship(30000));
    fleet.insert(Ship(80000));
    fleet.insert(Ship(40000));
    fleet.insert(Ship(10000));

    fleet.remove(30000);

    if (fleet.m_root->getID() == 50000 and fleet.m_root->getLeft()->getID() == 40000
        and fleet.m_root->getLeft()->getLeft()->getID() == 10000
        and fleet.m_root->getRight()->getID() == 80000)
        return true;
    return false;
}

bool Tester::testRemoveRootCaseWithOutLeftNode(){
    Fleet fleet;
    fleet.insert(Ship(40000));
    fleet.insert(Ship(50000));
    fleet.insert(Ship(30000));
    fleet.insert(Ship(20000));

    fleet.remove(30000);
    if (fleet.m_root->getID() == 40000 and fleet.m_root->getLeft()->getID() == 20000
        and fleet.m_root->getRight()->getID() == 50000)
        return true;
    return false;
}

void Tester::measureInsertionTime()
{
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);

    clock_t start1 = 0, stop1, start2 = 0, stop2; //stores the clock ticks while running the program
    int n = 1000;
    Fleet fleeta, fleetb;
    int ID = 0;

    double t1, t2;
    start1 = clock();
    for (int i = 0; i < n;i++) {
        ID = idGen.getRandNum();
        Ship ship(ID, static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleeta.insert(ship);
    }
    stop1 = clock();
    t1 = start1 - stop1;

    start2 = clock();
    for (int i = 0; i < 2*n;i++) {
        ID = idGen.getRandNum();
        Ship ship(ID, static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleeta.insert(ship);
    }
    stop2 = clock();
    t2 = start2 - stop2;
    cout << "proof Insertion " << t2/(2*t1) << endl;

}

void Tester::measureDeletionTime(){

    Random idGen(MINID,MAXID);
    Random typeGen(0,4);

    clock_t start1= 0, stop1, start2 = 0, stop2; //stores the clock ticks while running the program
    int n = 1000;
    Fleet fleeta, fleetb;
    int ID = 0;

    double t1, t2;

    for (int i = 0; i < n;i++) {
        ID = idGen.getRandNum();
        Ship ship(ID, static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleeta.insert(ship);
    }

    start1 = clock();
    while (fleeta.m_root != nullptr){
        fleeta.remove(fleeta.m_root->getID());
    }

    stop1 = clock();
    t1 = start1 - stop1;


    for (int i = 0; i < 2*n;i++) {
        ID = idGen.getRandNum();
        Ship ship(ID, static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleeta.insert(ship);
    }
    start2 = clock();
    while (fleetb.m_root != nullptr){
        fleetb.remove(fleetb.m_root->getID());
    }
    stop2 = clock();
    t2 = start2 - stop2;
    cout << "proof Deletion " << t2/(2*t1) << endl;

}