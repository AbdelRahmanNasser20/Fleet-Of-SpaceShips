// UMBC - CMSC 341 - Spring 2022 - Proj2
#include "fleet.h"
Fleet::Fleet(){
    m_root = nullptr;
}

Fleet::~Fleet(){
clear();

}

void Fleet::insert(const Ship& ship){

    // if its id is less than the min or greater than the max or its id is there, return
    if (ship.getID() < MINID or ship.getID() > MAXID or findShip(ship.getID()))
        return;

    m_root = ins(m_root, ship);

}

Ship* Fleet::ins(Ship* root, const Ship &ship) {
    // if the root is null ins the new ship
    if (root == nullptr) {
        return root = new Ship(ship.getID(), ship.getType(), ship.getState());
    }

    // if the ship id is less than the current root node
    else if (root->getID() > ship.getID()) {
        // insert left
        root->setLeft(ins(root->getLeft(), ship));

    }

    // insert right
    else if (root->getID() < ship.getID()) {

        root->setRight(ins(root->getRight(), ship));
    }

    else {
        return root;
    }

    updateHeight(root);

    if (abs(checkImbalance(root)) > 1){
        root = rebalance(root);
    }

    return root;

}

void Fleet::clear(){
    clearRec(m_root);
}

void Fleet::clearRec(Ship* root){
    // if root is not null
  if (root != nullptr) {

      // post order recurse through the nodes and dellocate them
      clearRec((root->getRight()));
      clearRec((root->getLeft()));
      delete root;
      root = nullptr;
  }

}

void Fleet::remove(int id) {
    m_root = deleteRecu(m_root, id);
    }

Ship* Fleet::removeRecu(Ship* ship, int id){

    if (ship == nullptr)
        return ship;

    if (id < ship->getID()) {
        ship->setLeft(removeRecu(ship->getLeft(), id));
    }

    else if (id > ship->getID())
        ship->setRight(removeRecu(ship->getRight(),id));

    // if we found the node to remove
    else {

        // if the node has no child nodes //delete that node
        if (ship->getHeight() == 0) {
            delete ship;
            ship = nullptr;
            return ship;
        }

        // if there is only a left child
        else if (ship->getLeft() != nullptr and ship->getRight() == nullptr){
//            cout << "Case 1 " << ship->getID() << endl;
            // get the biggest value in the left node
            Ship *temp = ship->getLeft();


            // copy over the data
            ship->setID(temp->getID());
            ship->setState(temp->getState());
            ship->setType(temp->getType());

            ship->setLeft(removeRecu(ship->getLeft(), temp->getID()));

        }

        // else two nodes or right node
        else {
            // find the ship to replaces current delete node
            Ship *temp = leftRecu(ship->getRight());

            // copy over the data
            ship->setID(temp->getID());
            ship->setState(temp->getState());
            ship->setType(temp->getType());

            ship->setRight(removeRecu(ship->getRight(), ship->getID()));

        }

    }

    updateHeight(ship);

    if (abs(checkImbalance(ship)) > 1){
        ship = rebalance(ship);
    }

    return ship;

}

// return the max of two ints
int Fleet::max(int a, int b ){
    if (a > b)
        return a;
    else
        return b;
}

void Fleet::updateHeight(Ship* aShip){
    // get the height of the the two children
    int heightLeftShip = height(aShip->getLeft());
    int heightRightShip = height(aShip->getRight());

    // the current height the the max of the two heights plus one
    aShip->setHeight(1 + max(heightLeftShip, heightRightShip));
}

int Fleet::height(Ship *root) {
    // if the ship is null it has a height a -1
    if (root == nullptr)
        return -1;
    // return the ship height
    return root->m_height;
}

int Fleet::checkImbalance(Ship* aShip){
    // set the default left/right height to -1
    int leftHeight = -1;
    int rightHeight = -1;

    // if the left node is not nullptr
    if (aShip->getLeft() != nullptr)
        leftHeight = aShip->getLeft()->getHeight(); // set left height to lef node height

    if (aShip->getRight() != nullptr)
        rightHeight = aShip->getRight()->getHeight();

    // return the difference of height
    return (leftHeight - rightHeight);
}

