#include <iostream>
#include <sstream>

using namespace std;

template <typename Type> class Node {
public:
  Type *pData = NULL; // stores the address of the
  Node *pNext = NULL; // stores the address of the next node
};

template <typename Type> class LinkedList {
public:
  int count = 0;          // keeps a count of the nodes in the list
  Node<Type> *pStart;     // stores the address of the start node
  Node<Type> *pLast;      // stores the address of the last node
  Node<Type> *pPredOfLoc; // stores the address of the predecessor or logical
                          // predecessor of node that is being searched
  Node<Type> *pLoc; // stores the address of the node that is being searched

  // initializing the variables with NULL in the default constructor
  LinkedList() {
    pStart = NULL;
    pLast = NULL;
    pPredOfLoc = NULL;
    pLoc = NULL;
  }

  // returns true if the list is empty otherwise returns false
  bool IsEmpty() { return pStart == NULL; }

  // inserts a new value at the front of the list
  void InsertAtFront(Type *pObj) {
    auto *pNodeObj = new Node<Type>();
    pNodeObj->pData = pObj;
    IsEmpty()
        ? (pLast = pNodeObj)
        : (pNodeObj->pNext =
               pStart); // if list empty then the new node is also the last node
    pStart = pNodeObj;
    count++; // incrementing the count of the Nodes in the list
  }

  // inserts a new value at the end of the list
  void InsertAtEnd(Type *pObj) {
    auto *pNodeObj = new Node<Type>();
    pNodeObj->pData = pObj;
    (IsEmpty() ? pStart : pLast->pNext) =
        pNodeObj; // if the list is empty then the last node is also the first
                  // node
    pLast = pNodeObj; // updating pLast with the new last node
    count++;          // incrementing the count of the Nodes in the list
  }

  /*a generic function that prints the list using the aName attribute of the
   * data part of Node COMPLEXITY: Big O(n) Big Omega(n)
   */
  void PrintList() {
    if (!IsEmpty()) {
      Node<Type> *temp = pStart;
      while (temp != NULL) {
        cout << temp->pData->aBranchName << endl;
        temp = temp->pNext;
      }
    }
  }
};

class Commit {
public:
  string aFileData; // stores the address of then
  int aCommitId;
  string aCommitMsg;

  Commit(string message, string data, int commitId) {
    aFileData = data;
    aCommitMsg = message;
    aCommitId = commitId;
  }
};

class CommitList : public LinkedList<Commit> {
public:
  void PrintList() {
    if (!IsEmpty()) {
      Node<Commit> *temp = pStart;
      while (temp != NULL) {
        cout << "ID: '" << temp->pData->aCommitId
            << "'\nMessage: " << temp->pData->aCommitMsg
            << "\nData: " << temp->pData->aFileData << "\n"
             << endl;
        temp = temp->pNext;
      }
    }
  }
};

class Branch {
public:
  string aBranchName;
  int aCommitCount = 0;
  CommitList aCommitList;

  void MakeCommit(string message, string data) {
    auto *newCommit = new Commit(message, data, aCommitCount++);
    aCommitList.InsertAtEnd(newCommit);
  }

  void CommitLog() {
    if (aCommitList.IsEmpty()) {
      cout << "No Commits to show" << endl;
    } else {
      aCommitList.PrintList();
    }
  }
};

class BranchList : public LinkedList<Branch> {
public:
  Branch *masterBranch;
  BranchList() {
    auto *newBranch = new Branch();
    newBranch->aBranchName = "master";
    InsertAtFront(newBranch);
    masterBranch = newBranch;
  }