Ship* Fleet::rebalance(Ship* aShip){
    int balance = checkImbalance(aShip);

    // Right rotation
    if (balance > 1 and checkImbalance(aShip->getLeft()) >= 0 ) {
        return rightRotation(aShip);
    }
    // left rotation
    if (balance < -1 and checkImbalance(aShip->getRight()) <= 0) {
        return leftRotation(aShip);
    }
    //left right rotation
    if (balance > 1 and checkImbalance(aShip->getLeft()) < 0 ) {
        // left rotate the ships left child
        aShip->setLeft(leftRotation(aShip->getLeft()));
        return rightRotation(aShip);
    }

    // right left rotation
    if (balance < -1 and checkImbalance(aShip->getRight()) > 0){
        aShip->setRight(rightRotation(aShip->getRight()));
        // right rotate the ships left child
        return leftRotation(aShip);
    }

    return aShip;
}

void Fleet::dumpTree() const {
    dump(m_root);
}

void Fleet::dump(Ship* aShip) const{
    if (aShip != nullptr){
        cout << "(";
        dump(aShip->m_left);//first visit the left child
        cout << aShip->m_id << ":" << aShip->m_height;//second visit the node itself
        dump(aShip->m_right);//third visit the right child
        cout << ")";
    }
}

void Fleet::listShips() const {
    listShipsInOrder(m_root);
}

void Fleet::listShipsInOrder(Ship* root) const{
    // in order list ships
    if (!root) {
        return;
    }
    listShipsInOrder(root->getLeft());
    cout << root->getID() << ":" << root->getStateStr() << ":" << root->getTypeStr() << endl;
    listShipsInOrder(root->getRight());
}

bool Fleet::setState(int id, STATE state){
    // if we find the ship
    Ship * foundShip = findShipRec(m_root, id);
    if (foundShip != nullptr) {
        foundShip->setState(state); // change its state
        return true;
    }
    return false;

}

void Fleet::removeLost(){
    findLostShip(m_root);
}

bool Fleet::findShip(int id) const {
    // if we find the ship return tree
    if (findShipRec(m_root, id) != nullptr)
        return true;
    return false;

}

Ship *Fleet::findShipRec(Ship *root, int id) const{
    // if its null or we found the ship return the ship
    if(root == nullptr or root->getID() == id)
        return root;

    // recurse left if the id is smaller than the node
    if (id < root->getID())
        return findShipRec(root->getLeft(), id);

    //else recurse right
    return findShipRec(root->getRight(), id);

}

// post order travelse of removing trees
void Fleet::findLostShip(Ship* ship){

    if (ship == nullptr)
        return;

    findLostShip(ship->getRight());

    findLostShip(ship->getLeft());

    // if ship is lost remove it
    if (ship->getStateStr() == "LOST")
        remove(ship->getID());

    }

Ship *Fleet::rightRotation(Ship *y) {

    Ship * x = y->getLeft();
    Ship * t1 = x->getRight();

    // rotation
    y->setLeft(t1);
    x->setRight(y);

    // updates the height
    updateHeight(y);
    updateHeight(x);

    return x;
}

Ship *Fleet::leftRotation(Ship *y) {
    Ship * x = y->getRight();
    Ship * t1 = x->getLeft();

    // rotation
    x->setLeft(y);
    y->setRight(t1);

    updateHeight(y);
    updateHeight(x);
    return x;
}

Ship * Fleet::leftRecu(Ship *ship) {
    // finds the smallest node in the subtree
    Ship *temp = ship;
    while (temp->getLeft() != nullptr){
        temp = temp->getLeft();
    }
    return temp;
}

Ship * Fleet::deleteRecu(Ship * ship, int id){

    if (ship == nullptr)
        return ship;

    // if
    if (id < ship->getID()) {
        ship->setLeft(deleteRecu(ship->getLeft(), id));
    }

    else if (id > ship->getID())
        ship->setRight(deleteRecu(ship->getRight(),id));

        // if we found the node to remove
    else {

        // if the node has no child nodes
         if (ship->getHeight() == 0) {
            delete ship;
            ship = nullptr;
            return ship;
        }

         // if has right child
        else if (ship->getLeft() == nullptr){
            //Case 1
            Ship* temp = ship->getRight();
            delete ship;
            //ship = nullptr;
            return temp;
        }

        // if has left child
        else if(ship->getRight() == nullptr){
            Ship* temp = ship->getLeft();
            delete ship;
            //ship = nullptr;
            return temp;
        }

        // if two children
        else {

            // find the ship to replaces current delete node
            Ship *temp = leftRecu(ship->getRight());

            // copy over the data
            ship->setID(temp->getID());
            ship->setState(temp->getState());
            ship->setType(temp->getType());

            //
            ship->setRight(deleteRecu(ship->getRight(), temp->getID()));
        }

    }
    // update heights of nodes
    updateHeight(ship);

    // check imbalance
    if (abs(checkImbalance(ship)) > 1){
        ship = rebalance(ship);
    }

    return ship;
}