  // inserts a Node in its logical position in a sorted list
  void InsertSorted(Branch *pObj) {
    if (pLoc == NULL) // check using pLoc
    {
      if (pPredOfLoc == NULL) {
        InsertAtFront(pObj); // if the logical position of the value is at start
                             // of the list
      } else {               // Insert newNode after pPredOfLoc.
        if (pPredOfLoc == pLast) {
          InsertAtEnd(pObj); // if the logical position of the value is at the
                             // end of the list
        } else {
          auto *pNodeObj = new Node<Branch>(); // making a new node for storing
                                               // it in the list
          pNodeObj->pData = pObj; // assigning the address of the pObj to thr
                                  // data part of the newly made node
          pNodeObj->pNext =
              pPredOfLoc->pNext; // assigning the address of the next node to
                                 // the next field of the new node
          pPredOfLoc->pNext =
              pNodeObj; // assigning the address of the new node the the logical
                        // predecessors next field
          count++;      // incrementing the count of the Nodes in the list
        }
      }
    }
  }

  bool Search(const string &rBranchName) {
    pLoc = pStart;     // starting Search from the first node
    pPredOfLoc = NULL; // predecessor of the first node being NULL
    if (!IsEmpty()) {
      while (pLoc != NULL && pLoc->pData->aBranchName < rBranchName) {
        pPredOfLoc = pLoc;
        pLoc = pLoc->pNext;
      }
      if (pLoc != NULL &&
          pLoc->pData->aBranchName !=
              rBranchName) { // if loc of value is not found then loc is NULL
        pLoc = NULL;
      }
    }
    return pLoc != NULL;
  }
  Branch *CheckOutBranch(const string &rBranchName) {
    if (Search(rBranchName)) {
      return pLoc->pData;
    }
    return NULL;
  }

  Branch *MakeNewBranch(const string &rBranchName) {
    if (!Search(rBranchName)) {
      Branch *temp = new Branch();
      temp->aBranchName = rBranchName;
      temp->MakeCommit(
          "Initial Commit",
          (masterBranch->aCommitList.IsEmpty()
               ? ""
               : masterBranch->aCommitList.pLast->pData->aFileData));
      InsertSorted(temp);
      return temp;
    } else {
      cout << "A branch with this name already exists" << endl;
      return NULL;
    }
  }

  void DeleteBranch(const string &rBranchName) {
    if (rBranchName == "master") {
      cout << "You cannot delete 'master' branch!" << endl;
    } else if (!Search(rBranchName)) {
      cout << "No branch with name '" + rBranchName + "' exists" << endl;
    } else {
      Delete();
    }
  }

private:
  void Delete() {             // deleting the values in a sorted list
    if (pPredOfLoc == NULL) { // if the value is in front of the list then this
                              // if block is executed
      if (pStart == pLast) // if there is only one element in the list then this
                           // block is executed
        pLast = NULL;
      pStart = pLoc->pNext; // assigning the new start of list to start
    } else { // if value is not at the start of the list then this if block is
             // executed
      if (pLoc == pLast) // if the value is found at the end of the list of the
                         // list then this if block is executed
        pLast = pPredOfLoc;
      pPredOfLoc->pNext = pLoc->pNext; // assigning the next successor of pLoc
    }
    delete pLoc; // deleting the node from heap
  }
};

BranchList gRepoList[50];

int main() {
  gRepoList[0].MakeNewBranch("cheems");
  Branch *current = gRepoList[0].CheckOutBranch("cheems");

  cout<<"Branch Name:"<<current->aBranchName<<endl;
  current->MakeCommit("a commit","hello");
  current->CommitLog();


  current = gRepoList[0].CheckOutBranch("master");
  current->MakeCommit("a commit in master branch","cheemsRulesOsum");
  cout<<"Branch Name:"<<current->aBranchName<<endl;
  current->CommitLog();

  current =   gRepoList[0].MakeNewBranch("huihui");
  cout<<"Branch Name:"<<current->aBranchName<<endl;
  current->CommitLog();

  current = gRepoList[0].CheckOutBranch("cheems");
  current->MakeCommit("a new commit","panik");
  cout<<"Branch Name:"<<current->aBranchName<<endl;
  current->CommitLog();

}
